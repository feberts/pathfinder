#include "graph.h"
#include <cassert>
#include <iostream>
#include <queue>

using namespace std;
using namespace graph;
using namespace internal;

Directed_edge::Directed_edge(Node * const dest, const Weight weight)
    : destination(dest), weight(weight)
{
    assert(dest && "dest is NULL");
}

void Graph::add_node(const Node_id id)
{
    assert(!m_nodes.count(id) && "node already added");

    m_nodes.emplace(id, id);
}

void Graph::add_edge(const Node_id first, const Node_id second, const Weight weight)
{
    assert(weight >= 0 && "no negative weights allowed");
    assert(m_nodes.count(first) && "start node does not exist");
    assert(m_nodes.count(second) && "destination node does not exist");

    add_directed_edge(first, second, weight);
    add_directed_edge(second, first, weight);
}

void Graph::add_directed_edge(const Node_id src, const Node_id dest, const Weight weight)
{
    assert(weight >= 0 && "no negative weights allowed");
    assert(m_nodes.count(src) && "start node does not exist");
    assert(m_nodes.count(dest) && "destination node does not exist");

    const Directed_edge edge(&m_nodes.at(dest), weight);
    m_nodes.at(src).departing_edges.push_back(edge);
}

void Graph::print_adjacency_list() const
{
    cout << "Adjacency list:\n";

    for(const auto & [id, node] : m_nodes)
    {
        cout << id << " -> ";

        for(const auto & edge : node.departing_edges)
        {
            cout << edge.destination->id << " (" << edge.weight << "), ";
        }

        cout << "\b\b \n";
    }
}

Path Graph::path(const Node_id src, const Node_id dest)
{
    assert(m_nodes.count(src) && "start node does not exist");
    assert(m_nodes.count(dest) && "destination node does not exist");

    dijkstra(src, dest);

    const Node * node = &m_nodes.at(dest);
    m_path_length = node->distance;
    Path path;

    if(node->previous || src == dest)
    {
        path.push(node->id);

        while(node->previous)
        {
            node = node->previous;
            path.push(node->id);
        }
    }

    return path;
}

struct Node_compare
{
    bool operator()(const Node * left, const Node * right) const
    {
        return left->distance > right->distance;
    }

    bool operator()(const Node & left, const Node & right) const
    {
        return left.distance > right.distance;
    }
};

typedef priority_queue<Node *, vector<Node *>, Node_compare> Node_priority_queue;

void Graph::dijkstra(const Node_id src, const Node_id dest)
{
    assert(m_nodes.count(src) && "start node does not exist");
    assert(m_nodes.count(dest) && "destination node does not exist");

    for(auto & [id, node] : m_nodes)
    {
        node.distance = DISTANCE_MAX;
        node.previous = nullptr;
        node.explored = false;
    }

    mark_blocked_nodes(dest); // exclude nodes from path search, but not the destination node
    // (this is an additional feature and not required for path finding)

    Node_priority_queue frontier;

    Node * node = &m_nodes.at(src);
    node->distance = 0;
    frontier.push(node);

    while(!frontier.empty())
    {
        node = frontier.top();
        frontier.pop();

        // if(node->explored) { continue; } // (*)
        if(node->id == dest) { return; }

        node->explored = true;

        // consider all neighbours of the current node:
        for(const auto & edge : node->departing_edges)
        {
            Node * const neighbour = edge.destination;

            if(neighbour->explored) { continue; } // alternatively use if-statement above (*)

            const Distance new_distance = node->distance + edge.weight;

            if(new_distance < neighbour->distance)
            {
                // a shorter path to that neighbour was found
                neighbour->distance = new_distance;
                neighbour->previous = node;
                frontier.push(neighbour);
            }
        }
    }
}

void Graph::block_nodes(const Node_ids & ids)
{
#ifndef NDEBUG
    for(const Node_id & id : ids)
        assert(m_nodes.count(id) && "node does not exist");
#endif

    m_blocked_nodes = ids;
}

void Graph::mark_blocked_nodes(const Node_id except)
{
    for(const Node_id id : m_blocked_nodes)
    {
        m_nodes.at(id).explored = true;
    }

    m_nodes.at(except).explored = false; // do not block this node
}

Distance Graph::path_length() const
{
    return m_path_length;
}

void Graph::print_nodes() const
{
    cout << "Nodes:\n";

    for(const auto & [id, node] : m_nodes)
    {
        cout << id
            << " : prev = " << (node.previous ? to_string(node.previous->id) : "NULL")
            << ", dist = " << ((node.distance != DISTANCE_MAX) ? to_string(node.distance) : "INF")
            << endl;
    }
}
