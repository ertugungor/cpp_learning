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
 * g++ -std=c++17 -o value_types.out value_types.cc
 * 

 */

#include <iostream>
#include <type_traits>
#include <typeinfo>

struct Inventory {};

struct Skills {};

struct Person {
  Person(Inventory& inv_ref) : inv_ref_(inv_ref){}
  static int total_number_;
  double hp_;
  Inventory& inv_ref_; 
  Skills skills_;
};
int Person::total_number_ = 1;
Inventory inv;

Person ReturnPerson () {
  return Person{inv};
}

int ReturnInt() {
  return 5;
}

/** 
 * It does not make sense btw, it's only for deme purpose
 * Core guidelines says don't do that `F.45: Don't return a T&&`
 */
int&& ReturnIntRefRef() {
  return 5;
}

// Reference: https://stackoverflow.com/questions/16637945/empirically-determine-value-category-of-c11-expression/16638081#16638081
template<typename T>
struct value_category {
    // Or can be an integral or enum value
    static constexpr auto value = "prvalue";
};

template<typename T>
struct value_category<T&> {
    static constexpr auto value = "lvalue";
};

template<typename T>
struct value_category<T&&> {
    static constexpr auto value = "xvalue";
};

#define VALUE_CATEGORY(expr) value_category<decltype((expr))>::value

constexpr auto column_size = 50;

void ShowValueTypes() {
  int int_ = 5;
  int& int_ref = int_;
  // int&& rvalue_ref = int_; // error: rvalue reference cannot be bound to an lvalue
  const int const_int = 10;
  int&& rvalue_ref = 10;
  int* int_ptr = &int_;

  // Names of a variables are of type lvalue
  printf("%-*s => %s\n", column_size, "int_", VALUE_CATEGORY(int_));
  // Names of a variables are of type lvalue
  printf("%-*s => %s\n", column_size, "int_ref", VALUE_CATEGORY(int_ref));
  // Const has no effect
  printf("%-*s => %s\n", column_size, "const_int ", VALUE_CATEGORY(const_int));
  // Names of a variables are of type lvalue (even if they are rvalue references)
  printf("%-*s => %s\n", column_size, "rvalue_ref", VALUE_CATEGORY(rvalue_ref));
  // pointers are no different
  printf("%-*s => %s\n", column_size, "int_ptr", VALUE_CATEGORY(int_ptr));

  // Function call with rvalue reference return type is of type xvalue 
  printf("%-*s => %s\n", column_size, "ReturnIntRefRef()", VALUE_CATEGORY(ReturnIntRefRef()));
  // std::move is one of these functions
  printf("%-*s => %s\n", column_size, "std::move(int_)", VALUE_CATEGORY(std::move(int_)));

  // Usual function returns are of type prvalue
  printf("%-*s => %s\n", column_size, "ReturnInt()", VALUE_CATEGORY(ReturnInt()));
  // Literals are of type prvalue
  printf("%-*s => %s\n", column_size, "3.5 ", VALUE_CATEGORY(3.5));
  // String literals are exceptional. They are of type lvalue
  printf("%-*s => %s\n", column_size, "\"String literal\"", VALUE_CATEGORY("String literal"));

  // Usual function returns are of type prvalue
  printf("%-*s => %s\n", column_size, "ReturnPerson()", VALUE_CATEGORY(ReturnPerson()));
  // Static members of rvalues are of type lvalue
  printf("%-*s => %s\n", column_size, "ReturnPerson().total_number_", VALUE_CATEGORY(ReturnPerson().total_number_));
  // Non-static non-reference members of rvalues are of type xvalue
  printf("%-*s => %s\n", column_size, "ReturnPerson().hp_", VALUE_CATEGORY(ReturnPerson().hp_));
  printf("%-*s => %s\n", column_size, "ReturnPerson().skills_", VALUE_CATEGORY(ReturnPerson().skills_));
  // Reference type members are of type lvalue
  printf("%-*s => %s\n", column_size, "ReturnPerson().inv_ref_", VALUE_CATEGORY(ReturnPerson().inv_ref_));
}

int main() {
  ShowValueTypes();
}