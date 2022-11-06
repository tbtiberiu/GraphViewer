#pragma once
#include <QPoint>

class Node
{
public:
    Node(int info = -1);
    Node(QPointF coord, int info = -1);
    QPointF getCoordinate() const;
    int getInfo() const;
    void setInfo(int info);
    void setCoord(QPointF coord);
    static float getDistance(const Node &nodeA, const Node &nodeB);
    friend bool operator==(const Node &nodeA, const Node &nodeB);

private:
    int m_info;
    QPointF m_coord;
};
