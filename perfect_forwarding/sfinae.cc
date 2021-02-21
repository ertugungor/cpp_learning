#include <iostream>
#include <string>

template <class T> 
struct HasNameField {
  typedef char yes[1];
  typedef char no[2];

  template <typename U, U> struct Check;

  template <typename C> 
  static yes& test(Check<std::string C::*, &C::name>*) {}

  template <typename> 
  static no& test(...) {}

  static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

template <class T> 
struct HasNameFunc {
  typedef char yes[1];
  typedef char no[2];

  template <typename U, U> struct Check;

  template <typename C> 
  static yes& test(Check<std::string (C::*)(), &C::Name>*) {}
  template <typename C> 
  static yes& test(Check<std::string (C::*)() const, &C::Name>*) {}

  template <typename> 
  static no& test(...) {}

  static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

struct NoNameField { int id; };
struct WithNameField { std::string name; };
struct WithWrongNameField { std::string not_name; };

struct NoNameFunc { int Id(); };
struct WithNameFunc { std::string Name() const; };
struct WithWrongNameFunc { std::string GetName(); };

void ShowHasNameField() {
  printf("%-*s => %s\n", 45, "NoNameField has field called name?", 
    HasNameField<NoNameField>::value ? "true" : "false");
  printf("%-*s => %s\n", 45, "WithNameField has field called name?", 
    HasNameField<WithNameField>::value ? "true" : "false");
  printf("%-*s => %s\n", 45, "WithWrongNameField has field called name?", 
    HasNameField<WithWrongNameField>::value ? "true" : "false");
}

void ShowHasNameFunc() {
  printf("%-*s => %s\n", 45, "NoNameFunc has function called Name?", 
    HasNameFunc<NoNameFunc>::value ? "true" : "false");
  printf("%-*s => %s\n", 45, "WithNameFunc has function called Name?", 
    HasNameFunc<WithNameFunc>::value ? "true" : "false");
  printf("%-*s => %s\n", 45, "WithWrongNameFunc has function called Name?", 
    HasNameFunc<WithWrongNameFunc>::value ? "true" : "false");
}

int main() {
  ShowHasNameField();
  ShowHasNameFunc();
}
