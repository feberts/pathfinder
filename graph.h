#ifndef GRAPH_H
#define GRAPH_H

#include <climits>
#include <stack>
#include <unordered_map>
#include <vector>

namespace graph
{
    class Graph;

    namespace internal
    {
        struct Node;
        struct Directed_edge;
    }

    typedef int Node_id;
    typedef std::stack<Node_id> Path;
    typedef std::vector<Node_id> Node_ids;

    typedef int Weight;
    typedef Weight Distance;
    const Distance DISTANCE_MAX = std::numeric_limits<Distance>::max();
}

struct graph::internal::Node
{
    const Node_id id;
    std::vector<Directed_edge> departing_edges;

    Node() = delete;
    Node(const Node_id id)
        : id(id), distance(DISTANCE_MAX), previous(nullptr), explored(false) { }

    // Dijkstra:

    Distance distance;
    Node * previous;
    bool explored;
};

struct graph::internal::Directed_edge
{
    Node * const destination;
    const Weight weight;

    Directed_edge() = delete;
    Directed_edge(Node * const dest, const Weight weight);
};

/*
class Graph tested with
 - directed and undirected graphs
 - connected and disconnected graphs
 - paths with the same start and destination node
 - paths with unreachable destination nodes
 - graphs with loops
*/
class graph::Graph
{
public: // Graph

    void add_node(const Node_id id);
    void add_edge(const Node_id first, const Node_id second, const Weight weight = 1);
    void add_directed_edge(const Node_id src, const Node_id dest, const Weight weight = 1);
    void print_adjacency_list() const;

private: // Graph

    std::unordered_map<Node_id, internal::Node> m_nodes;

public: // Dijkstra

    Path path(const Node_id src, const Node_id dest);
    Distance path_length() const;
    void block_nodes(const Node_ids & ids = {});
    void print_nodes() const;

private: // Dijkstra

    void dijkstra(const Node_id src, const Node_id dest);
    void mark_blocked_nodes(const Node_id except);

    Distance m_path_length = DISTANCE_MAX;
    Node_ids m_blocked_nodes;
};

#endif // GRAPH_H
