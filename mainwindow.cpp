#include "mainwindow.h"
#include <QDrag>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isFirstNode = false;
}

void MainWindow::mousePressEvent(QMouseEvent *ev) {
    mousePosWhenPressed = ev->pos();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::RightButton && ev->pos() == mousePosWhenPressed) {
        QPointF coordN = ev->pos();
        Node n(coordN);

        if (graph.nodeAtCoord(ev->pos(), blockedRadius))
            return;

        n.setInfo(graph.getNumberOfNodes() + 1);
        graph.addNode(n);
        update();
    } else if (ev->button() == Qt::RightButton)  {
        int nodeNum = graph.nodeAtCoord(mousePosWhenPressed, blockedRadius);
        graph.moveNode(nodeNum, ev->pos());
        isFirstNode = false;
        update();
    } else if (ev->button() == Qt::LeftButton) {
        vector<Node> nodes = graph.getNodes();
        Node selected;
        bool ok = false;
        for (Node &n : nodes) {
            float dist = Node::getDistance(n, Node(ev->pos()));

            if (dist <= nodeRadius) {
                selected = n;
                ok = true;
                break;
            }
        }

        if (!ok) {
            isFirstNode = false;
            return;
        }

        if (!isFirstNode) {
            firstNode = selected;
            isFirstNode = true;
        } else {
            if (firstNode == selected)
                return;

            graph.addEdge(Edge(firstNode, selected));
            if (!graph.isOriented())
                graph.addEdge(Edge(selected, firstNode));

            isFirstNode = false;
            update();
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    vector<Node> nodes = graph.getNodes();
    for (Node &n : nodes) {
        QPointF coord = n.getCoordinate();
        QRect r(coord.x() - nodeRadius, coord.y() - nodeRadius, 2 * nodeRadius, 2 * nodeRadius);
        QPen pen;
        pen.setColor(Qt::red);
        p.setPen(pen);
        p.drawEllipse(r);
        QString num = QString::number(n.getInfo());
        p.drawText(r, Qt::AlignCenter, num);
    }
    vector<Edge> edges = graph.getEdges();
    int arrowSize = 15;

    for (Edge &e : edges) {
        QLineF line(e.getFirstNode().getCoordinate(), e.getSecondNode().getCoordinate());
        p.drawLine(line);

        if (graph.isOriented()) {
            double angle = atan2(line.dy(), -line.dx());
            QPointF arrowP1 = line.p2()
                              + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                        cos(angle + M_PI / 3) * arrowSize);
            QPointF arrowP2 = line.p2()
                              + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                        cos(angle + M_PI - M_PI / 3) * arrowSize);

            p.drawLine(line.p2(), arrowP1);
            p.drawLine(line.p2(), arrowP2);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
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
