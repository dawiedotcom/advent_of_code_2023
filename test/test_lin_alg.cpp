#include <iostream>
#include <vector>

#include "test.h"
#include "lib.h"

#include "lin_alg.h"

int main() {
  init_test("lin alg");
  {
    TEST(1==1); 
    matrix_t<double> m(2,2);
    std::valarray<double> x(0.0, 2), a(0.0, 2);
    m[0,0] = 2.0;
    m[1,1] = 3.0;
    x[0] = 2.0;
    x[1] = 4.0;

    a = m*x;
    TEST(a[0] == 2.0 * x[0]);
    TEST(a[1] == 3.0 * x[1]);

    a = x*m;
    TEST(a[0] == 2.0 *x[0]);
    TEST(a[1] == 3.0 *x[1]);

    m[1,0] = 4.0;
    m[0,1] = 5.0;

    a = m*x;
    TEST(a[0] == 2.0 * x[0] + 5.0 * x[1]);
    TEST(a[1] == 4.0 * x[0] + 3.0 * x[1]);

    a = x*m;
    TEST(a[0] == 2.0 * x[0] + 4.0 * x[1]);
    TEST(a[1] == 5.0 * x[0] + 3.0 * x[1]);
  }
  {
    matrix_t<double> A(2,2);
    std::valarray<double> x(0.0, 2), x0(0.0, 2), b(0.0, 2);

    A[0, 0] = 4; 
    A[0, 1] = 1;
    A[1, 0] = 1;
    A[1, 1] = 3;

    x0[0] = 2;
    x0[1] = 1;

    b[0] = 1;
    b[1] = 2;

    x = conjugate_gradient(A, x0, b, 1e-14);

    //SHOW(x[0], x[1]);
    TEST(std::abs(x[0] - 1.0/11.0) < 1e-4);
    TEST(std::abs(x[1] - 7.0/11.0) < 1e-4);
  }
  {
    matrix_t<double> A(2,2);
    std::valarray<double> x(0.0, 2), b(0.0, 2);

    A[0, 0] = 4; 
    A[0, 1] = 1;
    A[1, 0] = 1;
    A[1, 1] = 3;

    b[0] = 1;
    b[1] = 2;

    //x = conjugate_gradient(A, x0, b, 1e-14);
    LUPdecompose_t lu(A);
    lu.solve(x, b);

    //SHOW(x[0], x[1]);
    TEST(std::abs(x[0] - 1.0/11.0) < 1e-4);
    TEST(std::abs(x[1] - 7.0/11.0) < 1e-4);
  }



  return report();
}
