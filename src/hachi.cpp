#include "include/hachi.h"
#include "include/mainwindow.h"

#include <string>
#include <iostream>
#include <map>

#include <QTimer>
#include <QWidget>
#include <QMainWindow>
#include <QCoreApplication>

Hachi::Hachi(MainWindow* parent) : parent{ parent } {
    chip8 = std::make_unique<Chip8>();
    clock_speed = chip8->get_clock_speed();
    map_keys();
    connect(parent, &MainWindow::keyPress, this, &Hachi::key_down);
    connect(parent, &MainWindow::keyRelease, this, &Hachi::key_up);
    connect(parent, &MainWindow::shutdown, this, &Hachi::shutdown);
}

Hachi::Hachi(MainWindow* parent, const std::string& filename) : parent{ parent }, filename{ filename } {
    chip8 = std::make_unique<Chip8>();
    clock_speed = chip8->get_clock_speed();
    map_keys();
    connect(parent, &MainWindow::keyPress, this, &Hachi::key_down);
    connect(parent, &MainWindow::keyRelease, this, &Hachi::key_up);
    connect(parent, &MainWindow::shutdown, this, &Hachi::shutdown);

    load_rom(filename);
}

void Hachi::load_rom(const std::string& filename) {
    chip8->load_default_sprites(0);
    chip8->load_rom(filename);
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, &QTimer::timeout, this, &Hachi::execute);
    running = true;
    timer->start(17);
}

void Hachi::map_keys() {
    keymap[Qt::Key_1] = 0x1;
    keymap[Qt::Key_2] = 0x2;
    keymap[Qt::Key_3] = 0x3;
    keymap[Qt::Key_4] = 0xC;
    keymap[Qt::Key_Q] = 0x4;
    keymap[Qt::Key_W] = 0x5;
    keymap[Qt::Key_E] = 0x6;
    keymap[Qt::Key_R] = 0xD;
    keymap[Qt::Key_A] = 0x7;
    keymap[Qt::Key_S] = 0x8;
    keymap[Qt::Key_D] = 0x9;
    keymap[Qt::Key_F] = 0xE;
    keymap[Qt::Key_Z] = 0xA;
    keymap[Qt::Key_X] = 0x0;
    keymap[Qt::Key_C] = 0xB;
    keymap[Qt::Key_V] = 0xF;
}

void Hachi::key_down(QKeyEvent* e) {
    auto key = e->key();
    if (keymap.find(key) != keymap.end()) {
        chip8->on_key_down(keymap[key]);
    }
}

void Hachi::key_up(QKeyEvent* e) {
    auto key = e->key();
    if (keymap.find(key) != keymap.end()) {
        chip8->on_key_up(keymap[key]);
    }
}

void Hachi::execute() {
    if (chip8->decrement_sound()) {
        emit beep();
    } else {
        emit stop_beep();
    }
    chip8->decrement_delay();
    emit refresh_screen(chip8->get_display()->get_pixel_buffer());

    for (size_t i{}; i < clock_speed / 60; i++) {
        QCoreApplication::processEvents();
        if (shutdown_requested) {
            shutdown_requested = false;
            break;
        }
        chip8->execute_cycle();
    }
}

void Hachi::shutdown() {
    shutdown_requested = true;
    if (timer) {
        timer->stop();
    }
    chip8->reboot();
    running = false;
}

bool Hachi::is_running() {
    return running;
}
