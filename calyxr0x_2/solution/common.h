#ifndef __COMMON_H
#define __COMMON_H

#include <vector>
#include <string>

class Solver {
public:
  std::vector<int> solve(std::string const& input);

protected:
  typedef std::vector< std::pair<int, int> > input_t;

  virtual std::vector<int> findAssignment(input_t const& input) = 0;
private:
  input_t decodeInput(std::string const& input);
  bool isValid(input_t const& input, std::vector<int> const& assignment);

  const static int BITS_PER_NUM = 6;
  const static int MASK = (1 << BITS_PER_NUM) - 1;
};

#endif
