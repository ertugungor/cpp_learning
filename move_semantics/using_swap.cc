/* 
 * Copy and Move operations using swap
 * 
 * Compile:
 * 
 * g++ -std=c++17 -o using_swap.out using_swap.cc
 * g++ -std=c++17 -fno-elide-constructors -o using_swap.out using_swap.cc
 * 
 */

#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include "../common/logger.h"

class Person{

public:
  Person(const char* name) : name_{new char[strlen(name) + 1]} {
    std::cout << "Person ctor has been called" << std::endl;
    memcpy(name_, name, strlen(name) + 1);
  }

  ~Person(){
    printf("Person dtor for object at %p has called. It's name_ ptr at %p is %-*s\n",
      this, name_, 15, name_);
    delete [] name_;
  }

  Person(const Person& rhs) : Person(rhs.name_) {
    std::cout << "Copy constructor has been called" << std::endl;
  }

  Person(Person&& rhs) noexcept : name_{std::move(rhs.name_)} {
    rhs.name_ = nullptr;
    std::cout << "Move constructor has been called" << std::endl;
  }

  Person& operator=(Person rhs) {
    std::cout << "Assignment operator has been called" << std::endl;
    swap(*this, rhs);
    return *this;
  }
  
  friend void swap(Person& first, Person& second) // nothrow
  {
    using std::swap;

    // by swapping the members of two objects,
    // the two objects are effectively swapped
    swap(first.name_, second.name_);
  }

  const char* GetName() const {
    return name_;
  }

private:
  char* name_;
};

Person CreatePerson(int rand_number) {
  if(rand_number == 1) {
    Person legendary_person {"Arthur Dayne"};
    LOG_INFO(legendary_person);
    return legendary_person;
  } else {
    // Let it leak
    Person* person_ptr = new Person{"Meryn Trant"};
    LOG_INFO(*person_ptr);
    return *person_ptr;
  }
}

void ShowSwapImplementation() {
  Person loyal_person{"Wyman Manderly"};
  LOG_INFO(loyal_person);
  // First creates rhs by move constructor then uses generic 
  // assignment operator as move assignment operator
  loyal_person = CreatePerson(1);
  printf("\n");

  Person creepy_person{"Craster"};
  LOG_INFO(creepy_person);
  // First creates rhs by move constructor then uses generic
  // assignment operator as move assignment operator
  creepy_person = CreatePerson(15);
  LOG_INFO(creepy_person);

  // copy assignment operator still works
  creepy_person = loyal_person;
  LOG_INFO(creepy_person);
}

int main() {
  ShowSwapImplementation();
}