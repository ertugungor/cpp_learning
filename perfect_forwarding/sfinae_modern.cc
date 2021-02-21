#include <iostream>
#include <string>

template <class T> 
struct HasNameField
{
  template <typename C> 
  static constexpr decltype(std::declval<C>().name, bool()) 
  test(int) {
    return true;
  }

  template <typename C> 
  static constexpr bool test(...) {
    return false;
  }

  static constexpr bool value = test<T>(int());
};

// Base Template
template <class T, class>
struct HasNameFieldImpl : std::false_type {};
// Partial Specialization
template <class T>
struct HasNameFieldImpl<T, 
  std::void_t<decltype(std::declval<T>().name)>> : std::true_type {};
template <class T>
struct HasNameField_ : HasNameFieldImpl<T, void> {};

struct NoNameField { int id; };
struct WithNameField { std::string name; };
struct WithWrongNameField { std::string not_name; };

// Shows how to use our type trait HasNameField_


template <class T,
          typename std::enable_if_t<HasNameField_<T>::value>* = nullptr>
void PrintName(T& t) {
  std::cout << t.name << std::endl;
}

template <class T,
          typename std::enable_if_t<!HasNameField_<T>::value>* = nullptr>
void PrintName(T& t) {
  std::cout << "No `name` field" << std::endl;
}

void ShowHasNameField() {
  printf("%-*s => %s\n", 45, "NoNameField has field called name?", 
    HasNameField<NoNameField>::value ? "true" : "false");
  printf("%-*s => %s\n", 45, "WithNameField has field called name?", 
    HasNameField<WithNameField>::value ? "true" : "false");
  printf("%-*s => %s\n", 45, "WithWrongNameField has field called name?", 
    HasNameField<WithWrongNameField>::value ? "true" : "false");
  
  std::cout << "using void_t and integral constants:" << std::endl;

  printf("%-*s => %s\n", 45, "NoNameField has field called name?", 
    HasNameField_<NoNameField>::value ? "true" : "false");
  printf("%-*s => %s\n", 45, "WithNameField has field called name?", 
    HasNameField_<WithNameField>::value ? "true" : "false");
  printf("%-*s => %s\n", 45, "WithWrongNameField has field called name?", 
    HasNameField_<WithWrongNameField>::value ? "true" : "false");
}

int main() {
  ShowHasNameField();

  WithNameField wnf {"Some string.."};
  PrintName(wnf);
  
  WithWrongNameField wwnf {"Some other.."};
  PrintName(wwnf);
}
