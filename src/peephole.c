#include "peephole.h"
#include "ast.h"
#include "flags.h"
#include <stdio.h>
#include <stdlib.h>

// Perform peephole optimization on AST level.
//
// This technique uses a tree rewriting method, some variation of BURS:
// https://en.wikipedia.org/wiki/BURS
//
// The goal is to look for and discard numbers with opposite signs which are
// being added to themselves, i.e `2-2`. The algorithm works as follows:
//
// - Do depth-first search, postorder traversal of AST.
// - Check if this node is not a root node.
// - Check if this node op is opposite of the parent op (one node is `-`, the
// other is `+`).
// - Check if I (this node) have a child op which itself has leaf nodes both
// representing a number.
// - Check if my parent node has the exact same child node with leaf nodes
// representing the same numbers.
// - If so then both these child nodes can be discarded. To do so make my other
// child node become the parent node.
// - Continue this recursively until all nodes are checked.

static node *get_leaf_nodes(node *n);
static char nodes_are_equal(node *node_a, node *node_b);

static node *get_leaf_nodes(node *n) {
  // checks if a given parent only has leaf nodes representing number values.
  // return the node if true.
  if ((n->left != NULL && n->left->type == number_value) &&
      (n->right != NULL && n->right->type == number_value)) {
    return n;
  }
  return NULL;
}

static char nodes_are_equal(node *node_a, node *node_b) {
  if (node_a->type == node_b->type) {
    if (node_a->left->value == node_b->left->value) {
      if (node_a->right->value == node_b->right->value) {
        return 1;
      }
    }
  }
  return 0;
}

void evaluate_ast_and_optimize_peephole(node *this_node, node *parent_node) {
  if (this_node == NULL)
    return;

  // eval child nodes.
  evaluate_ast_and_optimize_peephole(this_node->left, this_node);
  evaluate_ast_and_optimize_peephole(this_node->right, this_node);

  // eval this node.
  // parent not root.
  if (parent_node != NULL) {
    // if my op is opposite of my parents op.
    if ((this_node->type == operator_plus &&
         parent_node->type == operator_minus) ||
        (this_node->type == operator_minus &&
         parent_node->type == operator_plus)) {

      // check child nodes of the parent.
      node *parent_c = get_leaf_nodes(parent_node->left);
      if (parent_c == NULL) {
        parent_c = get_leaf_nodes(parent_node->right);
        // if it's still null then don't continue.
        if (parent_c == NULL)
          return;
      }

      // check child nodes of myself.
      node *this_c = get_leaf_nodes(this_node->left);
      node *new_p = this_node->right;
      if (this_c == NULL) {
        this_c = get_leaf_nodes(this_node->right);
        new_p = this_node->left;
        // if it's still null then don't continue.
        if (this_c == NULL)
          return;
      }

      if (nodes_are_equal(parent_c, this_c)) {
        // make the other child of this_node become the parent node.
        if (flag__print_opt) {
          printf("At %p, discarding nodes with leaf node values %d and %d.\n",
                 this_node, parent_c->left->value, parent_c->right->value);
        }
        *parent_node = *new_p;
      }
    }
  }
}
