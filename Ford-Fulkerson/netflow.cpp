#include "netflow.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <queue>

#define TOO_FEW_VERTICES "Too few vertices."
#define TOO_FEW_EDGES "Too few edges."
#define EDGE_WEIGHT_ZERO "Detected edge weight of 0."
#define EDGE_BAD_ENDPOINT "Edge interacts with nonexistent vertex."
#define SELF_LOOP "At least one self-loop."
#define MULTI_EDGES "Detected multi-edges."
#define NOT_ONE_SRC "Zero or more than one source."
#define NOT_ONE_SINK "Zero or more than one sink."

// Breadth first search
bool bfs(std::vector<std::vector<unsigned>>& adj_matrix, unsigned& source,
         unsigned& sink, std::unordered_map<unsigned, unsigned>& parent)
{
    // Take size of the given vector
    unsigned size = adj_matrix.size();
    // Initialise discovered vector
    std::vector<bool> discovered(size);
    // Mark all values in the vector as false
    for (unsigned i = 0; i < size; i++)
    {
        discovered[i] = false;
    }
    // Initialise queue
    std::queue<unsigned> q_bfs;
    // Mark the source node as true
    discovered[source] = true;
    // Mark the parent of the source node as -1 as the source node has no
    // parents
    parent[source] = -1;
    // Push the source node onto the queue
    q_bfs.push(source);
    // While the queue is not empty
    while (!q_bfs.empty())
    {
        // Take the current node as the top node of the queue
        unsigned current_node = q_bfs.front();
        // Pop of the top node
        q_bfs.pop();
        // Go through the adjacency matrix
        for (unsigned i = 0; i < size; i++) {
            // Take the weight of that node
            unsigned weight = adj_matrix[current_node][i];
            // If the index is marked as false
            if (discovered[i] == false && weight > 0)
            {
                // Push that node onto the queue
                q_bfs.push(i);
                // Parent of that node will become what is the current node
                // as per this iteration
                parent[i] = current_node;
                // Mark that node as visited
                discovered[i] = true;
            }
        }
    }
    // Return the resultant value of the sink
    return (discovered[sink]);
}

