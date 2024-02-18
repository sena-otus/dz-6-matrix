#include "matrix.h"
#include <cassert>
#include <iostream>
#include <ostream>

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  // NOLINTNEXTLINE(hicpp-named-parameter,readability-named-parameter)
int main(int, char const *[])
{
    // бесконечная матрица int заполнена значениями -1
  using MatrixOne = Matrix<int, -1>;
  MatrixOne matrix;
  assert(matrix.size() == 0); // все ячейки свободны
  auto a = matrix(0, 0);
  assert(a == -1);
  assert(matrix.size() == 0);
  matrix(100, 100) = 314;
  assert(matrix(100, 100) == 314);
  assert(matrix.size() == 1);
// выведется одна строка
// 100100314
    // for(auto c: matrix)
  for(auto c : matrix)
  {
    int x = 0;
    int y = 0;
    int v;
    std::tie(x, y, v) = c;
    std::cout << x << y << v << std::endl;
  }
  return 0;
}
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
