#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include "mainwindow.h"

#include <QImage>
#include <QPainter>
#include <QtWidgets>
#include <QOpenGLWidget>


class ScreenWidget : public QOpenGLWidget {
    Q_OBJECT

    MainWindow* parent;
    QImage image{};
    void paintEvent(QPaintEvent*) override;
public:
    ScreenWidget(MainWindow *parent);

public slots:
    void display(const QImage& img);
};

#endif // SCREENWIDGET_H
