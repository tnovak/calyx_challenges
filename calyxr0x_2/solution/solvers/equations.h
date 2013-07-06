#include "common.h"

class EquationsSolver : public Solver {
protected:
  virtual std::vector<int> findAssignment(input_t const& input);

 private:
  typedef std::vector< std::vector<int> > matrix_t;

  std::vector<int> gaussianElimination(matrix_t& m, std::vector<int>& b);
};
