#pragma once

#include <cstdint>

inline constexpr std::uint16_t MEM_SIZE{ 4 * 1024 };
inline constexpr std::uint8_t DISPLAY_X{ 64 };
inline constexpr std::uint8_t DISPLAY_Y{ 32 };
inline constexpr std::uint8_t REGISTER_COUNT{ 16 };
inline constexpr std::uint8_t UPSCALE{ 10 };
inline constexpr std::uint8_t PIXEL_WIDTH{ 1 * UPSCALE };
inline constexpr std::uint8_t PIXEL_HEIGHT{ 1 * UPSCALE };
inline constexpr std::uint8_t COLOUR_MAX{ 255 };
inline constexpr std::uint8_t COLOUR_MIN{ 0 };
inline constexpr std::uint8_t SET{ 1 };
inline constexpr std::uint8_t UNSET{ 0 };
inline constexpr std::uint16_t ROM_OFFSET{ 0x200 };
