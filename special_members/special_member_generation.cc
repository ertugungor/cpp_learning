/*
 * Shows examples abouts special member generation rules.
 *
 * Compile:
 * 
 * g++ -std=c++17 -o special_member_generation.o special_member_generation.cc
 *  
 */

#include <iostream>
#include <string>
#include "../common/logger.h"

class CopyablePerson {
private:
  std::string name_;
  int id_;

public:
  CopyablePerson(const std::string name, int id) : name_(name), id_(id) {}
  CopyablePerson(const CopyablePerson& rhs) : name_{rhs.name_}, id_{rhs.id_} {
    std::cout << "Copy ctor has been called." << std::endl;
  };
  const char* GetName(){
    return name_.c_str();
  }
};

class NotMovablePerson {
private:
  std::string name_;
  int id_;

public:
  NotMovablePerson(const std::string name, int id) : name_(name), id_(id) {}
  NotMovablePerson(const NotMovablePerson& rhs) : name_{rhs.name_}, id_{rhs.id_} {
    std::cout << "Copy ctor has been called." << std::endl;
  };

  NotMovablePerson(NotMovablePerson&& rhs) = delete;
  NotMovablePerson& operator=(NotMovablePerson&& rhs) = delete;

  const char* GetName(){
    return name_.c_str();
  }
};

class MovablePerson {
private:
  std::string name_;
  int id_;

public:
  MovablePerson(const std::string name, int id) : name_(name), id_(id) {}
  MovablePerson(MovablePerson&& rhs) = default;
  MovablePerson& operator=(MovablePerson&& rhs) = default;
  const char* GetName(){
    return name_.c_str();
  }
};

/*
 * Shows defining move operations implicitly deletes copy operations
 */
void ShowImplicitlyDeletedCopyOperations() {
  MovablePerson frog_boy{"Jojen Reed", 74};
  /* 
   * Compile error:
   * function "MovablePerson::MovablePerson(const MovablePerson &)" (declared implicitly) 
   * cannot be referenced -- it is a deleted function
   */
  // MovablePerson copy_person{frog_boy};
  
  // this is valid 
  MovablePerson move_person{std::move(frog_boy)};
}

/*
 * Shows defining copy constructor  implicitly generates copy assignment operator
 */
void ShowImplicitlyGeneratedCopyAssignment() {
  CopyablePerson beuatiful_knight{"Brienne of Tarth", 18};
  CopyablePerson varg{"Varamyr Sixskins", 85};
  beuatiful_knight = varg;
}

/*
 * Shows not-declared functions do not participate in overload options.
 * Here, there is no Rvalue reference taking function and rvalues can bind
 * lvalue references. Thus, copy constructor is called.
 */
void ShowNotDeclaredMoveOperations() {
  CopyablePerson true_king{"Stannis Baratheon", 1};
  CopyablePerson copy_king{true_king};
  CopyablePerson move_king{std::move(true_king)};
}

/*
 * Shows deleted functions do participate in overload options. Here, move construtor will be
 * the best match cause std::move returns xvalue(rvalue) and move constructor's parameter is
 * rvalue reference. Because of chosen function is a deleted function, it won't compile.
 */

void ShowDeletedMoveOperations() {
  NotMovablePerson last_dragon{"Rhaegar Targaryen", 55};
  /* 
   * Compile error:
   * 
   * function "NotMovablePerson::NotMovablePerson(NotMovablePerson &&rhs)"
   * cannot be referenced -- it is a deleted function
   */
  // NotMovablePerson move_king{std::move(last_dragon)};
}

int main() {
  ShowImplicitlyDeletedCopyOperations();
  ShowImplicitlyGeneratedCopyAssignment();
  ShowNotDeclaredMoveOperations();
  ShowDeletedMoveOperations();
}