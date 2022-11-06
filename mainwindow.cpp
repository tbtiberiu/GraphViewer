#include "mainwindow.h"
#include <QDrag>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    selectedNode = graph.getNodeAt(ev->pos());
    pressedButton = ev->button();
    mouseMoved = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if (pressedButton == Qt::RightButton) {
        if (selectedNode != nullptr && graph.getNodeAt(ev->pos()) == nullptr) {
            graph.moveNode(selectedNode, ev->pos());
        }
    }
    mouseMoved = true;
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    QPointF currentPosition = ev->pos();
    if (ev->button() == Qt::RightButton && selectedNode == nullptr && !mouseMoved) {
        Node* n = new Node(currentPosition, graph.getNumberOfNodes() + 1);
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
    std::unordered_set<Node*> nodes = graph.getNodes();
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(3);
    p.setPen(pen);

    for (const Node* n : nodes) {
        QPointF coord = n->getCoordinate();
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
    QPen pen;
    pen.setColor(Qt::darkGreen);
    pen.setWidth(2);
    p.setPen(pen);

    for (const Edge &e : edges) {
        QLineF line(e.getFirstNode()->getCoordinate(), e.getSecondNode()->getCoordinate());
        line.setLength(line.length() - 10);
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
    paintNodes(p);
    paintEdges(p);
}

void MainWindow::on_orientatButton_clicked()
{
    graph.setOrientation(true);
    update();
}

void MainWindow::on_neorientatButton_clicked()
{
    graph.setOrientation(false);
    update();
}
