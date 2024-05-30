#include "chip8.hpp"
#include "config.hpp"
#include "default_font.hpp"
#include "default_keymap.hpp"
#include "display.hpp"
#include "pixel_pos.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

chip8::chip8 () noexcept
    : mem{ std::make_unique<std::array<std::uint8_t, MEM_SIZE> > () },
      disp{ std::make_unique<display> () },
      gpr{ std::make_unique<std::array<std::uint8_t, REGISTER_COUNT> > () },
      stack{ std::make_unique<std::vector<std::uint16_t> > () },
      instr_ptr{ ROM_OFFSET },
      idx_reg{ 0 },
      delay_timer{ 0 },
      sound_timer{ 0 }
{
  for (std::size_t i = 0; i < default_font.size (); i++)
    {
      std::copy_n (default_font.at (i).begin (), default_font.at (i).size (),
                   mem->begin () + FONT_MEMORY_OFFSET
                       + (i * default_font.at (i).size ()));
    }
}

[[nodiscard]] bool
chip8::read_rom ([[maybe_unused]] std::string_view rom_name)
{
  std::string rom_file{};
  if constexpr (IS_TEST)
    {
      rom_file = "../../test/" + std::string{ rom_name } + ".ch8";
    }
  else
    {
      rom_file = "../../roms/" + std::string{ rom_name } + ".ch8";
    }

  std::filesystem::path rom_path = std::filesystem::current_path () / rom_file;
  std::ifstream file (rom_path, std::ios::in | std::ios::binary);
  if (!file.is_open ())
    {
      return false;
    }
  file.seekg (0, std::ios_base::end);
  auto length = file.tellg ();
  file.seekg (0, std::ios_base::beg);
  std::vector<char> buffer (static_cast<std::size_t> (length));
  file.read ((buffer.data ()), length);
  std::copy (buffer.begin (), buffer.end (), this->mem->begin () + ROM_OFFSET);
  return true;
}

[[nodiscard]] std::uint8_t
chip8::get_random () noexcept
{
  using std::mt19937;
  static mt19937 mt_gen{};
  return mt_gen () & 0xFF;
}

[[nodiscard]] instr
chip8::fetch () noexcept
{
  auto raw_inst = static_cast<std::uint16_t> (
      (*this->mem).at (instr_ptr + 1)
      | static_cast<std::uint16_t> ((*this->mem).at (instr_ptr) << 8U));
  instr decoded_inst = chip8::decode (raw_inst);
  instr_ptr += 2;
  return decoded_inst;
}

[[nodiscard]] constexpr instr
chip8::decode (uint16_t opcode) noexcept
{
  const auto first_nibble
      = static_cast<std::uint8_t> ((opcode >> 12U) & 0x0FU);
  const auto second_nibble
      = static_cast<std::uint8_t> ((opcode >> 8U) & 0x0FU);
  const auto third_nibble
      = static_cast<std::uint8_t> ((opcode >> 4U) & 0x00FU);
  const auto fourth_nibble = static_cast<std::uint8_t> ((opcode) & 0x000FU);
  return instr{
    .full_instr = opcode,
    .first_nibble = first_nibble,
    .second_nibble = second_nibble,
    .third_nibble = third_nibble,
    .fourth_nibble = fourth_nibble,
    .second_byte
    = static_cast<std::uint8_t> (((third_nibble << 4U) | fourth_nibble)),
    .except_first_nibble = static_cast<uint16_t> (
        (second_nibble << 8U) | (third_nibble << 4U) | fourth_nibble)
  };
}

[[nodiscard]] display *
chip8::get_display () const noexcept
{
  return this->disp.get ();
}

[[nodiscard]] std::uint8_t
chip8::get_gpr (std::uint8_t idx) const noexcept
{
  return (*this->gpr).at (idx);
}

void
chip8::set_gpr (std::uint8_t reg_idx, std::uint8_t val) noexcept
{
  (*this->gpr).at (reg_idx) = val;
}

constexpr void
chip8::set_idx_reg (std::uint16_t val) noexcept
{
  this->idx_reg = val;
}

constexpr void
chip8::inst_00E0 () const noexcept
{
  this->get_display ()->clear_window ();
}

constexpr void
chip8::inst_00EE () noexcept
{
  this->instr_ptr = (*this->stack).back ();
  (*this->stack).pop_back ();
}

constexpr void
chip8::inst_1nnn (const instr &inst) noexcept
{
  this->instr_ptr = inst.except_first_nibble;
}

