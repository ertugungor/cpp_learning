/*
 * Shows examples for core guidelines #21 and #67
 * 
 * C.21: If you define or =delete any copy, move, or destructor function, define or =delete them all
 * C.67: A polymorphic class should suppress copying
 *
 * g++ -std=c++17 -o c21_67.o c21_67.cc
 * 
 */

#include <iostream>
#include <memory>

class BasePerson {
public:
  BasePerson() = default;
  // A class intended to be used polymorphically should define virtual destructor
  virtual ~BasePerson() = default;
  // And it makes move opearations not-declared, so let's default them
  BasePerson(BasePerson&&) = default;
  BasePerson& operator=(BasePerson&&) = default;
  // Defaulting move operations deletes copy operations, so let's default them
  BasePerson(const BasePerson&) = default;
  BasePerson& operator=(const BasePerson&) = default;
  virtual const char* GetName() { return "BasePerson"; }
};

class DerivedPerson : public BasePerson {
public:
  DerivedPerson() = default;
  const char* GetName() override { return "DerivedPerson"; }
};

void ShowSlicingWhenCopy() {
  // As expected, prints DerivedPerson
  DerivedPerson person;
  std::cout << person.GetName() << std::endl;

  // As expected, prints DerivedPerson
  BasePerson& abs_ref = person;
  std::cout << abs_ref.GetName() << std::endl; 

  // Unexpectedly prints BasePerson. Slicing occurs.
  auto copy_ref = abs_ref;
  std::cout << copy_ref.GetName() << std::endl; 
}

class Base {
public:
  Base() = default;
  virtual ~Base() = default;
  Base(Base&&) = default;
  Base& operator=(Base&&) = default;
  Base(const Base&) = default;
  Base& operator=(const Base&) = default;

  virtual std::unique_ptr<Base> Clone() = 0;
  virtual const char* GetName() const { return "Base"; }
};

class Derived : public Base {
public:
  // Covariance type is not possible with unique_ptrs
  std::unique_ptr<Base> Clone() override {
    return std::make_unique<Derived>(*this);
  }
  const char* GetName() const override { return "Derived"; }
};


void ShowPolymorphicCopy() {
  Derived derived;
  std::cout << derived.GetName() << std::endl;

  Base& base_ref = derived;
  std::cout << base_ref.GetName() << std::endl; 

  auto copy_ref = base_ref.Clone();
  std::cout << copy_ref->GetName() << std::endl; 
}

int main() {
  std::cout << std::endl << std::endl;
  ShowSlicingWhenCopy();
  std::cout << std::endl << std::endl;
  ShowPolymorphicCopy();
}