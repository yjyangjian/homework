/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool testOrder(Queue<int>cohort){
    if(cohort.isEmpty()){
        return true;
    }
    int item = cohort.dequeue();
    while(!cohort.isEmpty()){
        if(item > cohort.peek()){
            return false;
            break;
        }
        item = cohort.dequeue();

    }
    return true;
}
STUDENT_TEST("测试验证队列顺序是否正确的辅助函数"){
    EXPECT(!testOrder({1, 3, 2}));
    EXPECT(!testOrder({1, 1, 4, 2}));
    EXPECT(testOrder({1, 2, 3}));
    EXPECT(testOrder({}));

}

Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    /* TODO: Implement this function. */
    if(!testOrder(a) || !testOrder(b)){
        error("顺序错误");
    }
    int aTopNumber;
    int bTopNumber;
    while(!a.isEmpty()&&!b.isEmpty()){
        aTopNumber = a.peek();
        bTopNumber = b.peek();
        if(aTopNumber < bTopNumber){
            result.enqueue(a.dequeue());
        }else if(aTopNumber > bTopNumber){
            result.enqueue(b.dequeue());
        }else{
            result.enqueue(a.dequeue());
            result.enqueue(b.dequeue());
        }

    }
    while(!a.isEmpty()){
        aTopNumber = a.dequeue();
        result.enqueue(aTopNumber);
    }
    while(!b.isEmpty()){
        bTopNumber = b.dequeue();
        result.enqueue(bTopNumber);
    }

    return result;
}

STUDENT_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5, 10};
    Queue<int> b = {1, 3, 7};
    Queue<int> expected = {1, 2, 3, 4, 5, 7, 10};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
STUDENT_TEST("binaryMerge one queue is zero "){
    Queue<int> a = {};
    Queue<int> b = {1, 2, 3};
    Queue<int> expected = {1, 2, 3};
    EXPECT_EQUAL(binaryMerge(a,b), expected);
    EXPECT_EQUAL(binaryMerge(b,a), expected);
}
STUDENT_TEST("binaryMerge two queue is zero "){
    Queue<int> a = {};
    Queue<int> b = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(binaryMerge(a,b), expected);
    EXPECT_EQUAL(binaryMerge(b,a), expected);
}
STUDENT_TEST("binaryMerge two queue is zero "){
    Queue<int> a = {};
    Queue<int> b = {1, 3, 2};
    EXPECT_ERROR(binaryMerge(a,b));
    EXPECT_ERROR(binaryMerge(b,a));
}
Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);
STUDENT_TEST("Time binaryMerge operation") {
    int n = 100000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
    int m = 1000000;
    Queue<int> c = createSequence(m);
    Queue<int> d = createSequence(m);
    TIME_OPERATION(c.size() + d.size(), binaryMerge(c, d));
    int l = 10000000;
    Queue<int> e = createSequence(l);
    Queue<int> f = createSequence(l);
    TIME_OPERATION(e.size() + f.size(), binaryMerge(e, f));
}


/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}
STUDENT_TEST("空队列") {
    Vector<Queue<int>> all = {{},
                             {},
                             {},
                             {},
                             {},
                             {}
                            };
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}
STUDENT_TEST("顺序错误") {
    Vector<Queue<int>> all = {{1, 2, 3},
                             {1, 4, 3},
                            };
    EXPECT_ERROR(naiveMultiMerge(all));
}
STUDENT_TEST("Time naiveMultiMerge operation") {
    for(int k = 1; k < 5; k++){
       for(int n = 100000; n < 10000000; n += 100000){
           Queue<int> input = createSequence(n);
            Vector<Queue<int>> all(k);
            distribute(input, all);
            TIME_OPERATION(input.size(), naiveMultiMerge(all));
        }
    }
}
/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMergeRef(Vector<Queue<int>>& all, int start, int end){
    Queue<int> result = {};
    if(end - start == 1){
        return binaryMerge(all[start],result);
    }
    int mid = start + (end - start) / 2;
    Queue<int> recMultiMergeLeft =recMultiMergeRef(all, start, mid);

    Queue<int> recMultiMergeRight = recMultiMergeRef(all, mid, end);

    return binaryMerge(recMultiMergeLeft,recMultiMergeRight);
}

Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    int start = 0;
    int end = all.size();
    result = recMultiMergeRef(all, start, end);
    /* TODO: Implement this function. */
    return result;
}

STUDENT_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 21;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}
STUDENT_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 200;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}
STUDENT_TEST("recMultiMerge, compare to naiveMultiMerge") {
    Vector<Queue<int>> all = {{},
                              {},
                              {}
                             };
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation") {
    int k = 10;
    for(int n = 10000; n < 110000; n += 10000){
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }

}

STUDENT_TEST("Time recMultiMerge operation") {
    int n = 10000;
    for(int k = 10; k < 10000; k *= 10){
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }

}
/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
