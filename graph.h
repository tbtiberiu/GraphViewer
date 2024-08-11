#pragma once
#include <unordered_set>
#include <QFile>
#include <QTextStream>
#include "edge.h"

class Graph
{
public:
    struct VectorHash
    {
        size_t operator()(const std::vector<int> &v) const
        {
            std::hash<int> hasher;
            size_t seed = 0;
            for (int i : v) {
                seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

public:
    Graph();
    void addNode(Node *node);
    void addEdge(Edge edge);
    int getNodesCount() const;
    Node *getNodeAt(QPointF pos);
    std::vector<Node *> getNodes() const;
    std::vector<Edge> getEdges() const;
    bool hasEdge(Edge edge) const;
    bool isOriented() const;
    void setOrientation(bool oriented);
    void moveNode(Node *node, QPointF pos);
    void updateAdjacencyMatrix();
    void saveChangesToFile(QString filename = "AboutMatrix.txt");
    void insertCycle(std::vector<int> &cycle);
    void generateRandomNodes(int n);
    void generateElementaryPath();
    void generateElementaryPaths(int count = 1);
    void generateElementaryCycle();
    void generateElementaryCycles(int count = 1);

private:
    bool m_oriented = true;
    int m_nodesCount = 0;
    std::vector<Node *> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<bool>> m_adjacencyMatrix;
    std::unordered_set<std::vector<int>, VectorHash> m_paths;
    std::unordered_set<std::vector<int>, VectorHash> m_cycles;
};
