#include "main.hpp"

#include "freertos/FreeRTOS.h"

constexpr TickType_t kDelayMs = 1000; // 1 second delay

// C linkage for app_main
extern "C" void app_main() {
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(kDelayMs));
    }
} // app_main
