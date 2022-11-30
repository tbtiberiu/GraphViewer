#include "graph.h"

Graph::Graph()
{
    // empty
}

void Graph::addNode(Node *node)
{
    m_nodes.push_back(node);
    updateAdjacencyMatrix();
    m_nodesCount++;
}

void Graph::addEdge(Edge edge)
{
    if (hasEdge(edge))
        return;

    m_edges.push_back(edge);
    updateAdjacencyMatrix();
}

int Graph::getNodesCount() const
{
    return m_nodesCount;
}

Node *Graph::getNodeAt(QPointF pos)
{
    Node n(pos);

    for (Node *nn : m_nodes)
        if (Node::getDistance(n, *nn) < Node::blockedRadius)
            return nn;
    return nullptr;
}

std::vector<Node *> Graph::getNodes() const
{
    return m_nodes;
}

std::vector<Edge> Graph::getEdges() const
{
    return m_edges;
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
    return m_oriented;
}

void Graph::setOrientation(bool orientated)
{
    m_oriented = orientated;
}

void Graph::moveNode(Node *node, QPointF pos)
{
    if (node == nullptr)
        return;
    node->setCoord(pos);
}

void Graph::updateAdjacencyMatrix()
{
    int size = getNodesCount();
    m_adjacencyMatrix = std::vector<std::vector<bool>>(size, std::vector<bool>(size));

    for (const Edge &edge : m_edges) {
        m_adjacencyMatrix[edge.getFirstNode()->getInfo() - 1][edge.getSecondNode()->getInfo() - 1]
            = true;
    }
}

void Graph::saveChangesToFile(QString filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    stream << "Nodes count: " << m_nodesCount << "\n\n";
    updateAdjacencyMatrix();
    stream << "Adjacency matrix:\n";
    for (auto &row : m_adjacencyMatrix) {
        for (int val : row) {
            stream << val << " ";
        }
        stream << "\n";
    }
    stream << "\n";

    stream << "Paths:\n";
    for (auto &path : m_paths) {
        size_t i = 0;
        for (; i < path.size() - 1; i++) {
            stream << path[i] << ", ";
        }
        stream << path[i] << "\n";
    }
    stream << "\n";

    stream << "Cycles:\n";
    for (auto &cycle : m_cycles) {
        size_t i = 0;
        for (; i < cycle.size() - 1; i++) {
            stream << cycle[i] << ", ";
        }
        stream << cycle[i] << "\n";
    }
    stream << "\n";

    stream << "Adjacency list:\n";
    for (size_t i = 0; i < m_adjacencyMatrix.size(); i++) {
        stream << i + 1 << ": ";
        size_t j = 0;
        for (; j < m_adjacencyMatrix.size(); j++) {
            if (m_adjacencyMatrix[i][j] == 1)
                stream << j + 1 << ", ";
        }
        stream << "\n";
    }
    stream << "\n";

    file.close();
}

void Graph::insertCycle(std::vector<int> &cycle) {
    for (size_t i = 0; i < cycle.size(); i++) {
        cycle.pop_back();
        std::rotate(cycle.rbegin(), cycle.rbegin() + 1, cycle.rend());
        cycle.push_back(cycle[0]);
        if (m_cycles.find(cycle) != m_cycles.end())
            return;
    }

    m_cycles.insert(cycle);
}

void Graph::generateRandomNodes(int n)
{
    QPoint center = {500, 340};
    int circleRadius = 300;
    int initialNodesCount = m_nodesCount;

    while (m_nodesCount < initialNodesCount + n) {
        int angle = QRandomGenerator::global()->bounded(0, 361);
        QPoint randPos = {int(center.x() + circleRadius*cos(angle)), int(center.y() + circleRadius*sin(angle))};
        if (!getNodeAt(randPos))
            addNode(new Node(randPos, m_nodesCount + 1));
    }
}

void Graph::generateElementaryPath()
{
    int start = 0;
    std::vector<int> path = {start + 1};
    std::vector<int> indexes(m_nodesCount - 1);
    std::iota(indexes.begin(), indexes.end(), 1);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indexes.begin(), indexes.end(), g);

    int indexFirstNode = start;
    for (const auto &indexSecondNode : indexes) {
        addEdge(Edge(m_nodes[indexFirstNode], m_nodes[indexSecondNode]));
        path.push_back(indexSecondNode + 1);
        if (indexSecondNode == m_nodesCount - 1)
            break;
        indexFirstNode = indexSecondNode;
    }

    m_paths.insert(path);
}

void Graph::generateElementaryPaths(int count)
{
    std::size_t initialSize = m_paths.size();
    while (m_paths.size() < initialSize + std::size_t(count))
        generateElementaryPath();
}

void Graph::generateElementaryCycle()
{
    std::vector<bool> cycleNodes = std::vector<bool>(m_nodesCount);
    std::vector<int> indexes(m_nodesCount);
    std::iota(indexes.begin(), indexes.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indexes.begin(), indexes.end(), g);
    int startPos = QRandomGenerator::global()->bounded(1, m_nodesCount);
    int start = indexes[startPos];
    std::vector<int> cycle = {start + 1};

    int indexFirstNode = start;
    for (int &indexSecondNode : indexes) {
        addEdge(Edge(m_nodes[indexFirstNode], m_nodes[indexSecondNode]));
        cycle.push_back(indexSecondNode + 1);
        if (indexSecondNode == start)
            break;
        indexFirstNode = indexSecondNode;
    }

    insertCycle(cycle);
}

void Graph::generateElementaryCycles(int count)
{
    std::size_t initialSize = m_paths.size();
    while (m_cycles.size() < initialSize + std::size_t(count)) {
        generateElementaryCycle();
    }
}
