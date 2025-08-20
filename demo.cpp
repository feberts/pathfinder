// std >= c++17

#include "graph.h"
#include <iostream>

int main()
{
    graph::Graph graph;

    for(graph::Node_id id = 1; id <= 6; ++id)
    {
        graph.add_node(id);
    }

    graph.add_edge(1, 2, 7);
    graph.add_edge(1, 3, 9);
    graph.add_edge(1, 6, 14);
    graph.add_edge(2, 3, 10);
    graph.add_edge(2, 4, 15);
    graph.add_edge(3, 4, 11);
    graph.add_edge(3, 6, 2);
    graph.add_edge(4, 5, 6);
    graph.add_edge(5, 6, 9);

    graph::Path path = graph.path(1, 5);

    if(path.empty())
    {
        std::cout << "no path could be found\n";
    }

    std::cout << "Path: ";

    while(!path.empty())
    {
        std::cout << path.top() << " ";
        path.pop();
    }

    std::cout << "\nLength: " << graph.path_length() << std::endl;

    // nodes can be temporarily excluded from pathfinding:
    // (nodes not in the argument list will be unblocked if they were previously blocked)
    graph.block_nodes({3, 6});
    graph.block_nodes(); // unblock all nodes

    graph.add_directed_edge(2, 5, 8); // add directed edge

    return 0;
}
