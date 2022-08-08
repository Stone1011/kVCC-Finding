//
// Created by 石依凡 on 2022/7/4.
//

#include "Graph.h"

using namespace std;

Graph::Graph(const string &path)
{
    ifstream fin(path);

    if(!fin.is_open())
    {
        throw std::runtime_error("Cannot open graph txt");
    }

    string line;
    while(fin.good())
    {
        getline(fin, line);
        if(line[0] == '#')
            continue;
        stringstream ss(line);
        int from, to;
        ss >> from >> to;
//        fin >> from >> to;
        addEdge(from, to);
        addEdge(to, from);
    }
}

void Graph::addEdge(int from, int to)
{
    edges[from].insert(to);
    if(from > degree)
        degree = from;
    if(to > degree)
        degree = to;
}

Graph Graph::genDir()
{
    Graph r;
    // (u, v) => <2u, 2u+1>, <2u+1, 2v>, <2v+1, 2u>
    for(const auto & each: edges)
    {
        r.addEdge(2*each.first, 2*each.first+1);
        int f = each.first;
        for(const auto & t: each.second)
        {
            if(t < f)
                continue;
            r.addEdge(2*f+1, 2*t);
            r.addEdge(2*t+1, 2*f);
        }
    }
    r.setK(k);

    return r;
}

set<int> Graph::locCut(int u, int v)
{
    set <int> r;

    // if v in N(u) or v == u, return NULL
    if(u == v or isin(v, edges[u]))
    {
        return r;
    }

    // Calc the max flow from u to v in DirG
    Graph dirG = genDir();
    auto minCutSet = dirG.minEdgeCut(2*u, 2*v+1);

    // if minCutSet num >= k, return NULL
    if(minCutSet.first >= k)
        return r;

    // else return minCutSet content
    for(auto each: minCutSet.second)
    {
        r.insert(each.first / 2);
    }
    return r;
}

void Graph::setK(int s)
{
    k = s;
}

// called by dir-graph
pair <int, set <pair <int, int> > > Graph::minEdgeCut(int u, int v)
{
    // return the maxFlowValue, and an edge set
    // initialization
    map <pair<int, int>, int> flow;
    for(auto each: edges)
    {
        int from = each.first;
        for(auto to: each.second)
        {
            flow[{from, to}] = 0;
            flow[{to, from}] = 0;
        }
    }

    // calc the max flow as flow
    Graph resiGraph = *this;
    int maxFlow = 0;
    while(true)
    {
        auto path = resiGraph.BFSFindPath(u, v);
        if(path.empty())
        {
            break;
        }
        maxFlow ++;
        for(int i = 0; i < path.size() - 1; i++)
        {
            int a = path[i], b = path[i+1];
            if(a == u or b == v)
                continue;
            flow[{a, b}]++;
            flow[{b, a}] = -flow[{a, b}];
            resiGraph.reverse(path[i], path[i+1]);
        }
    }

    // generate result
    // find min edge cut using DFS
    pair <int, set <Edge> > result;
    result.first = maxFlow;
    stack <vector <Edge> > waiting;
    // init stack
    vector <Edge> firstTemp;
    for(auto eachPair: flow)
    {
        auto edge = eachPair.first;
        auto state = eachPair.second;
        if(state == 1
            && edge.first != u && edge.second != v
            && edge.first % 2 == 0 && edge.second == edge.first + 1
        )
        {
            firstTemp.push_back(edge);
        }
    }
    if(!firstTemp.empty())
        waiting.push(firstTemp);
    stack <Graph> waitingGraph;
    // DFS
    Graph g = *this;
    vector <Edge> current;
    while(!waiting.empty())
    {
        if(!waiting.top().empty())
        {
            auto e = waiting.top().back();
            waiting.top().pop_back();
            current.push_back(e);
            if(current.size() > maxFlow)
            {
                current.pop_back();
                waiting.top().clear();
                continue;
            }
            waitingGraph.push(g);
            g.delEdge(e.first, e.second);
            // max flowTmp again
            map <pair<int, int>, int> flowTmp;
            for(auto each: edges)
            {
                int from = each.first;
                for(auto to: each.second)
                {
                    flowTmp[{from, to}] = 0;
                    flowTmp[{to, from}] = 0;
                }
            }
            Graph resiGraphTmp = g;
            int maxFlowTmp = 0;
            while(true)
            {
                auto path = resiGraphTmp.BFSFindPath(u, v);
                if(path.empty())
                {
                    break;
                }
                maxFlowTmp ++;
                for(int i = 0; i < path.size() - 1; i++)
                {
                    int a = path[i], b = path[i+1];
                    if(a == u or b == v)
                        continue;
                    flowTmp[{a, b}]++;
                    flowTmp[{b, a}] = -flowTmp[{a, b}];
                    resiGraphTmp.reverse(path[i], path[i+1]);
                }
            }
            if(maxFlowTmp == 0)
            {
                break;
            }
            vector <Edge> firstFirstTemp;
            for(auto eachPair: flowTmp)
            {
                auto edge = eachPair.first;
                auto state = eachPair.second;
                if(state == 1
                    && edge.first != u && edge.second != v
                    && edge.first % 2 == 0 && edge.second == edge.first + 1
                )
                {
                    firstFirstTemp.push_back(edge);
                }
            }
            if(firstFirstTemp.empty())
            {
                current.pop_back();
                waiting.top().clear();
                continue;
            }
            waiting.push(firstFirstTemp);
        }
        else
        {
            waiting.pop();
            current.pop_back();
            if(!waitingGraph.empty())
                g = waitingGraph.top();
            else
                throw runtime_error("waiting graph error");
            waitingGraph.pop();
        }
    }
    for(auto each: current)
    {
        result.second.insert(each);
    }

    return result;
}

