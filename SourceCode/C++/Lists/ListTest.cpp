// #include "LList.h"
#include "AList.h"
#include "ErrorRec.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>

using namespace std;

/* 
 * Note that typeid(l).name() is implementation specific. As a result, the code
 * may not return the true class name in error messages. It usually is close but
 * with a number in front that seems to be the number of characters in the class
 * name. However, this may not always be the case.
 */


// For reasons I cannot figure out, there are linking error if these static
// variables are put in the class. Prefer if not global.

// True if you want to create a text file to record errors
static const bool useFile = true;
// Instance of ErrorRec class which holds the number of errors and prints
// out error messages
static ErrorRec *record;
// Allows doSomething to produce result without arguments so it can be tested.
static string doSomethingResult;

/*
 * This program checks if all the methods in AList, LList and DList classes work
 * properly. All tests are done on ListItemType from the List implementation.
 */
class ListTest {
private:
  // The number of items stored in list during the test
  static const int TEST_SIZE = 9;

public:
  static void listIter(List &L) {
    ListItemType it;
/* *** ODSATag: listiter *** */
for (L.moveToStart(); !L.isAtEnd(); L.next()) {
  it = L.getValue();
  doSomething(it);
}
/* *** ODSAendTag: listiter *** */
  }

  static void doSomething(ListItemType it) {
    doSomethingResult.append(to_string(it));
  }

/* *** ODSATag: listfind *** */
// Return true if k is in list L, false otherwise
static bool find(List &L, ListItemType k) {
  for (L.moveToStart(); !L.isAtEnd(); L.next())
    if (k == L.getValue()) return true; // Found k
  return false;                       // k not found
}
/* *** ODSAendTag: listfind *** */

 /**
  * Test the find & iterator functions
  * 
  * l List to test.
  */
  static void testOther(List &l) {
    // Create non-empty list of items.
    ListItemType item = 10;
    int loc, locExpect;
    const int NUMBER = 9;
    for (int i = 0; i <= NUMBER; i++) {
      l.append(item);
      item += 10;
    }

    // The three items to test
    const int NUM_ITEMS = 3;
    const int VALUES[] = {NUMBER / 2  * 10, NUMBER * 10, 1 * 10};
    // find these three items.
    for (int i = 0; i < NUM_ITEMS; i++) {
      item = (ListItemType)VALUES[i];
      if (!find(l, item)) {
        record->printError("item " + to_string(item) + " not found on list");
      }
      loc = l.currPos();
      locExpect = item / 10 - 1;
      if (loc != locExpect) {
        record->printError("After find, current is " + to_string(loc) + " and not " + to_string(locExpect));
    }
  }

    // Test iterator

    // Expected result
    string expect;
    item = 10;
    for (int i = 0; i <= NUMBER; i++) {
      expect.append(to_string(item));
      item += 10;
    }
    // Call iterator
    listIter(l);
    // Check result
    if (doSomethingResult.compare(expect) != 0) {
      record->printError("After iterator got " + doSomethingResult + " but expected " + expect);
    }
  }

  /**
  * Test a list holding ListIntType to see if it works correctly.
  * 
  * l List to test.
  */
  static void testList(List &l) {
    // Check empty list
    checkEmp(l);

    // Test moveToStart, moveToEnd, prev, and next
    doSomethingOnEmpList(l);

    // Compare list with vector to test length, getValue,
    // currPos, and remove. Add items by inserting
    vector<ListItemType> tester;
    for (int i = 0; i < TEST_SIZE; i++) {
      // This assumes int and ListItemType are compatible.
      checkIns(l, tester, (ListItemType)(100 + i));
    }

    // Clear both lists
    reset(l, tester);

    // Compare list with C++ vector to test length, getValue,
    // currPos, and remove. Add items by appending
    for (int i = 0; i < TEST_SIZE; i++) {
      // No cast on last argument since assume ListItemType and int are compatible.
      checkApp(l, tester, 100 + i);
    }

    doSomethingOnNonEmpList(l, tester);
  }

  /* Set both lists provided to empty/new state.
  * The first list is OpenDSA.
  * The second list is a standard C++ one.
  */
  static void reset(List &l, vector<int> &tester) {
    l.clear();
    tester.clear();
  }

  /* Take a list that should be empty and perform operations
  * to see if it acts as expected.
  */
  static void doSomethingOnEmpList(List &l) {
    // Nothing changes
    l.moveToStart();
    l.moveToEnd();
    l.prev();
    l.next();
    checkEmp(l);
  }

