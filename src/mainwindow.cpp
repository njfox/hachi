#include "include/mainwindow.h"
#include "include/hachi.h"
#include "include/display.h"

#include <cstdint>

#include <QtWidgets>
#include <QtMultimedia/QMediaPlayer>
#include <QByteArray>

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(hachi);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    Chip8 chip8{};

    setup_menus();
    setup_sound();

    resize(QDesktopWidget().availableGeometry(this).size() * 0.68);
    move(screen()->geometry().center() - frameGeometry().center());


    chip8_display = new QImage(64, 32, QImage::Format_Mono);
    chip8_display->fill(0);
    canvas = new QPixmap(QPixmap::fromImage(chip8_display->scaled(canvas_width, canvas_height, Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    view->setScene(scene);
    setCentralWidget(view);
    scene->addPixmap(*canvas);
}

MainWindow::~MainWindow() {
    delete chip8_display;
    delete canvas;
    delete audioBuf;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    emit keyPress(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
    emit keyRelease(e);
}

void MainWindow::setup_menus() {
    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(new QAction("Open ROM"));
    fileMenu->addAction(new QAction("Close ROM"));

    auto emulationMenu = menuBar()->addMenu(tr("&Emulation"));
    emulationMenu->addAction(new QAction("Pause"));
    emulationMenu->addAction(new QAction("Resume"));
    emulationMenu->addAction(new QAction("Settings"));

    connect(fileMenu->actions().at(0), &QAction::triggered, this, &MainWindow::on_open_rom);
    connect(fileMenu->actions().at(1), &QAction::triggered, this, &MainWindow::on_close_rom);
}

void MainWindow::setup_sound() {
    auto fs = cmrc::hachi::get_filesystem();
    auto beep = fs.open("resources/beep.mp3");
    audioBuf = new QByteArray(beep.begin(), beep.size());


    mediaPlayer = new QMediaPlayer(this);
    QBuffer* mediaBuf = new QBuffer(audioBuf, mediaPlayer);
    mediaBuf->setData(*audioBuf);
    mediaBuf->open(QIODevice::ReadOnly);
    mediaPlayer->setMedia(QMediaContent(), mediaBuf);
}

void MainWindow::on_beep() {
    mediaPlayer->play();
}

void MainWindow::on_stop_beep() {
    mediaPlayer->stop();
}

void MainWindow::on_refresh_screen(const PixelBuffer* pixel_buffer) {
    for (int x{}; x < chip8_display->width(); x++) {
        for (int y{}; y < chip8_display->height(); y++) {
            chip8_display->setPixel(x, y, pixel_buffer->at(x).at(y));
        }
    }
    QGraphicsItem* item = scene->items().at(0);
    scene->removeItem(item);
    delete item;
    delete canvas;
    canvas = new QPixmap(QPixmap::fromImage(chip8_display->scaled(canvas_width, canvas_height, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    scene->addPixmap(*canvas);
}

void MainWindow::on_open_rom() {
    if (hachi) {
        on_close_rom();
    }
    auto filename = QFileDialog::getOpenFileName(this, tr("Open ROM"), QDir::homePath(), tr(".ch8"));
    hachi = new Hachi(this, filename.toStdString());

    connect(hachi, &Hachi::refresh_screen, this, &MainWindow::on_refresh_screen);
    connect(hachi, &Hachi::beep, this, &MainWindow::on_beep);
    connect(hachi, &Hachi::stop_beep, this, &MainWindow::on_stop_beep);
    connect(hachi, &Hachi::shutdown_complete, this, &MainWindow::shutdown_complete);
}

void MainWindow::on_close_rom() {
    emit shutdown();
    clear_screen();
}

void MainWindow::shutdown_complete() {
    if (hachi) {
        delete hachi;
    }
}

void MainWindow::clear_screen() {
    for (int x{}; x < chip8_display->width(); x++) {
        for (int y{}; y < chip8_display->height(); y++) {
            chip8_display->setPixel(x, y, 0);
        }
    }
    QGraphicsItem* item = scene->items().at(0);
    scene->removeItem(item);
    delete item;
    delete canvas;
    canvas = new QPixmap(QPixmap::fromImage(chip8_display->scaled(canvas_width, canvas_height, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    scene->addPixmap(*canvas);
}
