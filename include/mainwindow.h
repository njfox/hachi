#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chip8.h"
#include "hachi.h"
#include "display.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QKeyEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* e);

public slots:
    void on_open_rom();
    void on_close_rom();
    void on_beep();
    void on_refresh_screen(const PixelBuffer* pixel_buffer);

signals:
    void keyPress(QKeyEvent* e);

private:
    QGraphicsScene* scene{};
    QGraphicsView* view{};
    Hachi* hachi{};
    QPixmap* canvas{};
    QImage* chip8_display{};

    int canvas_width{1280};
    int canvas_height{640};

    void setup_menus();
    void clear_screen();
};
#endif // MAINWINDOW_H
