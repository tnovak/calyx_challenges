#include <cassert>
#include "solvers/equations.h"

using std::vector;
using std::swap;

vector<int> EquationsSolver::findAssignment(input_t const& input) {
  size_t n = input.size();

  matrix_t m(n);
  vector<int> b(n);

  for (size_t i = 0; i < n; ++i) {
    m[i].resize(n);
    m[i][input[i].first] = m[i][input[i].second] = 1;

    b[i] = 1;
  }

  return gaussianElimination(m, b);
}

std::vector<int>
EquationsSolver::gaussianElimination(matrix_t& m, std::vector<int>& b) {
  size_t n = m.size();

  // row reduction
  for (size_t p = 0, c = 0; c < n; ++c) {
    size_t row = -1;

    for (size_t i = p; i < n; ++i) {
      if (m[i][c] > 0) {
        row = i;
        break;
      }
    }

    if (row == (size_t)-1) continue;
    if (row != p) {
      swap(m[row], m[p]);
      swap(b[row], b[p]);
    }

    for (size_t i = p+1; i < n; ++i) {
      if (m[i][c] == 0) continue;

      b[i] ^= b[p];
      for (size_t j = c; j < n; ++j)
        m[i][j] ^= m[p][j];
    }

    ++p;
  }

  // back substitution
  std::vector<int> res(n, 0);
  for (int i = n-1; i >= 0; --i) {
    int k = -1, x = b[i];
    
    for (size_t j = 0; j < n; ++j) {
      if (m[i][j] == 0) continue;
      if (k == -1) k = j; else x ^= res[j];
    }

    if (k != -1) res[k] = x; else assert(x == 0);
  }
  
  return res;
}
