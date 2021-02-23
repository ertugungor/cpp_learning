#include <string>
#include <iostream>
#include <type_traits>

struct Base {};
struct Derived : Base {};
struct Other {};

template<typename T>
void SomeFunc(T&& param) {
  std::cout << "T&& param" << std::endl;
}

void SomeFunc(Base& base) {
  std::cout << "Base& param" << std::endl;
}

template<typename T>
void SomeFuncBetter(T&& param) {
  SomeFuncBetterImpl(std::forward<T>(param), std::is_base_of<std::decay_t<T>, Derived>{});
}

template<typename T>
void SomeFuncBetterImpl(T&& base, std::false_type) {
  std::cout << "Other params" << std::endl;
}

void SomeFuncBetterImpl(Base& base, std::true_type) {
  std::cout << "Base& param" << std::endl;
}
void ShowTagDispatchSolution() {
  Other other;
  SomeFuncBetter(other);
  Derived derived;
  SomeFuncBetter(derived);
}

void ShowOverloadingProblem() {
  Other other;
  SomeFunc(other);
  Derived derived;
  SomeFunc(derived);
}


int main() {
  ShowOverloadingProblem();
  ShowTagDispatchSolution();
}