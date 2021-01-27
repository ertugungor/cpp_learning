/**
 * Universal references and perfect forwarding
 * --------------------------------------------------------------------------------------
 * 
 * There is another kind of reference called "universal references" which can
 * bind to everything. (rvalues and lvalues without considering const or volatile specifiers)
 * 
 * Confusing point is it has the same appearance as rvalue references. 
 * Some T&& is universal reference if:
 * 
 * 1- Type deduction is present. It makes "auto&&" universal reference as well
 * 2- Type is exactly "T&&", not "const T&&" or "std::vector<T>&&"
 * 
 * -------------------------------------------------------------------------------------
 * 
 * Why do we need universal references?
 * 
 * Suppose you want to make a generic function taking n parameters. Your need an overload
 * for each parameter's different versions: T&, T&&, const T& and so on. We need a mechanism
 * to pass arguments as-is to the functions.
 * 
 * -------------------------------------------------------------------------------------
 * 
 * std::forward comes into play for this purpose. std::forward,
 * 
 * 1- is a conditional cast, 
 *  if you pass lvalue, it casts it to lvalue ref(T&)
 *  if you pass rvalue, it casts it to rvalue ref(T&&)
 * 
 * 2- no runtime operation
 * 
 * 
 * Compile:
 * 
 * g++ -std=c++17 -o forwarding.out forwarding.cc
 * 
 */

#include <utility>
#include <iostream>
#include <string>

#include <memory>

class Wrapped {
public:
  Wrapped() {
    std::cout << "Wrapped Default ctor" << std::endl;
  } 
  Wrapped(const Wrapped& rhs) {
    std::cout << "Wrapped Copy ctor" << std::endl;
  } 
  Wrapped(Wrapped&& rhs) {
    std::cout << "Wrapped Move ctor" << std::endl;
  } 
};

class Wrapper {
public:
  Wrapper(const Wrapped& wrapped) : wrapped_{wrapped} {
    std::cout << "Wrapper ctor" << std::endl;
  }
  /** 
   * Comment out this part and see change 
   * in function ShowPerfectForwardingMotivation()
   *
   * Wrapper(Wrapped&& wrapped) : wrapped_{std::move(wrapped)} {
   *  std::cout << "Wrapper ctor" << std::endl;
   * }
   */
private:
  Wrapped wrapped_;
};

void ShowPerfectForwardingMotivation() {
  /**
   * If Wrapper class doesn't override it's constructor
   * for rvalue reference, copy constructor is called twice
   * even if we pass rvalue (nameless Wrapped{})
   */
  Wrapped wrapped;
  Wrapper wrapper_lvalue{wrapped};

  Wrapper wrapper_rvalue{Wrapped{}};
}


/**
 * if some_func needs the parameters passed by reference
 * it does not work. Copies will be given to some_func
 */
template<typename T>
void bad_forwarder_1(T t) {
  some_func(t);
}

/**
 * Making reference solves previous problem. However we know that
 * non-const lvalue references cannot be bound to rvalues. Thus, following
 * lines do not compile
 * 
 * bad_forwarder_2(1.5f);
 * bad_forwarder_2(ReturnInt());
 * 
 */
template<typename T>
void bad_forwarder_2(T& t) {
  some_func(t);
}

/**
 * Let's make it const then. Nope, what if some_func wants to
 * take it's parameter by reference and change param's value?
 * 
 */
template<typename T>
void bad_forwarder_3(const T& t) {
  some_func(t);
}

/**
 * Solution is to overload for different parameter types then.
 */
template<typename T> void bad_forwarder_4(const T& t) { some_func(t); }
template<typename T> void bad_forwarder_5(T& t) { some_func(t); }

/**
 * What if some_func takes more than one parameter?
 * 
 * This is an exponential problem 2-parameter func results 4 overloads 
 */
template<typename T, typename K> void bad_forwarder_6(T& t, K& k) { some_func(t, k); }
template<typename T, typename K> void bad_forwarder_7(const T& t, K& k) { some_func(t, k); }
template<typename T, typename K> void bad_forwarder_7(T& t, const K& k) { some_func(t, k); }
template<typename T, typename K> void bad_forwarder_7(const T& t, const K& k) { some_func(t, k); }

/**
 * What if we also want to optimize the forwarder and try to elide copies 
 * made for rvalue params? That leads us to rvalue reference taking overloads
 * 
 * Things get worse and worse..
 *
 * Therefore we need some way to forward arbitrary number of function params
 * to original function by preserving value type and cv-qualifiers.
 * 
 * Solution is perfect forwarding. 
 */

void func(std::string& param) {
  std::cout << "func is initialized with std::string&" << std::endl;
  std::cout << "param is: " << param << std::endl;
}

void func(std::string&& param) {
  std::cout << "func is initialized with std::string&&" << std::endl;
  std::cout << "param is: " << param << std::endl;
}

template<typename T>
void perfect_forwarder(T&& param) {
  // perfect forwarding
  func(std::forward<T>(param));
}

void PerfectForwarding() {
  std::string song_name {"The Rains of Castemere"};
  perfect_forwarder(song_name);

  /** 
   * It doesn't compile. Forwarded function's parameter
   * takes non-const lvalue reference. This sitation shows
   * universal references preserve cv-qualifier.
   * 
   * const std::string const_song_name{"Jenny's Sons"};
   * perfect_forwarder(const_song_name);
   */

  perfect_forwarder(std::string{"Hands of Gold"});
}

class Person {
public:
  Person(int id, const std::string& name) : id_{id}, name_{name} {
    std::cout << "const std::string& name" << std::endl;
    std::cout << "name is: " << name_ << std::endl;
  }
  Person(int id, std::string&& name) : id_{id}, name_{std::move(name)} {
    std::cout << "std::string&& name" << std::endl;
    std::cout << "name is: " << name_ << std::endl;
  }
private:
  int id_;
  std::string name_;
};

template<typename... Args> 
void perfect_forwarder_variadic(Args&&... param) {
  Person person{std::forward<Args>(param)...};
}

void PerfectForwardingVariadic() {
  // perfect_forwarder_variadic(1, "The Song of the Seven"); does not work as we expect
  perfect_forwarder_variadic(63, std::string{"The Song of the Seven"});

  std::string song_name{"Rat Cook"};
  perfect_forwarder_variadic(62, song_name);
}

int main(){
  ShowPerfectForwardingMotivation();
  PerfectForwarding();
  PerfectForwardingVariadic();
}
