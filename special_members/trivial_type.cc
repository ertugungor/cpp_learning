/*
 * Shows examples of class types dependent on default members. 
 *
 * Compile:
 * 
 * g++ -std=c++17 -o trivial_type.o trivial_type.cc
 *  
 */

#include <iostream>
#include <string>
#include "../common/logger.h"
#include <type_traits>

class TrivialPerson {
private:
  float weight_;
  int id_;
public:
  TrivialPerson(float weight, int id) : weight_{weight}, id_{id} {}
  TrivialPerson() = default;
};

class NonTrivialPerson {
private:
  float weight_;
  int id_;
public:
  NonTrivialPerson(float weight, int id) : weight_{weight}, id_{id} {}
  NonTrivialPerson() {}
};

class StandardLayoutPerson {
private:
  float weight_;
  int id_;
public:
  StandardLayoutPerson(float weight, int id) : weight_{weight}, id_{id} {}
  StandardLayoutPerson() {}
};

class NonStandardLayoutPerson {
private:
  float weight_;
  int id_;
public:
  float height_;
  NonStandardLayoutPerson(float weight, int id, float height) 
    : weight_{weight}, id_{id}, height_{height} {}
  NonStandardLayoutPerson() {}
};

class NonStandardLayoutDerived : public StandardLayoutPerson {};

void ShowTriviality() {
  std::cout << std::boolalpha;
  std::cout << std::is_trivial_v<TrivialPerson> << std::endl;
  // Having a user defined default constructor that is not 
  // declared with `default` is a violence
  std::cout << std::is_trivial_v<NonTrivialPerson> << std::endl;
  // Having a base class is a violence
  std::cout << std::is_trivial_v<NonStandardLayoutDerived> << std::endl;
}

void ShowLayout() {  
  std::cout << std::is_standard_layout_v<StandardLayoutPerson> << std::endl;
  // Having members with different access modifiers is a violence
  std::cout << std::is_standard_layout_v<NonStandardLayoutPerson> << std::endl;
  // Having a base member that is standard layout is OK
  std::cout << std::is_standard_layout_v<NonStandardLayoutDerived> << std::endl;
}

struct AggragatePerson {
  int id_;
  float weight_;
  float height_;
  AggragatePerson() = default;
};

struct NonAggragatePerson {
  int id_;
  float weight_;
  float height_;
  NonAggragatePerson() {}
};

void ShowAggragate() {
  AggragatePerson person = {1, 2.5f, 3.5f};
  // does not compile
  // NonAggragatePerson non_person {2, 3.5f, 4.5f};
}

int main() {
  ShowTriviality();
  ShowLayout();
  ShowAggragate();
}
