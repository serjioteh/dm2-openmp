## Параллельная сортировка

**Сортировка:** сортировка слиянием (merge sort)

**Библиотека:** OpenMP

**Platform:** MacOSX

brew install gcc --without-multilib
svn co http://llvm.org/svn/llvm-project/openmp/trunk openmp
mkdir build && cd build
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DLIBOMP_ARCH=x86_64 ..
make && make install

