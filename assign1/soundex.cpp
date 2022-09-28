/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 */
//1.去除所有非字母字符：横线、空格、标点等
string removeNonLetters(string s) {
    string result;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}
STUDENT_TEST("测试空字符串、无字母字符串"){
    EXPECT_EQUAL(removeNonLetters(" "), "");
    EXPECT_EQUAL(removeNonLetters("12345"), "");
    EXPECT_EQUAL(removeNonLetters("_remake"), "remake");
    EXPECT_EQUAL(removeNonLetters(",,"), "");
}

//2.根据下表编码每个字母
string encodeString(string s){
    string result;
    string zero = "AEIOUHWY";
    string one = "BFPV";
    string two = "CGJKQSXZ";
    string three = "DT";
    string four = "L";
    string five = "MN";
    string six = "R";
    for (int i = 0; i < s.length(); i++){
        char ch = toUpperCase(s[i]);
        if (zero.find(ch) != string::npos)
            result += '0';
        if (one.find(ch) != string::npos)
            result += '1';
        if (two.find(ch) != string::npos)
            result += '2';
        if (three.find(ch) != string::npos)
            result += '3';
        if (four.find(ch) != string::npos)
            result += '4';
        if (five.find(ch) != string::npos)
            result += '5';
        if (six.find(ch) != string::npos)
            result += '6';
    }
    return result;
}
STUDENT_TEST("测试编码函数"){
    EXPECT_EQUAL(encodeString("ABCD"), "0123");
    EXPECT_EQUAL(encodeString(""), "");
    EXPECT_EQUAL(encodeString("1234"), "");
}
//3.从得到的编码中，合并相邻的重复数字。比如，222025会变成2025
string coalesceString(string s){
   string result;
   for (int i = 0; i < s.length(); i++){
       if (result.back() == s[i])
           continue;
       result += s[i];
   }
   return result;
}
STUDENT_TEST ("测试合并函数"){
    EXPECT_EQUAL(coalesceString("222025"), "2025");
    EXPECT_EQUAL(coalesceString(""), "");
    EXPECT_EQUAL(coalesceString("aannggeellaa"), "angela");
    EXPECT_EQUAL(coalesceString("aa__b_''c"), "a_b_'c");
}

//4.使用姓氏名的第一个字母替换第一个数字，并转换为大写形式
string replaceString(string s, string name){
    if (s.length() && name.length())
        s[0] = toUpperCase(name[0]);
    return s;
}
STUDENT_TEST("测试替换函数"){
    EXPECT_EQUAL(replaceString("12", "jack"), "J2");
    EXPECT_EQUAL(replaceString("", ""), "");
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
//5.删除字符串中的数字0
string removeZeroLetter(string s){
    string result;
    for(int i = 0; i < s.length(); i++){
        if(s[i] == '0')
            continue;
        result += s[i];
    }
    return result;
}
STUDENT_TEST("测试去零函数"){
    EXPECT_EQUAL(removeZeroLetter("asd0sa"), "asdsa");
    EXPECT_EQUAL(removeZeroLetter(""), "");
    EXPECT_EQUAL(removeZeroLetter(" _")," _");
    EXPECT_EQUAL(removeZeroLetter("0121asd"), "121asd");
}

//6.通过填充0或者截掉多余的部分，使得编码长度恰好为4
string adjustStrlen(string s){
    if(s.size() > 4)
        return s.substr(0,4);
    //i--的效率更高一些
    for(int i = 4 - s.size(); i > 0; i--){
        s += '0';
    }
    return s;
}
STUDENT_TEST("测试调整函数"){
    EXPECT_EQUAL(adjustStrlen("123"), "1230");
    EXPECT_EQUAL(adjustStrlen("123455"), "1234");
    EXPECT_EQUAL(adjustStrlen(""), "0000");
}

//7.姓氏编码程序
string soundex(string s) {
    /* TODO: Fill in this function. */
    string result;
    result = removeNonLetters(s);
    result = encodeString(result);
    result = coalesceString(result);
    result = replaceString(result, s);
    result = removeZeroLetter(result);
    result = adjustStrlen(result);
    return result;
}

STUDENT_TEST("测试顶层函数"){
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
    EXPECT_EQUAL(soundex(""), "0000");
    EXPECT_EQUAL(soundex(" ___"), "0000");
    EXPECT_EQUAL(soundex("北京"), "0000");
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
//8.姓氏编码检索交互部分
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    //1.从指定文件中读取姓氏的数据库
    ifstream in;
    Vector<string> databaseNames;
    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;
    while (true){
         //2.提示用户输入姓氏
        string name = getLine("Enter a surname (Return to quiet): ");
        if (name.empty()){
            cout << "All done!" << endl;
            break;
        }
        //3.计算该姓氏的soundex编码
        string code = soundex(name);
        cout << "Soundes code is " << code << endl;
        //4.遍历数据库，计算每个名字的 Soundex 编码，如果编码与用户输入的姓氏编码一致，则保存到一个Vector中
        Vector<string> matchNames;
        for (const string & name : databaseNames){
            if(soundex(name) == code)
                matchNames.add(name);
        }
        //5.按顺序输出匹配项
        matchNames.sort();
        cout << "Matches from database: " << matchNames << endl;

    }
    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
    s = " dog";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "dog");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here


