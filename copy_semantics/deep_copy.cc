/* 
 * Compiler generated copy operations just do memberwise copying (shallow copy). This
 * is dangerous for classes managing a resource whose handle is an object of non-class type
 * (raw pointer, POSIX file descriptor, etc). 
 * 
 * That's why in this example, copy constructor and copy assignment operator have been defined.
 *
 * 
 * Compile:
 * 
 * g++ -std=c++11 -o deep_copy deep_copy.cc
 * 
 */

#include <string.h>
#include <iostream>

class Person{

public:
  Person(const char* name) : name_(new char[strlen(name) + 1]) {
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
    if (this == &rhs) {
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

  const char* GetName() const {
    return name_;
  }

private:
  char* name_;
};

void ShowDeepCopy() {
  // Shows copy constructor
  Person noble_man{"Ned Stark"};
  printf("%-*s => %p\n", 30, "Address of `noble_man`", (void*)&noble_man);
  printf("%-*s => %s\n", 30, "Name of `noble_man` is: ", noble_man.GetName());
  printf("%-*s => %p\n", 30, "Address of noble_man.name_", (void*)noble_man.GetName());
  printf("\n");

  Person copy_man{noble_man};
  printf("%-*s => %p\n", 30, "Address of `copy_man`", (void*)&copy_man);
  printf("%-*s => %s\n", 30, "Name of `copy_man` is: ", copy_man.GetName());
  printf("%-*s => %p\n", 30, "Address of copy_man.name_", (void*)copy_man.GetName());
  printf("\n");

  // Shows copy assignment operator
  Person lady{"Catelyn Tully"};
  printf("%-*s => %p\n", 30, "Address of `lady`", (void*)&lady);
  printf("%-*s => %s\n", 30, "Name of `lady` is: ", lady.GetName());
  printf("%-*s => %p\n", 30, "Address of lady.name_", (void*)lady.GetName());
  printf("\n");

  noble_man = lady;
  printf("%-*s => %p\n", 30, "Address of `noble_man`", (void*)&noble_man);
  printf("%-*s => %s\n", 30, "Name of `noble_man` is: ", noble_man.GetName());
  printf("%-*s => %p\n", 30, "Address of noble_man.name_", (void*)noble_man.GetName());
}

int main() {
  ShowDeepCopy();
}
