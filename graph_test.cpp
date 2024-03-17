// g++ -std=c++17

#include "graph.h"
#include <ctime>
#include <iostream>

void demo();
void test_cases();
void performance();

int main()
{
    // demo();
    test_cases();
    // performance();

    return 0;
}

void demo()
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
}

using namespace std;
using namespace graph;

void fail(const string & msg, const int line)
{
    cerr << "FAIL, line " << line << ": " << msg << endl;
}

#define DIJKSTRA_TEST(...) dijkstra_test(__VA_ARGS__, __LINE__)

void dijkstra_test(Graph & graph,
                   const Node_id src, const Node_id dest,
                   const vector<Node_id> & expected_path,
                   const Distance expected_length,
                   const int line)
{
    Path path = graph.path(src, dest);

    if(graph.path_length() != expected_length)
    {
        fail("path does not have expected length", line);
        return;
    }

    if(path.size() != expected_path.size())
    {
        fail("path does not have expected number of nodes", line);
        return;
    }
    else
    {
        for(const Node_id expected_id : expected_path)
        {
            const Node_id id = path.top();
            path.pop();

            if(expected_id != id)
            {
                fail("path does not match expected path", line);
                return;
            }
        }
    }
}

void test_cases()
{
    cout << "dijkstra test start\n";

    {
        Graph graph;

        for(Node_id id = 1; id <= 6; ++id)
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

        DIJKSTRA_TEST(graph, 1, 1, {1}, 0);
        DIJKSTRA_TEST(graph, 1, 2, {1, 2}, 7);
        DIJKSTRA_TEST(graph, 1, 3, {1, 3}, 9);
        DIJKSTRA_TEST(graph, 1, 4, {1, 3, 4}, 20);
        DIJKSTRA_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);
        DIJKSTRA_TEST(graph, 1, 6, {1, 3, 6}, 11);

        DIJKSTRA_TEST(graph, 2, 2, {2}, 0);
        DIJKSTRA_TEST(graph, 2, 1, {2, 1}, 7);
        DIJKSTRA_TEST(graph, 2, 3, {2, 3}, 10);
        DIJKSTRA_TEST(graph, 2, 4, {2, 4}, 15);
        DIJKSTRA_TEST(graph, 2, 6, {2, 3, 6}, 12);

        DIJKSTRA_TEST(graph, 3, 3, {3}, 0);
        DIJKSTRA_TEST(graph, 3, 1, {3, 1}, 9);
        DIJKSTRA_TEST(graph, 3, 2, {3, 2}, 10);
        DIJKSTRA_TEST(graph, 3, 4, {3, 4}, 11);
        DIJKSTRA_TEST(graph, 3, 5, {3, 6, 5}, 11);
        DIJKSTRA_TEST(graph, 3, 6, {3, 6}, 2);

        DIJKSTRA_TEST(graph, 4, 4, {4}, 0);
        DIJKSTRA_TEST(graph, 4, 1, {4, 3, 1}, 20);
        DIJKSTRA_TEST(graph, 4, 2, {4, 2}, 15);
        DIJKSTRA_TEST(graph, 4, 3, {4, 3}, 11);
        DIJKSTRA_TEST(graph, 4, 5, {4, 5}, 6);
        DIJKSTRA_TEST(graph, 4, 6, {4, 3, 6}, 13);

        DIJKSTRA_TEST(graph, 5, 5, {5}, 0);
        DIJKSTRA_TEST(graph, 5, 1, {5, 6, 3, 1}, 20);
        DIJKSTRA_TEST(graph, 5, 3, {5, 6, 3}, 11);
        DIJKSTRA_TEST(graph, 5, 4, {5, 4}, 6);
        DIJKSTRA_TEST(graph, 5, 6, {5, 6}, 9);

        DIJKSTRA_TEST(graph, 6, 6, {6}, 0);
        DIJKSTRA_TEST(graph, 6, 1, {6, 3, 1}, 11);
        DIJKSTRA_TEST(graph, 6, 2, {6, 3, 2}, 12);
        DIJKSTRA_TEST(graph, 6, 3, {6, 3}, 2);
        DIJKSTRA_TEST(graph, 6, 4, {6, 3, 4}, 13);
        DIJKSTRA_TEST(graph, 6, 5, {6, 5}, 9);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 6; ++id)
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
        DIJKSTRA_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);

        graph.add_edge(3, 5, 10);
        DIJKSTRA_TEST(graph, 1, 5, {1, 3, 5}, 19);

        graph.add_node(7);
        graph.add_edge(6, 7, 1);
        graph.add_edge(7, 5, 1);
        DIJKSTRA_TEST(graph, 1, 5, {1, 3, 6, 7, 5}, 13);

        graph.add_edge(2, 5, 5);
        DIJKSTRA_TEST(graph, 1, 5, {1, 2, 5}, 12);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 5; ++id)
        {
            graph.add_node(id);
        }

        graph.add_edge(1, 2, 2);
        graph.add_edge(2, 3, 2);
        graph.add_edge(3, 4, 2);
        graph.add_edge(4, 5, 2);
        graph.add_edge(5, 1, 9);
        DIJKSTRA_TEST(graph, 1, 5, {1, 2, 3, 4, 5}, 8);
        DIJKSTRA_TEST(graph, 3, 5, {3, 4, 5}, 4);

        graph.add_node(6);
        graph.add_edge(1, 6, 6);
        graph.add_edge(6, 5, 1);
        DIJKSTRA_TEST(graph, 1, 5, {1, 6, 5}, 7);

        graph.add_edge(3, 5, 1);
        DIJKSTRA_TEST(graph, 1, 5, {1, 2, 3, 5}, 5);
    }

    {
        Graph graph;

        graph.add_node(1);
        DIJKSTRA_TEST(graph, 1, 1, {1}, 0);

        graph.add_node(2);
        DIJKSTRA_TEST(graph, 2, 2, {2}, 0);
        DIJKSTRA_TEST(graph, 1, 2, {}, graph::DISTANCE_MAX);
        DIJKSTRA_TEST(graph, 2, 1, {}, graph::DISTANCE_MAX);

        graph.add_edge(1, 2, 7);
        DIJKSTRA_TEST(graph, 1, 2, {1, 2}, 7);

        graph.add_node(3);
        DIJKSTRA_TEST(graph, 1, 3, {}, graph::DISTANCE_MAX);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 6; ++id)
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
        DIJKSTRA_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);

        graph.block_nodes({3});
        DIJKSTRA_TEST(graph, 1, 5, {1, 6, 5}, 23);

        graph.block_nodes({3, 6});
        DIJKSTRA_TEST(graph, 1, 5, {1, 2, 4, 5}, 28);

        graph.block_nodes({6});
        DIJKSTRA_TEST(graph, 1, 5, {1, 3, 4, 5}, 26);

        graph.block_nodes();
        DIJKSTRA_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 4; ++id)
        {
            graph.add_node(id);
        }

        graph.add_edge(1, 2, 1);
        graph.add_edge(2, 3, 1);
        graph.add_edge(3, 4, 1);
        DIJKSTRA_TEST(graph, 1, 4, {1, 2, 3, 4}, 3);

        graph.block_nodes({2});
        DIJKSTRA_TEST(graph, 1, 4, {}, graph::DISTANCE_MAX);

        graph.block_nodes({1});
        DIJKSTRA_TEST(graph, 1, 4, {1, 2, 3, 4}, 3);

        graph.block_nodes({4});
        DIJKSTRA_TEST(graph, 1, 4, {1, 2, 3, 4}, 3);

        graph.block_nodes({1, 4});
        DIJKSTRA_TEST(graph, 1, 4, {1, 2, 3, 4}, 3);

        graph.block_nodes({2, 3});
        DIJKSTRA_TEST(graph, 2, 3, {2, 3}, 1);

        graph.block_nodes({1, 2, 3, 4});
        DIJKSTRA_TEST(graph, 2, 2, {2}, 0);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 6; ++id)
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
        DIJKSTRA_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);

        graph.block_nodes({3});
        DIJKSTRA_TEST(graph, 1, 5, {1, 6, 5}, 23);
        DIJKSTRA_TEST(graph, 5, 1, {5, 6, 1}, 23);

        graph.block_nodes({3, 6});
        DIJKSTRA_TEST(graph, 1, 5, {1, 2, 4, 5}, 28);
        DIJKSTRA_TEST(graph, 5, 1, {5, 4, 2, 1}, 28);

        graph.block_nodes({3, 4, 6});
        DIJKSTRA_TEST(graph, 1, 5, {}, DISTANCE_MAX);
        DIJKSTRA_TEST(graph, 5, 1, {}, DISTANCE_MAX);
    }

    {
        Graph graph;

        graph.add_node(1);
        graph.add_node(2);
        graph.add_node(3);
        graph.add_node(4);

        graph.add_edge(1, 2, 7);
        graph.add_edge(2, 3, 5);
        graph.add_edge(3, 4, 1);

        DIJKSTRA_TEST(graph, 1, 4, {1, 2, 3, 4}, 13);

        graph.block_nodes({1});
        DIJKSTRA_TEST(graph, 1, 4, {1, 2, 3, 4}, 13);

        graph.block_nodes({4});
        DIJKSTRA_TEST(graph, 1, 4, {1, 2, 3, 4}, 13);

        graph.block_nodes({1, 4});
        DIJKSTRA_TEST(graph, 1, 4, {1, 2, 3, 4}, 13);

        graph.block_nodes({2});
        DIJKSTRA_TEST(graph, 1, 4, {}, DISTANCE_MAX);

        graph.block_nodes({3});
        DIJKSTRA_TEST(graph, 1, 4, {}, DISTANCE_MAX);

        graph.block_nodes({2, 3});
        DIJKSTRA_TEST(graph, 1, 4, {}, DISTANCE_MAX);

        graph.block_nodes({3, 4});
        DIJKSTRA_TEST(graph, 1, 4, {}, DISTANCE_MAX);
    }

    {
        Graph graph;

        graph.add_node(1);
        graph.add_node(2);
        graph.add_directed_edge(2, 1, 5);
        DIJKSTRA_TEST(graph, 1, 2, {}, DISTANCE_MAX);

        graph.add_node(3);
        graph.add_directed_edge(3, 2, 3);
        DIJKSTRA_TEST(graph, 1, 3, {}, DISTANCE_MAX);
        DIJKSTRA_TEST(graph, 3, 1, {3, 2, 1}, 8);

        graph.add_directed_edge(1, 3, 4);
        DIJKSTRA_TEST(graph, 1, 3, {1, 3}, 4);
        DIJKSTRA_TEST(graph, 2, 3, {2, 1, 3}, 9);
        DIJKSTRA_TEST(graph, 1, 2, {1, 3, 2}, 7);

        graph.add_directed_edge(1, 3, 3);
        DIJKSTRA_TEST(graph, 1, 3, {1, 3}, 3);
        DIJKSTRA_TEST(graph, 2, 3, {2, 1, 3}, 8);
        DIJKSTRA_TEST(graph, 1, 2, {1, 3, 2}, 6);

        graph.add_directed_edge(1, 2, 1);
        graph.add_directed_edge(2, 3, 1);
        DIJKSTRA_TEST(graph, 1, 3, {1, 2, 3}, 2);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 6; ++id)
        {
            graph.add_node(id);
        }

        graph.add_directed_edge(1, 2, 4);
        graph.add_directed_edge(2, 3, 6);
        graph.add_directed_edge(1, 5, 2);
        graph.add_directed_edge(5, 6, 2);
        graph.add_directed_edge(6, 4, 2);
        graph.add_directed_edge(4, 3, 2);
        DIJKSTRA_TEST(graph, 1, 3, {1, 5, 6, 4, 3}, 8);
        DIJKSTRA_TEST(graph, 3, 3, {3}, 0);

        graph.add_directed_edge(2, 4, 1);
        DIJKSTRA_TEST(graph, 1, 3, {1, 2, 4, 3}, 7);

        graph.add_node(7);
        DIJKSTRA_TEST(graph, 1, 7, {}, DISTANCE_MAX);

        graph.add_directed_edge(7, 1, 2);
        DIJKSTRA_TEST(graph, 1, 7, {}, DISTANCE_MAX);

        graph.add_directed_edge(4, 7, 2);
        DIJKSTRA_TEST(graph, 1, 7, {1, 2, 4, 7}, 7);

        graph.block_nodes({2});
        DIJKSTRA_TEST(graph, 1, 7, {1, 5, 6, 4, 7}, 8);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 4; ++id)
        {
            graph.add_node(id);
        }

        graph.add_edge(1, 2, 3);
        graph.add_edge(3, 4, 3);

        DIJKSTRA_TEST(graph, 1, 2, {1, 2}, 3);
        DIJKSTRA_TEST(graph, 3, 4, {3, 4}, 3);
        DIJKSTRA_TEST(graph, 1, 3, {}, DISTANCE_MAX);
        DIJKSTRA_TEST(graph, 4, 2, {}, DISTANCE_MAX);

        graph.add_edge(2, 3, 5);
        DIJKSTRA_TEST(graph, 1, 3, {1, 2, 3}, 8);
        DIJKSTRA_TEST(graph, 4, 2, {4, 3, 2}, 8);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 3; ++id)
        {
            graph.add_node(id);
        }

        graph.add_edge(1, 2, 2);
        graph.add_edge(2, 3, 0);
        graph.add_edge(1, 3, 1);
        DIJKSTRA_TEST(graph, 1, 3, {1, 3}, 1);

        graph.add_edge(1, 2, 0);
        DIJKSTRA_TEST(graph, 1, 3, {1, 2, 3}, 0);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 6; ++id)
        {
            graph.add_node(id);
        }

        graph.add_edge(1, 6, 100);
        graph.add_edge(1, 2, 1);
        graph.add_edge(2, 3, 1);
        graph.add_edge(3, 4, 1);
        graph.add_edge(4, 5, 1);
        graph.add_edge(5, 6, 1);
        DIJKSTRA_TEST(graph, 1, 6, {1, 2, 3, 4, 5, 6}, 5);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 6; ++id)
        {
            graph.add_node(id);
        }

        for(Node_id id = 1; id <= 6; ++id)
        {
            graph.add_edge(id, id, 0);
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

        DIJKSTRA_TEST(graph, 1, 1, {1}, 0);
        DIJKSTRA_TEST(graph, 1, 2, {1, 2}, 7);
        DIJKSTRA_TEST(graph, 1, 3, {1, 3}, 9);
        DIJKSTRA_TEST(graph, 1, 4, {1, 3, 4}, 20);
        DIJKSTRA_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);
        DIJKSTRA_TEST(graph, 1, 6, {1, 3, 6}, 11);
    }

    cout << "dijkstra test end\n";
}

