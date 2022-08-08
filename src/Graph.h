//
// Created by 石依凡 on 2022/7/4.
//

#ifndef KVCC_GRAPH_H
#define KVCC_GRAPH_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <queue>
#include <stack>
#include <cstring>
#include <algorithm>

using namespace std;

typedef pair<int, int> Edge;

struct Node
{
    int v;
    int dis;
    bool operator<(const Node & other) const
    {
        if(dis != other.dis)
            return dis < other.dis;
        else
            return v < other.v;
    }
};

class Graph
{
public:
    // Construction Methods
    Graph() = default;
    Graph(const Graph & other) = default;
    Graph(const string & path);
    void setK(int s);

    // Normal Methods
    void addEdge(int from, int to);
    void delEdge(int from, int to);
    void remove(int v);
    void remove(const set <int>& v);
    Graph subGraph(const set <int> & v);
    static vector <Graph> connectComponents(Graph g);
    Graph genDir();
    set <int> vertex();
    set <int> canAccess(int source);
    void printAsMathForm();

    // Max Flow Methods
    pair <int, set <Edge> > minEdgeCut(int u, int v);
    vector <int> BFSFindPath(int u, int v);
    void reverse(int u, int v);

    // K-VCC Methods
    set <int> locCut(int u, int v);
    set <int> globalCut();
    set <set<int> > kvcc();
    static set<set<int> > kvccEnum(Graph g, int k);
    static vector <Graph> overlapPartition(Graph G, const set <int>& S);

    // Reduction Methods
    bool isStrongVertex(int u);
    void sweep(int v, bool * pru, int * deposit);
    priority_queue <Node> getDistance(int u);

private:
    map<int, set <int> > edges;
    int k;
    int degree = -1;
};

bool isin(int u, const set <int>& v);
void report(string path, const set<set<int> > & result);

#endif //KVCC_GRAPH_H
