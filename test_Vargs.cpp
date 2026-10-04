/* test_Vargs.cpp___________________________________________________
|  TEST FILE FOR VARGS CLASS                                      |
|  Tests Vargs as a command-line argument container, including    |
|  actual CLI input, synthetic argv input, normal operations,     |
|  selector behavior, boundary conditions, and expected           |
|  exceptions.                                                    |
|_________________________________________________________________*/
// =========== HEADERS =========================================== //
  #include <iostream>
  #include <string>
  #include <vector>
  #include "Vargs.hpp"

  using namespace std;

// =========== DECLARATIONS ====================================== //
  size_t testsPassed = 0;
  size_t testsFailed = 0;

  const string TERM_RED      = "\033[31m";
  const string TERM_YEL_BOLD = "\033[93m";
  const string TERM_GRN      = "\033[32m";
  const string TERM_CYN      = "\033[36m";
  const string TERM_UNC      = "\033[0m";

  #define RED(a) TERM_RED + a + TERM_UNC
  #define YEL(a) TERM_YEL_BOLD + a + TERM_UNC
  #define GRN(a) TERM_GRN + a + TERM_UNC
  #define CYN(a) TERM_CYN + a + TERM_UNC

// =========== UTILITIES ========================================= //
  void title(const string& text) {
    cout << YEL(
      "\n============================================================\n"
      + text +
      "\n============================================================\n");
  }

  void heading(const string& text) {
    cout <<
      "\n------------------------------------------------------------\n";
    cout << YEL(text) << '\n';
    cout <<
      "------------------------------------------------------------\n";
  }

  void showResult(const string& name, bool passed) {
    cout << (passed ? GRN("[PASS]")
                    : RED("[FAIL]"))
         << " " << name << '\n';

    if (passed) testsPassed++;
    else        testsFailed++;
  }

  void showVargs(const string& name, Vargs& vargs) {
    cout << name << ": size=" << vargs.size()
         << ", sel=" << vargs.sget() << ", args={";

    vector<string> args = vargs.getall();

    for (size_t a = 0; a < args.size(); a++) {
      if (a != 0) cout << ", ";
      cout << '"' << args[a] << '"';
    }

    cout << "}\n";
  }

  template <typename Function>
  void expectOutOfRange(const string& name, Function operation) {
    try {
      operation();
      showResult(name + " [expected std::out_of_range]", false);
    }
    catch (const std::out_of_range& err) {
      showResult(name + " [caught std::out_of_range]", true);
      cout << "       what(): " << err.what() << '\n';
    }
    catch (...) {
      showResult(name + " [wrong exception type]", false);
    }
  }