// called by residual graph
vector<int> Graph::BFSFindPath(int u, int v)
{
    char * visited = new char[degree + 1];
    memset(visited, 0, (degree + 1));
    if(u == v)
    {
        delete [] visited;
        return {u};
    }
    queue <vector<int> > waiting;

    waiting.push({u});

    while(!waiting.empty())
    {
        auto currentPath = waiting.front();
        waiting.pop();

        if(currentPath.back() == v)
        {
            delete [] visited;
            return currentPath;
        }

        for(auto canAccess: edges[currentPath.back()])
        {
            if(visited[canAccess])
                continue;
            visited[canAccess] = 1;
            vector <int> p = currentPath;
            p.push_back(canAccess);
            waiting.push(p);
        }
    }
    delete [] visited;
    return {};
}

void Graph::reverse(int u, int v)
{
    edges[u].erase(v);
    if(edges[v].count(u))
        throw runtime_error("The graph contains a 2-length loop");
    edges[v].insert(u);
}

void Graph::delEdge(int from, int to)
{
    edges[from].erase(to);
}

set<int> Graph::globalCut()
{
    // select a source vertex with a minimum degree
    int minDeg = 1 << 28, u = 0;
    for(const auto& each: edges)
    {
        if(each.second.size() < minDeg)
        {
            minDeg = each.second.size();
            u = each.first;
        }
    }

    // init reduction
    bool * pru = new bool[degree + 1];
    int * deposit = new int[degree + 1];
    memset(deposit, 0, degree + 1);
    for(int i = 0; i < degree + 1; i++)
        pru[i] = false;
    sweep(u, pru, deposit);

    // u and other vertex
//    for(const auto& each: edges)
//    {
//        auto v = each.first;
//        if(u == v)
//            continue;
//        auto S = locCut(u, v);
//        if(!S.empty())
//        {
//            return S;
//        }
//    }
    auto tempQ = getDistance(u);
    while(!tempQ.empty())
    {
        auto v = tempQ.top().v;
        tempQ.pop();
        if(pru[v])
            continue;
        auto S = locCut(u, v);
        if(!S.empty())
        {
            delete [] pru;
            delete [] deposit;
            return S;
        }
        sweep(v, pru, deposit);
    }

    // u and neighbors
//    for(auto va: edges[u])
//    {
//        for(auto vb: edges[u])
//        {
//            if(va == vb)
//                continue;
//            auto S = locCut(va, vb);
//            if(!S.empty())
//            {
//                delete [] pru;
//                delete [] deposit;
//                return S;
//            }
//        }
//    }
    if(!isStrongVertex(u))
    {
        for(auto va: edges[u])
        {
            for(auto vb: edges[u])
            {
                if(va == vb)
                    continue;
                auto S = locCut(va, vb);
                if(!S.empty())
                {
                    delete [] pru;
                    delete [] deposit;
                    return S;
                }
            }
        }
    }

    delete [] pru;
    delete [] deposit;
    return {};
}

set<set<int> > Graph::kvcc()
{
    return kvccEnum(*this, k);
}

set<set<int> > Graph::kvccEnum(Graph g, int k)
{
    set <set<int> > allKVCC;
    // find k-core
    while(true)
    {
        int flag = 0;
        for (auto each: g.edges)
        {
            if (each.second.size() < k)
            {
                g.remove(each.first);
                flag = 1;
                break;
            }
        }
        if(!flag)
            break;
    }

//    g.printAsMathForm();

    // identify connected components
    vector <Graph> connectG = connectComponents(g);

    // update allKVCC
    for(auto gi: connectG)
    {
        auto S = gi.globalCut();
        if(S.empty())
        {
            allKVCC.insert(gi.vertex());
        }
        else
        {
            auto giSet = overlapPartition(gi, S);
            for(const auto& gij: giSet)
            {
                auto r = kvccEnum(gij, k);
                for(const auto& item: r)
                {
                    allKVCC.insert(item);
                }
            }
        }
    }

    return allKVCC;
}

