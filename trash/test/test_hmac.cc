#include <iostream>
#include "jwt/algorithm.hpp"

void basic_hmac_test()
{
  std::string_view sv = "secret" ;
  std::string_view d = "Some random data string";
  auto res = jwt::HMACSign<jwt::algo::HS256>::sign(sv, d);

  std::cout << res.first << std::endl;
}

int main() {
  basic_hmac_test();
  return 0;
}
