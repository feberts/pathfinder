// std >= c++17

#include "graph.h"
#include <ctime>
#include <iostream>

using namespace std;
using namespace graph;

void fail(const string & msg, const int line_number)
{
    cerr << "FAILING at line " << line_number << ": " << msg << endl;
}

#define GRAPH_TEST(...) graph_test(__VA_ARGS__, __LINE__)

void graph_test(Graph & graph,
                const Node_id src, const Node_id dest,
                const vector<Node_id> & expected_path,
                const Distance expected_length,
                const int line_number)
{
    Path path = graph.path(src, dest);

    if(graph.path_length() != expected_length)
    {
        fail("path does not have expected length", line_number);
        return;
    }

    if(path.size() != expected_path.size())
    {
        fail("path does not have expected number of nodes", line_number);
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
                fail("path does not match expected path", line_number);
                return;
            }
        }
    }
}

void test_cases()
{
    cout << "GRAPH TEST START\n";

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

        GRAPH_TEST(graph, 1, 1, {1}, 0);
        GRAPH_TEST(graph, 1, 2, {1, 2}, 7);
        GRAPH_TEST(graph, 1, 3, {1, 3}, 9);
        GRAPH_TEST(graph, 1, 4, {1, 3, 4}, 20);
        GRAPH_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);
        GRAPH_TEST(graph, 1, 6, {1, 3, 6}, 11);

        GRAPH_TEST(graph, 2, 2, {2}, 0);
        GRAPH_TEST(graph, 2, 1, {2, 1}, 7);
        GRAPH_TEST(graph, 2, 3, {2, 3}, 10);
        GRAPH_TEST(graph, 2, 4, {2, 4}, 15);
        GRAPH_TEST(graph, 2, 6, {2, 3, 6}, 12);

        GRAPH_TEST(graph, 3, 3, {3}, 0);
        GRAPH_TEST(graph, 3, 1, {3, 1}, 9);
        GRAPH_TEST(graph, 3, 2, {3, 2}, 10);
        GRAPH_TEST(graph, 3, 4, {3, 4}, 11);
        GRAPH_TEST(graph, 3, 5, {3, 6, 5}, 11);
        GRAPH_TEST(graph, 3, 6, {3, 6}, 2);

        GRAPH_TEST(graph, 4, 4, {4}, 0);
        GRAPH_TEST(graph, 4, 1, {4, 3, 1}, 20);
        GRAPH_TEST(graph, 4, 2, {4, 2}, 15);
        GRAPH_TEST(graph, 4, 3, {4, 3}, 11);
        GRAPH_TEST(graph, 4, 5, {4, 5}, 6);
        GRAPH_TEST(graph, 4, 6, {4, 3, 6}, 13);

        GRAPH_TEST(graph, 5, 5, {5}, 0);
        GRAPH_TEST(graph, 5, 1, {5, 6, 3, 1}, 20);
        GRAPH_TEST(graph, 5, 3, {5, 6, 3}, 11);
        GRAPH_TEST(graph, 5, 4, {5, 4}, 6);
        GRAPH_TEST(graph, 5, 6, {5, 6}, 9);

        GRAPH_TEST(graph, 6, 6, {6}, 0);
        GRAPH_TEST(graph, 6, 1, {6, 3, 1}, 11);
        GRAPH_TEST(graph, 6, 2, {6, 3, 2}, 12);
        GRAPH_TEST(graph, 6, 3, {6, 3}, 2);
        GRAPH_TEST(graph, 6, 4, {6, 3, 4}, 13);
        GRAPH_TEST(graph, 6, 5, {6, 5}, 9);
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
        GRAPH_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);

        graph.add_edge(3, 5, 10);
        GRAPH_TEST(graph, 1, 5, {1, 3, 5}, 19);

        graph.add_node(7);
        graph.add_edge(6, 7, 1);
        graph.add_edge(7, 5, 1);
        GRAPH_TEST(graph, 1, 5, {1, 3, 6, 7, 5}, 13);

        graph.add_edge(2, 5, 5);
        GRAPH_TEST(graph, 1, 5, {1, 2, 5}, 12);
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
        GRAPH_TEST(graph, 1, 5, {1, 2, 3, 4, 5}, 8);
        GRAPH_TEST(graph, 3, 5, {3, 4, 5}, 4);

        graph.add_node(6);
        graph.add_edge(1, 6, 6);
        graph.add_edge(6, 5, 1);
        GRAPH_TEST(graph, 1, 5, {1, 6, 5}, 7);

        graph.add_edge(3, 5, 1);
        GRAPH_TEST(graph, 1, 5, {1, 2, 3, 5}, 5);
    }

    {
        Graph graph;

        graph.add_node(1);
        GRAPH_TEST(graph, 1, 1, {1}, 0);

        graph.add_node(2);
        GRAPH_TEST(graph, 2, 2, {2}, 0);
        GRAPH_TEST(graph, 1, 2, {}, distance_max);
        GRAPH_TEST(graph, 2, 1, {}, distance_max);

        graph.add_edge(1, 2, 7);
        GRAPH_TEST(graph, 1, 2, {1, 2}, 7);

        graph.add_node(3);
        GRAPH_TEST(graph, 1, 3, {}, distance_max);
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
        GRAPH_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);

        graph.block_nodes({3});
        GRAPH_TEST(graph, 1, 5, {1, 6, 5}, 23);

        graph.block_nodes({3, 6});
        GRAPH_TEST(graph, 1, 5, {1, 2, 4, 5}, 28);

        graph.block_nodes({6});
        GRAPH_TEST(graph, 1, 5, {1, 3, 4, 5}, 26);

        graph.block_nodes();
        GRAPH_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);
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
        GRAPH_TEST(graph, 1, 4, {1, 2, 3, 4}, 3);

        graph.block_nodes({2});
        GRAPH_TEST(graph, 1, 4, {}, distance_max);

        graph.block_nodes({1});
        GRAPH_TEST(graph, 1, 4, {1, 2, 3, 4}, 3);

        graph.block_nodes({4});
        GRAPH_TEST(graph, 1, 4, {1, 2, 3, 4}, 3);

        graph.block_nodes({1, 4});
        GRAPH_TEST(graph, 1, 4, {1, 2, 3, 4}, 3);

        graph.block_nodes({2, 3});
        GRAPH_TEST(graph, 2, 3, {2, 3}, 1);

        graph.block_nodes({1, 2, 3, 4});
        GRAPH_TEST(graph, 2, 2, {2}, 0);
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
        GRAPH_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);

        graph.block_nodes({3});
        GRAPH_TEST(graph, 1, 5, {1, 6, 5}, 23);
        GRAPH_TEST(graph, 5, 1, {5, 6, 1}, 23);

        graph.block_nodes({3, 6});
        GRAPH_TEST(graph, 1, 5, {1, 2, 4, 5}, 28);
        GRAPH_TEST(graph, 5, 1, {5, 4, 2, 1}, 28);

        graph.block_nodes({3, 4, 6});
        GRAPH_TEST(graph, 1, 5, {}, distance_max);
        GRAPH_TEST(graph, 5, 1, {}, distance_max);
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

        GRAPH_TEST(graph, 1, 4, {1, 2, 3, 4}, 13);

        graph.block_nodes({1});
        GRAPH_TEST(graph, 1, 4, {1, 2, 3, 4}, 13);

        graph.block_nodes({4});
        GRAPH_TEST(graph, 1, 4, {1, 2, 3, 4}, 13);

        graph.block_nodes({1, 4});
        GRAPH_TEST(graph, 1, 4, {1, 2, 3, 4}, 13);

        graph.block_nodes({2});
        GRAPH_TEST(graph, 1, 4, {}, distance_max);

        graph.block_nodes({3});
        GRAPH_TEST(graph, 1, 4, {}, distance_max);

        graph.block_nodes({2, 3});
        GRAPH_TEST(graph, 1, 4, {}, distance_max);

        graph.block_nodes({3, 4});
        GRAPH_TEST(graph, 1, 4, {}, distance_max);
    }

    {
        Graph graph;

        graph.add_node(1);
        graph.add_node(2);
        graph.add_directed_edge(2, 1, 5);
        GRAPH_TEST(graph, 1, 2, {}, distance_max);

        graph.add_node(3);
        graph.add_directed_edge(3, 2, 3);
        GRAPH_TEST(graph, 1, 3, {}, distance_max);
        GRAPH_TEST(graph, 3, 1, {3, 2, 1}, 8);

        graph.add_directed_edge(1, 3, 4);
        GRAPH_TEST(graph, 1, 3, {1, 3}, 4);
        GRAPH_TEST(graph, 2, 3, {2, 1, 3}, 9);
        GRAPH_TEST(graph, 1, 2, {1, 3, 2}, 7);

        graph.add_directed_edge(1, 3, 3);
        GRAPH_TEST(graph, 1, 3, {1, 3}, 3);
        GRAPH_TEST(graph, 2, 3, {2, 1, 3}, 8);
        GRAPH_TEST(graph, 1, 2, {1, 3, 2}, 6);

        graph.add_directed_edge(1, 2, 1);
        graph.add_directed_edge(2, 3, 1);
        GRAPH_TEST(graph, 1, 3, {1, 2, 3}, 2);
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
        GRAPH_TEST(graph, 1, 3, {1, 5, 6, 4, 3}, 8);
        GRAPH_TEST(graph, 3, 3, {3}, 0);

        graph.add_directed_edge(2, 4, 1);
        GRAPH_TEST(graph, 1, 3, {1, 2, 4, 3}, 7);

        graph.add_node(7);
        GRAPH_TEST(graph, 1, 7, {}, distance_max);

        graph.add_directed_edge(7, 1, 2);
        GRAPH_TEST(graph, 1, 7, {}, distance_max);

        graph.add_directed_edge(4, 7, 2);
        GRAPH_TEST(graph, 1, 7, {1, 2, 4, 7}, 7);

        graph.block_nodes({2});
        GRAPH_TEST(graph, 1, 7, {1, 5, 6, 4, 7}, 8);
    }

    {
        Graph graph;

        for(Node_id id = 1; id <= 4; ++id)
        {
            graph.add_node(id);
        }

        graph.add_edge(1, 2, 3);
        graph.add_edge(3, 4, 3);

        GRAPH_TEST(graph, 1, 2, {1, 2}, 3);
        GRAPH_TEST(graph, 3, 4, {3, 4}, 3);
        GRAPH_TEST(graph, 1, 3, {}, distance_max);
        GRAPH_TEST(graph, 4, 2, {}, distance_max);

        graph.add_edge(2, 3, 5);
        GRAPH_TEST(graph, 1, 3, {1, 2, 3}, 8);
        GRAPH_TEST(graph, 4, 2, {4, 3, 2}, 8);
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
        GRAPH_TEST(graph, 1, 3, {1, 3}, 1);

        graph.add_edge(1, 2, 0);
        GRAPH_TEST(graph, 1, 3, {1, 2, 3}, 0);
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
        GRAPH_TEST(graph, 1, 6, {1, 2, 3, 4, 5, 6}, 5);
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

        GRAPH_TEST(graph, 1, 1, {1}, 0);
        GRAPH_TEST(graph, 1, 2, {1, 2}, 7);
        GRAPH_TEST(graph, 1, 3, {1, 3}, 9);
        GRAPH_TEST(graph, 1, 4, {1, 3, 4}, 20);
        GRAPH_TEST(graph, 1, 5, {1, 3, 6, 5}, 20);
        GRAPH_TEST(graph, 1, 6, {1, 3, 6}, 11);
    }

    cout << "GRAPH TEST END\n";
}

