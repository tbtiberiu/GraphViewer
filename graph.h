#pragma once
#include <QFile>
#include <QTextStream>
#include "edge.h"

using namespace std;
class Graph
{
public:
    Graph();
    void addNode(Node node);
    void addEdge(Edge edge);
    int getNumberOfNodes() const;
    vector<Node> getNodes() const;
    vector<Edge> getEdges() const;
    vector<vector<int>> getAdjacencyMatrix() const;
    bool hasEdge(Edge edge) const;
    bool isOriented() const;
    void setOrientation(bool orientation);
    void moveNode(int nodeName, QPointF coord);
    void updateAdjacencyMatrix();
    void saveAdjacencyMatrix(
        QString filename
        = "C:\\Users\\tibin\\Documents\\UNITBV\\AG\\GraphViewer\\AdjacencyMatrix.txt") const;
    int nodeAtCoord(QPointF coord, int radius) const;

private:
    bool oriented = true;
    vector<Node> m_nodes;
    vector<Edge> m_edges;
    vector<vector<int>> m_adjacencyMatrix;
};
