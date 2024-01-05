#include <iostream>

#include "sandy_matrix.h"

const char* kResetColor = "\033[0m";
const char* kWhiteColor = "\033[97m";
const char* kGreenColor = "\033[32m";
const char* kYellowColor = "\033[93m";
const char* kPurpleColor = "\033[35m";
const char* kBlackColor = "\033[30m";
const char* kRedColor = "\033[31m";
const int32_t kGrainOfSandCollapseThreshold = 4;


enum class ExpansionDirection {
  Left,
  Right,
  Up,
  Down
};


void GrainOfSand::concatenateCounts() {

  if (this->add_count != 0) {
    this->count += static_cast<uint32_t>(this->add_count);
    this->add_count = 0;
  }

}

uint8_t GrainOfSand::getColor() const {
  return this->count <= 4 ? static_cast<uint8_t>(this->count) : static_cast<uint8_t>(4);
}


const char* getColor(GrainOfSand* grain) {
  if (grain->count == 0)
    return kWhiteColor;
  else if (grain->count == 1)
    return kGreenColor;
  else if (grain->count == 2)
    return kYellowColor;
  else if (grain->count == 3)
    return kPurpleColor;
  else
    return kBlackColor;
}


SandyMatrix::SandyMatrix(GrainOfSand** matrix_, int height, int width): matrix(matrix_), height(height), width(width) {}

void SandyMatrix::extendMatrix(ExpansionDirection overflow_direct) {

  GrainOfSand** new_matrix;

  if (overflow_direct == ExpansionDirection::Up) {
    height++;

    new_matrix = new GrainOfSand*[height];
    new_matrix[0] = new GrainOfSand[width];

    for (int i = 1; i < height; ++i) {
      new_matrix[i] = new GrainOfSand[width];

      for (int j = 0; j < width; ++j)
        new_matrix[i][j] = matrix[i - 1][j];

    }

  } else if (overflow_direct == ExpansionDirection::Down) {
    height++;

    new_matrix = new GrainOfSand*[height];
    new_matrix[height-1] = new GrainOfSand[width];

    for (int i = 0; i < height - 1; ++i) {
      new_matrix[i] = new GrainOfSand[width];

      for (int j = 0; j < width; ++j)
        new_matrix[i][j] = matrix[i][j];

    }

  } else if (overflow_direct == ExpansionDirection::Left) {
    width++;

    new_matrix = new GrainOfSand*[height];

    for (int i = 0; i < height; ++i) {
      new_matrix[i] = new GrainOfSand[width];

      for (int j = 0; j < width - 1; ++j)
        new_matrix[i][j + 1] = matrix[i][j];

    }

  } else {
    width++;

    new_matrix = new GrainOfSand*[height];

    for (int i = 0; i < height; ++i) {
      new_matrix[i] = new GrainOfSand[width];

      for (int j = 0; j < width - 1; ++j)
        new_matrix[i][j] = matrix[i][j];

    }

  }

  delete[] matrix;
  matrix = new_matrix;
}

void SandyMatrix::printMatrix(const char* delimiter = "\t") const {
  printf("Step: %d\nHeight: %d\tWidth: %d\n", this->step, this->height, this->width);

  for (int i = 0; i < this->height; ++i) {
    printf("%s[%s ", kRedColor, kResetColor);

    for (int j = 0; j < this->width; ++j) {
      printf("%s@%s", getColor(&this->matrix[i][j]), kResetColor);
//      printf("%s%d%s", getColor(&this->matrix[i][j]), static_cast<int>(this->matrix[i][j].count), kResetColor);

      if (j < this->width - 1) {
        printf( "%s", delimiter);
      }

    }

    printf( " %s]%s\n", kRedColor, kResetColor);
  }

  printf( "\n");
}


void SandyMatrix::mergeCounts() const {

  for (int x = 0; x < this->height; ++x) {
    for (int y = 0; y < this->width; ++y) {

      this->matrix[x][y].concatenateCounts();

    }
  }

}


bool SandyMatrix::calcStep() {
  bool is_change = false;

  for (int x = 0; x < this->height; ++x) {
    for (int y = 0; y < this->width; ++y) {

      if (this->matrix[x][y].count >= kGrainOfSandCollapseThreshold) {
        this->matrix[x][y].add_count -= kGrainOfSandCollapseThreshold;

        if (x <= 0) {
          extendMatrix(ExpansionDirection::Up);
          x++;
        }

        if (x >= height - 1)
          extendMatrix(ExpansionDirection::Down);


        if (y <= 0) {
          extendMatrix(ExpansionDirection::Left);
          y++;
        }

        if (y >= width - 1)
          extendMatrix(ExpansionDirection::Right);


        this->matrix[x + 1][y].add_count++;
        this->matrix[x - 1][y].add_count++;
        this->matrix[x][y + 1].add_count++;
        this->matrix[x][y - 1].add_count++;

        is_change = true;
      }
    }
  }

  if (is_change) {
    this->step++;
    this->mergeCounts();
  }

  return is_change;
}

