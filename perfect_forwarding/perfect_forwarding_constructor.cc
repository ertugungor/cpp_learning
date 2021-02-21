#include <string>
#include <iostream>
#include <type_traits>

// This one should have been rule of zero class
class PersonTraits {
public:
  PersonTraits(int id, std::string name) : id_{id}, name_{name} {}
  // Copy and move ctors for demonstration purpose
  PersonTraits(const PersonTraits& rhs) : id_{rhs.id_}, name_{rhs.name_} {
    std::cout << "PersonTraits copy ctor" << std::endl;
  }
  PersonTraits(PersonTraits&& rhs) noexcept : id_{std::move(rhs.id_)}, name_{rhs.name_} {
    std::cout << "PersonTraits move ctor" << std::endl;
  }
  friend std::ostream& operator<<(std::ostream& os, const PersonTraits& person_trait) {
    os << "Id: " << person_trait.id_ << "- " << "Name: " << person_trait.name_;
    return os; 
  }
private:
  int id_;
  std::string name_;
};

class InefficientPerson {
public:
  InefficientPerson(const PersonTraits& trait) : trait_{trait} {}
private:
  PersonTraits trait_;
};

class TediousPerson {
public:
  TediousPerson(const PersonTraits& trait) : trait_{trait} {}
  TediousPerson(PersonTraits&& trait) : trait_{std::move(trait)} {}
private:
  PersonTraits trait_;
};

class PerfectPerson {
public:
  template<typename T>
  PerfectPerson(T&& trait) : trait_{std::forward<T>(trait)} {}
private:
  PersonTraits trait_;
};

void ShowInneficientPerson() {
  PersonTraits traits{4, "InnefficientPerson"};
  InefficientPerson inneficient_person_from_lvalue{traits};
  
  InefficientPerson inneficient_person_from_rvalue{PersonTraits{5, "InefficientPerson"}};
}

void ShowTediousPerson() {
  PersonTraits traits{2, "TediousPerson"};
  TediousPerson acceptable_person_from_lvalue{traits};

  TediousPerson acceptable_person_from_rvalue{PersonTraits{3, "TediousPerson"}};
}

void ShowPerfectPerson() {
  PersonTraits traits{5, "PerfectPerson"};
  PerfectPerson person_lvalue{traits};

  PerfectPerson person_rvalue{PersonTraits{6, "PerfectPerson"}};
}

void ShowPerfectProblem() {
  PersonTraits traits{5, "Lvalue Trait"};
  PerfectPerson person_from_lvalue{traits};

  PerfectPerson person_from_rvalue{PersonTraits{6, "Rvalue Trait"}};


  // compiler error
  // PerfectPerson copy_person{person_from_lvalue};

  // it works..
  const PerfectPerson const_perfect_person{traits};
  PerfectPerson copy_from_const{const_perfect_person};
}

class MostPerfectPerson {
public:
  template<
    typename T, 
    typename = std::enable_if_t<
      !std::is_base_of_v<MostPerfectPerson, std::decay_t<T>>>
  >
  MostPerfectPerson(T&& trait) : trait_{std::forward<T>(trait)} {}
private:
  PersonTraits trait_;
};

void ShowPerfectProblemSolution() {
  PersonTraits traits{5, "Lvalue Trait"};
  MostPerfectPerson person_from_lvalue{traits};

  MostPerfectPerson person_from_rvalue{PersonTraits{6, "Rvalue Trait"}};

  MostPerfectPerson copy_person{person_from_lvalue};
}

int main() {
  ShowInneficientPerson();
  std::cout << "-------------------" << std::endl;
  ShowTediousPerson();
  std::cout << "-------------------" << std::endl;
  ShowPerfectPerson();
  std::cout << "-------------------" << std::endl;
  ShowPerfectProblem();
  std::cout << "-------------------" << std::endl;
  ShowPerfectProblemSolution();
}