#include "include/cpu.h"

#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept>

#include <fmt/format.h>

void Cpu::set_clock_speed(uint64_t speed) {
    clock_speed = speed;
}

void Cpu::execute() {
    try {
        Opcode op = ram->at(registers.pc+1) | (ram->at(registers.pc) << 8);
        uint8_t prefix = op >> 12;
        (this->*opHandlers.at(prefix))(op);
        if (registers.pc % 2 != 0) {
            throw std::runtime_error{fmt::format("pc is odd: {}", registers.pc)};
        }
    }  catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

bool Cpu::decrement_delay() {
    if (registers.dt > 0) {
        registers.dt--;
    }
    return registers.dt;
}

bool Cpu::decrement_sound() {
    if (registers.st > 0) {
        registers.st--;
    }
    return registers.st;
}

bool Cpu::get_sound() {
    return registers.st;
}

bool Cpu::get_delay() {
    return registers.dt;
}

void Cpu::op_zero(Opcode op) {
    if (op == 0x00E0) { // CLS - clear display
        display->clear_screen();
        registers.pc += 2;
    } else if (op == 0x00EE) { // RET
        registers.pc = stack->at(registers.sp);
        registers.sp--;
    }
}

void Cpu::op_jp(Opcode op) {
    registers.pc = op & 0xFFF;
}

void Cpu::op_call(Opcode op) {
    registers.sp++;
    stack->at(registers.sp) = registers.pc;
    registers.pc = op & 0xFFF;
}

void Cpu::op_se_imm(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    auto operand = op & 0xFF;
    if (registers.general.at(reg) == operand) {
        registers.pc += 4;
    } else {
        registers.pc += 2;
    }
}

void Cpu::op_sne_imm(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    auto operand = op & 0xFF;
    if (registers.general.at(reg) != operand) {
        registers.pc += 4;
    } else {
        registers.pc += 2;
    }
}

void Cpu::op_se_reg(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto y = (op >> 4) & 0x0F;
    if (registers.general.at(x) == registers.general.at(y)) {
        registers.pc += 4;
    } else {
        registers.pc += 2;
    }
}

void Cpu::op_ld_imm(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    registers.general.at(reg) = op & 0xFF;
    registers.pc += 2;
}

void Cpu::op_add_imm(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    registers.general.at(reg) += (op & 0xFF);
    registers.pc += 2;
}

void Cpu::op_eight(Opcode op) {
    auto subcode = op & 0x0F;
    switch (subcode) {
    case 0:
        op_ld_reg(op);
        break;
    case 1:
        op_or_reg(op);
        break;
    case 2:
        op_and_reg(op);
        break;
    case 3:
        op_xor_reg(op);
        break;
    case 4:
        op_add_reg(op);
        break;
    case 5:
        op_sub_reg(op);
        break;
    case 6:
        op_shr(op);
        break;
    case 7:
        op_subn(op);
        break;
    case 0xE:
        op_shl(op);
        break;
    default:
        throw std::runtime_error{fmt::format("invalid 8 subcode: {}", subcode)};
    }
}

void Cpu::op_ld_reg(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto y = (op >> 4) & 0x0F;
    registers.general.at(x) = registers.general.at(y);
    registers.pc += 2;
};

void Cpu::op_or_reg(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto x_val = registers.general.at(x);
    auto y = (op >> 4) & 0x0F;
    auto y_val = registers.general.at(y);

    registers.general.at(x) = x_val | y_val;
    registers.pc += 2;
}

void Cpu::op_and_reg(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto x_val = registers.general.at(x);
    auto y = (op >> 4) & 0x0F;
    auto y_val = registers.general.at(y);
    registers.general.at(x) = x_val & y_val;
    registers.pc += 2;
}

void Cpu::op_xor_reg(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto x_val = registers.general.at(x);
    auto y = (op >> 4) & 0x0F;
    auto y_val = registers.general.at(y);
    registers.general.at(x) = x_val ^ y_val;
    registers.pc += 2;
}

void Cpu::op_add_reg(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto x_val = registers.general.at(x);
    auto y = (op >> 4) & 0x0F;
    auto y_val = registers.general.at(y);
    if (x_val + y_val > 255) {
        registers.general.at(0xF) = 1;
    } else {
        registers.general.at(0xF) = 0;
    }
    registers.general.at(x) = (x_val + y_val) & 0x0F;
    registers.pc += 2;
}

void Cpu::op_sub_reg(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto x_val = registers.general.at(x);
    auto y = (op >> 4) & 0x0F;
    auto y_val = registers.general.at(y);
    if (x_val > y_val) {
        registers.general.at(0xF) = 1;
    } else {
        registers.general.at(0xF) = 0;
    }
    registers.general.at(x) = x_val - y_val;
    registers.pc += 2;
}

void Cpu::op_shr(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto x_val = registers.general.at(x);
    if ((x_val & 1) == 1) {
        registers.general.at(0xF) = 1;
    } else {
        registers.general.at(0xF) = 0;
    }
    registers.general.at(x) = x_val / 2;
    registers.pc += 2;
}

void Cpu::op_subn(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto x_val = registers.general.at(x);
    auto y = (op >> 4) & 0x0F;
    auto y_val = registers.general.at(y);
    if (y_val > x_val) {
        registers.general.at(0xF) = 1;
    } else {
        registers.general.at(0xF) = 0;
    }
    registers.general.at(x) = y_val - x_val;
    registers.pc += 2;
}

void Cpu::op_shl(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto x_val = registers.general.at(x);
    if ((x_val >> 7) == 1) {
        registers.general.at(0xF) = 1;
    } else {
        registers.general.at(0xF) = 0;
    }
    registers.general.at(x) = x_val * 2;
    registers.pc += 2;
}

void Cpu::op_sne_reg(Opcode op) {
    auto x = (op >> 8) & 0x0F;
    auto y = (op >> 4) & 0x0F;
    if (registers.general.at(x) != registers.general.at(y)) {
        registers.pc += 4;
    } else {
        registers.pc += 2;
    }
}

void Cpu::op_ld_i(Opcode op) {
    registers.i = op & 0x0FFF;
    registers.pc += 2;
}

void Cpu::op_jp_rel(Opcode op) {
    registers.pc = (op & 0x0FFF) + registers.general.at(0);
}

void Cpu::op_rnd(Opcode op) {
    auto reg = op >> 12;
    std::uniform_int_distribution<uint8_t> uint8_dist10(0,255);
    uint8_t rnd = uint8_dist10(rng);
    registers.general.at(reg) = rnd & (op & 0xFF);
    registers.pc += 2;
}

void Cpu::op_drw(Opcode op) {
    auto x = registers.general.at((op >> 8) & 0x0F);
    auto y = registers.general.at((op >> 4) & 0x0F);
    auto sprite = &ram->at(registers.i);
    uint8_t size = op & 0x0F;
    if (display->draw(x, y, sprite, size)) {
        registers.general.at(0xF) = 1; // collision
    } else {
        registers.general.at(0xF) = 0;
    }
    registers.pc += 2;
}

void Cpu::op_skp_sknp(Opcode op) {
    uint8_t key = (op >> 8) & 0x0F;
    uint8_t subcode = op & 0xFF;
    if (subcode == 0x9E) {
        if (keyboard->is_key_down(key)) {
            registers.pc += 4;
            return;
        }
    } else if (subcode == 0xA1) {
        if (!keyboard->is_key_down(key)) {
            registers.pc += 4;
            return;
        }
    } else {
        throw std::runtime_error{ fmt::format("invalid skp or sknp subcode: {}", subcode) };
    }
    registers.pc += 2;
}

void Cpu::op_f(Opcode op) {
    auto subcode = op & 0xFF;
    switch (subcode) {
    case 0x07:
        op_ld_from_dt(op);
        break;
    case 0x0A:
        op_wait_key(op);
        break;
    case 0x15:
        op_ld_to_dt(op);
        break;
    case 0x18:
        op_ld_to_st(op);
        break;
    case 0x1E:
        op_add_i(op);
        break;
    case 0x29:
        op_set_i(op);
        break;
    case 0x33:
        op_store_bcd(op);
        break;
    case 0x55:
        op_store_regs(op);
        break;
    case 0x65:
        op_load_regs(op);
        break;
    default:
        throw std::runtime_error{fmt::format("invalid subcode for f opcode: ", subcode)};
    }
}

void Cpu::op_ld_from_dt(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    registers.general.at(reg) = registers.dt;
    registers.pc += 2;
}

void Cpu::op_wait_key(Opcode op) {
    uint8_t reg = (op >> 8) & 0x0F;
    auto key = keyboard->is_key_down();
    if (key == 0xFF) {
        return; // Stop advancing program counter if a key is not down
    }
    registers.general.at(reg) = key;
    registers.pc += 2;
}

void Cpu::op_ld_to_dt(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    registers.dt = registers.general.at(reg);
    registers.pc += 2;
}

void Cpu::op_ld_to_st(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    registers.st = registers.general.at(reg);
    registers.pc += 2;
}

void Cpu::op_add_i(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    registers.i += registers.general.at(reg);
    registers.pc += 2;
}

void Cpu::op_set_i(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    registers.i = registers.general.at(reg);
    registers.pc += 2;
}

void Cpu::op_store_bcd(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    auto value = registers.general.at(reg);
    auto dest = registers.i;
    ram->at(dest) = value / 100;
    ram->at(dest+1) = (value % 100) / 10;
    ram->at(dest+2) = value % 10;
    registers.pc += 2;
}

void Cpu::op_store_regs(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    auto dest = registers.i;
    for (uint8_t i{}; i <= reg; i++) {
        ram->at(dest) = registers.general.at(i);
        dest++;
    }
    registers.pc += 2;
}

void Cpu::op_load_regs(Opcode op) {
    auto reg = (op >> 8) & 0x0F;
    auto source = registers.i;
    for (uint8_t i{}; i <= reg; i++) {
        registers.general.at(i) = ram->at(source);
        source++;
    }
    registers.pc += 2;
}