std::vector<Edge> solveNetworkFlow(
    const std::vector<Edge>& flowNetwork,
    unsigned numVertices) {
    // Taking care of all error cases
    if (numVertices < 2) {
        throw std::runtime_error(TOO_FEW_VERTICES);
    } else if (flowNetwork.size() == 0) {
        throw std::runtime_error(TOO_FEW_EDGES);
    }
    for (unsigned i = 0; i < flowNetwork.size(); i++) {
        if (flowNetwork[i].weight == 0) {
            throw std::runtime_error(EDGE_WEIGHT_ZERO);
        } else if (flowNetwork[i].to > (numVertices - 1) || flowNetwork[i].from > (numVertices - 1)) {
            throw std::runtime_error(EDGE_BAD_ENDPOINT);
        } else if (flowNetwork[i].to == flowNetwork[i].from) {
            throw std::runtime_error(SELF_LOOP);
        }
        for (unsigned j = i + 1; j < flowNetwork.size(); j++) {
            if ((flowNetwork[i].to == flowNetwork[j].to) && (flowNetwork[i].from == flowNetwork[j].from)) {
                throw std::runtime_error(MULTI_EDGES);
            }
        }
    }

    // Initialise nodes matrices for further usage and initialise source and sink
    std::vector<unsigned> source_nodes;
    std::vector<unsigned> neighbor_nodes;
    unsigned sink, source;
    unsigned sink_c = 0;
    // Take all the values from the provided vector and put them
    // into the source nodes vector
    for (unsigned i = 0; i < flowNetwork.size(); i++) {
        if (std::find(source_nodes.begin(), source_nodes.end(),
                      flowNetwork[i].from) == source_nodes.end()) {
            source_nodes.push_back(flowNetwork[i].from);
        } else
            continue;
    }
    // Account for all the neighbor nodes and put them into the vector of
    // the same name
    for (unsigned i = 0; i < flowNetwork.size(); i++) {
        if (std::find(neighbor_nodes.begin(), neighbor_nodes.end(),
                      flowNetwork[i].to) == neighbor_nodes.end()) {
            neighbor_nodes.push_back(flowNetwork[i].to);
        } else
            continue;
    }
    // Identify the source
    for (unsigned i = 0; i < neighbor_nodes.size(); i++) {
        if (std::find(neighbor_nodes.begin(), neighbor_nodes.end(),
                      source_nodes[i]) == neighbor_nodes.end()) {
            source = source_nodes[i];
        }
    }
    // Identify the sink
    for (unsigned i = 0; i < neighbor_nodes.size(); i++) {
        if (std::find(source_nodes.begin(), source_nodes.end(),
                      neighbor_nodes[i]) == source_nodes.end()) {
            sink = neighbor_nodes[i];
            sink_c++;
        }
    }
    // Further error handling
    if (source_nodes.size() > neighbor_nodes.size()) {
        throw std::runtime_error(NOT_ONE_SRC);
    } else if (sink_c != 1) {
        throw std::runtime_error(NOT_ONE_SINK);
    }
    // Initialise vectors
    std::vector <std::vector<unsigned>> adjacency_matrix;
    std::vector <std::vector<unsigned>> residual_graph;
    std::vector <std::vector<unsigned>> max_flow;
    for (unsigned i = 0; i < numVertices; i++) {
        std::vector<unsigned> row;
        adjacency_matrix.push_back(row);
        for (unsigned j = 0; j < numVertices; j++) {
            adjacency_matrix[i].push_back(0);
        }
    }
    for (unsigned i = 0; i < numVertices; i++) {
        std::vector<unsigned> row;
        max_flow.push_back(row);
        for (unsigned j = 0; j < numVertices; j++) {
            max_flow[i].push_back(0);
        }
    }
    // Take weights from the given vector and port them into the adjacency matrix
    for (unsigned i = 0; i < flowNetwork.size(); i++) {
        adjacency_matrix[flowNetwork[i].from][flowNetwork[i].to] = \
        flowNetwork[i].weight;
    }
    residual_graph = adjacency_matrix;
    // Create a map
    std::unordered_map<unsigned, unsigned> parent;
    // Run bfs constantly till all solutions aren't found
    while (bfs(residual_graph, source, sink, parent))
    {
        // Create a new vector to identify paths
        std::vector<unsigned> path;
        // This takes the source value and first actual path received from the
        // bfs function
        path.push_back(parent.begin()->first);
        path.push_back(parent.begin()->second);
        // Take all the paths from the parent map and put them in the vector
        // in such a way that the graphs can be visualised
        for (auto &x: parent) {
            // Checks done to verify that we are taking the right paths
            // And the value is there so that we can make sure there
            // aren't any invalid paths in the mix
            if (x.first == path.back() && x.second < 4294967295) {
                path.push_back(x.second);
            }
        }
        if (path.back() != source)
        {
            break;
        }
        // Reverse the vector as the current vector is in the reverse order of
        // what it should be, being from sink to source
        std::reverse(path.begin(), path.end());
        // Vector to account for path weights
        std::vector<unsigned> path_weight;
        // Record for the path weights from the paths we just figured out above
        for (unsigned i = 0; i < path.size() - 1; i++) {
            path_weight.push_back(residual_graph[path[i]][path[i + 1]]);
        }
        // Variable for bottleneck
        unsigned bottleneck = 0;
        // Take the path that is of the least value in the path vector
        bottleneck = *min_element(path_weight.begin(), path_weight.end());
        // Adjust all the vectors according to the bottleneck
        for (unsigned i = 0; i < path.size() - 1; i++) {
            residual_graph[path[i]][path[i + 1]] -= bottleneck;
            residual_graph[path[i + 1]][path[i]] += bottleneck;
            max_flow[path[i]][path[i + 1]] += bottleneck;
        }
    }
    // Put all of this into a result vector and return it
    std::vector<Edge> result;
    for (unsigned i = 0; i < max_flow.size(); i++)
    {
        for (unsigned j = 0; j < max_flow.size(); j++)
        {
            if (max_flow[i][j] != 0)
            {
                Edge temp = Edge(i, j, max_flow[i][j]);
                bruh.push_back(temp);
            }
        }
    }

    return {result};
}