// =========== MAIN ============================================== //
  int main(int argc, char* argv[]) {
    title("TEST: Vargs class");

// ----------- COMMAND LINE INPUT -------------------------------- //
    heading("COMMAND LINE INPUT");

    cout << "argc = " << argc << '\n';

    for (int a = 0; a < argc; a++) {
      cout << "argv[" << a << "] = \"" << argv[a] << "\"\n";
    }

    Vargs cliVargs(argc, argv);
    showVargs("cliVargs", cliVargs);

    showResult("argv[0] excluded from object",
               cliVargs.size() == static_cast<size_t>(argc - 1));

    if (argc > 1) {
      showResult("First CLI argument becomes ." + CYN("arg(1)"),
                 cliVargs.arg(1) == argv[1]);
    }
    else {
      showResult("No CLI arguments => empty Vargs",
                 cliVargs.size() == 0);
    }

    showResult("CLI constructor initializes selector to zero",
               cliVargs.sget() == 0);

// ----------- DEFAULT CONSTRUCTION ------------------------------ //
    heading("DEFAULT CONSTRUCTION");

    Vargs emptyVargs;
    showVargs("emptyVargs", emptyVargs);

    showResult("Constructor default .size() == 0",
               emptyVargs.size() == 0);

    showResult("Constructor default ." + CYN("sget()") + " == 0",
               emptyVargs.sget() == 0);

// ----------- SYNTHETIC COMMAND LINE ---------------------------- //
    char arg0[] = "croll";
    char arg1[] = "-b";
    char arg2[] = "source/main.cpp";
    char arg3[] = "name=hello world";
    char arg4[] = "-xyz";

    char* testArgv[] = {arg0, arg1, arg2, arg3, arg4, nullptr};
    int testArgc = 5;

    cout << "\nSynthetic command line represented as argv[]:\n";

    for (int a = 0; a < testArgc; a++) {
      cout << "argv[" << a << "] = \""
           << testArgv[a] << "\"\n";
    }

    Vargs fullVargs(testArgc, testArgv);
    showVargs("fullVargs", fullVargs);

    showResult("constructor excludes argv[0]",
               fullVargs.size() == 4 &&
               fullVargs.arg(1) == "-b");

    showResult("constructor preserves argument strings",
               fullVargs.arg(1) == "-b" &&
               fullVargs.arg(2) == "source/main.cpp" &&
               fullVargs.arg(3) == "name=hello world" &&
               fullVargs.arg(4) == "-xyz");

    showResult("synthetic constructor initializes selector to zero",
               fullVargs.sget() == 0);

// ----------- INDEXED ACCESS ------------------------------------ //
    heading("INDEXED ACCESS: arg()");

    showResult(CYN("arg(1)"), fullVargs.arg(1) == "-b");
    showResult(CYN("arg(last)"),
               fullVargs.arg(fullVargs.size()) == "-xyz");

    expectOutOfRange(CYN("arg(0)"), [&]() { fullVargs.arg(0); });
    expectOutOfRange(CYN("arg()") + " one past end",
                     [&]() { fullVargs.arg(fullVargs.size() + 1); });
    expectOutOfRange(CYN("arg()") + " on empty Vargs",
                     [&]() { emptyVargs.arg(1); });

// ----------- SELECTOR VALUE ------------------------------------ //
    heading("SELECTOR VALUE: sget(), smov(), sclr()");

    Vargs selVargs(testArgc, testArgv);

    showResult(CYN("sget()") + " initially returns zero",
               selVargs.sget() == 0);

    selVargs.smov(1);
    showResult(CYN("smov(1)") + " selects first argument",
               selVargs.sget() == 1 &&
               selVargs.getsel() == "-b");

    selVargs.smov(3);
    showResult(CYN("smov(3)") + " selects third argument",
               selVargs.sget() == 3 &&
               selVargs.getsel() == "name=hello world");

    selVargs.smov(selVargs.size());
    showResult(CYN("smov(size()") + ") selects last argument",
               selVargs.sget() == selVargs.size() &&
               selVargs.getsel() == "-xyz");

    selVargs.smov(0);
    showResult(CYN("smov(0)") + " clears selector",
               selVargs.sget() == 0);

    expectOutOfRange(CYN("smov()") + " past end",
                     [&]() { selVargs.smov(selVargs.size() + 1); });

    showResult("failed " + CYN("smov()") + " leaves selector unchanged",
               selVargs.sget() == 0);

    emptyVargs.smov(0);
    showResult(CYN("smov(0)") + " valid on empty Vargs",
               emptyVargs.sget() == 0);

    expectOutOfRange(CYN("smov(1)") + " on empty Vargs",
                     [&]() { emptyVargs.smov(1); });

    selVargs.smov(2);
    selVargs.sclr();
    showResult(CYN("sclr()") + " resets selector",
               selVargs.sget() == 0);

    emptyVargs.sclr();
    showResult(CYN("sclr()") + " valid on empty Vargs",
               emptyVargs.sget() == 0);

// ----------- SELECTOR MOVEMENT --------------------------------- //
    heading("SELECTOR MOVEMENT: sadv(), sret()");

    Vargs navVargs(testArgc, testArgv);

    navVargs.sadv();
    showResult(CYN("sadv()") + " moves 0 -> 1",
               navVargs.sget() == 1 &&
               navVargs.getsel() == "-b");

    navVargs.sadv();
    showResult(CYN("sadv()") + " moves 1 -> 2",
               navVargs.sget() == 2 &&
               navVargs.getsel() == "source/main.cpp");

    navVargs.sadv();
    navVargs.sadv();
    showResult("repeated " + CYN("sadv()") + " reaches last argument",
               navVargs.sget() == navVargs.size() &&
               navVargs.getsel() == "-xyz");

    expectOutOfRange(CYN("sadv()") + " past last argument",
                     [&]() { navVargs.sadv(); });

    showResult("failed " + CYN("sadv()") + " leaves selector unchanged",
               navVargs.sget() == navVargs.size());

    navVargs.sret();
    showResult(CYN("sret()") + " moves 4 -> 3",
               navVargs.sget() == 3 &&
               navVargs.getsel() == "name=hello world");

    navVargs.sret();
    navVargs.sret();
    showResult("repeated " + CYN("sret()") + " reaches first argument",
               navVargs.sget() == 1 &&
               navVargs.getsel() == "-b");

    expectOutOfRange(CYN("sret()") + " from first argument",
                     [&]() { navVargs.sret(); });

    showResult("failed " + CYN("sret()") + " leaves selector unchanged",
               navVargs.sget() == 1);

    navVargs.sclr();

    expectOutOfRange(CYN("sret()") + " from selector zero",
                     [&]() { navVargs.sret(); });

    showResult("failed " + CYN("sret()") + " from zero leaves selector zero",
               navVargs.sget() == 0);

    expectOutOfRange(CYN("sadv()") + " on empty Vargs",
                     [&]() { emptyVargs.sadv(); });

    expectOutOfRange(CYN("sret()") + " on empty Vargs",
                     [&]() { emptyVargs.sret(); });

// ----------- SELECTED ELEMENT ---------------------------------- //
    heading("SELECTED ELEMENT: getsel(), setsel()");

    Vargs selectedVargs(testArgc, testArgv);

    expectOutOfRange(CYN("getsel()") + " with selector zero",
                     [&]() { selectedVargs.getsel(); });

    expectOutOfRange(CYN("setsel()") + " with selector zero",
                     [&]() { selectedVargs.setsel("should-not-set"); });

    selectedVargs.smov(1);

    showResult(CYN("getsel()") + " returns first selected element",
               selectedVargs.getsel() == "-b");

    selectedVargs.setsel("changed-first");

    showResult(CYN("setsel()") + " changes first selected element",
               selectedVargs.getsel() == "changed-first" &&
               selectedVargs.arg(1) == "changed-first" &&
               selectedVargs.sget() == 1);

    selectedVargs.smov(3);
    selectedVargs.setsel("changed-third");

    showResult(CYN("setsel()") + " changes selected element only",
               selectedVargs.arg(1) == "changed-first" &&
               selectedVargs.arg(2) == "source/main.cpp" &&
               selectedVargs.arg(3) == "changed-third" &&
               selectedVargs.arg(4) == "-xyz");

    selectedVargs.smov(selectedVargs.size());

    showResult(CYN("getsel()") + " works on last element",
               selectedVargs.getsel() == "-xyz");

    expectOutOfRange(CYN("getsel()") + " on empty Vargs",
                     [&]() { emptyVargs.getsel(); });

    expectOutOfRange(CYN("setsel()") + " on empty Vargs",
                     [&]() { emptyVargs.setsel("x"); });

// ----------- DIRECT INSERTION ---------------------------------- //
    heading("DIRECT INSERTION: insel()");

    Vargs insertZero(testArgc, testArgv);
    vector<string> beforeInsertZero = insertZero.getall();

    insertZero.insel("ignored");

    showResult(CYN("insel()") + " with selector zero does nothing",
               insertZero.getall() == beforeInsertZero &&
               insertZero.sget() == 0);

    Vargs insertFirst(testArgc, testArgv);
    insertFirst.smov(1);
    insertFirst.insel("new-first");

    showVargs("insertFirst", insertFirst);

    showResult(CYN("insel()") + " at selector 1 inserts first",
               insertFirst.size() == 5 &&
               insertFirst.sget() == 1 &&
               insertFirst.arg(1) == "new-first" &&
               insertFirst.arg(2) == "-b" &&
               insertFirst.getsel() == "new-first");

    Vargs insertMiddle(testArgc, testArgv);
    insertMiddle.smov(3);
    insertMiddle.insel("new-third");

    showResult(CYN("insel()") + " inserts at selected position",
               insertMiddle.size() == 5 &&
               insertMiddle.sget() == 3 &&
               insertMiddle.arg(2) == "source/main.cpp" &&
               insertMiddle.arg(3) == "new-third" &&
               insertMiddle.arg(4) == "name=hello world" &&
               insertMiddle.getsel() == "new-third");

    Vargs insertLast(testArgc, testArgv);
    insertLast.smov(insertLast.size());
    insertLast.insel("insert-at-four");

    showResult(CYN("insel()") + " at last selector inserts at that position",
               insertLast.size() == 5 &&
               insertLast.sget() == 4 &&
               insertLast.arg(4) == "insert-at-four" &&
               insertLast.arg(5) == "-xyz" &&
               insertLast.getsel() == "insert-at-four");

    Vargs emptyInsert;
    emptyInsert.insel("ignored");

    showResult(CYN("insel()") + " on empty Vargs does nothing",
               emptyInsert.size() == 0 &&
               emptyInsert.sget() == 0);

// ----------- DIRECT DELETION ----------------------------------- //
    heading("DIRECT DELETION: delsel()");

    Vargs deleteVargs(testArgc, testArgv);

    expectOutOfRange(CYN("delsel()") + " with selector zero",
                     [&]() { deleteVargs.delsel(); });

    showResult("failed " + CYN("delsel()") + " leaves object unchanged",
               deleteVargs.size() == 4 &&
               deleteVargs.sget() == 0 &&
               deleteVargs.arg(1) == "-b" &&
               deleteVargs.arg(4) == "-xyz");

    deleteVargs.smov(2);

    try {
      string deleted = deleteVargs.delsel();

      showResult(CYN("delsel()") + " returns and deletes selected element",
                 deleted == "source/main.cpp" &&
                 deleteVargs.size() == 3 &&
                 deleteVargs.arg(1) == "-b" &&
                 deleteVargs.arg(2) == "name=hello world" &&
                 deleteVargs.arg(3) == "-xyz");

      showResult(CYN("delsel()") + " resets selector",
                 deleteVargs.sget() == 0);
    }
    catch (const std::exception& err) {
      showResult(CYN("delsel()") + " returns and deletes selected element", false);
      showResult(CYN("delsel()") + " resets selector", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs deleteFirst(testArgc, testArgv);
    deleteFirst.smov(1);

    try {
      string deleted = deleteFirst.delsel();

      showResult(CYN("delsel()") + " deletes first element",
                 deleted == "-b" &&
                 deleteFirst.size() == 3 &&
                 deleteFirst.arg(1) == "source/main.cpp" &&
                 deleteFirst.sget() == 0);
    }
    catch (const std::exception& err) {
      showResult(CYN("delsel()") + " deletes first element", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs deleteLast(testArgc, testArgv);
    deleteLast.smov(deleteLast.size());

    try {
      string deleted = deleteLast.delsel();

      showResult(CYN("delsel()") + " deletes last element",
                 deleted == "-xyz" &&
                 deleteLast.size() == 3 &&
                 deleteLast.arg(3) == "name=hello world" &&
                 deleteLast.sget() == 0);
    }
    catch (const std::exception& err) {
      showResult(CYN("delsel()") + " deletes last element", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs singleDelete;
    singleDelete.push("only");
    singleDelete.smov(1);

    try {
      string deleted = singleDelete.delsel();

      showResult(CYN("delsel()") + " deletes sole element",
                 deleted == "only" &&
                 singleDelete.size() == 0 &&
                 singleDelete.sget() == 0);
    }
    catch (const std::exception& err) {
      showResult(CYN("delsel()") + " deletes sole element", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs emptyDelete;

    expectOutOfRange(CYN("delsel()") + " on empty Vargs",
                     [&]() { emptyDelete.delsel(); });

// ----------- COPY OUT ------------------------------------------ //
    heading("COPY OUT: getall()");

    Vargs copyVargs(testArgc, testArgv);
    vector<string> copied = copyVargs.getall();

    showResult(CYN("getall()") + " returns all arguments",
               copied.size() == 4 &&
               copied[0] == "-b" &&
               copied[1] == "source/main.cpp" &&
               copied[2] == "name=hello world" &&
               copied[3] == "-xyz");

    copied[0] = "modified-copy";

    showResult("modifying " + CYN("getall()") + " result does not modify Vargs",
               copyVargs.arg(1) == "-b");

    showResult(CYN("getall()") + " on empty Vargs returns empty vector",
               emptyVargs.getall().empty());

// ----------- PUSH ---------------------------------------------- //
    heading("STACK-STYLE MODIFICATION: push()");

    Vargs pushVargs;

    pushVargs.push("one");

    showVargs("after push(\"one\")", pushVargs);

    showResult(CYN("push()") + " into empty Vargs",
               pushVargs.size() == 1 &&
               pushVargs.arg(1) == "one" &&
               pushVargs.sget() == 0);

    pushVargs.smov(1);
    pushVargs.push("two");

    showResult(CYN("push()") + " preserves existing selector",
               pushVargs.size() == 2 &&
               pushVargs.sget() == 1 &&
               pushVargs.getsel() == "one" &&
               pushVargs.arg(2) == "two");

    pushVargs.push("three");

    showResult("repeated " + CYN("push()") + " appends in order",
               pushVargs.size() == 3 &&
               pushVargs.sget() == 1 &&
               pushVargs.arg(1) == "one" &&
               pushVargs.arg(2) == "two" &&
               pushVargs.arg(3) == "three");

// ----------- POP ----------------------------------------------- //
    heading("STACK-STYLE MODIFICATION: pop()");

    Vargs popUnselected;
    popUnselected.push("one");
    popUnselected.push("two");
    popUnselected.push("three");
    popUnselected.smov(1);

    try {
      string popped = popUnselected.pop();

      showResult(CYN("pop()") + " removes and returns last argument",
                 popped == "three" &&
                 popUnselected.size() == 2 &&
                 popUnselected.arg(2) == "two");

      showResult(CYN("pop()") + " preserves selector if selected element remains",
                 popUnselected.sget() == 1 &&
                 popUnselected.getsel() == "one");
    }
    catch (const std::exception& err) {
      showResult(CYN("pop()") + " removes and returns last argument", false);
      showResult(CYN("pop()") + " preserves selector if selected element remains",
                 false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs popSelected;
    popSelected.push("one");
    popSelected.push("two");
    popSelected.push("three");
    popSelected.smov(3);

    try {
      string popped = popSelected.pop();

      showResult(CYN("pop()") + " of selected last element decrements selector",
                 popped == "three" &&
                 popSelected.size() == 2 &&
                 popSelected.sget() == 2 &&
                 popSelected.getsel() == "two");
    }
    catch (const std::exception& err) {
      showResult(CYN("pop()") + " of selected last element decrements selector",
                 false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs popNull;
    popNull.push("one");
    popNull.push("two");

    try {
      popNull.pop();

      showResult(CYN("pop()") + " preserves null selector",
                 popNull.size() == 1 &&
                 popNull.sget() == 0);
    }
    catch (const std::exception& err) {
      showResult(CYN("pop()") + " preserves null selector", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs popSingle;
    popSingle.push("only");
    popSingle.smov(1);

    try {
      string popped = popSingle.pop();

      showResult(CYN("pop()") + " selected sole element => selector zero",
                 popped == "only" &&
                 popSingle.size() == 0 &&
                 popSingle.sget() == 0);
    }
    catch (const std::exception& err) {
      showResult(CYN("pop()") + " selected sole element => selector zero", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs emptyBack;

    expectOutOfRange(CYN("pop()") + " on empty Vargs",
                     [&]() { emptyBack.pop(); });

// ----------- FPUSH --------------------------------------------- //
    heading("STACK-STYLE MODIFICATION: fpush()");

    Vargs fpushVargs;

    fpushVargs.fpush("one");

    showResult(CYN("fpush()") + " into empty Vargs",
               fpushVargs.size() == 1 &&
               fpushVargs.arg(1) == "one" &&
               fpushVargs.sget() == 0);

    fpushVargs.fpush("zero");

    showResult(CYN("fpush()") + " inserts at front with null selector",
               fpushVargs.size() == 2 &&
               fpushVargs.arg(1) == "zero" &&
               fpushVargs.arg(2) == "one" &&
               fpushVargs.sget() == 0);

    Vargs fpushSelected;
    fpushSelected.push("one");
    fpushSelected.push("two");
    fpushSelected.push("three");
    fpushSelected.smov(2);

    fpushSelected.fpush("zero");

    showVargs("fpushSelected", fpushSelected);

    showResult(CYN("fpush()") + " increments nonzero selector",
               fpushSelected.size() == 4 &&
               fpushSelected.sget() == 3 &&
               fpushSelected.getsel() == "two" &&
               fpushSelected.arg(1) == "zero");

    Vargs fpushFirst;
    fpushFirst.push("one");
    fpushFirst.push("two");
    fpushFirst.smov(1);
    fpushFirst.fpush("zero");

    showResult(CYN("fpush()") + " preserves selected element at first position",
               fpushFirst.sget() == 2 &&
               fpushFirst.getsel() == "one");

// ----------- FPOP ---------------------------------------------- //
    heading("STACK-STYLE MODIFICATION: fpop()");

    Vargs fpopNull;
    fpopNull.push("zero");
    fpopNull.push("one");
    fpopNull.push("two");

    try {
      string popped = fpopNull.fpop();

      showResult(CYN("fpop()") + " removes and returns first argument",
                 popped == "zero" &&
                 fpopNull.size() == 2 &&
                 fpopNull.arg(1) == "one");

      showResult(CYN("fpop()") + " preserves null selector",
                 fpopNull.sget() == 0);
    }
    catch (const std::exception& err) {
      showResult(CYN("fpop()") + " removes and returns first argument", false);
      showResult(CYN("fpop()") + " preserves null selector", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs fpopMiddle;
    fpopMiddle.push("zero");
    fpopMiddle.push("one");
    fpopMiddle.push("two");
    fpopMiddle.smov(2);

    try {
      string popped = fpopMiddle.fpop();

      showResult(CYN("fpop()") + " decrements selector to preserve element",
                 popped == "zero" &&
                 fpopMiddle.size() == 2 &&
                 fpopMiddle.sget() == 1 &&
                 fpopMiddle.getsel() == "one");
    }
    catch (const std::exception& err) {
      showResult(CYN("fpop()") + " decrements selector to preserve element", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs fpopSelectedFirst;
    fpopSelectedFirst.push("zero");
    fpopSelectedFirst.push("one");
    fpopSelectedFirst.smov(1);

    try {
      string popped = fpopSelectedFirst.fpop();

      showResult(CYN("fpop()") + " selected first element => selector zero",
                 popped == "zero" &&
                 fpopSelectedFirst.size() == 1 &&
                 fpopSelectedFirst.sget() == 0 &&
                 fpopSelectedFirst.arg(1) == "one");
    }
    catch (const std::exception& err) {
      showResult(CYN("fpop()") + " selected first element => selector zero", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs fpopSingle;
    fpopSingle.push("only");
    fpopSingle.smov(1);

    try {
      string popped = fpopSingle.fpop();

      showResult(CYN("fpop()") + " selected sole element => selector zero",
                 popped == "only" &&
                 fpopSingle.size() == 0 &&
                 fpopSingle.sget() == 0);
    }
    catch (const std::exception& err) {
      showResult(CYN("fpop()") + " selected sole element => selector zero", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs emptyFront;

    expectOutOfRange(CYN("fpop()") + " on empty Vargs",
                     [&]() { emptyFront.fpop(); });

// ----------- TEST SUMMARY -------------------------------------- //
    heading("TEST SUMMARY");

    cout << "Passed: " << testsPassed << '\n';
    cout << "Failed: " << testsFailed << '\n';
    cout << "Total:  " << testsPassed + testsFailed << '\n';

    return testsFailed == 0 ? 0 : 1;
  }

//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ //
