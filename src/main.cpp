#include <iostream>
#include "Graph.h"

using namespace std;

int main()
{
    Graph graph("/Users/stone/Downloads/KVCC-Data/MiniDataSet.txt");
    graph.setK(3);
//    graph.printAsMathForm();

    // BFS Test OK
    // auto r = graph.BFSFindPath(1, 2);

    // LOC-CUT test OK
    // auto r = graph.locCut(0, 5);

    // K-VCC test OK
    report("/Users/stone/Downloads/KVCC-Data/MiniDataSet_k=3.txt", graph.kvcc());
    return 0;
}