  /* Takes a list that should be empty and makes sure it acts as expected */
  static void checkEmp(List &l) {
    // Test length with empty list
    if (l.length() != 0) {
      record->printError("On empty list an unexpected length from " + string(typeid(l).name()) + ". \nLength of list: " + to_string(l.length()) + "\nLength expected: 0");
    }

    // isEmpty should return true
    if (!l.isEmpty()) {
      record->printError(
          "The isEmpty method in " + string(typeid(l).name()) + " does not return true when the list is empty.");
    }

    // Test currPos with empty list
    if (l.currPos() != 0) {
      record->printError("An unexpected current in empty " + string(typeid(l).name()) + ". \nCurrent in list: " + to_string(l.currPos()) + "\nValue expected: 0");
    }

    ListItemType item;
    // Test remove with empty list
    try {
      item = l.remove();
      record->printError("An unexpected result in empty " + string(typeid(l).name()) + ". \nremove did not throw expected exception but returned " + to_string(item));
    } catch (std::out_of_range &ex) {
      // Do nothing since expect this exception
    }

    // check getting value on empty list
    try {
      item = l.getValue();
      record->printError("An unexpected result in empty " + string(typeid(l).name()) + ". \ngetValue did not throw expected exception but returned " + to_string(item));
    } catch (std::out_of_range &ex) {
      // Do nothing since expect this exception
    }

    // Test move to bad positions
    if (l.moveToPos(-1)) {
      record->printError("An empty " + string(typeid(l).name()) + " returned true for moveToPos(-1)");
    }

    // Test clear
    l.clear();
     // Test length with empty list
    if (l.length() != 0) {
      record->printError("On empty list an unexpected length from " + string(typeid(l).name()) + ". \nLength of list: " + to_string(l.length()) + "\nLength expected: 0");
    }

    // isEmpty should return true
    if (!l.isEmpty()) {
      record->printError(
          "The isEmpty method in " + string(typeid(l).name()) + " does not return true when the list is empty.");
    }

    // Test currPos with empty list
    if (l.currPos() != 0) {
      record->printError("An unexpected current in empty " + string(typeid(l).name()) + ". \nCurrent in list: " + to_string(l.currPos()) + "\nValue expected: 0");
    }
  }

  /**
   * Takes a list that should be nonempty and makes sure it acts as expected
   *
   * l List that should be nonempty for testing.
   * tester Same logical list as l but standard Java one.
   */
  static void doSomethingOnNonEmpList(List &l, vector<ListItemType> &tester) {
    // iterator to move around this vector.
    vector<int>::iterator it;
   // iterator to move around temp vector.
    vector<int>::iterator itTemp;

    // Test moveToStart and remove
    l.moveToStart();
    check(l, tester, 0);
    ListItemType removed = l.remove();
    it = tester.begin();
    ListItemType expected = *it;
    tester.erase(it);
    if (removed != expected) {
      record->printError("Unexpected removed value at the beginning of " + string(typeid(l).name()) + ".\nRemoved value: "
          + to_string(removed) + "\nExpected value: " + to_string(expected));
    }
    check(l, tester, 0);
    // Restore values
    l.insert(expected);
    it = tester.begin();
    tester.insert(it, expected);

    // Test prev
    l.prev();
    check(l, tester, 0);

    // Test next
    l.next();
    check(l, tester, 1);

    // Test moveToEnd and remove
    l.moveToEnd();

    ListItemType item;
    // Test remove at end of list
    try {
      item = l.remove();
      record->printError("An unexpected result at end of " + string(typeid(l).name()) + ". \nremove did not throw expected exception");
    } catch (std::out_of_range &ex) {
      // Do nothing since expect this exception
    }
 
    // Test getValue at end of list
    try {
      item = l.getValue();
      record->printError("An unexpected result result at end of " + string(typeid(l).name()) + ". \ngetValue did not throw expected exception");
    } catch (std::out_of_range &ex) {
      // Do nothing since expect this exception
    }

    // Curr is out of bound
    l.prev();
    check(l, tester, tester.size() - 1);
    removed = l.remove();
    it = tester.end();
    it--;
    expected = *it;
    tester.erase(it);
    if (removed != expected) {
      record->printError("Unexpected removed value at the end of " + string(typeid(l).name()) + ".\nRemoved value: " + to_string(removed)
          + "\nExpected value: " + to_string(expected));
    }
    // Curr is out of bound
    l.prev();
    check(l, tester, tester.size() - 1);
    // Restore values
    l.append(expected);
    tester.push_back(expected);

    // Keep removing items from the middle of the list
    vector<ListItemType> temp;
    int size = tester.size();
    int curr = size / 2;
    l.moveToPos(curr);
    it = tester.begin() + curr;
    for (int i = 0; i < size; i++) {
      removed = l.remove();
      expected = *it;
      it = tester.erase(it);
      if (removed != expected) {
        record->printError("Unexpected removed value at the index of " + to_string(curr) + " in " + string(typeid(l).name())
           + ".\nRemoved value: " + to_string(removed) + "\nExpected value: " + to_string(expected));
      }
      // If the lists are empty, call checkEmp. If curr is at tail, it is out of bound
      if (tester.empty()) {
        checkEmp(l);
      } else if (l.isAtEnd()) {
        l.prev();
        check(l, tester, curr - 1);
        l.next();
      } else {
        check(l, tester, curr);
      }
      // If this is the even-number-th removal, decrease current position by one
      if (i % 2 == 0) {
        temp.push_back(expected);
        l.prev();
        curr--;
        it = tester.begin() + curr;
      } else {
        itTemp = temp.begin();
        temp.insert(itTemp, expected);
      }
    }
    // Restore values
    itTemp = temp.begin();
    for (int i = 0; i < size; i++) {
      ListItemType tempRem = *itTemp;
      itTemp = temp.erase(itTemp);
      l.append(tempRem);
      tester.push_back(tempRem);
    }
  }

