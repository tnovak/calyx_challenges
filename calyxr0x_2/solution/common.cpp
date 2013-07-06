#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <memory>

#include "common.h"
#include "solvers/bicoloring.h"
#include "solvers/equations.h"
#include "solvers/2sat.h"

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::cerr;

Solver::input_t Solver::decodeInput(string const& input) {
  vector<int> x;
  int cur = 0, p = 0;

  assert((input.size() & 1) == 0);
  for (size_t i = 0; i < input.size(); i += 2) {
    int val;

    sscanf(input.substr(i, 2).c_str(), "%x", &val);
    cur |= (val << p);
    p += 8;

    while (p >= BITS_PER_NUM) {
      x.push_back(cur & MASK);

      p -= BITS_PER_NUM;
      cur >>= BITS_PER_NUM;
    }
  }

  assert((x.size() & 1) == 0);

  Solver::input_t decoded;
  for (size_t i = 0; i < x.size(); i += 2) {
    decoded.push_back(std::make_pair(x[i], x[i+1]));
  }

  return decoded;
}

bool Solver::isValid(input_t const& input, vector<int> const& assignment) {
  if (input.size() != assignment.size()) {
    return false;
  }

  for (input_t::const_iterator it = input.begin(); it != input.end(); ++it) {
    if ((assignment[it->first] ^ assignment[it->second]) == 0) {
      return false;
    }
  }

  return true;
}

vector<int> Solver::solve(string const& input) {
  input_t decoded = decodeInput(input);

  vector<int> res = findAssignment(decoded);
  assert(isValid(decoded, res));

  return res;
}

Solver *getSolver(string const& algorithm) {
  if (algorithm == "bicoloring") {
    return new BicoloringSolver();
  } else if (algorithm == "equations") {
    return new EquationsSolver();
  } else if (algorithm == "2sat") {
    return new SatSolver();
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " input [bicoloring|equations|2sat]\n";
    return EXIT_FAILURE;
  }

  string algo;
  if (argc > 2) {
    algo = argv[2];
  } else {
    algo = "bicoloring";
  }

  std::auto_ptr<Solver> solver(getSolver(algo));
  if (solver.get() == NULL) {
    cerr << "Invalid algorithm\n";
    return EXIT_FAILURE;
  }

  vector<int> result = solver->solve(argv[1]);
  std::copy(result.begin(), result.end(),
            std::ostream_iterator<int>(cout, ""));
  cout << "\n";

  return EXIT_SUCCESS;
}

