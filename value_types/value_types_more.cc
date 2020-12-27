/**
 * Lvalue and Rvalue and their references
 * 
 * This file demonstrates some examples about primary value categories in C++,
 * which are prvalue, lvalue and xvalues.
 *
 * --------------------------------------------------------------------------------------
 * 
 * Mixed categories:
 * 
 * Glvalue => lvalue or xvalue
 * Rvalue  => prvalue or xvalue
 * 
 * Lvalues can be think of what we can take address of. They have names and
 * correspond to some location in memory. Variable names, class members, array
 * elements are some examples.
 * 
 * Prvalues (pure rvalues) are temporary values, which may or may not have a
 * location in memory. Temporary variables, literals(except string), function
 * calls that has non-reference return type are some examples.
 * 
 * Xvalues (expiring values) are objects just near their lifetime, so they can
 * be reused.
 * 
 * --------------------------------------------------------------------------------------
 * 
 * => Rvalue references only bind to rvalues which can either be xvalue or prvalue.
 * => Lvalue references bind lvalues. Also const lvalue references can bind to rvalues.
 * (It makes sense cause modifying a temp variable would be problematic)
 * 
 * --------------------------------------------------------------------------------------
 * 
 * => The biggest caveat here is rvalue references itself can be lvalue in some contexts
 * 
 * void f(Object&& obj){
 *    // obj is an lvalue in this scope, it has a name, an identity.
 * }
 * 
 * This takes us to the some sort of rule of thumb: If it has a name, then it's an lvalue.
 * 
 * 
 * Compile:
 * 
 * g++ -std=c++17 -o value_types_more.out value_types_more.cc
 * 

 */

#include <string.h>
#include <iostream>
#include <vector>

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

  Person& operator=(const Person& rhs) {
    if (this == &rhs) {
      return *this;
    }
    std::cout << "Copy assignment operator has been called" << std::endl;
    size_t name_size = strlen(rhs.name_) + 1;
    char* new_name = new char[name_size];
    memcpy(new_name, rhs.name_, name_size);
    delete [] name_;
    name_ = new_name;
    return *this;
  }

  /* 
   * noexcept is needed to be used by operations that give strong exception
   * safety and without noexcept would call copy operations.
   * 
   * std::vector<Person> persons;
   * persons.push_back("Samwell Tarly"); // that would copy without noexcept
   * 
   */
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


void ShowValueTypes(){
  Person king_in_the_north{"Rob Stark"};
  printf("%-*s => %p\n", 50, "Address of `king_in_the_north`", (void*)&king_in_the_north);
  printf("%-*s => %p\n", 50, "Address of king_in_the_north.name_ member", (void*)king_in_the_north.GetName());
  printf("%-*s => %s\n", 50, "Name of `king_in_the_north` is: ", king_in_the_north.GetName());
  printf("\n");

  Person copy_person{king_in_the_north};
  printf("%-*s => %p\n", 50, "Address of `copy_person`", (void*)&copy_person);
  printf("%-*s => %p\n", 50, "Address of copy_person.name_ member", (void*)copy_person.GetName());
  printf("%-*s => %s\n", 50, "Name of `copy_person` is: ", copy_person.GetName());
  printf("\n");

  Person move_person{std::move(king_in_the_north)};
  printf("%-*s => %p\n", 50, "Address of `move_person`", (void*)&move_person);
  printf("%-*s => %p\n", 50, "Address of move_person.name_ member", (void*)move_person.GetName());
  printf("%-*s => %s\n", 50, "Name of `move_person` is: ", move_person.GetName());
  printf("%-*s => %p\n", 50, "Address of `king_in_the_north`", (void*)&king_in_the_north);
  printf("%-*s => %p\n", 50, "Address of king_in_the_north.name_ member", (void*)king_in_the_north.GetName());
  printf("%-*s => %s\n", 50, "Name of `king_in_the_north` is: ", king_in_the_north.GetName());
  printf("\n");

  king_in_the_north = Person{"Temp person"};
  printf("%-*s => %p\n", 50, "Address of `king_in_the_north`", (void*)&king_in_the_north);
  printf("%-*s => %p\n", 50, "Address of king_in_the_north.name_ member", (void*)king_in_the_north.GetName());
  printf("%-*s => %s\n", 50, "Name of `king_in_the_north` is: ", king_in_the_north.GetName());
  printf("\n");

  // error: rvalue reference to type 'Person' cannot bind to lvalue of type 'Person'
  // Person&& person_ref_ref = king_in_the_north;

  // std::move returns an xvalue and rvalue references can bind 
  // rvalues (either prvalue or xvalue), so it's OK
  Person&& person_ref_ref = std::move(king_in_the_north);
  printf("%-*s => %p\n", 50, "Address of `person_ref_ref`", (void*)&person_ref_ref);
  printf("%-*s => %p\n", 50, "Address of person_ref_ref.name_ member", (void*)person_ref_ref.GetName());
  printf("%-*s => %s\n", 50, "Name of `person_ref_ref` is: ", person_ref_ref.GetName());
  printf("%-*s => %p\n", 50, "Address of `king_in_the_north`", (void*)&king_in_the_north);
  printf("%-*s => %p\n", 50, "Address of king_in_the_north.name_ member", (void*)king_in_the_north.GetName());
  printf("%-*s => %s\n", 50, "Name of `king_in_the_north` is: ", king_in_the_north.GetName());
  printf("\n");

  // Copy? person_ref_ref is of type Person&&, why doesn't move constructor get called?
  // person_ref_ref is an lvalue, it has a name. It should be move again with std::move if
  // we want to call move constructor. 
  Person another_copy{person_ref_ref};
  printf("%-*s => %p\n", 50, "Address of `another_copy`", (void*)&another_copy);
  printf("%-*s => %p\n", 50, "Address of another_copy.name_ member", (void*)another_copy.GetName());
  printf("%-*s => %s\n", 50, "Name of `another_copy` is: ", another_copy.GetName());
  printf("\n");

  // What we really intented
  Person moved_person{std::move(person_ref_ref)};
  printf("%-*s => %p\n", 50, "Address of `moved_person`", (void*)&moved_person);
  printf("%-*s => %p\n", 50, "Address of moved_person.name_ member", (void*)moved_person.GetName());
  printf("%-*s => %s\n", 50, "Name of `moved_person` is: ", moved_person.GetName());
  printf("\n");

  // or we could have directly called std::move on `king_in_the_north` object 
  // which we steal resource from at the first place. It's return value is an xvalue
  // and move constructor's Person&& parameter can bind to it. So, move constructor gets called.
 
  /*
    Person direct_move_person{std::move(king_in_the_north)};
    printf("%-*s => %p\n", 50, "Address of `direct_move_person`", (void*)&direct_move_person);
    printf("%-*s => %p\n", 50, "Address of direct_move_person.name_ member", (void*)direct_move_person.GetName());
    printf("%-*s => %s\n", 50, "Name of `direct_move_person` is: ", direct_move_person.GetName());
    printf("\n");
  */
}

int main(){
  ShowValueTypes();
}
