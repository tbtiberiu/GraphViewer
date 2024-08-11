#include "mainwindow.h"
#include <QDrag>
#include <QInputDialog>
#include <QPainterPath>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupConnections();

    bool ok;
    n = QInputDialog::getInt(this, tr("GraphViewer"), tr("Number of nodes:"), 5, 0, 40, 1, &ok);
    if (!ok)
        return;
    nPaths = QInputDialog::getInt(this, tr("GraphViewer"), tr("Number of paths:"), 1, 0, 20, 1, &ok);
    if (!ok)
        return;
    nCycles = QInputDialog::getInt(this, tr("GraphViewer"), tr("Number of cycles:"), 1, 0, 20, 1, &ok);
    if (!ok)
        return;

    graph.generateRandomNodes(n);
    graph.generateElementaryCycles(nPaths);
    graph.generateElementaryCycles(nCycles);
    graph.saveChangesToFile();

    n = 0;
    nPaths = 0;
    nCycles = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    selectedNode = graph.getNodeAt(ev->position());
    pressedButton = ev->button();
    mouseMoved = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if (pressedButton == Qt::RightButton && selectedNode != nullptr
        && graph.getNodeAt(ev->position()) == nullptr) {
        graph.moveNode(selectedNode, ev->position());
    }
    mouseMoved = true;
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    QPointF currentPosition = ev->position();
    if (ev->button() == Qt::RightButton && selectedNode == nullptr && !mouseMoved) {
        Node *n = new Node(currentPosition, graph.getNodesCount() + 1);
        graph.addNode(n);
    } else if (ev->button() == Qt::LeftButton) {
        if (selectedNode == nullptr)
            return;

        if (firstSelectedNode == nullptr) {
            firstSelectedNode = selectedNode;
            return;
        }

        if (firstSelectedNode == selectedNode)
            return;

        graph.addEdge(Edge(firstSelectedNode, selectedNode));
        if (!graph.isOriented())
            graph.addEdge(Edge(selectedNode, firstSelectedNode));
        firstSelectedNode = nullptr;
    }

    selectedNode = nullptr;
    update();
}

void MainWindow::paintNodes(QPainter &p)
{
    std::vector<Node *> nodes = graph.getNodes();
    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(2);
    p.setPen(pen);

    for (const Node *n : nodes) {
        QPointF coord = n->getCoordinate();
        p.setBrush(Qt::black);
        QRect r(coord.x() - Node::radius,
                coord.y() - Node::radius,
                2 * Node::radius,
                2 * Node::radius);
        p.drawEllipse(r);
        QString num = QString::number(n->getInfo());
        p.drawText(r, Qt::AlignCenter, num);
    }
}

void MainWindow::paintEdges(QPainter &p)
{
    std::vector<Edge> edges = graph.getEdges();

    for (const Edge &e : edges) {
        QPen pen;
        pen.setColor(e.getFirstNode()->getColor());
        pen.setWidth(2);
        p.setPen(pen);

        QLineF line(e.getFirstNode()->getCoordinate(), e.getSecondNode()->getCoordinate());
        line.setLength(line.length() - Node::radius - 3);
        line.setP1(line.p1() + e.getSecondNode()->getCoordinate() - line.p2());
        p.drawLine(line);

        if (graph.isOriented()) {
            double angle = atan2(line.dy(), -line.dx());
            QPointF arrowP1 = line.p2()
                              + QPointF(sin(angle + M_PI / 3) * Edge::arrowSize,
                                        cos(angle + M_PI / 3) * Edge::arrowSize);
            QPointF arrowP2 = line.p2()
                              + QPointF(sin(angle + M_PI - M_PI / 3) * Edge::arrowSize,
                                        cos(angle + M_PI - M_PI / 3) * Edge::arrowSize);

            p.drawLine(line.p2(), arrowP1);
            p.drawLine(line.p2(), arrowP2);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paintEdges(p);
    paintNodes(p);
}

void MainWindow::handleOrientedButtonClicked()
{
    graph.setOrientation(true);
    update();
}

void MainWindow::handleNonOrientedButtonClicked()
{
    graph.setOrientation(false);
    update();
}

void MainWindow::handleNodesSpinValueChanged(int val)
{
    n = val;
}


void MainWindow::handlePathsSpinValueChanged(int val)
{
    nPaths = val;
}


void MainWindow::handleCyclesSpinValueChanged(int val)
{
    nCycles = val;
}


void MainWindow::handleNodesButtonClicked()
{
    graph.generateRandomNodes(n);
    update();
}


void MainWindow::handlePathsButtonClicked()
{
    graph.generateElementaryPaths(nPaths);
    update();
}


void MainWindow::handleCyclesButtonClicked()
{
    graph.generateElementaryCycles(nCycles);
    update();
}

void MainWindow::handleSaveButtonClicked()
{
    graph.saveChangesToFile();
}

void MainWindow::setupConnections()
{
    connect(ui->orientedButton, &QPushButton::clicked, this, &MainWindow::handleOrientedButtonClicked);
    connect(ui->nonorientedButton, &QPushButton::clicked, this, &MainWindow::handleNonOrientedButtonClicked);
    connect(ui->nodesSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::handleNodesSpinValueChanged);
    connect(ui->pathsSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::handlePathsSpinValueChanged);
    connect(ui->cyclesSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::handleCyclesSpinValueChanged);
    connect(ui->nodesButton, &QPushButton::clicked, this, &MainWindow::handleNodesButtonClicked);
    connect(ui->pathsButton, &QPushButton::clicked, this, &MainWindow::handlePathsButtonClicked);
    connect(ui->cyclesButton, &QPushButton::clicked, this, &MainWindow::handleCyclesButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::handleSaveButtonClicked);
}
