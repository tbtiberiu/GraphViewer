#include "graph.h"

Graph::Graph()
{
    // empty
}

void Graph::addNode(Node node)
{
    m_nodes.push_back(node);
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

vector<Node> Graph::getNodes() const
{
    return m_nodes;
}
vector<Edge> Graph::getEdges() const
{
    return m_edges;
}

vector<vector<int>> Graph::getAdjacencyMatrix() const
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

void Graph::updateAdjacencyMatrix()
{
    int size = getNumberOfNodes();
    m_adjacencyMatrix = vector<vector<int>>(size, vector<int>(size));

    for (Edge &edge : m_edges) {
        m_adjacencyMatrix[edge.getFirstNode().getInfo() - 1][edge.getSecondNode().getInfo() - 1] = 1;
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

int Graph::nodeAtCoord(QPointF coord, int radius) const
{
    Node n(coord);

    for (const Node &nn : m_nodes)
        if (Node::getDistance(n, nn) < radius)
            return nn.getInfo();
    return 0;
}

void Graph::moveNode(int nodeName, QPointF coord) {
    if (nodeAtCoord(coord, 20)) {
        return;
    }

    for (Node &node : m_nodes) {
        if(nodeName == node.getInfo()) {
            node.setCoord(coord);
            for (Edge &edge : m_edges) {
                if (edge.getFirstNode().getInfo() == nodeName) {
                    edge.updateFirstNode(coord);
                } else if (edge.getSecondNode().getInfo() == nodeName) {
                    edge.updateSecondNode(coord);
                }
            }
        }
    }
}
