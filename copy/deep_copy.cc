#include <string.h>
#include <iostream>

class Person{

public:
  Person(const char* name) : name_(new char[strlen(name) + 1])
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
    Person smart_person {"Lord Varys"};
    return smart_person;
  }else {
    // Let it leak..
    Person* person_ptr = new Person{"Ramsay Snow"};
    PrintInfo(*person_ptr);
    return *person_ptr;
  }
}

void ShowCopySemantics(){
  // Shows copy constructor
  Person person{"Ned Stark"};
  PrintInfo(person);
  Person other_person{person};
  PrintInfo(other_person);

  // Shows copy assignment operator
  Person new_person{"Catelyn Tully"};
  PrintInfo(new_person);
  person = new_person;
  PrintInfo(person);
}

void ShowRVO(){
  // Return value got copied
  Person created_person = CreatePerson(13);
  PrintInfo(created_person);

  // Return value did not get copied unless -fno-elide-constructors provided
  Person another_created_person = CreatePerson(1);
  PrintInfo(another_created_person);
}

int main(){
  ShowCopySemantics();
  ShowRVO();
}


/* 
 * Compiler generated copy operations just do memberwise copying (shallow copy). This
 * is dangerous for classes managing a resource whose handle is an object of non-class type
 * (raw pointer, POSIX file descriptor, etc). 
 * 
 * That's why in this example, copy constructor and copy assignment operator have been defined.
 */


/*
 * Compile:
 * 
 * g++ -std=c++11 -o deep_copy deep_copy.cc
 * g++ -std=c++11 -fno-elide-constructors -o deep_copy.out deep_copy.cc
 * 
 */