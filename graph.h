#pragma once
#include <unordered_set>
#include <QFile>
#include <QTextStream>
#include "edge.h"

class Graph
{
public:
    Graph();
    void addNode(Node* node);
    void addEdge(Edge edge);
    int getNumberOfNodes() const;
    Node *getNodeAt(QPointF pos);
    std::unordered_set<Node*> getNodes() const;
    std::vector<Edge> getEdges() const;
    std::vector<std::vector<int>> getAdjacencyMatrix() const;
    bool hasEdge(Edge edge) const;
    bool isOriented() const;
    void setOrientation(bool orientation);
    void moveNode(Node *node, QPointF pos);
    void updateAdjacencyMatrix();
    void saveAdjacencyMatrix(
        QString filename
        = "C:\\Users\\tibin\\Documents\\UNITBV\\AG\\GraphViewer\\AdjacencyMatrix.txt") const;

private:
    bool oriented = true;
    std::unordered_set<Node*> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<int>> m_adjacencyMatrix;
};
