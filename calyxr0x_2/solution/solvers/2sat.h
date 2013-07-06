#include <set>
#include "common.h"

class SatSolver : public Solver {
protected:
  typedef std::vector<int> adjlist_t;
  typedef std::vector<adjlist_t> graph_t;
  typedef std::set<int> component_t;

  virtual std::vector<int> findAssignment(input_t const& input);

private:
  void dfs(graph_t const& g, std::vector<int>& used, component_t& c, int u);

  int toNode(int var, bool negate) {
    return 2*var + negate;
  }
  int toVar(int node) {
    return node / 2;
  }
  bool isNegated(int node) {
    return node & 1;
  }
};
