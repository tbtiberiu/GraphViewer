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
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *p);
    void paintNodes(QPainter &p);
    void paintEdges(QPainter &p);
    ~MainWindow();

private slots:
    void handleOrientedButtonClicked();
    void handleNonOrientedButtonClicked();

    void handleNodesSpinValueChanged(int val);
    void handlePathsSpinValueChanged(int val);
    void handleCyclesSpinValueChanged(int val);

    void handleNodesButtonClicked();
    void handlePathsButtonClicked();
    void handleCyclesButtonClicked();

    void handleSaveButtonClicked();

private:
    void setupConnections();

private:
    Graph graph;
    Ui::MainWindow *ui;

    Qt::MouseButton pressedButton;
    Node *firstSelectedNode = nullptr;
    Node *selectedNode = nullptr;
    bool mouseMoved = false;

    int n;
    int nPaths;
    int nCycles;
};
