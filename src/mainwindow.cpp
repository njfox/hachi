#include "include/mainwindow.h"
#include "include/hachi.h"
#include "include/display.h"

#include <QtWidgets>
#include <cstdint>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    Chip8 chip8{};

    setup_menus();

    resize(QDesktopWidget().availableGeometry(this).size() * 0.68);
    move(screen()->geometry().center() - frameGeometry().center());


    chip8_display = new QImage(64, 32, QImage::Format_Mono);
    canvas = new QPixmap(QPixmap::fromImage(chip8_display->scaled(canvas_width, canvas_height, Qt::KeepAspectRatio, Qt::SmoothTransformation)));

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    view->setScene(scene);
    setCentralWidget(view);
    scene->addPixmap(*canvas);
}

MainWindow::~MainWindow() {
    delete scene;
    delete view;
    delete hachi;
    delete chip8_display;
    delete canvas;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (hachi) {
        QApplication::sendEvent(hachi, e);
    }
}

void MainWindow::setup_menus() {
    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(new QAction("Open ROM"));
    fileMenu->addAction(new QAction("Close ROM"));

    auto emulationMenu = menuBar()->addMenu(tr("&Emulation"));
    emulationMenu->addAction(new QAction("Pause"));
    emulationMenu->addAction(new QAction("Resume"));
    emulationMenu->addAction(new QAction("Settings"));

    connect(fileMenu->actions().at(0), SIGNAL(triggered()), this, SLOT(on_open_rom()));
    connect(fileMenu->actions().at(1), SIGNAL(triggered()), this, SLOT(on_close_rom()));
}

void MainWindow::on_beep() {

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
    hachi = new Hachi(filename.toStdString());

    connect(hachi, &Hachi::refresh_screen, this, &MainWindow::on_refresh_screen);
    connect(hachi, &Hachi::beep, this, &MainWindow::on_beep);
    hachi->start();
}

void MainWindow::on_close_rom() {
    if (hachi) {
        hachi->requestInterruption();
        hachi->wait();
    }
    clear_screen();
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
