#include <cassert>
#include "solvers/bicoloring.h"

using std::vector;

vector<int> BicoloringSolver::findAssignment(input_t const& input) {
  size_t n = input.size();
  vector<int> x(n, -1);
  graph_t g(n);

  for (size_t i = 0; i < n; ++i) {
    g[input[i].first].push_back(input[i].second);
    g[input[i].second].push_back(input[i].first);
  }

  for (size_t i = 0; i < n; ++i) {
    if (x[i] == -1) {
      dfs(g, x, i, 1);
    }
  }

  return x;
}

void BicoloringSolver::dfs(graph_t const& g, vector<int>& val, int u,
                           int color) {

  int next = color ^ 1;
  val[u] = color;

  for (adjlist_t::const_iterator it = g[u].begin(); it != g[u].end(); ++it) {
    if (val[*it] == -1) {
      dfs(g, val, *it, next);
    } else {
      assert(val[*it] == next);
    }
  }
}
