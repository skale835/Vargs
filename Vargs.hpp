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
      std::vector<std::string> args;
      std::size_t sel;
        /* The fields are simply a vector<string> and a selector. The 
           selector is zero for no selection, and n in selecting the    
           nth element. Operations modify selector as described. */

    public:
      Vargs();
      Vargs(int argc, char* argv[]);
        /* Creating Vargs without arguments results in an empty 
           object. Check each function for empty-vector behavior.*/

      // QUERIES
      std::size_t size();
        /* Return size */
      std::string arg(std::size_t pos);
        /* Return argument at argued position */
      std::vector<std::string> getall();
        /* Return vector<string> of the arguments */

      // SELECTOR METHODS
      void sadv();
        /* Advance selector. Throw error if out of range. */
      void sret();
        /* Retard selector.  Throw error if out of range. */
      void smov(std::size_t pos);
        /* Move selector to argued value. Throw error if out of range */
      void sclr();
        /* Clear (or reset) selector, by setting to zero. */
      std::size_t sget();
        /* Return selector value */    

      void setsel(std::string theArg);
        /* Set selected element to theArg */
      std::string getsel();
        /* Get selected element */

      // DIRECT OPERATIONS
      void insel(std::string theArg);
        /* Insert theArg, such that sel's current value will point to it. 
           Does nothing if sel = 0 */
      std::string delsel();               
        /* Delete selected argument. sel = 0 throws error. sel resets with
           delete.*/

      // STACK-STYLE OPERATIONS
      void push(std::string theArg);
        /* Add theArg to end. Selector remains. */           
      std::string pop(); 
        /* Remove and return last element. Decrement selector if selected 
           element was popped. Throw error if empty. */
      void fpush(std::string theArg); 
        /* Add theArg to start. Selector increments if > 0. */
      std::string fpop();
        /* Remove and return first element. Selector decrements if > 0*/
  };
// =========== METHODS =========================================== //
// ----------- CONSTRUCTOR --------------------------------------- //
  Vargs::Vargs() {
    this->sel = 0;
  }

  Vargs::Vargs(int argc, char* argv[]) : sel(0) {
    for (int a = 1; a < argc; a++) {
      args.push_back(argv[a]);
    }
  }

// ----------- DESTRUCTOR ----------------------------------------- //
     /* None needed */

// ----------- .size() -------------------------------------------- // TEST
  std::size_t Vargs::size() {
    return this->args.size();
  }

// ----------- .arg() --------------------------------------------- // TEST
  std::string Vargs::arg(std::size_t pos) { 
    if (pos == 0 || pos > this->args.size()) { 
      throw std::out_of_range("Vargs::arg(): pos out of range");
    }
    return this->args.at(pos-1);
  }

// ----------- getall() ------------------------------------------ // TEST
  std::vector<std::string> Vargs::getall() {
    return this->args;
  }

// ----------- sadv() ------------------------------------------- // TEST
  void Vargs::sadv() { 
    if(sel == args.size()) {
      throw std::out_of_range("Vargs::sadv(): selector at end");
    }
    sel++;
  }

// ----------- sret() ------------------------------------------- // TEST
  void Vargs::sret() {
    if(sel <= 1) {
      throw std::out_of_range("Vargs::sret(): selector at start");
    }
    sel--;
  }

// ----------- smov() ------------------------------------------- // TEST
  void Vargs::smov(std::size_t pos) {
    if(pos > args.size()) {
      throw std::out_of_range("Vargs::smov(): pos past end");
    }
    sel = pos;
  }

// ----------- sclr() -------------------------------------------- // TEST
  void Vargs::sclr() {
    sel = 0;
  }

// ----------- sget() -------------------------------------------- // TEST
  std::size_t Vargs::sget() {
    return sel;
  }

// ----------- setsel()------------------------------------------ // TEST
  void Vargs::setsel(std::string theArg) {
    args.at(sel-1) = theArg;
  }

// ----------- getsel()------------------------------------------ // TEST
  std::string Vargs::getsel() {
    return args.at(sel-1);
  }

// ----------- insel() ------------------------------------------ // TEST
  void Vargs::insel(std::string theArg) {
    if (!sel) return;
    args.insert(args.begin()+sel-1,theArg);
  }

// ----------- delsel() ------------------------------------------ // TEST
  std::string Vargs::delsel() {
    std::string toReturn = this->getsel();
    args.erase(args.begin()+sel-1);
    this->sclr();
    return toReturn;
  }

// ----------- push() -------------------------------------------- // TEST 
  void Vargs::push(std::string theArg) {
    args.push_back(theArg);
  }

// ----------- pop() --------------------------------------------- // TEST
  std::string Vargs::pop() {
      /* pop_back() undefined for empty vector. */
    if (this->args.size() == 0) {
      throw(std::out_of_range("Vargs::pop(): Empty vargs"));
    }
    std::string toReturn = this->args.at(args.size()-1);

    if (this->sel == args.size()) { /* Decrement to avoid OOR */     
      sel--;
    }

    args.pop_back();
    return toReturn;
  }

// ----------- fpush() ------------------------------------------- // TEST
  void Vargs::fpush(std::string theArg) {
    args.insert(args.begin(),theArg);
    if (sel) sel++;
  }


// ----------- fpop() -------------------------------------------- // TEST
  std::string Vargs::fpop() {
    if (args.size() == 0) {
      throw(std::out_of_range("Vargs::fpop() Empty vargs"));
    }
    std::string toReturn = args.at(0);
    if (sel) sel--;
    args.erase(args.begin());
    return toReturn;
  }

  #endif // Vargs_H
//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ // TEST
