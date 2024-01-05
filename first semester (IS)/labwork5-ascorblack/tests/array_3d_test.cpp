#include <gtest/gtest.h>
#include <cmath>

#include <lib/Array3D.h>


TEST(Array3DTest, AccessOperator) {
  Array3D array_3d = Array3D::make_array(1, 1, 1);

  ASSERT_EQ(array_3d[0][0][0], uint32_t(0));
}

TEST(Array3DTest, AssignmentOperator) {
  Array3D array_3d = Array3D::make_array(1, 1, 2);
  array_3d[0][0][0] = 20;
  array_3d[0][0][1] = array_3d[0][0][0];
  array_3d[0][0][0] = 534;

  ASSERT_EQ(array_3d[0][0][0], uint32_t(534));
  ASSERT_EQ(array_3d[0][0][1], uint32_t(20));
}

TEST(Array3DTest, ComplexAssigmentOperator) {
  int32_t depth = 4;
  int32_t rows = 20;
  int32_t cols = 20;

  Array3D array_3d = Array3D::make_array(depth, rows, cols);

  uint32_t cur_number = 0;

  for (int i = 0; i < depth; ++i) {
    for (int j = 0; j < rows; ++j) {
      for (int k = 0; k < cols; ++k) {
        array_3d[i][j][k] = cur_number;
        ++cur_number;
      }
    }
  }

  cur_number = 0;
  for (int i = 0; i < depth; ++i) {
    for (int j = 0; j < rows; ++j) {
      for (int k = 0; k < cols; ++k) {
        ASSERT_EQ(array_3d[i][j][k], cur_number);
        ++cur_number;
      }
    }
  }
}


TEST(Array3DTest, BoundaryNumber) {
  Array3D array_3d = Array3D::make_array(1, 1, 1);
  array_3d[0][0][0] = static_cast<uint32_t>(std::pow(2, 17) - 1);

  ASSERT_EQ(array_3d[0][0][0], static_cast<uint32_t>(std::pow(2, 17) - 1));
}

TEST(Array3DTest, OverFlow) {
  Array3D array_3d = Array3D::make_array(1, 1, 2);

  array_3d[0][0][0] = static_cast<uint32_t>(std::pow(2, 17));
  array_3d[0][0][1] = static_cast<uint32_t>(std::pow(2, 17) + 2);

  ASSERT_EQ(array_3d[0][0][0], uint32_t(0));
  ASSERT_EQ(array_3d[0][0][1], uint32_t(2));
}

TEST(Array3DTest, BadArrayNewLength) {
  ASSERT_THROW(Array3D::make_array(-1, 1, 1), std::bad_array_new_length);
  ASSERT_THROW(Array3D::make_array(1, -1, 1), std::bad_array_new_length);
  ASSERT_THROW(Array3D::make_array(1, 1, -1), std::bad_array_new_length);
  ASSERT_THROW(Array3D::make_array(10000, 10000, 10000), std::bad_array_new_length);
}


TEST(Array3DTest, OutOfRange) {
  int32_t depth = 10;
  int32_t rows = 10;
  int32_t cols = 10;

  Array3D array_3d = Array3D::make_array(depth, rows, cols);

  ASSERT_THROW(array_3d[0][-1][0], std::out_of_range);
  ASSERT_THROW(array_3d[-1][-1][-1], std::out_of_range);
  ASSERT_THROW(array_3d[0][-1][-1], std::out_of_range);
  ASSERT_THROW(array_3d[0][0][-1], std::out_of_range);
  ASSERT_THROW(array_3d[11][0][0], std::out_of_range);
}


TEST(Array3DTest, Input) {
  int32_t depth = 5;
  int32_t rows = 1;
  int32_t cols = 1;

  Array3D array_3d = Array3D::make_array(depth, rows, cols);

  std::string test_input = "34534 23454 9653 1254 2054";
  std::stringstream input_stream(test_input);

  int32_t index = 0;

  while (!input_stream.eof()) {
    input_stream >> array_3d[index++][0][0];
  }

  ASSERT_EQ(array_3d[0][0][0], uint32_t(34534));
  ASSERT_EQ(array_3d[1][0][0], uint32_t(23454));
  ASSERT_EQ(array_3d[2][0][0], uint32_t(9653));
  ASSERT_EQ(array_3d[3][0][0], uint32_t(1254));
  ASSERT_EQ(array_3d[4][0][0], uint32_t(2054));

}


TEST(Array3DTest, MultArray3DByNumber) {
  int32_t depth = 20;
  int32_t rows = 20;
  int32_t cols = 20;

  Array3D array_3d = Array3D::make_array(depth, rows, cols);

  uint32_t cur_number = 0;
  uint32_t multiplier = 15;

  for (int i = 0; i < depth; ++i) {
    for (int j = 0; j < rows; ++j) {
      for (int k = 0; k < cols; ++k) {
        array_3d[i][j][k] = cur_number;
        ++cur_number;
      }
    }
  }

  Array3D new_array_3d = array_3d * multiplier;

  for (int i = depth - 1; i >= 0; --i) {
    for (int j = rows - 1; j >= 0; --j) {
      for (int k = cols - 1; k >= 0; --k) {
        --cur_number;

        ASSERT_EQ(new_array_3d[i][j][k], cur_number * multiplier);
      }
    }
  }
}


TEST(Array3DTest, SumArrays3D) {
  int32_t depth = 20;
  int32_t rows = 20;
  int32_t cols = 20;
  uint32_t size = depth * rows * cols;

  Array3D array_3d_1 = Array3D::make_array(depth, rows, cols);
  Array3D array_3d_2 = Array3D::make_array(depth, rows, cols);

  uint32_t cur_number = 0;

  for (int i = 0; i < depth; ++i) {
    for (int j = 0; j < rows; ++j) {
      for (int k = 0; k < cols; ++k) {
        array_3d_1[i][j][k] = cur_number;
        array_3d_2[i][j][k] = size - cur_number;
        ++cur_number;
      }
    }
  }

  Array3D new_array_3d = array_3d_1 + array_3d_2;

  for (int i = depth - 1; i >= 0; --i) {
    for (int j = rows - 1; j >= 0; --j) {
      for (int k = cols - 1; k >= 0; --k) {
        --cur_number;

        ASSERT_EQ(new_array_3d[i][j][k], size);
      }
    }
  }
}


TEST(Array3DTest, SubstArrays3D) {
  int32_t depth = 31;
  int32_t rows = 31;
  int32_t cols = 31;
  uint32_t size = depth * rows * cols;

  Array3D array_3d_1 = Array3D::make_array(depth, rows, cols);
  Array3D array_3d_2 = Array3D::make_array(depth, rows, cols);

  uint32_t cur_number = 0;

  for (int i = 0; i < depth; ++i) {
    for (int j = 0; j < rows; ++j) {
      for (int k = 0; k < cols; ++k) {
        array_3d_2[i][j][k] = cur_number;
        array_3d_1[i][j][k] = size + cur_number;
        ++cur_number;
      }
    }
  }

  Array3D new_array_3d = array_3d_1 - array_3d_2;

  for (int i = depth - 1; i >= 0; --i) {
    for (int j = rows - 1; j >= 0; --j) {
      for (int k = cols - 1; k >= 0; --k) {
        --cur_number;

        ASSERT_EQ(new_array_3d[i][j][k], size);
      }
    }
  }
}


TEST(Array3DTest, TESTTEST) {
    int32_t depth = 1000;
    int32_t rows = 1000;
    int32_t cols = 1000;
    uint32_t size = depth * rows * cols;

    Array3D array_3d_1 = Array3D::make_array(depth, rows, cols);

}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
