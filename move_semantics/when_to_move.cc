/*
 * Shows an example of when we need move operations. Copying temporary objects is inefficient
 *
 * Compile:
 * 
 * g++ -O0 -fno-elide-constructors -std=c++11 -o when_to_move.out when_to_move.cc
 * 
 * Compiler options are used to disable optimizations and to show the worst case scenario.
 * 
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <random>

class Person{

public:
  Person(const char* name) : name_(new char[strlen(name) + 1])
  {
    std::cout << "Person ctor has been called" << std::endl;
    memcpy(name_, name, strlen(name) + 1);
  }

  ~Person(){
    printf("Person dtor for object at %p has called. It's name_ ptr at %p is %-*s\n",
      this, name_, 15, name_);
    delete [] name_;
  }

  Person(const Person& rhs) : Person(rhs.name_)
  {
    std::cout << "Copy constructor has been called" << std::endl;
  }

  Person& operator=(const Person& rhs){
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

  const char* GetName() const{
    return name_;
  }

private:
  char* name_;
};

Person CreatePerson(){
  Person local_person {"Ser Rodrik Cassel"};
  printf("%-*s => %p\n", 50, "Address of `legendary_person`", (void*)&local_person);
  printf("%-*s => %s\n", 50, "Name of `legendary_person` is: ", local_person.GetName());
  printf("%-*s => %p\n\n", 50, "Address of legendary_person->name_ member", (void*)local_person.GetName());
  return local_person;
}

void ShowUnnecessaryCopy(){
  Person created_person = CreatePerson();
  printf("%-*s => %p\n", 50, "Address of `created_person`", (void*)&created_person);
  printf("%-*s => %s\n", 50, "Name of `created_person` is: ", created_person.GetName());
  printf("%-*s => %p\n\n", 50, "Address of created_person->name_ member", (void*)created_person.GetName());
}

int main(){
  ShowUnnecessaryCopy();
}

// Person CreatePerson(){
//   Person local_person {"Ser Rodrik Cassel"};
//   return local_person;
// }

// void ShowUnnecessaryCopy(){
//   Person created_person = CreatePerson();
// }