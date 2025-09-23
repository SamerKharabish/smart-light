#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdint>

// Forward declaration to avoid including FreeRTOS.h in the header
extern "C" {
typedef std::uint32_t TickType_t;
}

constexpr TickType_t kDelayMs = 10;

#endif // MAIN_HPP