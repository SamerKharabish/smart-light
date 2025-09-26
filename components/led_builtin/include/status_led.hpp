/**
 * @file status_led.hpp
 * @author Samer Kharabish (kharabishsamer@outlook.com)
 * @brief Class to indicate statuses with the built-in LED
 * @version 0.1.0
 * @date 2025-09-23
 *
 * @copyright Copyright (c) 2025 MIT License
 *
 */

#ifndef STATUS_LED_HPP_
#define STATUS_LED_HPP_

#include "common_types.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#include <cstdint>

namespace LedBuiltin {

/**
 * @brief Logic level for active low or active high
 *
 */
enum class LogicLevel : bool { ACTIVE_LOW = false, ACTIVE_HIGH = true };
/**
 * @brief Struct to define a LED blinking pattern
 *
 */
struct Pattern {
    const std::uint16_t* durationMs; // Array of durations in milliseconds
    std::size_t count;               // Number of elements in the duration array
    const char* name;                // Name of the pattern (optional, may be nullptr)
};

// Builtin patterns
extern const Pattern SLOW;       // 500ms on, 500ms off
extern const Pattern FAST;       // 100ms on, 100ms off
extern const Pattern DOUBLE;     // 250ms on, 250ms off, 250ms on, 1000ms off
extern const Pattern HEARTBEAT;  // 500ms on, 500ms off
extern const Pattern CONNECTING; // 300ms on, 300ms off
extern const Pattern ERROR;      // 600ms on, 600ms off
// 150ms on, 150ms off, 150ms on, 150ms off, 150ms on, 150ms off, // S: three dots
// 450ms on, 150ms off, 450ms on, 150ms off, 450ms on, 150ms off, // O: three dashes
// 150ms on, 150ms off, 150ms on, 150ms off, 150ms on, 1500ms off // S: three dots
extern const Pattern SOS;

class StatusLed final {
  public:
    /**
     * @brief Construct a new Status Led object
     *
     * @param gpio The GPIO number where the LED is connected
     * @param activeLow Whether the LED is active-LOW or active-HIGH (default: active-HIGH)
     */
    StatusLed(gpio_num_t gpio, LogicLevel activeLow = LogicLevel::ACTIVE_HIGH) noexcept;

    /**
     * @brief Turn the LED off and stop the timer
     *
     */
    ~StatusLed(void) noexcept;

    /**
     * @brief Initialize the status LED
     *
     * @return esp_err_t
     * @retval ESP_OK on success
     * @retval ESP_ERR_INVALID_ARG GPIO number error
     */
    [[nodiscard]] esp_err_t init(void) noexcept;

    /**
     * @brief Turn the LED on
     *
     */
    esp_err_t turnOn(void) noexcept;

    /**
     * @brief Turn the LED off
     *
     * @return esp_err_t
     * @retval ESP_OK on success
     * @retval ESP_ERR_INVALID_STATE GPIO not initialized
     */
    esp_err_t turnOff(void) noexcept;

    /**
     * @brief Toggle the LED state
     *
     * @return esp_err_t
     * @retval ESP_OK on success
     * @retval ESP_ERR_INVALID_STATE GPIO not initialized
     */
    esp_err_t toggle(void) noexcept;

    /**
     * @brief Get the current state of the LED
     *
     * @return Types::State The current state (ON or OFF)
     */
    [[nodiscard]] Types::State getState(void) const noexcept { return currentState_; }

    /**

     * @brief Set a blinking pattern
     *
     * @param pattern The pattern to set (nullptr to stop any pattern)
     * @return esp_err_t
     * @retval ESP_OK on success
     * @retval ESP_ERR_INVALID_ARG invalid pattern
     * @retval ESP_ERR_INVALID_STATE GPIO not initialized
     */
    esp_err_t setPattern(const Pattern* pattern) noexcept;

  private:
    /**
     * @brief Drive the LED to the specified state
     *
     * @param state The desired state (ON or OFF)
     */
    void drive(Types::State state) noexcept;

    /**
     * @brief Timer callback to handle pattern advancement
     *
     * @param xTimer The timer handle
     */
    static void patternTimerCallback(TimerHandle_t xTimer) noexcept;

    /**
     * @brief Advance to the next step in the current pattern
     *
     */
    void advancePattern(void) noexcept;

  private:
    gpio_num_t gpio_;
    LogicLevel level_;
    bool initialized_;
    Types::State currentState_;

    const Pattern* currentPattern_;
    std::size_t indexInPattern_;

    TimerHandle_t xPatternTimer_;
    StaticTimer_t xTimerStorage_;
    portMUX_TYPE mux_ = portMUX_INITIALIZER_UNLOCKED;
};
} // namespace LedBuiltin

#endif // STATUS_LED_HPP_