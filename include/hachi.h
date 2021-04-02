#ifndef HACHI_H
#define HACHI_H

#include "include/chip8.h"
#include "display.h"

#include <array>
#include <cstdint>
#include <string>

#include <QKeyEvent>
#include <QObject>
#include <QThread>


class Hachi : public QThread {
    Q_OBJECT

    Chip8 chip8{};
    const std::string filename;

protected:
    void keyPressEvent(QKeyEvent* e);

signals:
    void refresh_screen(const PixelBuffer* pixel_buffer);
    void beep();

public:
    Hachi(const std::string filename): filename{ filename }{};
    void run() override;
};

#endif // HACHI_H
