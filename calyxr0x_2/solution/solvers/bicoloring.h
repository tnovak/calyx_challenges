#include "common.h"

class BicoloringSolver : public Solver {
protected:
  typedef std::vector<int> adjlist_t;
  typedef std::vector<adjlist_t> graph_t;

  virtual std::vector<int> findAssignment(input_t const& input);

private:
  void dfs(graph_t const& g, std::vector<int>& val, int u, int color);
};
