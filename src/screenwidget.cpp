#include "include/screenwidget.h"


ScreenWidget::ScreenWidget(MainWindow *parent) : parent{ parent } {
    connect(parent, &MainWindow::drawScreen, this, &ScreenWidget::display);
}

void ScreenWidget::display(const QImage& img) {
    image = img;
    this->update();
}

void ScreenWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawImage(this->rect(), image);
}
