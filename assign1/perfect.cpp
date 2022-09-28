/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
#include <cmath>
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}


/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}


/* .以N的平方根为分界线，直接计算该除数对应的成对因子
 */
long smarterSum(long n) {
    //edge case 边界测试放在最前边
    if (n == 1)
      return 0;

    long total = 1;
    //避免浮点型表示的整数不精确的情况,加一个0.5
    for (long divisor = 2; divisor <=(sqrt(n) + 0.5); divisor++) {
        if (n % divisor == 0){
            total += divisor;
            // divisor != sqrt(n)这种写法使得整形跟浮点型进行了比较 不要使用
            if (divisor != n / divisor)
                total += n / divisor;
        }
    }
        return total;
}

/* 判断完全数
 */
bool isPerfectSmarter(long n) {
    /* TODO: Fill in this function. */
    return (n != 0) && (n == smarterSum(n));
}

/* 找到完全数.
 */

void findPerfectsSmarter (long stop) {
    /* TODO: Fill in this function. */
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

bool isPrime(long n){
    return smarterSum(n) == 1;
}

/* 欧几里得优化找完全数
 */

long findNthPerfectEuclid(long n) {
    // 先初始化计数器
    for (long k = 1; k < LONG_MAX; k++){
        //计算梅森数
       long m = pow(2,k) - 1;
       //确定是否为素数
        if (isPrime(m)){
            if (--n == 0)
            return pow(2,k -1 ) * m;
        }

    }
    return 0;
}



/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here
STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(40000, findPerfects(40000));
    TIME_OPERATION(80000, findPerfects(80000));
    TIME_OPERATION(160000, findPerfects(160000));
    TIME_OPERATION(320000, findPerfects(320000));
}

STUDENT_TEST("Confirm -6  is perfect") {
    EXPECT(!isPerfect(-6));
}

STUDENT_TEST("Confirm divisorSum of small inputs") {
    for(long i = 1; i < 500; i++){
        EXPECT_EQUAL(divisorSum(i), smarterSum(i));
    }

}


STUDENT_TEST("测试欧几里得算法") {
    EXPECT_EQUAL(findNthPerfectEuclid(1),6);
    EXPECT_EQUAL(findNthPerfectEuclid(2),28);
    EXPECT_EQUAL(findNthPerfectEuclid(3),496);
    EXPECT_EQUAL(findNthPerfectEuclid(4),8128);
    EXPECT_EQUAL(findNthPerfectEuclid(5),33550336);
}

STUDENT_TEST("Time trials of findPerfectsSmarter on doubling input sizes") {
    TIME_OPERATION(400000, findPerfectsSmarter(400000));
    TIME_OPERATION(800000, findPerfectsSmarter(800000));
    TIME_OPERATION(1600000, findPerfectsSmarter(1600000));
    TIME_OPERATION(3200000, findPerfectsSmarter(3200000));
}


