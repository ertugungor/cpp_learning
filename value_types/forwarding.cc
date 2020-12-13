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
 */

#include <utility>
#include <iostream>
#include <string>

void func(std::string& param){
  std::cout << "func is initialized with std::string&" << std::endl;
}

void func(std::string&& param){
  std::cout << "func is initialized with std::string&&" << std::endl;
}

template<typename T>
void f(T&& param){
  // perfect forwarding
  func(std::forward<T>(param));
}


int main(){
  std::string song_name {"The Rains of Castemere"};
  f(song_name);

  f("Hands of Gold");
}