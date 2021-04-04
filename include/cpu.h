#ifndef CPU_H
#define CPU_H

#include "include/display.h"
#include "include/keyboard.h"

#include <array>
#include <cstdint>
#include <random>

using std::array;

struct Registers {
    array<uint8_t, 16> general{{}};
    uint8_t sp{};
    uint8_t dt{};
    uint8_t st{};
    uint16_t i{};
    uint16_t pc{ 0x200 };
};

class Cpu {
    uint64_t clock_speed;
    Display* display;
    Keyboard* keyboard;
    array<uint8_t, 0x1000>* ram;
    array<uint16_t, 16>* stack;
    Registers registers{};

    using Opcode = uint16_t;
    using OpHandler = void(Cpu::*)(Opcode);
    const array<OpHandler, 16> opHandlers {
        &Cpu::op_zero,
        &Cpu::op_jp,
        &Cpu::op_call,
        &Cpu::op_se_imm,
        &Cpu::op_sne_imm,
        &Cpu::op_se_reg,
        &Cpu::op_ld_imm,
        &Cpu::op_add_imm,
        &Cpu::op_eight,
        &Cpu::op_sne_reg,
        &Cpu::op_ld_i,
        &Cpu::op_jp_rel,
        &Cpu::op_rnd,
        &Cpu::op_drw,
        &Cpu::op_skp_sknp,
        &Cpu::op_f
    };

    std::mt19937 rng{};

    void op_zero(Opcode op);
    void op_jp(Opcode op);
    void op_call(Opcode op);
    void op_se_imm(Opcode op);
    void op_sne_imm(Opcode op);
    void op_se_reg(Opcode op);
    void op_ld_imm(Opcode op);
    void op_add_imm(Opcode op);
    void op_eight(Opcode op);
    // Begin 8nnn opcodes
    void op_ld_reg(Opcode op);
    void op_or_reg(Opcode op);
    void op_and_reg(Opcode op);
    void op_xor_reg(Opcode op);
    void op_add_reg(Opcode op);
    void op_sub_reg(Opcode op);
    void op_shr(Opcode op);
    void op_subn(Opcode op);
    void op_shl(Opcode op);
    // End 8nnn opcodes
    void op_sne_reg(Opcode op);
    void op_ld_i(Opcode op);
    void op_jp_rel(Opcode op);
    void op_rnd(Opcode op);
    void op_drw(Opcode op);
    void op_skp_sknp(Opcode op);
    void op_f(Opcode op);
    // Begin fnxx opcodes
    void op_ld_from_dt(Opcode op);
    void op_wait_key(Opcode op);
    void op_ld_to_dt(Opcode op);
    void op_ld_to_st(Opcode op);
    void op_add_i(Opcode op);
    void op_set_i(Opcode op);
    void op_store_bcd(Opcode op);
    void op_store_regs(Opcode op);
    void op_load_regs(Opcode op);
public:
    Cpu(uint64_t clock_speed, Display* display, Keyboard* keyboard, array<uint8_t, 0x1000>* ram, array<uint16_t, 16>* stack) :
        clock_speed{ clock_speed },
        display{ display },
        keyboard{ keyboard },
        ram{ ram },
        stack{ stack } {
            rng.seed();
    };
    void execute();
    void set_clock_speed(uint64_t speed);
    bool decrement_sound();
    bool decrement_delay();
    bool get_sound();
    bool get_delay();
};

#endif // CPU_H
