#pragma once
#include "node.h"

class Edge
{
public:
    Edge();
    Edge(Node *firstNode, Node *secondNode);
    Node* getFirstNode() const;
    Node* getSecondNode() const;
    friend bool operator==(const Edge &edgeA, const Edge &edgeB);

public:
    static const int arrowSize = 15;

private:
    Node *m_firstNode, *m_secondNode;
};
