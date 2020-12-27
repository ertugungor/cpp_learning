/* 
 * Move semantics could be used for avoiding unnecessary copies of temp values
 * (rvalues).
 * 
 * Move constructor:
 *  - Transfer resource from parameter.
 *  - Leave the parameter in a valid state.
 * 
 * Move assigment operator:
 *  - Clean the old resources.
 *  - Transfer resource from parameter.
 *  - Leave the parameter in a valid state.
 * 
 */

/*
 * Compile:
 * 
 * g++ -std=c++11 -o move_semantics.out move_semantics.cc
 * g++ -std=c++11 -fno-elide-constructors -o move_semantics.out move_semantics.cc
 * 
 */

#include <string.h>
#include <iostream>
#include <vector>
#include <string>

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

/*
 * This function depicts a case where we could answer why do "I need move
 * semantics if we have RVO" question. After resizing, std::vector would copy
 * every element to new memory address if we did not provide move semantics.
 * 
 */
void ShowMoveSemanticsInSTL() {
  // Intentionally small vector to trigger resizing
  std::vector<Person> persons{};
  persons.reserve(2);

  Person wild_king{"Mance Rayder"};
  printf("%-*s => %p\n", 50, "Address of `wild_king`", (void*)&wild_king);
  printf("%-*s => %p\n", 50, "Address of wild_king.name_ member", (void*)wild_king.GetName());
  printf("%-*s => %s\n", 50, "Name of `wild_king` is: ", wild_king.GetName());

  // Trigger resize by adding more than capacity of the vector
  for(auto i=0; i<3; ++i) {
    printf("\n");
    persons.push_back(wild_king);
    printf("What's inside persons vector after #%d push?\n", i+1);
    for(const auto& person : persons) {
      printf("%-*s => %p\n", 50, "Address of `person`", (void*)&person);
    }
  }
  printf("\n");
}

Person CreatePerson(int rand_number) {
  if(rand_number == 1) {
    Person legendary_person {"Arthur Dayne"};
    printf("%-*s => %p\n", 50, "Address of `legendary_person`", (void*)&legendary_person);
    printf("%-*s => %s\n", 50, "Name of `legendary_person` is: ", legendary_person.GetName());
    printf("%-*s => %p\n\n", 50, "Address of legendary_person->name_ member", (void*)legendary_person.GetName());
    return legendary_person;
  } else {
    // Let it leak
    Person* person_ptr = new Person{"Meryn Trant"};
    printf("%-*s => %p\n", 50, "Address of `person_ptr`", (void*)person_ptr);
    printf("%-*s => %s\n", 50, "Name of `person_ptr` is: ", person_ptr->GetName());
    printf("%-*s => %p\n\n", 50, "Address of person_ptr->name_ member", (void*)person_ptr->GetName());
    return *person_ptr;
  }
}

void ShowMoveSemantics() {
  // Shows move constructor with typical rvalue usages
  printf("\n\n");
  // Compiler can optimize away the move constructor unless 
  // -fno-elide-constructors provided
  Person person_from_function{CreatePerson(1)};
  printf("%-*s => %p\n", 50, "Address of `person_from_function`", (void*)&person_from_function);
  printf("%-*s => %s\n", 50, "Name of `person_from_function` is: ", person_from_function.GetName());
  printf("%-*s => %p\n\n", 50, "Address of person_from_function.name_ member", (void*)person_from_function.GetName());
  printf("\n");

  Person person_from_temp{Person{"Joffrey Baratheon"}};
  printf("%-*s => %p\n", 50, "Address of `person_from_temp`", (void*)&person_from_temp);
  printf("%-*s => %s\n", 50, "Name of `person_from_temp` is: ", person_from_temp.GetName());
  printf("%-*s => %p\n\n", 50, "Address of person_from_temp.name_ member", (void*)person_from_temp.GetName());
  printf("\n");

  // Shows move assignment
  Person creepy_person{"Craster"};
  printf("%-*s => %p\n", 50, "Address of `creepy_person`", (void*)&creepy_person);
  printf("%-*s => %s\n", 50, "Name of `creepy_person` is: ", creepy_person.GetName());
  printf("%-*s => %p\n\n", 50, "Address of creepy_person.name_ member", (void*)creepy_person.GetName());
  printf("\n");

  creepy_person = CreatePerson(15);
  printf("%-*s => %p\n", 50, "Address of `creepy_person`", (void*)&creepy_person);
  printf("%-*s => %s\n", 50, "Name of `creepy_person` is: ", creepy_person.GetName());
  printf("%-*s => %p\n\n", 50, "Address of creepy_person.name_ member", (void*)creepy_person.GetName());
  printf("\n");
}

/*
 * std::move is a function for casting it's parameter to rvalue reference.
 * It doesn't move anything. It unconditionally casts it's parameter.
 * 
 */ 
void ShowStdMove() {
  // Typical copy
  Person naive_person{"Sansa Stark"};
  printf("%-*s => %p\n", 50, "Address of `naive_person`", (void*)&naive_person);
  printf("%-*s => %s\n", 50, "Name of `naive_person` is: ", naive_person.GetName());
  printf("%-*s => %p\n\n", 50, "Address of naive_person.name_ member", (void*)naive_person.GetName());

  Person copy_constructed_person{naive_person};
  printf("%-*s => %p\n", 50, "Address of `copy_constructed_person`", (void*)&copy_constructed_person);
  printf("%-*s => %s\n", 50, "Name of `copy_constructed_person` is: ", copy_constructed_person.GetName());
  printf("%-*s => %p\n\n", 50, "Address of copy_constructed_person.name_ member", (void*)copy_constructed_person.GetName());

  // Intentional resource transfer with std::move
  Person move_constructed_person{std::move(naive_person)};
  printf("%-*s => %p\n", 50, "Address of `move_constructed_person`", (void*)&move_constructed_person);
  printf("%-*s => %s\n", 50, "Name of `move_constructed_person` is: ", move_constructed_person.GetName());
  printf("%-*s => %p\n\n", 50, "Address of move_constructed_person.name_ member", (void*)move_constructed_person.GetName());

  // Whap happens to moved-from object? Is using it dangerous?
  printf("%-*s => %p\n", 50, "Address of `naive_person`", (void*)&naive_person);
  printf("%-*s => %s\n", 50, "Name of `naive_person` is: ", naive_person.GetName());
  printf("%-*s => %p\n\n", 50, "Address of naive_person.name_ member", (void*)naive_person.GetName());
}

int main() {
  ShowMoveSemantics();
  printf("\n");
  ShowStdMove();
  printf("\n");
  ShowMoveSemanticsInSTL();
  printf("\n");
}

