/**
 * @file gtest_matrix.cpp
 *
 * @brief test matrix
 *  */

#include "matrix.h"
#include <gtest/gtest.h>


namespace {

   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  TEST(Google_test, matrix) {
    {
      using Matrix1 = Matrix<int, -1>;
      Matrix1 matrix1;
      EXPECT_EQ(matrix1.size(), 0);
      auto a = matrix1(0, 0);
      EXPECT_EQ(a, -1);
      EXPECT_EQ(matrix1.size(), 0);
      matrix1(100, 100) = 314;
      EXPECT_EQ(matrix1(100, 100), 314);
      EXPECT_EQ(matrix1.size(), 1);
      matrix1(101, 102) = 315;
      EXPECT_EQ(matrix1(101, 102), 315);
      EXPECT_EQ(matrix1.size(), 2);
      matrix1(101, 102) = -1;
      EXPECT_EQ(matrix1(101, 102), -1);
      EXPECT_EQ(matrix1.size(), 1);
    }
  }

    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}
