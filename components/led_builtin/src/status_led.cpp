#include "status_led.hpp"

#include "esp_log.h"

using namespace Types;

namespace LedBuiltin {

static const char* TAG = "StatusLed";

static constexpr uint16_t P_SLOW_[] = {500U, 500U};
static constexpr uint16_t P_FAST_[] = {100U, 100U};
static constexpr uint16_t P_DOUBLE_[] = {250U, 250U, 250U, 1000U};
static constexpr uint16_t P_HEARTBEAT_[] = {100U, 1000U};
static constexpr uint16_t P_CONNECTING_[] = {300U, 300U};
static constexpr uint16_t P_ERROR_[] = {600U, 600U};
static constexpr uint16_t P_SOS[] = {
    150U, 150U, 150U, 150U, 150U, 150U, // S: three dots
    450U, 150U, 450U, 150U, 450U, 150U, // O: three dashes
    150U, 150U, 150U, 150U, 150U, 1500U // S: three dots
};

const Pattern SLOW{P_SLOW_, sizeof(P_SLOW_) / sizeof(P_SLOW_[0U]), "SLOW"};
const Pattern FAST{P_FAST_, sizeof(P_FAST_) / sizeof(P_FAST_[0U]), "FAST"};
const Pattern DOUBLE{P_DOUBLE_, sizeof(P_DOUBLE_) / sizeof(P_DOUBLE_[0U]), "DOUBLE_BLINK"};
const Pattern HEARTBEAT{P_HEARTBEAT_, sizeof(P_HEARTBEAT_) / sizeof(P_HEARTBEAT_[0U]), "HEARTBEAT"};
const Pattern CONNECTING{
    P_CONNECTING_, sizeof(P_CONNECTING_) / sizeof(P_CONNECTING_[0U]), "CONNECTING"};
const Pattern ERROR{P_ERROR_, sizeof(P_ERROR_) / sizeof(P_ERROR_[0U]), "ERROR"};
const Pattern SOS{P_SOS, sizeof(P_SOS) / sizeof(P_SOS[0U]), "SOS"};

StatusLed::StatusLed(gpio_num_t gpio, LogicLevel level) noexcept
    : gpio_{gpio},
      level_{level},
      initialized_{false},
      currentState_{State::OFF},
      currentPattern_{nullptr},
      indexInPattern_{0U},
      xPatternTimer_{nullptr},
      xTimerStorage_{} {}

StatusLed::~StatusLed(void) noexcept {
    if (xPatternTimer_ != nullptr) {
        (void)xTimerStop(xPatternTimer_, 0U);
        xPatternTimer_ = nullptr;
    }
}

esp_err_t StatusLed::init(void) {
    esp_err_t err = ESP_OK;

    if (!initialized_) {
        gpio_config_t gpioCfg{
            .pin_bit_mask = (1ULL << static_cast<std::uint32_t>(gpio_)),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        err = gpio_config(&gpioCfg);

        if (err == ESP_OK) {
            drive(State::OFF); // Ensure LED is off initially

            // Create the timer for handling patterns
            xPatternTimer_ = xTimerCreateStatic(
                PATTER_TIMER_NAME, DEFAULT_PATTERN_PERIOD_TICKS, // placeholder
                pdFALSE, this,                                   // timer ID for callback
                &StatusLed::patternTimerCallback, &xTimerStorage_);

            if (xPatternTimer_ == nullptr) {
                ESP_LOGE(TAG, "Failed to create pattern timer");
            } else {
                initialized_ = true;
                ESP_LOGI(TAG, "Initialized on GPIO %d", gpio_);
            }
        }
    } else {
        ESP_LOGW(TAG, "Already initialized!");
    }

    return err;
}

esp_err_t StatusLed::setPattern(const Pattern* pattern) noexcept {
    esp_err_t err = ESP_OK;

    if ((pattern != nullptr) && (pattern->durationMs != nullptr) && (pattern->count > 0U)) {
        portENTER_CRITICAL(&mux_);
        (void)xTimerStop(xPatternTimer_, 0U);

        // Start from a known state
        drive(State::OFF);

        currentPattern_ = pattern;
        indexInPattern_ = 0U;
        std::uint32_t firstPeriodMs = static_cast<std::uint32_t>(currentPattern_->durationMs[0U]);
        if (firstPeriodMs == 0U) {
            firstPeriodMs = 1U; // Avoid zero period
        }

        // Start the one-shot timer with the first period
        (void)xTimerChangePeriod(xPatternTimer_, pdMS_TO_TICKS(firstPeriodMs), 0U);
        (void)xTimerStart(xPatternTimer_, 0U);

        portEXIT_CRITICAL(&mux_);
    } else {
        err = ESP_ERR_INVALID_ARG;
    }

    return err;
}

void StatusLed::patternTimerCallback(TimerHandle_t xTimer) noexcept {
    if (xTimer != nullptr) {
        StatusLed* self = static_cast<StatusLed*>(pvTimerGetTimerID(xTimer));
        if (self != nullptr) {
            self->advancePattern();
        }
    }
}

void StatusLed::advancePattern(void) noexcept {
    portENTER_CRITICAL(&mux_);

    if (currentState_ == State::ON) {
        drive(State::OFF);
    } else {
        drive(State::ON);
    }

    if ((currentPattern_ != nullptr) && (currentPattern_->durationMs != nullptr) &&
        (currentPattern_->count > 0U)) {
        indexInPattern_ = (indexInPattern_ + 1U) % currentPattern_->count;
        std::uint32_t nextPeriodMs =
            static_cast<std::uint32_t>(currentPattern_->durationMs[indexInPattern_]);
        if (nextPeriodMs == 0U) {
            nextPeriodMs = 1U; // Avoid zero period
        }

        // Restart the one-shot timer with the new period
        (void)xTimerChangePeriod(xPatternTimer_, pdMS_TO_TICKS(nextPeriodMs), 0U);
        (void)xTimerStart(xPatternTimer_, 0U);
    } else {
        // No pattern set, stop the timer
        (void)xTimerStop(xPatternTimer_, 0U);
    }

    portEXIT_CRITICAL(&mux_);
}

void StatusLed::turnOff(void) {
    portENTER_CRITICAL(&mux_);

    (void)xTimerStop(xPatternTimer_, 0U);
    currentPattern_ = nullptr;

    drive(State::OFF);

    portEXIT_CRITICAL(&mux_);
}

void StatusLed::turnOn(void) {
    portENTER_CRITICAL(&mux_);

    (void)xTimerStop(xPatternTimer_, 0U);
    currentPattern_ = nullptr;

    drive(State::ON);

    portEXIT_CRITICAL(&mux_);
}

void StatusLed::toggle(void) {
    if (currentState_ == State::ON) {
        turnOff();
    } else {
        turnOn();
    }
}

void StatusLed::drive(State state) noexcept {
    const std::uint32_t level = (level_ == LogicLevel::ACTIVE_LOW)
                                    ? static_cast<std::uint32_t>(!static_cast<bool>(state))
                                    : static_cast<std::uint32_t>(state);
    (void)gpio_set_level(gpio_, level);
    currentState_ = state;
}
} // namespace LedBuiltin