void performance()
{
    // creating a grid-shaped graph, then performing a path search between opposite corners

    const int COLS = 1000;
    const int ROWS = 1000;

    const Weight WEIGHT = 1;

    auto node_id = [=] (const int row, const int col) { return row * COLS + col; };

    Graph graph;

    clock_t time;
    cout << scientific;

    // ===== nodes =====

    time = clock();

    for(int row = 0; row < ROWS; ++row)
    {
        for(int col = 0; col < COLS; ++col)
        {
            graph.add_node(node_id(row, col));
        }
    }

    time = clock() - time;
    cout << "Time nodes:    " << (float)time / CLOCKS_PER_SEC << endl;

    // ===== edges =====

    time = clock();

    for(int row = 0; row < ROWS - 1; ++row) // vertical and horizontal edges
    {
        for(int col = 0; col < COLS - 1; ++col)
        {
            graph.add_edge(node_id(row, col), node_id(row, col + 1), WEIGHT);
            graph.add_edge(node_id(row, col), node_id(row + 1, col), WEIGHT);
        }
    }

    for(int col = 0; col < COLS - 1; ++col) // bottom row, horizontal edges
    {
        graph.add_edge(node_id(ROWS - 1, col), node_id(ROWS - 1, col + 1), WEIGHT);
    }

    for(int row = 0; row < ROWS - 1; ++row) // rightmost collumn, vertical edges
    {
        graph.add_edge(node_id(row, COLS - 1), node_id(row + 1, COLS - 1), WEIGHT);
    }

    time = clock() - time;
    cout << "Time edges:    " << (float)time / CLOCKS_PER_SEC << endl;

    // ===== Dijkstra =====

    const Node_id src = node_id(0, 0);
    const Node_id dest = node_id(ROWS - 1, COLS - 1);

    time = clock();

    // for(int n = 0; n < 1000; ++n)
    graph.path(src, dest);

    time = clock() - time;
    cout << "Time Dijkstra: " << (float)time / CLOCKS_PER_SEC << endl;
}