constexpr void
chip8::inst_2nnn (const instr &inst) noexcept
{
  (*this->stack).push_back (this->instr_ptr);
  this->instr_ptr = inst.except_first_nibble;
}

constexpr void
chip8::inst_3xkk (const instr &inst) noexcept
{
  if ((*this->gpr).at (inst.second_nibble) == inst.second_byte)
    {
      this->instr_ptr += 2;
    }
}

constexpr void
chip8::inst_4xkk (const instr &inst) noexcept
{
  if ((*this->gpr).at (inst.second_nibble) != inst.second_byte)
    {
      this->instr_ptr += 2;
    }
}

constexpr void
chip8::inst_5xy0 (const instr &inst) noexcept
{
  if ((*this->gpr).at (inst.second_nibble)
      == (*this->gpr).at (inst.third_nibble))
    {
      this->instr_ptr += 2;
    }
}

constexpr void
chip8::inst_6xkk (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, inst.second_byte);
}

constexpr void
chip8::inst_7xkk (const instr &inst) noexcept
{
  (*this->gpr).at (inst.second_nibble) += inst.second_byte;
}

constexpr void
chip8::inst_8xy0 (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, this->get_gpr (inst.third_nibble));
}

constexpr void
chip8::inst_8xy1 (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, this->get_gpr (inst.third_nibble)
                                         | this->get_gpr (inst.second_nibble));
}

constexpr void
chip8::inst_8xy2 (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, this->get_gpr (inst.third_nibble)
                                         & this->get_gpr (inst.second_nibble));
}

constexpr void
chip8::inst_8xy3 (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, this->get_gpr (inst.third_nibble)
                                         ^ this->get_gpr (inst.second_nibble));
}

constexpr void
chip8::inst_8xy4 (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, this->get_gpr (inst.third_nibble)
                                         + this->get_gpr (inst.second_nibble));
  ((this->get_gpr (inst.second_nibble) + this->get_gpr (inst.third_nibble))
   > 255)
      ? this->set_gpr (0xF, 1)
      : this->set_gpr (0xF, 0);
}

constexpr void
chip8::inst_8xy5 (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, this->get_gpr (inst.second_nibble)
                                         - this->get_gpr (inst.third_nibble));
  (this->get_gpr (inst.second_nibble) > this->get_gpr (inst.third_nibble))
      ? this->set_gpr (0xF, 1)
      : this->set_gpr (0xF, 0);
}

constexpr void
chip8::inst_8xy6 (const instr &inst) noexcept
{
  this->set_gpr (0xF, this->get_gpr (inst.third_nibble) & 0x01U);
  this->set_gpr (inst.second_nibble, this->get_gpr (inst.third_nibble) >> 1U);
}

constexpr void
chip8::inst_8xy7 (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, this->get_gpr (inst.third_nibble)
                                         - this->get_gpr (inst.second_nibble));
  (this->get_gpr (inst.third_nibble) > this->get_gpr (inst.second_nibble))
      ? this->set_gpr (0xF, 1)
      : this->set_gpr (0xF, 0);
}

constexpr void
chip8::inst_8xyE (const instr &inst) noexcept
{
  this->set_gpr (0xF, this->get_gpr (inst.third_nibble) & 0x80U);
  this->set_gpr (
      inst.second_nibble,
      static_cast<std::uint8_t> (this->get_gpr (inst.third_nibble) << 1));
}

constexpr void
chip8::inst_9xy0 (const instr &inst) noexcept
{
  if ((*this->gpr).at (inst.second_nibble)
      != (*this->gpr).at (inst.third_nibble))
    {
      this->instr_ptr += 2;
    }
}

constexpr void
chip8::inst_Annn (const instr &inst) noexcept
{
  this->set_idx_reg (inst.except_first_nibble);
}

constexpr void
chip8::inst_Bnnn (const instr &inst) noexcept
{
  this->instr_ptr = inst.except_first_nibble + this->get_gpr (0);
}

constexpr void
chip8::inst_Cxkk (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, chip8::get_random () & inst.second_byte);
}