// creating a grid-shaped graph, then performing a path search between opposite corners
void performance()
{
    cout << "Starting performance measurement:\n";

    const int COLS = 1000;
    const int ROWS = 1000;

    const Weight WEIGHT = 1;

    auto node_id = [=] (const int row, const int col) { return row * COLS + col; };

    Graph graph;

    clock_t time;
    cout << scientific;

    // ===== create nodes =====

    time = clock();

    for(int row = 0; row < ROWS; ++row)
    {
        for(int col = 0; col < COLS; ++col)
        {
            graph.add_node(node_id(row, col));
        }
    }

    time = clock() - time;
    cout << "time nodes:       " << (float)time / CLOCKS_PER_SEC << endl;

    // ===== add edges =====

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

    for(int row = 0; row < ROWS - 1; ++row) // rightmost column, vertical edges
    {
        graph.add_edge(node_id(row, COLS - 1), node_id(row + 1, COLS - 1), WEIGHT);
    }

    time = clock() - time;
    cout << "time edges:       " << (float)time / CLOCKS_PER_SEC << endl;

    // ===== pathfinding =====

    const Node_id src = node_id(0, 0);
    const Node_id dest = node_id(ROWS - 1, COLS - 1);

    time = clock();

    graph.path(src, dest);

    time = clock() - time;
    cout << "time pathfinding: " << (float)time / CLOCKS_PER_SEC << endl;
}

int main()
{
    test_cases();
    performance();

    return 0;
}