vector<Graph> Graph::overlapPartition(Graph G, const set<int>& S)
{
    vector <Graph> result;
    set <int> VG = G.vertex();
    for(auto each: S)
        VG.erase(each);
    Graph G1 = G.subGraph(VG);
    auto connectG1 = connectComponents(G1);
    for(auto each: connectG1)
    {
        set <int> VG1 = each.vertex();
        for(int each: S)
            VG1.insert(each);
        Graph tempGraph = G.subGraph(VG1);
        result.push_back(tempGraph);
    }

    return result;
}

void Graph::remove(int v)
{
    edges.erase(v);
    for(auto & each: edges)
    {
        if(each.second.count(v))
            each.second.erase(v);
    }
}

void Graph::remove(const set<int>& v)
{
    for(auto e: v)
        remove(e);
}

vector<Graph> Graph::connectComponents(Graph g)
{
    vector <Graph> result;
    while(true)
    {
        auto vs = g.vertex();
        if(vs.empty())
            break;
        int source = * vs.begin();
        set <int> canAccess = g.canAccess(source);
        Graph graph = g.subGraph(canAccess);
        graph.setK(g.k);
        result.push_back(graph);
        g.remove(canAccess);
    }

    return result;
}

set<int> Graph::vertex()
{
    set <int> r;
    for(auto each: edges)
    {
        r.insert(each.first);
    }
    return r;
}

Graph Graph::subGraph(const set<int> &vs)
{
    Graph r;
    r.setK(k);
    for(auto e: edges)
    {
        int u;
        u = e.first;
        if(vs.count(u))
        {
            for(auto v: e.second)
            {
                if(vs.count(v))
                {
                    r.addEdge(u, v);
                }
            }
        }
    }
    return r;
}

set<int> Graph::canAccess(int source)
{
    queue <int> q;
    set <int> access;

    q.push(source);
    while(!q.empty())
    {
        int temp = q.front();
        q.pop();
        if(!access.count(temp))
        {
            access.insert(temp);
            for (auto a: edges[temp])
            {
                q.push(a);
            }
        }
    }

    return access;
}

bool Graph::isStrongVertex(int u)
{
    for(auto v1: edges[u])
    {
        for(auto v2: edges[u])
        {
            if(v1 >= v2)
                continue;
            auto N1 = edges[v1];
            auto N2 = edges[v2];
            vector <int> result;
            set_intersection(N1.begin(), N1.end(), N2.begin(), N2.end(), inserter(result, result.begin()));
            if(result.size() < k)
                return false;
        }
    }
    return true;
}

void Graph::sweep(int v, bool *pru, int *deposit)
{
    pru[v] = true;
    for(auto w: edges[v])
    {
        if(pru[w])
            continue;
        deposit[w]++;
        if(isStrongVertex(v) or deposit[w] >= k)
            sweep(w, pru, deposit);
    }
}

priority_queue<Node> Graph::getDistance(int u)
{
    priority_queue <Node> qu;
    queue <pair<int, int> > q;
    set <int> access;

    q.push({u, 0});
    while(!q.empty())
    {
        auto temp = q.front();
        if(temp.first != u)
            qu.push({temp.first, temp.second});
        q.pop();
        if(!access.count(temp.first))
        {
            access.insert(temp.first);
            for (auto a: edges[temp.first])
            {
                q.push({a, temp.second + 1});
            }
        }
    }

    return qu;
}

void Graph::printAsMathForm()
{
    for(auto each: edges)
    {
        int u = each.first;
        for(auto v: each.second)
        {
            if(v > u)
                cout << "UndirectedEdge[" << u << "," << v << "],";
        }
    }
}

bool isin(int u, const set <int>& v)
{
    return v.count(u);
}

void report(string path, const set<set<int> > & result)
{
    ofstream fout(path);
    if(!fout.is_open())
        throw runtime_error("Cannot open report file");

    fout << "VCC Num: " << result.size() << endl;
    int i = 1;
    for(const auto & each: result)
    {
        fout << endl;
        fout << "K-VCC(No. " << i << "): node_nums = " << each.size() << endl;
        for(auto v: each)
        {
            fout << "Node_ID: " << v << endl;
        }
        i++;
    }
}