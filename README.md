# Algorithm Design: $k$-VCC Search

## Shiyifan  2020202264

## 1 Problem Overview

$k$-VCC ($k$-Vertex Connected Component) refers to a **maximal subgraph** $G'$ of an undirected graph $G$, such that $G'$ is a $k$-vertex connected graph. This means that at least $k$ vertices need to be removed to break the connectivity of $G'$, or in other words, the minimum vertex cut set of $G'$ has a cardinality of $k$.

$k$-VCC has wide applications in many domains. For example, in social networks, computing all $k$-VCCs in a social relation graph can help identify highly correlated communities in the network, providing valuable data support for advertising and recommendation systems. In co-authorship networks, identifying $k$-VCCs can help determine research groups in the network.

The purpose of this experiment is to find all $k$-VCCs in an undirected graph network.

## 2 Experiment Completion

This experiment has completed the search for all $k$-VCCs in medium and small-sized datasets and implemented simple optimizations based on the content of the paper.

The program cannot complete the search within an acceptable time for extremely large datasets.

## 3 Experiment Description

This experiment is implemented in the C++ programming language, using an object-oriented approach. The class `Graph` and related methods are designed to implement the search for $k$-VCCs. The main parts of the program include:

* Using the max flow algorithm (Edmond-Karp Algorithm) to search for the maximum flow from a given source node to a sink node in a directed graph.
* Using depth-first search to find the minimum edge cut set of the directed graph based on the max flow algorithm.
* Transforming the minimum edge cut set of the directed graph into the minimum vertex cut set of the original graph using the method of splitting directed graph edges.
* Using the given algorithm to find the minimum cut of the entire graph.
* Using the properties of $k$-VCCs to find all $k$-VCCs in the entire graph.
* Using optimization algorithms to optimize the solving process.

The STL libraries used in this program are:

```cpp
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
```

The design of the `Graph` class in the program is as follows:

```cpp
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
```

The program uses two global functions:

```cpp
bool isin(int u, const set <int>& v);
void report(string path, const set<set<int> > & result);
```

The program defines the type `Edge` and the structure `Node` as follows:

```cpp
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
```

## 4  Test Results

For the given dataset (MiniDataSet), running the program (note that the overall information of the graph should be removed beforehand) yields the following results for different values of $k$ (2, 3, 5, 7):

When $k=2$:

```
VCC Num: 4

K-VCC(No. 1): node_nums = 47
Node_ID: 0
Node_ID: 2
Node_ID: 3
Node_ID: 5
Node_ID: 6
Node_ID: 8
Node_ID: 9
Node_ID: 11
Node_ID: 14
Node_ID: 15
Node_ID: 16
Node_ID: 17
Node_ID: 19
Node_ID: 21
Node_ID: 22
Node_ID: 23
Node_ID: 26
Node_ID: 27
Node_ID: 29
Node_ID: 32
Node_ID: 36
Node_ID: 37
Node_ID: 40
Node_ID: 41
Node_ID: 42
Node_ID: 43
Node_ID: 44
Node_ID: 46
Node_ID: 47
Node_ID: 48
Node_ID: 50
Node_ID: 51
Node_ID: 52
Node_ID: 53
Node_ID: 55
Node_ID: 57
Node_ID: 58
Node_ID: 60
Node_ID: 64
Node_ID: 65
Node_ID: 67
Node_ID: 69
Node_ID: 71
Node_ID: 72
Node_ID: 73
Node_ID: 76
Node_ID: 78

K-VCC(No. 2): node_nums = 16
Node_ID: 1
Node_ID: 4
Node_ID: 10
Node_ID: 12
Node_ID: 28
Node_ID: 30
Node_ID: 31
Node_ID: 35
Node_ID: 50
Node_ID: 54
Node_ID: 62
Node_ID: 63
Node_ID: 66
Node_ID: 68
Node_ID: 70
Node_ID: 77

K-VCC(No. 3): node_nums = 5
Node_ID: 18
Node_ID: 19
Node_ID: 20
Node_ID: 39
Node_ID: 61

K-VCC(No. 4): node_nums = 6
Node_ID: 25
Node_ID: 33
Node_ID: 38
Node_ID: 67
Node_ID: 74
Node_ID: 75
```

When $k=3$:

```
# k = 3
VCC Num: 5

K-VCC(No. 1): node_nums = 15
Node_ID: 0
Node_ID: 2
Node_ID: 3
Node_ID: 5
Node_ID: 6
Node_ID: 11
Node_ID: 15
Node_ID: 17
Node_ID: 23
Node_ID: 29
Node_ID: 42
Node_ID: 44
Node_ID: 50
Node_ID: 55
Node_ID: 60

K-VCC(No. 2): node_nums = 15
Node_ID: 1
Node_ID: 4
Node_ID: 10
Node_ID: 12
Node_ID: 28
Node_ID: 30
Node_ID: 31
Node_ID: 35
Node_ID: 54
Node_ID: 62
Node_ID: 63
Node_ID: 66
Node_ID: 68
Node_ID: 70
Node_ID: 77

K-VCC(No. 3): node_nums = 30
Node_ID: 8
Node_ID: 9
Node_ID: 14
Node_ID: 16
Node_ID: 21
Node_ID: 22
Node_ID: 23
Node_ID: 26
Node_ID: 27
Node_ID: 32
Node_ID: 36
Node_ID: 37
Node_ID: 40
Node_ID: 41
Node_ID: 43
Node_ID: 46
Node_ID: 47
Node_ID: 48
Node_ID: 51
Node_ID: 52
Node_ID: 53
Node_ID: 57
Node_ID: 58
Node_ID: 64
Node_ID: 65
Node_ID: 69
Node_ID: 71
Node_ID: 72
Node_ID: 76
Node_ID: 78

K-VCC(No. 4): node_nums = 4
Node_ID: 18
Node_ID: 20
Node_ID: 39
Node_ID: 61

K-VCC(No. 5): node_nums = 6
Node_ID: 25
Node_ID: 33
Node_ID: 38
Node_ID: 67
Node_ID: 74
Node_ID: 75
```

