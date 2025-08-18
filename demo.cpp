// g++ -std=c++17

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

    graph.print_adjacency_list();

    graph::Path path = graph.path(1, 5); // empty stack, if no path was found

    std::cout << "Path: ";

    while(!path.empty())
    {
        std::cout << path.top() << " ";
        path.pop();
    }

    std::cout << "\nLength: " << graph.path_length() << std::endl;

    graph.print_nodes();

    graph.block_nodes({3, 6}); // exclude nodes from path search
    graph.block_nodes(); // allow all nodes again

    graph.add_directed_edge(2, 5, 8);
    
    return 0;
}
