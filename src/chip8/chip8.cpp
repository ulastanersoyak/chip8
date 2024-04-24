#include "chip8.hpp"
#include "config.hpp"
#include "default_font.hpp"
#include "display.hpp"
#include "pixel_pos.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

chip8::chip8() noexcept
    : mem{std::make_unique<std::array<std::uint8_t, MEM_SIZE>>()},
      disp{std::make_unique<display>()},
      gpr{std::make_unique<std::array<std::uint8_t, REGISTER_COUNT>>()},
      instr_ptr{ROM_OFFSET}, idx_reg{0},
      stack{std::make_unique<std::vector<std::uint16_t>>()}, delay_timer{0},
      sound_timer{0} {
  for (std::size_t i = 0; i < default_font.size(); i++) {
    std::copy_n(default_font.at(i).begin(), default_font.at(i).size(),
                mem->begin() + FONT_MEMORY_OFFSET +
                    (i * default_font.at(i).size()));
  }
}

[[nodiscard]] bool chip8::read_rom([[maybe_unused]] std::string_view rom_name) {
  std::string rom_file = "../../roms/" + std::string{rom_name} + ".ch8";
  std::filesystem::path rom_path = std::filesystem::current_path() / rom_file;
  std::ifstream file(rom_path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    std::cout << "failed to open the file: " << rom_name << ".ch8\n";
    return false;
  }
  file.seekg(0, std::ios_base::end);
  auto length = file.tellg();
  file.seekg(0, std::ios_base::beg);
  std::vector<std::uint8_t> buffer(static_cast<std::size_t>(length));
  file.read(reinterpret_cast<char *>(buffer.data()), length);
  std::copy(buffer.begin(), buffer.end(), this->mem->begin() + ROM_OFFSET);
  return true;
}

[[nodiscard]] instr chip8::fetch() noexcept {
  auto raw_inst = static_cast<std::uint16_t>(
      (*this->mem).at(instr_ptr + 1) |
      static_cast<std::uint16_t>((*this->mem).at(instr_ptr) << 8));
  instr decoded_inst = chip8::decode(raw_inst);
  instr_ptr += 2;
  return decoded_inst;
}

[[nodiscard]] instr chip8::decode(uint16_t opcode) noexcept {
  const auto first_nibble = static_cast<std::uint8_t>((opcode >> 12) & 0x0F);
  const auto second_nibble = static_cast<std::uint8_t>((opcode >> 8) & 0x0F);
  const auto third_nibble = static_cast<std::uint8_t>((opcode >> 4) & 0x00F);
  const auto fourth_nibble = static_cast<std::uint8_t>((opcode) & 0x000F);
  return instr{.first_nibble = first_nibble,
               .second_nibble = second_nibble,
               .third_nibble = third_nibble,
               .fourth_nibble = fourth_nibble,
               .second_byte = static_cast<std::uint8_t>(
                   ((third_nibble << 4) | fourth_nibble)),
               .except_first_nibble = static_cast<uint16_t>(
                   (second_nibble << 8) | (third_nibble << 4) | fourth_nibble)};
}

[[nodiscard]] display *chip8::get_display() const noexcept {
  return this->disp.get();
}

[[nodiscard]] std::uint8_t chip8::get_gpr(std::uint8_t idx) const noexcept {
  return (*this->gpr).at(idx);
}

void chip8::jump(std::uint16_t addr) noexcept { this->instr_ptr = addr; }

void chip8::call_subroutine(std::uint16_t addr) noexcept {
  (*this->stack).push_back(this->instr_ptr);
  this->instr_ptr = addr;
}

void chip8::ret_subroutine() noexcept {
  this->instr_ptr = (*this->stack).back();
  (*this->stack).pop_back();
}

void chip8::set_gpr(std::uint8_t reg_idx, std::uint8_t val) noexcept {
  (*this->gpr).at(reg_idx) = val;
}

void chip8::add_to_gpr(std::uint8_t reg_idx, std::uint8_t val) noexcept {
  (*this->gpr).at(reg_idx) += val;
}

void chip8::set_idx_reg(std::uint16_t val) noexcept { this->idx_reg = val; }

void chip8::draw(const pos &starting_position, std::uint8_t size) noexcept {
  (*this->gpr).at(0xF) = 0;
  for (std::uint8_t row = 0; row < size; row++) {
    std::uint8_t sprite_data = (*this->mem).at(this->idx_reg + row);
    std::vector<pos> positions{};
    for (int column = 0; column < 8; ++column) {
      if ((sprite_data & (1 << (7 - column))) != 0) {
        positions.emplace_back(starting_position.x + column,
                               starting_position.y + row);
      }
    }
    if (this->get_display()->flip_pixel(positions)) {
      (*this->gpr).at(0xF) = 1;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void chip8::execute(const instr &inst) {
  switch (inst.first_nibble) {
  case 0x0:
    if (inst.fourth_nibble == 0xE) {
      this->ret_subroutine();
    } else {
      this->get_display()->clear_window();
    }
    break;

  case 0x1:
    this->jump(inst.except_first_nibble);
    break;

  case 0x2:
    this->call_subroutine(inst.except_first_nibble);
    break;

  case 0x3:
    if ((*this->gpr).at(inst.second_nibble) == inst.second_byte) {
      this->instr_ptr += 2;
    }
    break;

  case 0x4:
    if ((*this->gpr).at(inst.second_nibble) != inst.second_byte) {
      this->instr_ptr += 2;
    }
    break;

  case 0x5:
    if ((*this->gpr).at(inst.second_nibble) ==
        (*this->gpr).at(inst.third_nibble)) {
      this->instr_ptr += 2;
    }
    break;

  case 0x6:
    this->set_gpr(inst.second_nibble, inst.second_byte);
    break;

  case 0x7:
    this->add_to_gpr(inst.second_nibble, inst.second_byte);
    break;

  case 0x8:
    switch (inst.fourth_nibble) {
    case 0x1:
      break;
    case 0x2:
      break;
    case 0x3:
      break;
    case 0x4:
      break;
    }
    break;

  case 0x9:
    if ((*this->gpr).at(inst.second_nibble) !=
        (*this->gpr).at(inst.third_nibble)) {
      this->instr_ptr += 2;
    }
    break;

  case 0xA:
    this->set_idx_reg(inst.except_first_nibble);
    break;

  case 0xD:
    pos initial_pos = {.x = static_cast<uint16_t>(
                           this->get_gpr(inst.second_nibble) % DISPLAY_X),
                       .y = static_cast<uint16_t>(
                           this->get_gpr(inst.third_nibble) % DISPLAY_Y)};
    this->draw(initial_pos, inst.fourth_nibble);
    break;
  }
}
