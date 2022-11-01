/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
 //忽略了对其他符号运算符的处理例如+ =这些重点在递归的处理
string operatorsFrom(string str) {
    if(str.empty()){
        return "";
    }
    else if(ispunct(str[0])){
        return  str.substr(0, 1) + operatorsFrom(str.substr(1));
    }else{
        return operatorsFrom(str.substr(1));
        }
    }

STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("[ a ]"), "[]");
    EXPECT_EQUAL(operatorsFrom("asd"), "");
    EXPECT_EQUAL(operatorsFrom("[({"), "[({");
    EXPECT_EQUAL(operatorsFrom("北京cup"), "");
    EXPECT_EQUAL(operatorsFrom("int main() { int x  2  (vec[2]  3) x  (1  random()) }"), "(){([])(())}");
}


/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool operatorsAreMatched(string ops) {
    /* TODO: Implement this function. */
    if(ops.size() == 0){
        return true;
    }else if(stringContains(ops,"()")){
        ops = ops.replace(ops.find("()"), 2, "" );
        return  operatorsAreMatched(ops);
    }else if(stringContains(ops,"[]")){
        ops = ops.replace(ops.find("[]"), 2, "" );
        return  operatorsAreMatched(ops);
    }else if(stringContains(ops,"{}")){
        ops = ops.replace(ops.find("{}"), 2, "" );
        return  operatorsAreMatched(ops);
    }else{
        return false;
    }

    return false;
}


STUDENT_TEST("operatorsAreMatched on simple example") {
    EXPECT(!operatorsAreMatched("}{"));
    EXPECT(!operatorsAreMatched("{"));
    EXPECT(!operatorsAreMatched("}"));
    EXPECT(!operatorsAreMatched("{}}"));
    EXPECT(operatorsAreMatched(""));
    EXPECT(!operatorsAreMatched("!"));
    EXPECT(operatorsAreMatched("(())"));
    EXPECT(operatorsAreMatched("(){([])(())}"));
}
/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x  2  (vec[2]  3) x  (1  random()) }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
