#ifndef GRAPH
#define GRAPH

#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include "buffer.h"

class Node;
class Graph;

// probably needs moved

class Node {
   public:
    Node(int id);

    int getId();

    std::string operation_type_;
    std::string name_;

    std::vector<std::string> arg_order_;
    std::map<std::string, std::shared_ptr<Node>> children_;

    std::shared_ptr<Buffer> output_;

    std::vector<int> shape_;

    void printOutput();

   private:
    int id_;

    friend class Graph;
};

class Graph {
   public:
    Graph();

    std::shared_ptr<Node> newNode();

    // TODO: is there other processing we want to do here?
    void newEdge(int from, int to);

    // variables must spawn from an operation
    // there is no declaration operation
    // e.g.
    //      let A = tensor(256, 512); // this is allowed
    //      let B;                    // this is not allowed
    //
    // NOTE: edges_ isn't being updated here
    //       is it needed as a field?
    std::string createVariable(const std::string& name, const std::string& operation_type,
                               const std::vector<std::string>& arguments);

    void createConstant(int constant);

    bool isVariable(const std::string& name);

    // topological sort to compute the graph
    void evaluate();

    void listNodes();

    void printNodeValues();

   private:
    std::map<int, std::set<int>> edges_;          // id -> { neighbor_ids... } outgoing edges
    std::map<int, std::shared_ptr<Node>> nodes_;  // id -> Node*

    std::map<int, std::shared_ptr<Node>> constant_map_;
    std::map<std::string, std::shared_ptr<Node>> variable_map_;

    // this is for when a variable is reassigned
    // e.g. let A = tensor(1, 2)
    //      A = tensor(3, 4)      // this will have a different alias in the graph
    std::map<std::string, std::string> alias_map_;

    int node_index_ = 0;
};

#endif
