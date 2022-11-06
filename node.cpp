#include "node.h"

Node::Node(int info)
{
    m_info = info;
}

Node::Node(QPointF coord, int info)
{
    m_coord = coord;
    m_info = info;
}

QPointF Node::getCoordinate() const
{
    return m_coord;
}

int Node::getInfo() const
{
    return m_info;
}

void Node::setInfo(int info)
{
    m_info = info;
}

void Node::setCoord(QPointF coord) {
    m_coord = coord;
}

float Node::getDistance(const Node &nodeA, const Node &nodeB)
{
    qreal x = nodeB.m_coord.x() - nodeA.m_coord.x();
    qreal y = nodeB.m_coord.y() - nodeA.m_coord.y();
    return sqrt(pow(x, 2) + pow(y, 2));
}

bool operator==(const Node &nodeA, const Node &nodeB)
{
    return nodeA.m_info == nodeB.m_info;
}
