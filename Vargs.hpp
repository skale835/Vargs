/*_Vargs.hpp________________________________________________________ 
|  Class for commandline arguments                                  |
|                                                                   |
|  Copyright (c) 2026 Sameer Kale [skale835@proton.me]              |
|  SPDX-License-Identifier: MIT                                     |
|                                                                   |
|  How to use:                                                      |
|  1) Save Vargs.hpp in an accessible location                      |
|  2) In your code, #include "Vargs.hpp"                            |
|  3) Pass argc and argv to the Vargs constructor:                  |
|     ```                                                           |
|     int main(int argc, char* argv[])                              |
|       Vargs vargs(argc, argv)                                     |
|     ```                                                           |
|  4) Use the functions on Vargs:: as described in the class        |
|     declaration.                                                  |
|                                                                   |
|  Refer to code & comments for further details.                    |
|                                                                   |
|__________________________________________________________________*/
// =========== HEADERS =========================================== //
  #ifndef VARGS_H
  #define VARGS_H
  #include <vector>
  #include <string>
  #include <stdexcept>

// =========== CLASS ============================================= //

  class Vargs {
    private:
      std::vector<std::string> vec; 
      std::size_t i; 
        /* The fields are simply a vector<string> and an index. The 
           index is a simple number rather than the iterable-pointer-to-
           element--thingy in std::vector. */

    public:
      Vargs();
      Vargs(int argc, char* argv[]);
        /* Creating Vargs without arguments results in an empty 
           object. Check each function for empty-vector behavior.*/

      std::size_t size();
        /* Return size */
      std::size_t inpos();
        /* Return position of index */

      std::string arg(std::size_t ind);
        /* Return argument at argued position */

      void advin();
        /* Advance index. Can throw out_of_range. */
      void retin();
        /* Retard index. Can throw out_of_range. */
      void movin(std::size_t ind);
        /* Move index to argued value. */
      std::string getin();
        /* Return indicated argument */    
      void setin(std::string theArg);
        /* Set indicated argument to theArg */

      std::vector<std::string> getall();
        /* Return vector<string> of the arguments */

      void push(std::string theArg);
        /* Add theArg to end */
      std::string pop(); 
        /* Remove and return last element */
      void fpush(std::string theArg); 
        /* Add theArg to start */
      std::string fpop();
        /* Remove and return first element */
  };
// =========== METHODS =========================================== //
// ----------- CONSTRUCTOR --------------------------------------- //
  Vargs::Vargs() {
    this->i = 0;
  }

  Vargs::Vargs(int argc, char* argv[]) : i(0) {
    for (int a = 1; a < argc; a++) {
      vec.push_back(argv[a]);
    }
  }

// ----------- DESTRUCTOR ---------------------------------------- //
     /* None needed */

// ---------- .size() -------------------------------------------- //
  std::size_t Vargs::size() {           /* Never fail */ 
    return this->vec.size();
  }

// ---------- .inpos() ------------------------------------------- //
  std::size_t Vargs::inpos() {          /* Never fail */
    return this->i;
  }


// ---------- .arg() --------------------------------------------- //
  std::string Vargs::arg(std::size_t ind) { 
    if (ind  >= this->vec.size()) { 
      throw std::out_of_range("Vargs::arg(): index out of range");
    }
    return this->vec.at(ind);
  }

// ---------- .advin() ------------------------------------------- //
  void Vargs::advin() {
    if (i + 1 >= this->vec.size()) {
      throw std::out_of_range("Vargs::advin(): no further args");
    }
    i++;
  }

// ---------- .retin() ------------------------------------------- //
  void Vargs::retin() {
    if (i == 0) { 
      throw std::out_of_range("Vargs::retin(): no preceding args");
    }
    i--;
  }

// ---------- .movin() ------------------------------------------- //
  void Vargs::movin(std::size_t ind) {
    if (ind + 1 >= this->vec.size()) { 
      throw std::out_of_range("Vargs::movin(): index out of range");
    }
    this->i = ind;
  }

// ----------- getin() ------------------------------------------- //
  std::string Vargs::getin() {            /* NOTE1 => never fail */

    return this->vec.at(this->i); 
  }

// ----------- setin() ------------------------------------------- //
  void Vargs::setin(std::string theArg) { /* NOTE1 => never fail */
    this->vec.at(this->i) = theArg;
  }

// ----------- getall() ------------------------------------------ //
  std::vector<std::string> Vargs::getall() {
    return this->vec;
  }

// ----------- push() -------------------------------------------- // 
  void Vargs::push(std::string theArg) {
    this->vec.push_back(theArg);
  }

// ----------- pop() --------------------------------------------- //
  std::string Vargs::pop() {
      /* pop_back() undefined for empty vector. */
    if (this->vec.size() == 0) {
      throw(std::out_of_range("Vargs::pop(): Empty vargs"));
    }
    std::string toReturn = this->vec.at(vec.size()-1);

    this->vec.pop_back();
    return toReturn;
  }

// ----------- fpush() ------------------------------------------- //
  void Vargs::fpush(std::string theArg) {
    this->vec.insert(this->vec.begin(),theArg);
  }


// ----------- fpop() -------------------------------------------- //
  std::string Vargs::fpop() {
      /* pop_back() undefined for empty vector. */
    if (this->vec.size() == 0) {
      throw(std::out_of_range("Vargs::fpop() Empty vargs"));
    }
    std::string toReturn = this->vec.at(0);
    this->vec.erase(this->vec.begin());
    return toReturn;
  }

  #endif // Vargs_H
//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ //
