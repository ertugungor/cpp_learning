#include <string>
#include <type_traits>
#include <iostream>

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

class PassByValuePerson {
public:
  PassByValuePerson(PersonTraits trait) : trait_{std::move(trait)} {}
private:
  PersonTraits trait_;
};

void ShowPerfectForwarding() {
  std::cout << "From lvalue =>" << std::endl;
  PersonTraits traits{5, "Lvalue Trait"};
  MostPerfectPerson person_from_lvalue{traits};

  std::cout << "From rvalue =>" << std::endl;
  MostPerfectPerson person_from_rvalue{PersonTraits{6, "Rvalue Trait"}};

  std::cout << "From copy =>" << std::endl;
  MostPerfectPerson copy_person{person_from_lvalue};

  std::cout << "From temp =>" << std::endl;
  MostPerfectPerson move_person{std::move(person_from_lvalue)};
}

void ShowPassByValue() {
  std::cout << "From lvalue =>" << std::endl;
  PersonTraits traits{5, "Lvalue Trait"};
  PassByValuePerson person_from_lvalue{traits};

  std::cout << "From rvalue =>" << std::endl;
  PassByValuePerson person_from_rvalue{PersonTraits{6, "Rvalue Trait"}};

  std::cout << "From copy =>" << std::endl;
  PassByValuePerson copy_person{person_from_lvalue};
  
  std::cout << "From temp =>" << std::endl;
  PassByValuePerson move_person{std::move(person_from_lvalue)};
}

int main() {
  ShowPerfectForwarding();
  std::cout << "------------------" << std::endl;
  ShowPassByValue();
}
