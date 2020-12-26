#include <string.h>
#include <iostream>

class Person{

public:
  Person(const char* name) : name_(new char[strlen(name)])
  {
    std::cout << "Person ctor for " << name_ << "has been called" << std::endl;
    memcpy(name_, name, strlen(name));
  }

  ~Person(){
    printf("Person dtor for object at %p has called. It's name_ ptr at %p is %-*s\n",
      this, name_, 15, name_);
    delete [] name_;
  }

  const char* GetName() const{
    return name_;
  }

private:
  char* name_;
};

void PrintInfo(const Person& person){
  printf("Info for %s..\n", person.GetName());
  printf("%-*s => %p\n", 30, "Address ", (void*)&person);
  printf("%-*s => %p\n\n", 30, "Address of name_ member", (void*)person.GetName());
}

int main(){
  Person rebel_king{"Robert Baratheon"};
  PrintInfo(rebel_king);
  Person copy_king{rebel_king};
  PrintInfo(copy_king);
} // Scope ended. Both objects' destructor will be called and try to delete same pointer = Undefined behavior


/* 
 * Compiler generated copy operations just do memberwise copying (shallow copy). This
 * is dangerous for classes managing a resource whose handle is an object of non-class type
 * (raw pointer, POSIX file descriptor, etc). 
 * 
 * Rule of three
 * 
 * If a class requires a user-defined destructor, a user-defined copy constructor, or a 
 * user-defined copy assignment operator, it almost certainly requires all three.
 * 
 */


/*
 * Compile:
 * 
 * g++ -std=c++11 -o shallow_copy.out shallow_copy.cc
 * 
 */