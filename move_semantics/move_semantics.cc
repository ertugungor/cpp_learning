#include <string.h>
#include <iostream>
#include <vector>
#include <string>

class Person{

public:
  Person(const char* name) : name_{new char[strlen(name) + 1]}
  {
    std::cout << "Person ctor for " << name << " has been called" << std::endl;
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
  Person(Person&& rhs) noexcept : name_{std::move(rhs.name_)}{
    rhs.name_ = nullptr;
    std::cout << "Move constructor has been called" << std::endl;
  }

  Person& operator=(Person&& rhs) noexcept{
    if (this != &rhs){
      std::cout << "Move assignment operator has been called" << std::endl;
      delete [] name_;
      name_ = rhs.name_;
      rhs.name_ = nullptr;
    }
    return *this;
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

Person CreatePerson(int rand_number){
  if(rand_number == 1){
    Person legendary_person {"Arthur Dayne"};
    return legendary_person;
  }else {
    // Let it leak..
    Person* person_ptr = new Person{"Meryn Trant"};
    PrintInfo(*person_ptr);
    return *person_ptr;
  }
}

Person global_person{"Brynden Rivers"};
Person CreateGlobalPerson(){
  return global_person;
}

Person CreateStdMovePerson(){
  Person person{"std::move person"};
  // We are not helping the compiler, Do not do that!!
  return std::move(global_person);
}

void ShowRVOPitfalls(){
  Person global_person = CreateGlobalPerson();
  PrintInfo(global_person);
  Person std_move_person = CreateStdMovePerson();
  PrintInfo(std_move_person);
}
/*
 * This function depicts a case where we could answer why do "I need move
 * semantics if we have RVO" question. After resizing, std::vector would copy
 * every element to new memory address if we did not provide move semantics.
 * 
 */
void ShowMoreMoveSemantics(){
  // Intentionally small vector to trigger resizing
  std::vector<Person> persons{};
  persons.reserve(2);

  Person some_person{"Some person"};
  PrintInfo(some_person);

  // Trigger resize by adding more than capacity of the vector
  for(auto i=0; i<3; ++i){
    persons.push_back(some_person);
    printf("What's inside persons vector after #%d push?\n", i+1);
    for(const auto& person : persons){
      PrintInfo(person);
    }
  }
}

void ShowMoveSemantics(){
  // Shows move constructor with typical rvalue usages

  // Compiler can optimize away the move constructor unless 
  // -fno-elide-constructors provided
  Person person_from_function{CreatePerson(1)};
  PrintInfo(person_from_function);

  Person person_from_temp{Person{"Joffrey Baratheon"}};
  PrintInfo(person_from_temp);

  // Shows move assignment
  Person other_person{"Craster"};
  PrintInfo(other_person);
  other_person = CreatePerson(15);
  PrintInfo(other_person);
}

/*
 * std::move is a function for casting it's parameter to rvalue reference.
 * It doesn't move anything. It unconditionally casts it's parameter.
 * 
 */ 
void ShowStdMove(){
  // Typical copy
  Person emotional_person{"Sansa Stark"};
  PrintInfo(emotional_person);
  Person copy_constructed_person{emotional_person};
  PrintInfo(copy_constructed_person);

  // Intentional resource transfer with std::move
  Person move_constructed_person{std::move(emotional_person)};
  PrintInfo(move_constructed_person);
  // Whap happens to moved-from object? Is using it dangerous?
  PrintInfo(emotional_person);
}

int main(){
  ShowMoveSemantics();
  ShowStdMove();
  ShowMoreMoveSemantics();
  ShowRVOPitfalls();
}

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
 * g++ -std=c++11 -fno-elide-constructors -o deep_copy.out deep_copy.cc
 * 
 */