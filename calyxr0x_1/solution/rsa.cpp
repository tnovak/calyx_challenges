#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <limits>
#include <stdint.h>

#include "gmpxx.h"

using namespace std;

bool factorize(uint64_t x, uint64_t& p, uint64_t& q) {
  for (uint32_t y = numeric_limits<uint32_t>::max(); y >= 2; --y) {
    if (x % y == 0) {
      p = y; q = x / y;
      return true;
    }
  }

  return false;
}

inline mpz_class big(uint64_t x) {
  char buf[64]; sprintf(buf, "%llu", x);
  return mpz_class(buf, 10);
}

void mod_inv(uint64_t a, uint64_t n, mpz_class& x, mpz_class& y) {
  if (n == 0) {
    x = 1; y = 0;
    return;
  }

  mod_inv(n, a % n, x, y);

  mpz_class t = x;
  x = y;
  y = t - big(a/n)*y;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage %s modulus exponent\n", argv[0]);
    return EXIT_FAILURE;
  }

  uint64_t n, e;

  n = strtoull(argv[1], NULL, 16);
  e = strtoull(argv[2], NULL, 16);

  fprintf(stderr, "Factorizing 0x%llx...\n", n);

  uint64_t p, q;
  assert(factorize(n, p, q));

  fprintf(stderr, "p   = 0x%llx\n", p);
  fprintf(stderr, "q   = 0x%llx\n", q);

  uint64_t phi = (p-1) * (q-1);
  fprintf(stderr, "phi = 0x%llx\n", phi);

  // Calculate decription key
  mpz_class x, y;
  mod_inv(e, phi, x, y);
  if (x < 0) {
    mpz_class phi_ = big(phi);
    x = (x + phi_) % phi_;
  }

  fprintf(stderr, "d   = 0x%s\n", x.get_str(16).c_str());

  return EXIT_SUCCESS;
}
