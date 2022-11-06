#include "graph.h"

Graph::Graph()
{
    // empty
}

void Graph::addNode(Node* node)
{
    m_nodes.insert(node);
    updateAdjacencyMatrix();
}

void Graph::addEdge(Edge edge)
{
    if (hasEdge(edge))
        return;

    m_edges.push_back(edge);
    updateAdjacencyMatrix();
}

int Graph::getNumberOfNodes() const
{
    return (int) m_nodes.size();
}

Node* Graph::getNodeAt(QPointF pos) {
    Node n(pos);

    for (Node* nn : m_nodes)
        if (Node::getDistance(n, *nn) < Node::blockedRadius)
            return nn;
    return nullptr;
}

std::unordered_set<Node*> Graph::getNodes() const
{
    return m_nodes;
}

std::vector<Edge> Graph::getEdges() const
{
    return m_edges;
}

std::vector<std::vector<int>> Graph::getAdjacencyMatrix() const
{
    return m_adjacencyMatrix;
}

bool Graph::hasEdge(Edge edge) const
{
    for (const Edge &x : m_edges)
        if (edge == x)
            return true;
    return false;
}

bool Graph::isOriented() const
{
    return oriented;
}

void Graph::setOrientation(bool orientation)
{
    oriented = orientation;
}

void Graph::moveNode(Node* node, QPointF pos) {
    if (node == nullptr)
        return;
    node->setCoord(pos);
}

void Graph::updateAdjacencyMatrix()
{
    int size = getNumberOfNodes();
    m_adjacencyMatrix = std::vector<std::vector<int>>(size, std::vector<int>(size));

    for (const Edge &edge : m_edges) {
        m_adjacencyMatrix[edge.getFirstNode()->getInfo() - 1][edge.getSecondNode()->getInfo() - 1] = 1;
    }

    saveAdjacencyMatrix();
}

void Graph::saveAdjacencyMatrix(QString filename) const
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    stream << "Adjacency matrix:\n";
    for (auto &row : m_adjacencyMatrix) {
        for (int val : row) {
            stream << val << " ";
        }
        stream << "\n";
    }

    file.close();
}
