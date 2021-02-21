#include <string>
#include <iostream>
#include <vector>

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

class PersonInventory {
public:
  PersonInventory(const std::vector<float> values, std::vector<std::string> items)
    : values_{values}, items_{items} {}
  // Copy and move ctors for demonstration purpose
  PersonInventory(const PersonInventory& rhs) : values_{rhs.values_}, items_{rhs.items_} {
    std::cout << "PersonInventory copy ctor" << std::endl;
  }
  PersonInventory(PersonInventory&& rhs) noexcept : values_{rhs.values_}, items_{rhs.items_} {
    std::cout << "PersonInventory move ctor" << std::endl;
  }
  friend std::ostream& operator<<(std::ostream& os, const PersonInventory& person_inventory) {
    for(size_t idx = 0; idx < person_inventory.items_.size(); ++idx) {
      os << "item: " << person_inventory.items_[idx] << " - "
         << "value: " << person_inventory.values_[idx] << std::endl;
    }
    return os; 
  }
private:
  std::vector<float> values_;
  std::vector<std::string> items_;
};

class PerfectPerson {
public:
  template<typename T1, typename T2>
  PerfectPerson (T1&& t1, T2&& t2) 
    : trait_{std::forward<T1>(t1)}, inventory_{std::forward<T2>(t2)} {}
  friend std::ostream& operator<<(std::ostream& os, const PerfectPerson& perfect_person) {
    os << "Trait: " << std::endl << perfect_person.trait_ << std::endl 
      << "Inventory: " << std::endl << perfect_person.inventory_;
    return os;
  }
private:
  PersonTraits trait_;
  PersonInventory inventory_;
};

int main() {
  PersonTraits trait_lvalue{1, "trait_lvalue"};

  std::vector<float> values {5.5, 3.5, 2.5};
  std::vector<std::string> items {"Sword", "Shield", "Dagger"};
  PersonInventory inventory_lvalue{values, items};
  PerfectPerson perfect_person_from_lvalue{trait_lvalue, inventory_lvalue};
  std::cout << perfect_person_from_lvalue << std::endl;
  std::cout << "--------------" << std::endl;

  PerfectPerson perfect_person_from_rvalue{
    PersonTraits{2, "trait_rvalue"},
    PersonInventory{{1.25,2.75}, {"sack", "book"}}
  };
  std::cout << perfect_person_from_rvalue << std::endl;
  std::cout << "--------------" << std::endl;

  PerfectPerson perfect_person_from_mixed{trait_lvalue, std::move(inventory_lvalue)};
  std::cout << perfect_person_from_mixed << std::endl;
  std::cout << "--------------" << std::endl;

  // it works..
  PerfectPerson copy_person {perfect_person_from_lvalue};
}