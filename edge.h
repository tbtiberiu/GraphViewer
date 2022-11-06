#pragma once
#include "node.h"

class Edge
{
public:
    Edge();
    Edge(Node firstNode, Node secondNode);
    Node getFirstNode() const;
    Node getSecondNode() const;
    void updateFirstNode(QPointF coord);
    void updateSecondNode(QPointF coord);
    friend bool operator==(const Edge &edgeA, const Edge &edgeB);

private:
    Node m_firstNode, m_secondNode;
};