constexpr void
chip8::inst_Dxyn (const instr &inst) noexcept
{
  pos starting_position = {
    .x
    = static_cast<uint16_t> (this->get_gpr (inst.second_nibble) % DISPLAY_X),
    .y = static_cast<uint16_t> (this->get_gpr (inst.third_nibble) % DISPLAY_Y)
  };

  (*this->gpr).at (0xF) = 0;
  for (std::uint8_t row = 0; row < inst.fourth_nibble; row++)
    {
      std::uint8_t sprite_data = (*this->mem).at (this->idx_reg + row);
      std::vector<pos> positions{};
      for (int column = 0; column < 8; ++column)
        {
          if ((sprite_data & (1 << (7 - column))) != 0)
            {
              positions.emplace_back (starting_position.x + column,
                                      starting_position.y + row);
            }
        }
      if (this->get_display ()->flip_pixel (positions))
        {
          (*this->gpr).at (0xF) = 1;
        }
    }
}

constexpr void
chip8::inst_Fx33 (const instr &inst) noexcept
{
  std::uint8_t num = this->get_gpr (inst.second_nibble);
  (*this->mem).at (this->idx_reg) = static_cast<std::uint8_t> (num / 100);
  (*this->mem).at (this->idx_reg + 1)
      = static_cast<std::uint8_t> ((num % 100) / 10);
  (*this->mem).at (this->idx_reg + 2) = num % 10;
}

constexpr void
chip8::inst_Fx55 (const instr &inst) noexcept
{
  std::uint8_t size = inst.second_nibble;
  for (std::uint8_t idx = 0; idx <= size; idx++)
    {
      (*this->mem).at (this->idx_reg + idx) = this->get_gpr (idx);
    }
}

constexpr void
chip8::inst_Fx65 (const instr &inst) noexcept
{
  std::uint8_t size = inst.second_nibble;
  for (std::uint8_t idx = 0; idx <= size; idx++)
    {
      this->set_gpr (idx, (*this->mem).at (this->idx_reg + idx));
    }
}

constexpr void
chip8::inst_Fx07 (const instr &inst) noexcept
{
  this->set_gpr (inst.second_nibble, this->delay_timer);
}

constexpr void
chip8::inst_Fx18 (const instr &inst) noexcept
{
  this->sound_timer = this->get_gpr (inst.second_nibble);
}

constexpr void
chip8::inst_Fx1E (const instr &inst) noexcept
{
  this->set_idx_reg (this->idx_reg + this->get_gpr (inst.second_nibble));
}

void
chip8::execute (const instr &inst) noexcept
{
  switch (inst.first_nibble)
    {
    case 0x0:
      if (inst.fourth_nibble == 0xE)
        {
          this->inst_00EE ();
        }
      else
        {
          this->inst_00E0 ();
        }
      break;
    case 0x1:
      this->inst_1nnn (inst);
      break;
    case 0x2:
      this->inst_2nnn (inst);
      break;
    case 0x3:
      this->inst_3xkk (inst);
      break;
    case 0x4:
      this->inst_4xkk (inst);
      break;
    case 0x5:
      this->inst_5xy0 (inst);
      break;
    case 0x6:
      this->inst_6xkk (inst);
      break;
    case 0x7:
      this->inst_7xkk (inst);
      break;
    case 0x8:
      switch (inst.fourth_nibble)
        {
        case 0x0:
          this->inst_8xy0 (inst);
          break;
        case 0x1:
          this->inst_8xy1 (inst);
          break;
        case 0x2:
          this->inst_8xy2 (inst);
          break;
        case 0x3:
          this->inst_8xy3 (inst);
          break;
        case 0x4:
          this->inst_8xy4 (inst);
          break;
        case 0x5:
          this->inst_8xy5 (inst);
          break;
        case 0x6:
          this->inst_8xy6 (inst);
          break;
        case 0x7:
          this->inst_8xy7 (inst);
          break;
        case 0xE:
          this->inst_8xyE (inst);
          break;
        }
      break;
    case 0x9:
      this->inst_9xy0 (inst);
      break;
    case 0xA:
      this->inst_Annn (inst);
      break;
    case 0xB:
      this->inst_Bnnn (inst);
      break;
    case 0xC:
      this->inst_Cxkk (inst);
      break;
    case 0xD:
      this->inst_Dxyn (inst);
      break;
    case 0xF:
      switch (inst.fourth_nibble)
        {
        case 0x3:
          this->inst_Fx33 (inst);
          break;
        case 0x5:
          if (inst.third_nibble == 0x5)
            {
              this->inst_Fx55 (inst);
            }
          else
            {
              this->inst_Fx65 (inst);
            }
          break;
        case 0x7:
          this->inst_Fx07 (inst);
          break;
        case 0x8:
          this->inst_Fx18 (inst);
          break;
        case 0xE:
          this->inst_Fx1E (inst);
        }
    }
}
