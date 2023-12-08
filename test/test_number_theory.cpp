#include <iostream>
#include <vector>

#include "test.h"

#include "number_theory.h"

int main() {
  init_test("number theory");
  {
    TEST(gcd<int>(3, 2) == 1);
    TEST(gcd<int>(6, 2) == 2);
    TEST(gcd<int>(6, 9) == 3);
    TEST(gcd<int>(1, 1) == 1);
  }
  {
    TEST(lcm<int>(3, 2) == 6);
    TEST(lcm<int>(6, 4) == 12);
    TEST(lcm<int>(1, 1) == 1);
    TEST(lcm<int>(2, 2) == 2);
  }
  {
    std::vector<int> n1 = {2, 3 , 5, 7};
    //TEST(gcd(n1.begin(), n1.end()) == 1);
    TEST(gcd(n1) == 1);
    //TEST(gcd(n.begin(), n.end()) == 1);

    std::vector<int> n2 = {2, 4 , 6, 10};
    TEST(gcd(n2) == 2);
  }
  {
    std::vector<int> n1 = {2, 3 , 5, 7};
    //TEST(gcd(n1.begin(), n1.end()) == 1);
    TEST(lcm(n1) == 2*3*5*7);
    //TEST(gcd(n.begin(), n.end()) == 1);

    std::vector<int> n2 = {2, 4 , 6, 10};
    TEST(lcm(n2) == 60);
  }

  return report();
}
