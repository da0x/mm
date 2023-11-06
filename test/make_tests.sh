#!/bin/sh
echo '// a.hpp 
struct A {
    int a;
};
' > a.hpp
echo '// b.hpp
struct B {
    int b;
};
' > b.hpp
echo '// a.cpp
#include "a.hpp"
#include "b.hpp"
#include <iostream>
int main(){
    A a;
    B b;
    std::cout << "This is a test" << std::endl;
}
' > a.cpp
echo '// b.cpp
#include "b.hpp"
B b;
' > b.cpp
bat a.hpp
bat b.hpp
bat a.cpp
bat b.cpp
