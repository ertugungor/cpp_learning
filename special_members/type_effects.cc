/*
 * Shows examples of class types dependent on default members. 
 * It demonstrates what difference using `=default` or {} can make.
 * Compile:
 * 
 * g++ -std=c++17 -o trivial_type.o trivial_type.cc
 * 
 * g++ -std=c++2a -o trivial_type.o trivial_type.cc
 * 
 * Compile with C++20 to see changes made to aggregate types 
 */

#include <iostream>
#include <string>
#include "../common/logger.h"
#include <type_traits>

struct TrivialPerson {
  int id_;
  float weight_;
  TrivialPerson() = default;
};

struct NonTrivialPerson {
  int id_;
  float weight_;
  NonTrivialPerson() {}
};

void ShowTrivial() {
  static_assert(std::is_trivial_v<TrivialPerson>);
  // Having a user defined default constructor that is not 
  // declared with `default` is a violence
  static_assert(!std::is_trivial_v<NonTrivialPerson>);
}

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

class StandardLayoutDerived : public StandardLayoutPerson {};

void ShowLayout() {
  // Having user defined constructor is OK
  static_assert(std::is_standard_layout_v<StandardLayoutPerson>);
  // Having members with different access modifiers is a violence
  static_assert(!std::is_standard_layout_v<NonStandardLayoutPerson>);
  // Having a base member that is standard layout is OK
  static_assert(std::is_standard_layout_v<StandardLayoutDerived>);
}

struct AggregatePerson {
  int id_;
  float weight_;
  AggregatePerson() = default;
};

struct NonAggregatePerson {
  int id_;
  float weight_;
  NonAggregatePerson() {};
};

void ShowAggragate() {
  // Having defaulted constructor is OK for being aggregate in C++17, but not C++20
  static_assert(std::is_aggregate_v<AggregatePerson>);
  // Having user defined default constructor is a violence
  static_assert(!std::is_aggregate_v<NonAggregatePerson>);
}

void ShowAggregateInit() {
  { 
    // Default-initialized with constructors do nothing. Both have garbage value
    AggregatePerson aggregate;
    NonAggregatePerson non_aggregate;
    std::cout << aggregate.id_ << " - " << aggregate.weight_ << std::endl;
    std::cout << non_aggregate.id_ << " - " << non_aggregate.weight_ << std::endl;
    std::cout << std::endl;
  }

  {
    // Aggragate initialized
    AggregatePerson aggregate{};
    // Default initialized and constructor does nothing, so gargabe value
    NonAggregatePerson non_aggregate{};
    std::cout << aggregate.id_ << " - " << aggregate.weight_ << std::endl;
    std::cout << non_aggregate.id_ << " - " << non_aggregate.weight_ << std::endl;
    std::cout << std::endl;
  }

  {
    // Aggregate init
    AggregatePerson aggregate{1, 2.5};
    std::cout << aggregate.id_ << " - " << aggregate.weight_ << std::endl;
    // It's OK to leave out some members so long as order is preserved
    AggregatePerson aggregate_2{10};
    std::cout << aggregate_2.id_ << " - " << aggregate_2.weight_ << std::endl;
    // does not compile
    // NonAggregatePerson non_aggregate{1, 2.5};
  }
}

int main() {
  ShowTrivial();
  ShowLayout();
  ShowAggragate();
  ShowAggregateInit();
}
