/* test_Vargs.cpp___________________________________________________
|  TESt FILE FOR VARGS CLASS                                      |
|  Tests Vargs as a command-line argument container, including    |
|  actual CLI input, synthetic argv input, normal operations,     |
|  boundary conditions, and expected exceptions.                  |
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
  const string TERM_YEL      = "\033[33m";
  const string TERM_YEL_BOLD = "\033[1;33m";
  const string TERM_GRN      = "\033[32m";
  const string TERM_CYN      = "\033[36m";
  const string TERM_UNC      = "\033[0m";
  #define RED(a)      TERM_RED      + a + TERM_UNC
  #define YEL(a)      TERM_YEL      + a + TERM_UNC
  #define YEL_BOLD(a) TERM_YEL_BOLD + a + TERM_UNC
  #define GRN(a)      TERM_GRN      + a + TERM_UNC
  #define CYN(a)      TERM_CYN      + a + TERM_UNC
// =========== UTILITIES ========================================= //
  void title(const string& text) {
    cout << YEL_BOLD(
       "\n============================================================\n"
       + text + '\n'
       + "============================================================\n");
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
         << ", pos=" << vargs.pos() << ", args={";

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
      showResult("First CLI argument becomes .arg(0)",
                 cliVargs.arg(0) == argv[1]);
    }
    else {
      showResult("No CLI arguments => empty Vargs",
                 cliVargs.size() == 0);
    }

    /* Test the constructor produces default construction */
    heading("DEFAULT CONSTRUCTION");

    Vargs emptyVargs;
    showVargs("emptyVargs", emptyVargs);
    showResult("Constructor default ." + CYN("size()") + " == 0",
               emptyVargs.size() == 0);
    showResult("Constructor default ." + CYN("pos()") + " == 0",
               emptyVargs.pos() == 0);

    char arg0[] = "croll";   /* Some made-up command */
    char arg1[] = "-b";
    char arg2[] = "source/main.cpp";
    char arg3[] = "name=hello world";
    char arg4[] = "-xyz";
    char* testArgv[] = {arg0, arg1, arg2, arg3, arg4, nullptr};
    int testArgc = 5;          /* CLI: argV[argc] = NULL */

    cout << "\nSynthetic command line represented as argv[]:\n";
    for (int a = 0; a < testArgc; a++) {
      cout << "argv[" << a << "] = \"" << testArgv[a] << "\"\n";
    }

    Vargs fullVargs(testArgc, testArgv);
    showVargs("fullVargs", fullVargs);
    showResult("constructor excludes argv[0]",
               fullVargs.size() == 4 && fullVargs.arg(0) == "-b");
    showResult("constructor preserves argument strings",
               fullVargs.arg(1) == "source/main.cpp" &&
               fullVargs.arg(2) == "name=hello world" &&
               fullVargs.arg(3) == "-xyz");



    heading("INDEXED ACCESS: arg()");

    showResult(CYN("arg(0)"), fullVargs.arg(0) == "-b");
    showResult(CYN("arg(last)"), fullVargs.arg(3) == "-xyz");
    expectOutOfRange(CYN("arg()") + " on empty Vargs",
                     [&]() { emptyVargs.arg(0); });
    expectOutOfRange(CYN("arg()") + " one past end",
                     [&]() { fullVargs.arg(fullVargs.size()); });


    heading("NAVIGATION: getin(), advin(), retin()");

    Vargs navVargs(testArgc, testArgv);
    showResult("initial " + CYN("pos()") + " == 0", navVargs.pos() == 0);
    showResult("initial " + CYN("getin()"), navVargs.getin() == "-b");

    navVargs.advin();
    showResult(CYN("advin()") + " moves 0 -> 1",
               navVargs.pos() == 1 &&
               navVargs.getin() == "source/main.cpp");

    navVargs.advin();
    navVargs.advin();
    showResult("repeated " + CYN("advin()") + " reaches last argument",
               navVargs.pos() == 3 && navVargs.getin() == "-xyz");

    expectOutOfRange(CYN("advin()") + " past last argument",
                     [&]() { navVargs.advin(); });
    showResult("failed " + CYN("advin()") + " leaves position unchanged",
               navVargs.pos() == 3);

    navVargs.retin();
    showResult(CYN("retin()") + " moves 3 -> 2",
               navVargs.pos() == 2 &&
               navVargs.getin() == "name=hello world");

    navVargs.retin();
    navVargs.retin();
    showResult("repeated " + CYN("retin()") + " returns to first argument",
               navVargs.pos() == 0 && navVargs.getin() == "-b");

    expectOutOfRange(CYN("retin()") + " before first argument",
                     [&]() { navVargs.retin(); });
    showResult("failed " + CYN("retin()") + " leaves position unchanged",
               navVargs.pos() == 0);

    expectOutOfRange(CYN("getin()") + " on empty Vargs",
                     [&]() { emptyVargs.getin(); });
    expectOutOfRange(CYN("advin()") + " on empty Vargs",
                     [&]() { emptyVargs.advin(); });
    expectOutOfRange(CYN("retin()") + " on empty Vargs",
                     [&]() { emptyVargs.retin(); });

    Vargs single;
    single.push("only");
    showVargs("single", single);
    showResult("single argument is current at pos 0",
               single.size() == 1 &&
               single.pos() == 0 &&
               single.getin() == "only");
    expectOutOfRange("single-element " + CYN("advin()"),
                     [&]() { single.advin(); });
    expectOutOfRange("single-element " + CYN("retin()"),
                     [&]() { single.retin(); });


    heading("MODIFICATION: setin()");

    Vargs setVargs(testArgc, testArgv);
    setVargs.setin("changed-first-argument");
    showResult(CYN("setin()") + " changes current argument",
               setVargs.getin() == "changed-first-argument" &&
               setVargs.arg(0) == "changed-first-argument");

    setVargs.advin();
    setVargs.setin("changed-second-argument");
    showResult(CYN("setin()") + " changes indicated argument only",
               setVargs.arg(0) == "changed-first-argument" &&
               setVargs.arg(1) == "changed-second-argument" &&
               setVargs.arg(2) == "name=hello world");

    expectOutOfRange(CYN("setin()") + " on empty Vargs",
                     [&]() { emptyVargs.setin("x"); });


    heading("COPY OUT: getall()");

    Vargs copyVargs(testArgc, testArgv);
    vector<string> copied = copyVargs.getall();
    showResult(CYN("getall()") + " returns all arguments",
               copied.size() == 4 &&
               copied[0] == "-b" && copied[3] == "-xyz");

    copied[0] = "modified-copy";
    showResult("modifying " + CYN("getall()") + " result does not modify Vargs",
               copyVargs.arg(0) == "-b");


    heading("STACK-STYLE MODIFICATION: push(), pop(), fpush(), fpop()");

    Vargs backVargs;
    backVargs.push("one");
    showVargs("after push(\"one\")", backVargs);
    showResult(CYN("push()") + " into empty Vargs",
               backVargs.size() == 1 && backVargs.arg(0) == "one");

    backVargs.push("two");
    backVargs.push("three");
    showVargs("after push(\"two\"), push(\"three\")", backVargs);
    showResult(CYN("push()") + " appends in order",
               backVargs.size() == 3 &&
               backVargs.arg(0) == "one" &&
               backVargs.arg(2) == "three");

    try {
      string popped = backVargs.pop();
      showResult(CYN("pop()") + " returns and removes last argument",
                 popped == "three" &&
                 backVargs.size() == 2 &&
                 backVargs.arg(1) == "two");
    }
    catch (const std::exception& err) {
      showResult(CYN("pop()") + " returns and removes last argument", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs emptyBack;
    expectOutOfRange(CYN("pop()") + " on empty Vargs",
                     [&]() { emptyBack.pop(); });



    Vargs frontVargs;
    frontVargs.fpush("one");
    showVargs("after fpush(\"one\")", frontVargs);
    showResult(CYN("fpush()") + " into empty Vargs",
               frontVargs.size() == 1 && frontVargs.arg(0) == "one");

    frontVargs.fpush("zero");
    showVargs("after fpush(\"zero\")", frontVargs);
    showResult(CYN("fpush()") + " inserts at front",
               frontVargs.size() == 2 &&
               frontVargs.arg(0) == "zero" &&
               frontVargs.arg(1) == "one");

    try {
      string fpopped = frontVargs.fpop();
      showResult(CYN("fpop()") + " returns and removes first argument",
                 fpopped == "zero" &&
                 frontVargs.size() == 1 &&
                 frontVargs.arg(0) == "one");
    }
    catch (const std::exception& err) {
      showResult(CYN("fpop()") + " returns and removes first argument", false);
      cout << "       unexpected exception: " << err.what() << '\n';
    }

    Vargs emptyFront;
    expectOutOfRange(CYN("fpop()") + " on empty Vargs",
                     [&]() { emptyFront.fpop(); });


    heading("TEST SUMMARY");
    cout << "Passed: " << testsPassed << '\n';
    cout << "Failed: " << testsFailed << '\n';
    cout << "Total:  " << testsPassed + testsFailed << '\n';

    return testsFailed == 0 ? 0 : 1;
  }

//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ //
