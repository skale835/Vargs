/*_Vargs.hpp________________________________________________________ 
|  Class for commandline arguments                                  |
|                                                                   |
|  Copyright (c) 2026 Sameer Kale                                   |
|  SPDX-License-Identifier: MIT                                     |
|__________________________________________________________________*/
// =========== Headers =========================================== //
  #ifndef VARGS_H
  #define VARGS_H
  #include <vector>
  #include <string>
  #include <stdexcept>

// =========== Class ============================================= //

  class Vargs {
    private:
      std::vector<std::string> vec;
      std::size_t i;

    public:
      Vargs();
      Vargs(int argc, char* argv[]);

      std::size_t size();
        /* return size */
      std::size_t pos();
        /* return position of index */

      std::string arg(std::size_t ind);
        /* return argument at argued position */

      void advin();
        /* Advance index */
      void retin();
        /* Retard index */
      void movin();
        /* Retard index */
      std::string getin();
        /* Return indicated argument */    
      void setin(std::string theArg);
        /* Return indicated argument */

      std::vector<std::string> getall();
        /* Return vector<string> arguments */

      void push(std::string theArg);
      std::string pop(); 
      void fpush(std::string theArg); 
      std::string fpop(); 
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

// ---------- .pos() --------------------------------------------- //
  std::size_t Vargs::pos() {            /* Never fail */
    return this->i;
  }


// ---------- .arg() --------------------------------------------- //
  std::string Vargs::arg(std::size_t ind) { 
    return this->vec.at(ind);
  }

// ---------- .advin() ------------------------------------------- //
  void Vargs::advin() {
    if (i + 1 >= vec.size()) {
      throw std::out_of_range("Vargs::advit(): no further args");
    }
    i++;
  }

// ---------- .retin() ------------------------------------------- //
  void Vargs::retin() {
    if (i == 0) { 
      throw std::out_of_range("Vargs::retit(): no preceding args");
    }
    i--;
  }

// ----------- getin() ------------------------------------------- //
  std::string Vargs::getin() {

    return this->vec.at(this->i); 
  }

// ----------- setin() ------------------------------------------- //
  void Vargs::setin(std::string theArg) { /* NOTE1 => never fail */
    this->vec.at(this->i) = theArg;
  }

// ----------- getall() ------------------------------------------ //
  std::vector<std::string> Vargs::getall() {
    return vec;
  }

// ----------- push() -------------------------------------------- // TEST
  void Vargs::push(std::string theArg) {
    vec.push_back(theArg);
  }

// ----------- pop() --------------------------------------------- // TEST
  std::string Vargs::pop() {
    if (i == 0) {        /* pop_back() undefined for empty vector */
      throw(std::out_of_range("Empty vargs"));
    }
    std::string toReturn = vec.at(vec.size());
    vec.pop_back();
    return toReturn;
  }

// ----------- fpush() ------------------------------------------- // TEST
  void Vargs::fpush(std::string theArg) {
    vec.insert(vec.begin(),theArg);
  }


// ----------- fpop() -------------------------------------------- // TEST
  std::string Vargs::fpop() {
    if (i == 0) {        /* pop_back() undefined for empty vector */
      throw(std::out_of_range("Empty vargs"));
    }
    std::string toReturn = this->vec.at(0);
    vec.erase(vec.begin());
    return toReturn;
  }

  #endif // Vargs_H
//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ //
