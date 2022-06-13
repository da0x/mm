  ./make_tests.sh 
  echo ------- $ mmpp
  mmpp > Makefile
  bat Makefile
  echo ------- $ make
  make
  echo ------- $ make clean
  make clean
  echo ------- $ ./mmpp
  ./mmpp
  ./unmake_tests.sh 
  rm mmpp