  /**
   * Check that inserting on both the OpenDSA (first list) and standard
   * Java list (second list) works and does the same thing. item is what
   * is to be inserted.
   */
  static void checkIns(List &l, vector<ListItemType> &tester, ListItemType item) {
    // Insert the item to both lists
    vector<int>::iterator it = tester.begin() + l.currPos();
    tester.insert(it, item);
    if (!l.insert(item)) {
      record->printError("The insert method in " + string(typeid(l).name()) + " returned false.");
    }
    // Verify lists are the same.
    check(l, tester, l.currPos());
  }

  /*
   * Check that appending on both the OpenDSA (first list) and standard
   * C++ list (second list) works and does the same thing. item is what
   * to append to lists.
   */
  static void checkApp(List &l, vector<ListItemType> &tester, ListItemType item) {
    // Append the item to both lists
    tester.push_back(item);
    if (!l.append(item)) {
      record->printError("The append method in " + string(typeid(l).name()) + " returned false.");
    }
    // Verify lists are the same.
    check(l, tester, l.currPos());
  }

  /* Check that two list: OpenDSA (first list) and standard
   * C++ list (second list) are the same thing. curr is 
   * the index of the current item on the list.
   */
  static void check(List &l, vector<ListItemType> &tester, int curr)
  {
    // Check the length of list
    if (l.length() != tester.size()) {
      record->printError("An unexpected length of " + string(typeid(l).name()) + ". \nLength of list: " + to_string(l.length()) + "\nLength expected: " + to_string(tester.size()));
    }

    // isEmpty should return false
    if (l.isEmpty()) {
      record->printError(
          "The isEmpty method in " + string(typeid(l).name()) + " does not return false when the list is not empty.");
    }

    // Check the current position
    if (l.currPos() != curr) {
      record->printError("An unexpected current position of " + string(typeid(l).name()) + ". \nCurrent position of list: " + to_string(l.currPos()) + "\nPosition expected: " + to_string(curr));
    }

    // Check the value stored in the current position
   // Note vector does not work if empty list.
    if (l.getValue() != tester.at(curr)) {
      record->printError("An unexpected list item in " + string(typeid(l).name()) + ". \nItem in list: " + to_string(l.getValue()) + "\nValue expected: " + to_string(tester.at(curr)));
    }

    // Check values in list
    l.moveToStart();
    for (int i = 0; i < tester.size(); i++) {
      if (l.getValue() != tester.at(i)) {
        record->printError("An unexpected value at the index of " + to_string(i) + " in " + string(typeid(l).name()) + ". \nValue in list: " + to_string(l.getValue()) + "\nValue expected: " + to_string(tester.at(i)));
      }
      l.next();
    }
    l.moveToPos(curr);
  }
};

/* Runs tests on generic AList and LList Class. */
int main(void) {
  // Create a file to record errors if necessary
  record = new ErrorRec("ListTest", useFile);

  // Run tests of AList on a new list.
  AList *al = new AList();
  ListTest::testList(*al);
  // Test other functions that not in interface
  al->clear();
  doSomethingResult = "";
  ListTest::testOther(*al);
  // Remove list.
  delete al;

/* TODO - note interleaved with AList in Java version
  // Run tests of LList on a new list.
  AList *ll = new LList();
  ListTest::testList(*ll);
  // Test other functions that not in interface
  al->clear();
  ListTest::testOther(*ll);
 // Remove list.
  delete ll;
*/

  record->feedback();

  return 0;
}

// Decision made not to do timing test at this time that are in the Java versions.
