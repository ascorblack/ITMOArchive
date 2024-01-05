cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -S . -B ./build
cmake --build ./build --target number_tests
cd ./build
ctest -V
