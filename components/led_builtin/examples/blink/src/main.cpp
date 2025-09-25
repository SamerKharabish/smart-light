#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

static constexpr gpio_num_t LED_BUILTIN = GPIO_NUM_2;

static TickType_t kDelayMs = 1000; // 1 second delay

// C linkage for app_main
extern "C" void app_main() {
    // Configure the GPIO pin as output
    gpio_reset_pin(LED_BUILTIN);
    gpio_set_direction(LED_BUILTIN, GPIO_MODE_OUTPUT);
    while (true) {
        // Turn the LED on
        gpio_set_level(LED_BUILTIN, 1); // High is on
        vTaskDelay(pdMS_TO_TICKS(kDelayMs));

        // Turn the LED off
        gpio_set_level(LED_BUILTIN, 0); // Low is off
        vTaskDelay(pdMS_TO_TICKS(kDelayMs));
    }
} // app_main