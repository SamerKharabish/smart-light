#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "status_led.hpp"

static const char* TAG = "StatusLedExample";

using namespace LedBuiltin;

static constexpr gpio_num_t LED_BUILTIN = GPIO_NUM_2;
static constexpr LogicLevel LED_BUILTIN_LOGIC_LEVEL = LogicLevel::ACTIVE_HIGH;

static constexpr uint16_t P_TRIPLE[] = {250U, 250U, 250U, 250U, 250U, 1000U};
const Pattern TRIPLE = {P_TRIPLE, sizeof(P_TRIPLE) / sizeof(P_TRIPLE[0U]), "TRIPLE"};

static constexpr TickType_t kDelayMs = 3000; // 3 second delay

// C linkage for app_main
extern "C" void app_main() {

    esp_log_level_set("*", ESP_LOG_DEBUG); // Can be changed to ESP_LOG_INFO to suppress debug logs

    esp_err_t err = ESP_OK;
    StatusLed statusLed(LED_BUILTIN, LED_BUILTIN_LOGIC_LEVEL);
    ESP_ERROR_CHECK(statusLed.init());

    statusLed.turnOn();
    ESP_LOGD(TAG, "LED turned ON");
    vTaskDelay(pdMS_TO_TICKS(kDelayMs));

    statusLed.turnOff();
    ESP_LOGD(TAG, "LED turned OFF");
    vTaskDelay(pdMS_TO_TICKS(kDelayMs));

    for (int i = 0; i <= 5; ++i) {
        statusLed.toggle();
        ESP_LOGD(TAG, "LED toggled to: %s",
                 (statusLed.getState() == Types::State::ON) ? "ON" : "OFF");
    }

    err = statusLed.setPattern(&TRIPLE);
    if (err == ESP_OK) {
        ESP_LOGD(TAG, "Set pattern: %s", TRIPLE.name);
        vTaskDelay(pdMS_TO_TICKS(kDelayMs));
    } else if (err == ESP_ERR_INVALID_ARG) {
        ESP_LOGW(TAG, "Invalid pattern!");
    }

    err = statusLed.setPattern(&DOUBLE);
    if (err == ESP_OK) {
        ESP_LOGD(TAG, "Set pattern: %s", DOUBLE.name);
        vTaskDelay(pdMS_TO_TICKS(kDelayMs));
    } else if (err == ESP_ERR_INVALID_ARG) {
        ESP_LOGW(TAG, "Invalid pattern!");
    }

    err = statusLed.setPattern(&HEARTBEAT);
    if (err == ESP_OK) {
        ESP_LOGD(TAG, "Set pattern: %s", HEARTBEAT.name);
        vTaskDelay(pdMS_TO_TICKS(kDelayMs));
    } else if (err == ESP_ERR_INVALID_ARG) {
        ESP_LOGW(TAG, "Invalid pattern!");
    }

    err = statusLed.setPattern(&SOS);
    if (err == ESP_OK) {
        ESP_LOGD(TAG, "Set pattern: %s", SOS.name);
    } else if (err == ESP_ERR_INVALID_ARG) {
        ESP_LOGW(TAG, "Invalid pattern!");
    }
} // app_main