When $k=5$:

```
# k = 5
VCC Num: 4

K-VCC(No. 1): node_nums = 6
Node_ID: 2
Node_ID: 3
Node_ID: 6
Node_ID: 15
Node_ID: 42
Node_ID: 60

K-VCC(No. 2): node_nums = 6
Node_ID: 4
Node_ID: 30
Node_ID: 54
Node_ID: 62
Node_ID: 66
Node_ID: 68

K-VCC(No. 3): node_nums = 19
Node_ID: 9
Node_ID: 14
Node_ID: 21
Node_ID: 22
Node_ID: 32
Node_ID: 36
Node_ID: 37
Node_ID: 40
Node_ID: 41
Node_ID: 43
Node_ID: 47
Node_ID: 52
Node_ID: 58
Node_ID: 64
Node_ID: 69
Node_ID: 71
Node_ID: 72
Node_ID: 76
Node_ID: 78

K-VCC(No. 4): node_nums = 6
Node_ID: 27
Node_ID: 46
Node_ID: 48
Node_ID: 51
Node_ID: 53
Node_ID: 57
```

When $k=7$:

```
# k = 7
VCC Num: 1

K-VCC(No. 1): node_nums = 15
Node_ID: 9
Node_ID: 32
Node_ID: 36
Node_ID: 37
Node_ID: 40
Node_ID: 41
Node_ID: 43
Node_ID: 47
Node_ID: 52
Node_ID: 58
Node_ID: 64
Node_ID: 69
Node_ID: 72
Node_ID: 76
Node_ID: 78
```

## Appendix A  Test Dataset

The point-to-point representation of the MiniDataSet is as follows:

```
0 5
0 11
0 17
0 23
0 29
0 44
0 55
1 30
1 31
1 35
1 54
1 62
1 63
1 68
1 77
2 3
2 6
2 15
2 42
2 60
3 6
3 15
3 42
3 50
3 60
4 10
4 30
4 54
4 62
4 66
4 68
4 70
5 11
5 17
5 23
5 29
5 44
5 55
6 11
6 15
6 42
6 60
7 45
8 23
8 27
8 44
8 51
8 53
9 32
9 36
9 37
9 40
9 41
9 43
9 47
9 58
9 64
9 72
9 76
9 78
10 66
10 68
10 70
11 15
11 55
11 60
12 28
12 31
12 35
12 50
12 63
12 77
13 33
13 34
14 21
14 22
14 32
14 41
14 47
14 71
14 73
15 42
15 50
15 60
16 26
16 36
16 40
16 65
16 67
17 23
17 29
17 55
18 19
18 20
18 39
18 61
19 22
19 39
19 73
20 39
20 61
21 22
21 32
21 41
21 47
21 49
21 71
21 72
22 32
22 47
22 71
22 73
22 78
23 27
23 29
23 44
23 53
24 49
25 33
25 74
25 75
26 36
26 40
26 48
26 57
26 65
26 67
27 46
27 48
27 51
27 53
27 57
28 31
28 50
28 63
29 55
30 54
30 62
30 63
30 66
30 68
30 77
31 35
31 63
31 77
32 37
32 41
32 43
32 47
32 64
32 71
32 72
32 76
32 78
33 38
33 67
33 74
35 63
35 77
36 37
36 40
36 43
36 46
36 48
36 52
36 57
36 64
36 65
36 69
36 72
37 40
37 41
37 43
37 47
37 52
37 58
37 64
37 72
37 76
37 78
38 67
38 74
38 75
39 61
40 43
40 46
40 48
40 52
40 64
40 65
40 69
40 72
40 76
41 43
41 47
41 52
41 64
41 71
41 72
41 76
41 78
42 44
42 50
42 60
43 47
43 52
43 58
43 64
43 69
43 71
43 72
43 76
43 78
46 48
46 51
46 53
46 57
47 58
47 64
47 71
47 72
47 76
47 78
48 51
48 53
48 57
51 53
51 57
52 64
52 65
52 69
52 72
52 76
53 57
54 62
54 66
54 68
54 70
55 60
56 59
58 64
58 72
58 78
62 66
62 68
62 77
63 77
64 69
64 72
64 76
64 78
65 69
66 68
66 70
67 75
68 70
69 72
69 76
71 72
71 76
72 76
72 78
74 75
```

