#include "main.hpp"

#include "freertos/FreeRTOS.h"

// C linkage for app_main
extern "C" void app_main() {
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(kDelayMs));
    }
} // app_main
