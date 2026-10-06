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
  #include "testutils/testutils.h"

  using namespace std;
  using namespace test;

// =========== MAIN ============================================== //
  int main(int argc, char* argv[]) {
    printTitle("TEST: Vargs class");

// ----------- COMMAND LINE INPUT -------------------------------- //
    printHeading("COMMAND LINE INPUT");

    printVariable("argc", argc);

    for (int a = 0; a < argc; a++) {
      cout << "argv[" << a << "] = \"" << argv[a] << "\"\n";
    }

    Vargs cliVargs(argc, argv);
    printVariable("cliVargs.size()", cliVargs.size());
    printVariable("cliVargs.sget()", cliVargs.sget());
    printArray("cliVargs.getall()", cliVargs.getall());

    printResult("argv[0] excluded from object",
               cliVargs.size() == static_cast<size_t>(argc - 1));

    if (argc > 1) {
      printResult("First CLI argument becomes ." + TU_CYN("arg(1)"),
                 cliVargs.arg(1) == argv[1]);
    }
    else {
      printResult("No CLI arguments => empty Vargs",
                 cliVargs.size() == 0);
    }

    printResult("CLI constructor initializes selector to zero",
               cliVargs.sget() == 0);

// ----------- DEFAULT CONSTRUCTION ------------------------------ //
    printHeading("DEFAULT CONSTRUCTION");

    Vargs emptyVargs;
    printVariable("emptyVargs.size()", emptyVargs.size());
    printVariable("emptyVargs.sget()", emptyVargs.sget());
    printArray("emptyVargs.getall()", emptyVargs.getall());

    printResult("Constructor default .size() == 0",
               emptyVargs.size() == 0);

    printResult("Constructor default ." + TU_CYN("sget()") + " == 0",
               emptyVargs.sget() == 0);

// ----------- SYNTHETIC COMMAND LINE ---------------------------- //
    char arg0[] = "croll";
    char arg1[] = "-b";
    char arg2[] = "source/main.cpp";
    char arg3[] = "name=hello world";
    char arg4[] = "-xyz";

    char* testArgv[] = {arg0, arg1, arg2, arg3, arg4};
    int testArgc = 5;

    printNote("Synthetic command line represented as argv[]:");
    printArray("testArgv", testArgv);

    Vargs fullVargs(testArgc, testArgv);
    printVariable("fullVargs.size()", fullVargs.size());
    printVariable("fullVargs.sget()", fullVargs.sget());
    printArray("fullVargs.getall()", fullVargs.getall());

    printResult("constructor excludes argv[0]",
               fullVargs.size() == 4 &&
               fullVargs.arg(1) == "-b");

    printResult("constructor preserves argument strings",
               fullVargs.arg(1) == "-b" &&
               fullVargs.arg(2) == "source/main.cpp" &&
               fullVargs.arg(3) == "name=hello world" &&
               fullVargs.arg(4) == "-xyz");

    printResult("synthetic constructor initializes selector to zero",
               fullVargs.sget() == 0);

// ----------- INDEXED ACCESS ------------------------------------ //
    printHeading("INDEXED ACCESS: arg()");

    printResult(TU_CYN("arg(1)"), fullVargs.arg(1) == "-b");
    printResult(TU_CYN("arg(last)"),
               fullVargs.arg(fullVargs.size()) == "-xyz");
    EXPECT_NG(fullVargs.arg(0), TU_CYN("arg(0)"), std::out_of_range);
    EXPECT_NG(fullVargs.arg(fullVargs.size() + 1), TU_CYN("arg()") + " one past end", std::out_of_range);
    EXPECT_NG(emptyVargs.arg(1), TU_CYN("arg()") + " on empty Vargs", std::out_of_range);

// ----------- SELECTOR VALUE ------------------------------------ //
    printHeading("SELECTOR VALUE: sget(), smov(), sclr()");

    Vargs selVargs(testArgc, testArgv);

    printResult(TU_CYN("sget()") + " initially returns zero",
               selVargs.sget() == 0);

    selVargs.smov(1);
    printResult(TU_CYN("smov(1)") + " selects first argument",
               selVargs.sget() == 1 &&
               selVargs.getsel() == "-b");

    selVargs.smov(3);
    printResult(TU_CYN("smov(3)") + " selects third argument",
               selVargs.sget() == 3 &&
               selVargs.getsel() == "name=hello world");

    selVargs.smov(selVargs.size());
    printResult(TU_CYN("smov(size()") + ") selects last argument",
               selVargs.sget() == selVargs.size() &&
               selVargs.getsel() == "-xyz");

    selVargs.smov(0);
    printResult(TU_CYN("smov(0)") + " clears selector",
               selVargs.sget() == 0);
    EXPECT_NG(selVargs.smov(selVargs.size() + 1), TU_CYN("smov()") + " past end", std::out_of_range);

    printResult("failed " + TU_CYN("smov()") + " leaves selector unchanged",
               selVargs.sget() == 0);

    emptyVargs.smov(0);
    printResult(TU_CYN("smov(0)") + " valid on empty Vargs",
               emptyVargs.sget() == 0);
    EXPECT_NG(emptyVargs.smov(1), TU_CYN("smov(1)") + " on empty Vargs", std::out_of_range);

    selVargs.smov(2);
    selVargs.sclr();
    printResult(TU_CYN("sclr()") + " resets selector",
               selVargs.sget() == 0);

    emptyVargs.sclr();
    printResult(TU_CYN("sclr()") + " valid on empty Vargs",
               emptyVargs.sget() == 0);

// ----------- SELECTOR MOVEMENT --------------------------------- //
    printHeading("SELECTOR MOVEMENT: sadv(), sret()");

    Vargs navVargs(testArgc, testArgv);

    navVargs.sadv();
    printResult(TU_CYN("sadv()") + " moves 0 -> 1",
               navVargs.sget() == 1 &&
               navVargs.getsel() == "-b");

    navVargs.sadv();
    printResult(TU_CYN("sadv()") + " moves 1 -> 2",
               navVargs.sget() == 2 &&
               navVargs.getsel() == "source/main.cpp");

    navVargs.sadv();
    navVargs.sadv();
    printResult("repeated " + TU_CYN("sadv()") + " reaches last argument",
               navVargs.sget() == navVargs.size() &&
               navVargs.getsel() == "-xyz");
    EXPECT_NG(navVargs.sadv(), TU_CYN("sadv()") + " past last argument", std::out_of_range);

    printResult("failed " + TU_CYN("sadv()") + " leaves selector unchanged",
               navVargs.sget() == navVargs.size());

    navVargs.sret();
    printResult(TU_CYN("sret()") + " moves 4 -> 3",
               navVargs.sget() == 3 &&
               navVargs.getsel() == "name=hello world");

    navVargs.sret();
    navVargs.sret();
    printResult("repeated " + TU_CYN("sret()") + " reaches first argument",
               navVargs.sget() == 1 &&
               navVargs.getsel() == "-b");
    EXPECT_NG(navVargs.sret(), TU_CYN("sret()") + " from first argument", std::out_of_range);

    printResult("failed " + TU_CYN("sret()") + " leaves selector unchanged",
               navVargs.sget() == 1);

    navVargs.sclr();
    EXPECT_NG(navVargs.sret(), TU_CYN("sret()") + " from selector zero", std::out_of_range);

    printResult("failed " + TU_CYN("sret()") + " from zero leaves selector zero",
               navVargs.sget() == 0);
    EXPECT_NG(emptyVargs.sadv(), TU_CYN("sadv()") + " on empty Vargs", std::out_of_range);
    EXPECT_NG(emptyVargs.sret(), TU_CYN("sret()") + " on empty Vargs", std::out_of_range);

// ----------- SELECTED ELEMENT ---------------------------------- //
    printHeading("SELECTED ELEMENT: getsel(), setsel()");

    Vargs selectedVargs(testArgc, testArgv);
    EXPECT_NG(selectedVargs.getsel(), TU_CYN("getsel()") + " with selector zero", std::out_of_range);
    EXPECT_NG(selectedVargs.setsel("should-not-set"), TU_CYN("setsel()") + " with selector zero", std::out_of_range);

    selectedVargs.smov(1);

    printResult(TU_CYN("getsel()") + " returns first selected element",
               selectedVargs.getsel() == "-b");

    selectedVargs.setsel("changed-first");

    printResult(TU_CYN("setsel()") + " changes first selected element",
               selectedVargs.getsel() == "changed-first" &&
               selectedVargs.arg(1) == "changed-first" &&
               selectedVargs.sget() == 1);

    selectedVargs.smov(3);
    selectedVargs.setsel("changed-third");

    printResult(TU_CYN("setsel()") + " changes selected element only",
               selectedVargs.arg(1) == "changed-first" &&
               selectedVargs.arg(2) == "source/main.cpp" &&
               selectedVargs.arg(3) == "changed-third" &&
               selectedVargs.arg(4) == "-xyz");

    selectedVargs.smov(selectedVargs.size());

    printResult(TU_CYN("getsel()") + " works on last element",
               selectedVargs.getsel() == "-xyz");
    EXPECT_NG(emptyVargs.getsel(), TU_CYN("getsel()") + " on empty Vargs", std::out_of_range);
    EXPECT_NG(emptyVargs.setsel("x"), TU_CYN("setsel()") + " on empty Vargs", std::out_of_range);

// ----------- DIRECT INSERTION ---------------------------------- //
    printHeading("DIRECT INSERTION: insel()");

    Vargs insertZero(testArgc, testArgv);
    vector<string> beforeInsertZero = insertZero.getall();

    insertZero.insel("ignored");

    printResult(TU_CYN("insel()") + " with selector zero does nothing",
               insertZero.getall() == beforeInsertZero &&
               insertZero.sget() == 0);

    Vargs insertFirst(testArgc, testArgv);
    insertFirst.smov(1);
    insertFirst.insel("new-first");

    printVariable("insertFirst.size()", insertFirst.size());
    printVariable("insertFirst.sget()", insertFirst.sget());
    printArray("insertFirst.getall()", insertFirst.getall());

    printResult(TU_CYN("insel()") + " at selector 1 inserts first",
               insertFirst.size() == 5 &&
               insertFirst.sget() == 1 &&
               insertFirst.arg(1) == "new-first" &&
               insertFirst.arg(2) == "-b" &&
               insertFirst.getsel() == "new-first");

    Vargs insertMiddle(testArgc, testArgv);
    insertMiddle.smov(3);
    insertMiddle.insel("new-third");

    printResult(TU_CYN("insel()") + " inserts at selected position",
               insertMiddle.size() == 5 &&
               insertMiddle.sget() == 3 &&
               insertMiddle.arg(2) == "source/main.cpp" &&
               insertMiddle.arg(3) == "new-third" &&
               insertMiddle.arg(4) == "name=hello world" &&
               insertMiddle.getsel() == "new-third");

    Vargs insertLast(testArgc, testArgv);
    insertLast.smov(insertLast.size());
    insertLast.insel("insert-at-four");

    printResult(TU_CYN("insel()") + " at last selector inserts at that position",
               insertLast.size() == 5 &&
               insertLast.sget() == 4 &&
               insertLast.arg(4) == "insert-at-four" &&
               insertLast.arg(5) == "-xyz" &&
               insertLast.getsel() == "insert-at-four");

    Vargs emptyInsert;
    emptyInsert.insel("ignored");

    printResult(TU_CYN("insel()") + " on empty Vargs does nothing",
               emptyInsert.size() == 0 &&
               emptyInsert.sget() == 0);

// ----------- DIRECT DELETION ----------------------------------- //
    printHeading("DIRECT DELETION: delsel()");

    Vargs deleteVargs(testArgc, testArgv);
    EXPECT_NG(deleteVargs.delsel(), TU_CYN("delsel()") + " with selector zero", std::out_of_range);

    printResult("failed " + TU_CYN("delsel()") + " leaves object unchanged",
               deleteVargs.size() == 4 &&
               deleteVargs.sget() == 0 &&
               deleteVargs.arg(1) == "-b" &&
               deleteVargs.arg(4) == "-xyz");

    deleteVargs.smov(2);

    EXPECT_OK(
      {
      string deleted = deleteVargs.delsel();

      printResult(TU_CYN("delsel()") + " returns and deletes selected element",
                 deleted == "source/main.cpp" &&
                 deleteVargs.size() == 3 &&
                 deleteVargs.arg(1) == "-b" &&
                 deleteVargs.arg(2) == "name=hello world" &&
                 deleteVargs.arg(3) == "-xyz");

      printResult(TU_CYN("delsel()") + " resets selector",
                 deleteVargs.sget() == 0);
      },
      TU_CYN("delsel()") + " completes without exception"
    );

    Vargs deleteFirst(testArgc, testArgv);
    deleteFirst.smov(1);

    EXPECT_OK(
      {
      string deleted = deleteFirst.delsel();

      printResult(TU_CYN("delsel()") + " deletes first element",
                 deleted == "-b" &&
                 deleteFirst.size() == 3 &&
                 deleteFirst.arg(1) == "source/main.cpp" &&
                 deleteFirst.sget() == 0);
      },
      TU_CYN("delsel()") + " completes without exception"
    );

    Vargs deleteLast(testArgc, testArgv);
    deleteLast.smov(deleteLast.size());

    EXPECT_OK(
      {
      string deleted = deleteLast.delsel();

      printResult(TU_CYN("delsel()") + " deletes last element",
                 deleted == "-xyz" &&
                 deleteLast.size() == 3 &&
                 deleteLast.arg(3) == "name=hello world" &&
                 deleteLast.sget() == 0);
      },
      TU_CYN("delsel()") + " completes without exception"
    );

    Vargs singleDelete;
    singleDelete.push("only");
    singleDelete.smov(1);

    EXPECT_OK(
      {
      string deleted = singleDelete.delsel();

      printResult(TU_CYN("delsel()") + " deletes sole element",
                 deleted == "only" &&
                 singleDelete.size() == 0 &&
                 singleDelete.sget() == 0);
      },
      TU_CYN("delsel()") + " completes without exception"
    );

    Vargs emptyDelete;
    EXPECT_NG(emptyDelete.delsel(), TU_CYN("delsel()") + " on empty Vargs", std::out_of_range);

// ----------- COPY OUT ------------------------------------------ //
    printHeading("COPY OUT: getall()");

    Vargs copyVargs(testArgc, testArgv);
    vector<string> copied = copyVargs.getall();

    printResult(TU_CYN("getall()") + " returns all arguments",
               copied.size() == 4 &&
               copied[0] == "-b" &&
               copied[1] == "source/main.cpp" &&
               copied[2] == "name=hello world" &&
               copied[3] == "-xyz");

    copied[0] = "modified-copy";

    printResult("modifying " + TU_CYN("getall()") + " result does not modify Vargs",
               copyVargs.arg(1) == "-b");

    printResult(TU_CYN("getall()") + " on empty Vargs returns empty vector",
               emptyVargs.getall().empty());

// ----------- PUSH ---------------------------------------------- //
    printHeading("STACK-STYLE MODIFICATION: push()");

    Vargs pushVargs;

    pushVargs.push("one");

    printVariable("pushVargs.size()", pushVargs.size());
    printVariable("pushVargs.sget()", pushVargs.sget());
    printArray("pushVargs.getall()", pushVargs.getall());

    printResult(TU_CYN("push()") + " into empty Vargs",
               pushVargs.size() == 1 &&
               pushVargs.arg(1) == "one" &&
               pushVargs.sget() == 0);

    pushVargs.smov(1);
    pushVargs.push("two");

    printResult(TU_CYN("push()") + " preserves existing selector",
               pushVargs.size() == 2 &&
               pushVargs.sget() == 1 &&
               pushVargs.getsel() == "one" &&
               pushVargs.arg(2) == "two");

    pushVargs.push("three");

    printResult("repeated " + TU_CYN("push()") + " appends in order",
               pushVargs.size() == 3 &&
               pushVargs.sget() == 1 &&
               pushVargs.arg(1) == "one" &&
               pushVargs.arg(2) == "two" &&
               pushVargs.arg(3) == "three");

// ----------- POP ----------------------------------------------- //
    printHeading("STACK-STYLE MODIFICATION: pop()");

    Vargs popUnselected;
    popUnselected.push("one");
    popUnselected.push("two");
    popUnselected.push("three");
    popUnselected.smov(1);

    EXPECT_OK(
      {
      string popped = popUnselected.pop();

      printResult(TU_CYN("pop()") + " removes and returns last argument",
                 popped == "three" &&
                 popUnselected.size() == 2 &&
                 popUnselected.arg(2) == "two");

      printResult(TU_CYN("pop()") + " preserves selector if selected element remains",
                 popUnselected.sget() == 1 &&
                 popUnselected.getsel() == "one");
      },
      TU_CYN("pop()") + " completes without exception"
    );

    Vargs popSelected;
    popSelected.push("one");
    popSelected.push("two");
    popSelected.push("three");
    popSelected.smov(3);

    EXPECT_OK(
      {
      string popped = popSelected.pop();

      printResult(TU_CYN("pop()") + " of selected last element decrements selector",
                 popped == "three" &&
                 popSelected.size() == 2 &&
                 popSelected.sget() == 2 &&
                 popSelected.getsel() == "two");
      },
      TU_CYN("pop()") + " completes without exception"
    );

    Vargs popNull;
    popNull.push("one");
    popNull.push("two");

    EXPECT_OK(
      {
      popNull.pop();

      printResult(TU_CYN("pop()") + " preserves null selector",
                 popNull.size() == 1 &&
                 popNull.sget() == 0);
      },
      TU_CYN("pop()") + " completes without exception"
    );

    Vargs popSingle;
    popSingle.push("only");
    popSingle.smov(1);

    EXPECT_OK(
      {
      string popped = popSingle.pop();

      printResult(TU_CYN("pop()") + " selected sole element => selector zero",
                 popped == "only" &&
                 popSingle.size() == 0 &&
                 popSingle.sget() == 0);
      },
      TU_CYN("pop()") + " completes without exception"
    );

    Vargs emptyBack;
    EXPECT_NG(emptyBack.pop(), TU_CYN("pop()") + " on empty Vargs", std::out_of_range);

// ----------- FPUSH --------------------------------------------- //
    printHeading("STACK-STYLE MODIFICATION: fpush()");

    Vargs fpushVargs;

    fpushVargs.fpush("one");

    printResult(TU_CYN("fpush()") + " into empty Vargs",
               fpushVargs.size() == 1 &&
               fpushVargs.arg(1) == "one" &&
               fpushVargs.sget() == 0);

    fpushVargs.fpush("zero");

    printResult(TU_CYN("fpush()") + " inserts at front with null selector",
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

    printVariable("fpushSelected.size()", fpushSelected.size());
    printVariable("fpushSelected.sget()", fpushSelected.sget());
    printArray("fpushSelected.getall()", fpushSelected.getall());

    printResult(TU_CYN("fpush()") + " increments nonzero selector",
               fpushSelected.size() == 4 &&
               fpushSelected.sget() == 3 &&
               fpushSelected.getsel() == "two" &&
               fpushSelected.arg(1) == "zero");

    Vargs fpushFirst;
    fpushFirst.push("one");
    fpushFirst.push("two");
    fpushFirst.smov(1);
    fpushFirst.fpush("zero");

    printResult(TU_CYN("fpush()") + " preserves selected element at first position",
               fpushFirst.sget() == 2 &&
               fpushFirst.getsel() == "one");

// ----------- FPOP ---------------------------------------------- //
    printHeading("STACK-STYLE MODIFICATION: fpop()");

    Vargs fpopNull;
    fpopNull.push("zero");
    fpopNull.push("one");
    fpopNull.push("two");

    EXPECT_OK(
      {
      string popped = fpopNull.fpop();

      printResult(TU_CYN("fpop()") + " removes and returns first argument",
                 popped == "zero" &&
                 fpopNull.size() == 2 &&
                 fpopNull.arg(1) == "one");

      printResult(TU_CYN("fpop()") + " preserves null selector",
                 fpopNull.sget() == 0);
      },
      TU_CYN("fpop()") + " completes without exception"
    );

    Vargs fpopMiddle;
    fpopMiddle.push("zero");
    fpopMiddle.push("one");
    fpopMiddle.push("two");
    fpopMiddle.smov(2);

    EXPECT_OK(
      {
      string popped = fpopMiddle.fpop();

      printResult(TU_CYN("fpop()") + " decrements selector to preserve element",
                 popped == "zero" &&
                 fpopMiddle.size() == 2 &&
                 fpopMiddle.sget() == 1 &&
                 fpopMiddle.getsel() == "one");
      },
      TU_CYN("fpop()") + " completes without exception"
    );

    Vargs fpopSelectedFirst;
    fpopSelectedFirst.push("zero");
    fpopSelectedFirst.push("one");
    fpopSelectedFirst.smov(1);

    EXPECT_OK(
      {
      string popped = fpopSelectedFirst.fpop();

      printResult(TU_CYN("fpop()") + " selected first element => selector zero",
                 popped == "zero" &&
                 fpopSelectedFirst.size() == 1 &&
                 fpopSelectedFirst.sget() == 0 &&
                 fpopSelectedFirst.arg(1) == "one");
      },
      TU_CYN("fpop()") + " completes without exception"
    );

    Vargs fpopSingle;
    fpopSingle.push("only");
    fpopSingle.smov(1);

    EXPECT_OK(
      {
      string popped = fpopSingle.fpop();

      printResult(TU_CYN("fpop()") + " selected sole element => selector zero",
                 popped == "only" &&
                 fpopSingle.size() == 0 &&
                 fpopSingle.sget() == 0);
      },
      TU_CYN("fpop()") + " completes without exception"
    );

    Vargs emptyFront;
    EXPECT_NG(emptyFront.fpop(), TU_CYN("fpop()") + " on empty Vargs", std::out_of_range);

    return 0;
  }

//+++++++++++ EOF ++++++++++++++++++++++++++++++++++++++++++++++++ //
