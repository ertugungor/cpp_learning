/**
 * When func(Bar{}) is called, compiler will perform a name lookup and try to
 * find candidates for the call. For function templates, type deduction occurs
 * and if it results an invalid type, this function is removed from overload
 * options. At the end, if there is no candidate left, compiler issues an error 
 * 
 * Why do templates that end up with invalid types not throw an error? Because
 * of the SFINAE (Substituion failure is not an error) concept. It doesn't have
 * to be an error, it's just got removed from overload set. By utilizing this
 * we can perform some nice compile-time inspections.
 * 
 */

#include <iostream>

template<typename T>
typename T::type func(T t) {
  std::cout << "template func" << std::endl;
  return 5;
}
void func(...) { std::cout << "func: Sink hole" << std::endl; }

struct Foo { using type = int; };
struct Bar { using not_type = int; };

template<typename T>
void func2(T t) {
  t.SomeRandomFunction();
}
void func2(...) { std::cout << "func2: Sink hole" << std::endl; }

void ShowHardError() {
  func2(Foo{});
  func2(Bar{});
}

void ShowImmediateContextFailure() {
  func(Foo{});
  func(Bar{});
}

int main() {
  ShowImmediateContextFailure();
  std::cout << "----------------" << std::endl;
  ShowHardError();
}