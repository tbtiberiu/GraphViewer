#pragma once
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "ui_mainwindow.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *p);
    ~MainWindow();
    const int nodeRadius = 10;
    const int blockedRadius = nodeRadius + 10;

private slots:
    void on_orientatButton_clicked();
    void on_neorientatButton_clicked();

private:
    Graph graph;
    Ui::MainWindow *ui;

    bool isFirstNode;
    Node firstNode;
    QPointF mousePosWhenPressed = {0, 0};
};
