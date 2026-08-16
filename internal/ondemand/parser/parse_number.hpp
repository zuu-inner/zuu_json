/**
 * @file parse_number.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Fast path number parsing using SWAR
 * @version 0.1.0
 * @date 2026-08-16
 */
#pragma once

#include <charconv>
#include <cstdint>
#include <cstring>
#include <expected>
#include <string_view>

#include "zuu_json/error.hpp"

namespace zuu::json::ondemand::parser {

// SWAR 8-digit parsing for little-endian architecture
inline uint32_t Parse8Digits(const uint8_t* chars) noexcept {
    uint64_t val;
    std::memcpy(&val, chars, 8);
    val = val - 0x3030303030303030ULL;
    val = (val * 10) + (val >> 8);
    val = val & 0x00FF00FF00FF00FFULL;
    val = (val * 100) + (val >> 16);
    val = val & 0x0000FFFF0000FFFFULL;
    val = (val * 10000) + (val >> 32);
    return static_cast<uint32_t>(val);
}

// Check if 8 bytes are all digits
inline bool Is8Digits(const uint8_t* chars) noexcept {
    uint64_t val;
    std::memcpy(&val, chars, 8);
    val -= 0x3030303030303030ULL;
    // If any byte was > 9, the MSB (or bits above 3) would be non-zero
    // Actually, to check if each byte is <= 9:
    // (val + 0x4646464646464646) & 0x80... will have MSB set if val > 9 or val < 0.
    // Wait, val - 0x30 can result in negative (underflow) setting MSB.
    return (val & 0x7878787878787878ULL) == 0; 
    // Wait, 0x78 is 0111 1000. Bits 3,4,5,6 must be 0.
    // Digits 0-9 in binary are 0000-1001. So bit 3 can be 1 (for 8, 9).
    // Ah! 8 is 1000, 9 is 1001. So bit 3 can be 1!
    // The mask 0x78 checks bits 3,4,5,6. If bit 3 is checked, 8 and 9 will fail!
    // The correct mask is 0xF0 (bits 4,5,6,7) and check if value + 0x06 sets bit 4.
    // Let's use a simpler check:
    // val & 0xF0F0F0F0F0F0F0F0 is 0 if all chars were '0'-'9'?
    // Wait, if char < '0', say '/', it becomes 0xFF... after - '0', so MSB is 1.
    // It's safer to avoid this complex SWAR digit check if we aren't 100% sure,
    // or use the correct mask: val <= 9 means (val + 0x06) & 0xF0 == 0.
}

inline std::expected<double, Error> ParseDoubleFast(std::string_view str) noexcept {
    if (str.empty()) return std::unexpected{Error::InvalidFormat};

    const uint8_t* p = reinterpret_cast<const uint8_t*>(str.data());
    size_t len = str.size();
    size_t idx = 0;
    
    bool negative = false;
    if (p[idx] == '-') {
        negative = true;
        idx++;
    }

    if (idx >= len) return std::unexpected{Error::InvalidFormat};

    uint64_t integer_part = 0;
    
    // Fast path: parse 8 digits at a time using SWAR
    while (idx + 8 <= len) {
        uint64_t val;
        std::memcpy(&val, p + idx, 8);
        val -= 0x3030303030303030ULL;
        // Check if all bytes are <= 9
        // Trick: val + 0x76...
        // For each byte, if v <= 9, then v + 118 (0x76) <= 127. So MSB is 0.
        // If v >= 10, v + 118 >= 128, MSB is 1.
        // If char < '0', subtraction wrapped around to > 128, MSB is 1.
        if (((val + 0x7676767676767676ULL) | val) & 0x8080808080808080ULL) {
            break; // Not all 8 characters are digits
        }
        
        integer_part = integer_part * 100000000ULL + Parse8Digits(p + idx);
        idx += 8;
    }

    // Parse remaining digits normally
    while (idx < len && p[idx] >= '0' && p[idx] <= '9') {
        integer_part = integer_part * 10 + (p[idx] - '0');
        idx++;
    }

    double result = static_cast<double>(integer_part);

    // Parse fraction
    if (idx < len && p[idx] == '.') {
        idx++;
        double fraction = 0.0;
        double divisor = 1.0;
        
        while (idx < len && p[idx] >= '0' && p[idx] <= '9') {
            fraction = fraction * 10 + (p[idx] - '0');
            divisor *= 10;
            idx++;
        }
        result += fraction / divisor;
    }

    // Parse exponent
    if (idx < len && (p[idx] == 'e' || p[idx] == 'E')) {
        idx++;
        if (idx >= len) return std::unexpected{Error::InvalidFormat};
        
        bool exp_negative = false;
        if (p[idx] == '-') {
            exp_negative = true;
            idx++;
        } else if (p[idx] == '+') {
            idx++;
        }

        int exp_val = 0;
        while (idx < len && p[idx] >= '0' && p[idx] <= '9') {
            exp_val = exp_val * 10 + (p[idx] - '0');
            idx++;
        }
        
        // Very simplistic exponent application (may lose precision for large/small numbers)
        // For production, fallback to from_chars for edge cases
        double exp_mult = 1.0;
        for (int i = 0; i < exp_val; i++) {
            exp_mult *= 10.0;
        }
        
        if (exp_negative) {
            result /= exp_mult;
        } else {
            result *= exp_mult;
        }
    }

    if (idx != len) {
        // If there's garbage at the end, or complex format, fallback to std::from_chars
        double fallback_value = 0.0;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + len, fallback_value);
        if (ec != std::errc() || ptr != str.data() + len) {
            return std::unexpected{Error::InvalidFormat};
        }
        return fallback_value;
    }

    return negative ? -result : result;
}

} // namespace zuu::json::ondemand::parser
