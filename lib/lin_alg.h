#pragma once

#include <ostream>
#include <iostream>
#include <iomanip>
#include <valarray>
#include <cassert>

#include "lib.h"

/*
 * Printing for std::valarray.
 */
template<typename ValType>
std::ostream& operator<<(std::ostream& out, const std::valarray<ValType>& arr) {
  out << "[";
  size_t i=0, size = arr.size();
  for (auto const& x : arr) {
    out << x;
    if (i++ < size - 1)
      std::cout << ", ";
  }
  out << "]";
  return out;
}

/* 
 * Basic class to represent a matrix. Provides operators for multiplying from left and right
 * by valarrays of the approprate size.
 *
 * See: 2023 day 24
 */
template <typename ValType>
class matrix_t {
private:
  size_t n_rows, n_cols;
  std::valarray<ValType> m;

  constexpr size_t to_index(const size_t r, const size_t c) const {
    return r + n_rows*c;
  }
public:
  matrix_t(size_t nr, size_t nc)
    : n_rows(nr)
    , n_cols(nc)
    , m(0.0, nr*nc)
  {

  }

  matrix_t(const matrix_t<ValType>& other) 
    : n_rows(other.n_rows)
    , n_cols(other.n_cols)
    , m(other.m)
  {
  }

  ValType& operator[](const size_t r, const size_t c) {
    return m[to_index(r, c)];
  }

  const ValType& operator[](const size_t r, const size_t c) const {
    return m[to_index(r, c)];
  }

  constexpr size_t get_n_rows() const {
    return n_rows;
  }
  constexpr size_t get_n_cols() const {
    return n_cols;
  }
};

/*
 * Multiply A*x with x a column vector.
 */
template <typename ValType>
std::valarray<ValType> operator*(const matrix_t<ValType>& m, const std::valarray<ValType>& v) {
  assert(m.get_n_cols() == v.size());
  std::valarray<ValType> result(0.0, m.get_n_rows());
  for (size_t c=0; c<m.get_n_cols(); c++) {
    for (size_t r=0; r<m.get_n_rows(); r++) {
      result[r] += m[r, c] * v[c];
    }
  }
  return result;
}

/*
 * Multiply x*A with x a row vector.
 */
template <typename ValType>
std::valarray<ValType> operator*(const std::valarray<ValType> v, const matrix_t<ValType>& m) {
  assert(m.get_n_rows() == v.size());
  std::valarray<ValType> result(0.0, m.get_n_cols());
  for (size_t c=0; c<m.get_n_cols(); c++) {
    for (size_t r=0; r<m.get_n_rows(); r++) {
      result[c] += v[r] * m[r, c];
    }
  }
  return result;
}

template <typename ValType>
std::ostream& operator<<(std::ostream& out, const matrix_t<ValType> m) {
  for (size_t r=0; r<m.get_n_rows(); r++) {
    out << "| ";
    for (size_t c=0; c<m.get_n_cols(); c++) {
      out << std::setw(7) << m[r, c];
      //if (c < m.get_n_cols() - 2
    }
    out << "|\n";
  }
  return out;
}

/*
 * Solves a linear system Ax=b for known A and B and intial guess x using the conjugate gradient method.
 * See https://en.wikipedia.org/wiki/Conjugate_gradient_method
 * TODO: Needs more testing
 */
template <typename ValType>
std::valarray<ValType> conjugate_gradient(matrix_t<ValType>& A, std::valarray<ValType>& x, std::valarray<ValType>& b, ValType tol=1e-10) {
  assert(x.size() == b.size());
  assert(A.get_n_cols() == A.get_n_rows());
  assert(A.get_n_cols() == x.size());

  std::valarray<ValType> residual = b - A * x;
  std::valarray<ValType> search_direction = residual;
  ValType old_sqr_resid_norm = (residual*residual).sum();
  std::valarray<ValType> result = x; //(0.0, x.size());

  size_t i=0;
  while (i++ < 20 && old_sqr_resid_norm > tol) {
    std::valarray<ValType> A_search_dir = A * search_direction;
    ValType step_size = old_sqr_resid_norm / (search_direction * A_search_dir).sum();
    result += step_size * search_direction;
    residual -= step_size * A_search_dir;
    ValType new_sqr_resid_norm = (residual * residual).sum();

    ValType sqr_resid_norm_ratio = new_sqr_resid_norm / old_sqr_resid_norm;

    search_direction = residual + sqr_resid_norm_ratio * search_direction;

    old_sqr_resid_norm = new_sqr_resid_norm;
    //SHOW(i, old_sqr_resid_norm);
  }
  return result;
}

/*
 * Class to represent the LUP decomposition of a square matrix. 
 * See: 
 *   + https://en.wikipedia.org/wiki/LU_decomposition
 *   + Press, Teukolsky, Vetterling, Flannery (2007) Section 2.3, Numerical Recipes.
 * Used:
 *   + AoC 2023 day 24
 */
template<typename ValType>
class LUPdecompose_t {
private:
  size_t N;
  //matrix_t<ValType>& m;
  matrix_t<ValType> lu;
  std::valarray<int> perm;
  ValType d;

  void decompose() {
    int i, j, k, imax;
    ValType tmp, max;
    std::valarray<ValType> scale(1.0, N);

    for (i=0; i<N; i++) {
      perm[i] = i;
      max = 0.0;
      for (j=0; j<N; j++) 
        if ((tmp=std::abs(lu[i, j])) > max)
          max = tmp;
      if (max == 0.0)
        throw("LUPdecompose_t: Singular matrix");
      scale[i] = 1.0/max;
    }

    for (k=0; k<N; k++) {
      max = 0.0;
      for (i=k; i<N; i++) {
        tmp = scale[i] * std::abs(lu[i, k]);
        if (tmp > max) {
          max = tmp;
          imax = i;
        }
      }

      if (k != imax) {
        for (j=0; j<N; j++) {
          tmp = lu[imax, j];
          lu[imax, j] = lu[k, j];
          lu[k, j] = tmp;
        }

        scale[imax] = scale[k];
      }

      perm[k] = imax;

      for (i=k+1; i<N; i++) {
        tmp = lu[i, k] /= lu[k, k];
        for (j=k+1; j<N; j++)
          lu[i, j] -= tmp * lu[k, j];
      }
    }
  }
public:

  LUPdecompose_t(const matrix_t<ValType>& m) 
    : N(m.get_n_cols())
    , lu(m)
    , perm(0, N)
    , d(1.0)
  {
    assert(m.get_n_cols() == m.get_n_rows());
    decompose();
  }

  /* Solves the linear system m * x = b  */
  void solve(std::valarray<ValType>& x, const std::valarray<ValType>& b) {
    int i, j, ii=0, ip;
    ValType sum;

    for (i=0; i<N; i++) 
      x[i] = b[i];
    for (i=0; i<N; i++) {
      ip = perm[i];
      sum = x[ip];
      x[ip] = x[i];
      if (ii != 0)
        for (j=ii-1; j<i; j++)
          sum -= lu[i, j] * x[j];
      else if (sum != 0.0)
        ii = i+1;
      x[i] = sum;
    }
    for (i=N-1; i>=0; i--) {
      sum=x[i];
      for (j=i+1; j<N; j++) 
        sum -= lu[i, j] * x[j];
      x[i] = sum/lu[i, i];
    }
  }
};
