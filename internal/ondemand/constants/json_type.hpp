/**
 * @file json_type.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Brief description
 * @version 0.3.0
 * @date 2026-08-12
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

#include <cstdint>

namespace zuu::json::constants {

enum class JsonType : uint8_t {
	Null,
	Bool,
	Number,
	String,
	Object,
	Array
};

} // namespace zuu::json::constants