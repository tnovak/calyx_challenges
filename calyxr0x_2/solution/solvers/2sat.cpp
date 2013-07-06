#include "solvers/2sat.h"

using std::vector;

vector<int> SatSolver::findAssignment(input_t const& input) {
  size_t n = input.size();
  graph_t g(2*n);

  for (size_t i = 0; i < n; ++i) {
    int u = input[i].first, v = input[i].second;

    int A = toNode(u, false), negA = toNode(u, true);
    int B = toNode(v, false), negB = toNode(v, true);

    g[A].push_back(negB);
    g[B].push_back(negA);
    g[negA].push_back(B);
    g[negB].push_back(A);
  }

  vector<int> used(g.size(), 0);
  vector<int> x(n, -1);

  // since the implication graph is undirected, DFS can be used to find
  // connected components
  for (size_t i = 0; i < g.size(); ++i) {
    if (!used[i]) {
      component_t c;
      dfs(g, used, c, i);

      for (component_t::const_iterator it = c.begin(); it != c.end(); ++it) {
        int u = toVar(*it);

        if (x[u] != -1) break;
        x[u] = !isNegated(*it);
      }
    }
  }

  return x;
}

void SatSolver::dfs(graph_t const& g, vector<int>& used,
                    component_t& c, int u) {

  used[u] = 1;
  c.insert(u);

  for (adjlist_t::const_iterator it = g[u].begin(); it != g[u].end(); ++it) {
    if (!used[*it]) {
      dfs(g, used, c, *it);
    }
  }
}
