#include "edge.h"

Edge::Edge()
{
    // empty
}

Edge::Edge(Node firstNode, Node secondNode)
{
    m_firstNode = firstNode;
    m_secondNode = secondNode;
}

Node Edge::getFirstNode() const
{
    return m_firstNode;
}

Node Edge::getSecondNode() const
{
    return m_secondNode;
}

void Edge::updateFirstNode(QPointF coord)
{
    m_firstNode.setCoord(coord);
}

void Edge::updateSecondNode(QPointF coord)
{
    m_secondNode.setCoord(coord);
}

bool operator==(const Edge &edgeA, const Edge &edgeB)
{
    return edgeA.m_firstNode == edgeB.m_firstNode && edgeA.m_secondNode == edgeB.m_secondNode;
}
