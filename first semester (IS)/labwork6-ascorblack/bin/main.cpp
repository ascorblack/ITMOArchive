#include <lib/HammingArchiver.cpp>


int main(int argc, char** argv) {

  HammingArchiver hamming_archiver(argc, argv);
  hamming_archiver.Execute();

  return 0;
}
