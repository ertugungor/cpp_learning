#include <string.h>
#include <iostream>

class CopyOpsGeneratedPerson{
  public:
    CopyOpsGeneratedPerson(std::string name, int id) : name_{name}, id_{id} {}
    ~CopyOpsGeneratedPerson() { std::cout << "CopyOpsGeneratedPerson dtor" << std::endl; }
    int GetId() { return id_; }
    void PrintInfo() { printf("Name: %-*s\nId: %-*d\n\n", 15, name_.c_str(), 5, id_); }
  private:
    std::string name_;
    int id_;
};

void ShowGeneratedCopyCtor(){
  CopyOpsGeneratedPerson mad_king{"Aerys II Targaryen", 15};
  printf("%-*s => %p\n", 30, "Address of `mad_king`", (void*)&mad_king);
  mad_king.PrintInfo();

  // Implicitly generated copy ctor called
  CopyOpsGeneratedPerson copy_king = mad_king;
  printf("%-*s => %p\n", 30, "Address of `copy_king`", (void*)&copy_king);
  copy_king.PrintInfo();

  CopyOpsGeneratedPerson king_slayer{"Jaime Lannister", 17};
  printf("%-*s => %p\n", 30, "Address of `king_slayer`", (void*)&king_slayer);
  king_slayer.PrintInfo();

  // Implicitly generated copy assignment operator called
  king_slayer = mad_king;
  printf("%-*s => %p\n", 30, "Address of `king_slayer`", (void*)&king_slayer);
  king_slayer.PrintInfo();
}

class MemberPerson{
  public:
    MemberPerson(int id) : id_{id} {}
    MemberPerson(const MemberPerson&) = delete;
    int GetId() {return id_;}
  private:
    int id_;
};

class NoCopyCtorPerson{
  public:
    NoCopyCtorPerson(std::string name, int id) : name_{name}, id_{id}, member_person_{id+1} {}
    ~NoCopyCtorPerson() { std::cout << "CopyOpsGeneratedPerson dtor" << std::endl; }
    int GetId() { return id_; }
    void PrintInfo() { printf("Name: %-*s\nId: %-*d\nMember id: %-*d\n\n", 15, name_.c_str(), 5, id_, 5, member_person_.GetId()); }
  private:
    std::string name_;
    int id_;
    MemberPerson member_person_;
};

void ShowNoCopyCtor(){
  NoCopyCtorPerson no_one{"Jaqen H'ghar", 0};
  printf("%-*s => %p\n", 30, "Address of `no_one` ", (void*)&no_one);
  no_one.PrintInfo();

  /* error: 
   * copy constructor of 'NoCopyCtorPerson' is implicitly deleted
   * because field 'member_person_' has a deleted copy constructor
   */
  // NoCopyCtorPerson someone {no_one};

  // However, it does not affect the copy assignment operator
  NoCopyCtorPerson the_one {"Azor Ahai", 1};
  printf("%-*s => %p\n", 30, "Address of `the_one`", (void*)&the_one);
  the_one.PrintInfo();

  the_one = no_one;
  printf("%-*s => %p\n", 30, "Address of `the_one`", (void*)&the_one);
  the_one.PrintInfo();
}

class NoCopyAssignPerson{
  public:
    NoCopyAssignPerson(std::string name, int& id_ref) : name_{name}, id_ref_{id_ref} {}
    ~NoCopyAssignPerson() { std::cout << "NoCopyAssignPerson dtor" << std::endl; }
    void PrintInfo() { printf("Name: %-*s\nId ref: %-*d\n\n", 15, name_.c_str(), 5, id_ref_); }
  private:
    std::string name_;
    int& id_ref_;
};

void ShowNoAssign(){
  int id = 7;
  NoCopyAssignPerson onion_knight{"Davos Seaworth", id};
  printf("%-*s => %p\n", 30, "Address of `onion_knight`", (void*)&onion_knight);
  onion_knight.PrintInfo();

  NoCopyAssignPerson copy_knight{onion_knight};
  printf("%-*s => %p\n", 30, "Address of `copy_knight`", (void*)&copy_knight);
  copy_knight.PrintInfo();
  
  int another_id = 9;
  NoCopyAssignPerson legendary_knight{"Gerold Hightower", another_id};
  printf("%-*s => %p\n", 30, "Address of `legendary_knight`", (void*)&legendary_knight);
  legendary_knight.PrintInfo();

  /* error
   * copy assignment operator of 'NoCopyAssignPerson' is implicitly deleted
   * because field 'id_ref_' is of reference type 'int &'
   */
  // legendary_knight = onion_knight;
}

int main(){
  ShowGeneratedCopyCtor();
  printf("\n\n");
  ShowNoCopyCtor();
  printf("\n\n");
  ShowNoAssign();
}