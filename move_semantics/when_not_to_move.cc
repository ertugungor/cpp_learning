/*
 * Shows examples of when using std::move explicitly is incorrect or redundant.
 *
 * Compile:
 * 
 * g++ -std=c++17  -Wpessimizing-move -Wredundant-move -o when_not_to_move.o when_not_to_move.cc
 *  
 */

#include <iostream>
#include <string>

class Person {

public:
  Person(const char* name) : name_(new char[strlen(name) + 1]) {
    std::cout << "Person ctor has been called" << std::endl;
    memcpy(name_, name, strlen(name) + 1);
  }

  ~Person() {
    printf("Person dtor for object at %p has called. It's name_ ptr at %p is %-*s\n",
      this, name_, 15, name_);
    delete [] name_;
  }

  Person(const Person& rhs) : Person(rhs.name_) {
    std::cout << "Copy constructor has been called" << std::endl;
  }

  Person& operator=(const Person& rhs) {
    if (this == &rhs){
      return *this;
    }
    std::cout << "Copy assignment operator has been called" << std::endl;
    size_t name_size = strlen(rhs.name_) + 1;
    char* new_name = new char[name_size];
    memcpy(new_name, rhs.name_, name_size);
    printf("Person object at %p is deleting it's name_ ptr. It was at %p %-*s\n",
      this, name_, 15, name_);
    delete [] name_;
    name_ = new_name;
    return *this;
  }

  Person(Person&& rhs) noexcept : name_{std::move(rhs.name_)} {
    rhs.name_ = nullptr;
    std::cout << "Move constructor has been called" << std::endl;
  }

  Person& operator=(Person&& rhs) noexcept {
    if (this != &rhs){
      std::cout << "Move assignment operator has been called" << std::endl;
      delete [] name_;
      name_ = rhs.name_;
      rhs.name_ = nullptr;
    }
    return *this;
  }

  const char* GetName() const {
    return name_;
  }

private:
  char* name_;
};

Person ShowRVO() {
  Person person{"LocalObject"};
  /* 
   * Copy elision happens.
   * 
   * Compilers are smart and they can create the `person` object
   * at the caller's stack frame without copying.
   */
  return person;
}

Person ShowPessimizingMove() {
  Person person{"LocalObject"};
  // It doesn't behave as you thought it would
  return std::move(person);
}

Person ShowImplicitMove(Person person_param) {
  /* Returning a parameter prevents RVO, move constructor will
   * be implicitly called if there is one.
   */
  return person_param;
}

Person ShowRedundantMove(Person person_param) {
  return std::move(person_param);
}


int main() {
  Person rvo_person = ShowRVO();
  printf("\n\n");
  Person move_ctor_person = ShowPessimizingMove();
  printf("\n\n");
  Person implicity_person = ShowImplicitMove(rvo_person);
  printf("\n\n");
  Person redundant_person = ShowRedundantMove(rvo_person);
  printf("\n\n");
  return 0;
}





