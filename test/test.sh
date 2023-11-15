set -x

./make_tests.sh 
mmpp makefile 
bat makefile
make
make clean
mmpp
touch a.hpp
make
./unmake_tests.sh 
rm test
