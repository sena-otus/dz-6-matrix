#include "matrix.h"
#include <cassert>
#include <iostream>
#include <ostream>
#include <vector>

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  // NOLINTNEXTLINE(hicpp-named-parameter,readability-named-parameter)
int main(int, char const *[])
{
  {
    std::cout << "Матрица со значением по-умолчанию 0\n";
    using Matrix0 = Matrix<int,0>;
    Matrix0 matrix0;
    for(int ii = 0; ii <= 9; ++ii)
    {
      matrix0(ii, ii) = ii;
      matrix0(ii, 9-ii) = 9-ii;
    }

    for(int ii = 1; ii <= 8; ++ii)
    {
      for(int jj = 1; jj <= 8; ++jj)
      {
        std::cout << (int)matrix0(ii, jj) << " ";
      }
      std::cout << "\n";
    }

    std::cout << "Количество занятых ячеек: " << matrix0.size() << "\n";

    for(auto && mi : matrix0)
    {
      std::cout << "[" << mi.idx().i << "," << mi.idx().j << "] = " << mi.val() << "\n";
    }

    std::cout << "\nДополнительное задание ((matrix[100][100] = 314) = 0) = 217\n";
    ((matrix0(100,100) = 314) = 0) = 217;
    std::cout << "[100,100] = " << (int)matrix0(100,100) << "\n";
  }


  {
    std::cout << "\nМатрица int со значениями по-умолчанию -1\n";
    using Matrix1 = Matrix<int, -1>;
    Matrix1 matrix1;
    assert(matrix1.size() == 0); // все ячейки свободны
    auto a = matrix1(0, 0);
    assert(a == -1);
    assert(matrix1.size() == 0);
    matrix1(100, 100) = 314;
    assert(matrix1(100, 100) == 314);
    assert(matrix1.size() == 1);
    // выведется одна строка
    // 100100314
    for(auto c : matrix1)
    {
      int x = 0;
      int y = 0;
      int v = 0;
      std::tie(x, y, v) = c;
      std::cout << x << y << v << std::endl;
    }
  }

  return 0;
}
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
