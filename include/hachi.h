#ifndef HACHI_H
#define HACHI_H

#include "chip8.h"
#include "display.h"
#include "mainwindow.h"

#include <array>
#include <cstdint>
#include <map>
#include <string>

#include <QKeyEvent>
#include <QObject>
#include <QTimer>


class Hachi : public QObject {
    Q_OBJECT

    MainWindow* parent{};
    std::unique_ptr<Chip8> chip8{};
    const std::string filename;
    uint64_t clock_speed{};
    QTimer* timer{};
    std::map<int, uint8_t> keymap{{}};
    bool shutdown_requested{ false };
    bool running{ false };

    void map_keys();

private slots:
    void execute();

public slots:
    void key_down(QKeyEvent* e);
    void key_up(QKeyEvent* e);
    void shutdown();

signals:
    void refresh_screen(const PixelBuffer& pixel_buffer);
    void beep();
    void stop_beep();

public:
    Hachi(MainWindow* parent = nullptr);
    Hachi(MainWindow* parent, const std::string& filename);
    void load_rom(const std::string& filename);
    bool is_running();
};

#endif // HACHI_H
