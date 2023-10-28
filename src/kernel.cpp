#include "kernel.h"

#include <iostream>
#include <memory>

#include "buffer.h"
#include "graph.h"
#include "ops.h"

namespace kernel {

// TODO: how will broadcasting be handled?

void computeNode(std::shared_ptr<Node> node) {
    const std::string& opt = node->operation_type_;
    if (opt == Operations::TENSOR || opt == Operations::CONSTANT || opt == Operations::NORMAL) {
        constant(node);
    } else if (opt == Operations::MATMUL) {
        matmul(node);
    } else {
        std::cerr << "kernel::computeNode error: unrecognized node operation type " << opt << std::endl;
        exit(-1);
    }
}

// naive implementation
// TODO: make it not naive
void matmul(std::shared_ptr<Node> node) {
    std::shared_ptr<Node> left_node = node->children_[node->arg_order_[0]];
    std::shared_ptr<Node> right_node = node->children_[node->arg_order_[1]];

    int l = left_node->shape_.size();
    int r = right_node->shape_.size();

    std::vector<int> l_index(l, 0);
    std::vector<int> r_index(r, 0);
    std::vector<int> out_index(l, 0);

    // TODO: shapes beyond 2-D

    for (int i = 0; i < left_node->shape_[l - 2]; i++) {
        for (int j = 0; j < right_node->shape_[r - 1]; j++) {
            float dot = 0;
            for (int k = 0; k < right_node->shape_[r - 2]; k++) {
                l_index[l - 2] = i;
                l_index[l - 1] = k;

                r_index[r - 2] = k;
                r_index[r - 1] = j;

                float a = left_node->output_->getIndex<float>(calculateIndex(l_index, left_node->shape_));
                float b = right_node->output_->getIndex<float>(calculateIndex(r_index, right_node->shape_));

                dot += a * b;
            }

            out_index[l - 2] = i;
            out_index[l - 1] = j;

            node->output_->setIndex(calculateIndex(out_index, node->shape_), (void*)(&dot));
        }
    }
}

void constant(std::shared_ptr<Node> node) {
    // pretty sure nothing needs done here
}

}  // namespace kernel
