#include "matrix.h"
#include <ostream>

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  // NOLINTNEXTLINE(hicpp-named-parameter,readability-named-parameter)
int main(int, char const *[])
{
    // бесконечная матрица int заполнена значениями -1
  Matrix<int, -1> matrix;
  assert(matrix.size() == 0); // все ячейки свободны
  auto a = matrix(0, 0);
  assert(a == -1);
  assert(matrix.size() == 0);
  matrix(100, 100) = 314;
  assert(matrix(100, 100) == 314);
  assert(matrix.size() == 1);
// выведется одна строка
// 100100314
  for(auto && c: matrix)
  {
    // int x;
    // int y;
    // int v;
//    std::tie(x, y, v) = c;
//    std::cout << x << y << v << std::endl;
    std::cout << c.first << ": "  << c.second << std::endl;
  }
  return 0;
}
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
