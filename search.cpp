// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "simpio.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    //考虑空字符串
       if(!s.size())
           return "";
       int start = 0;
       int newString = 0;
       //只去除字符串首尾的标点符号，但不管字符串内部的标点
       while(start < s.size() && ispunct(s.at(start)))
           start++;
       if(start > 0)
           s.erase(0,start);

       int end = s.length() - 1;
       while(end > 0 && ispunct(s.at(end)))
           end--;
       if(end < s.size()-1)
           s.erase(end+1, s.size());
       // 确认字符串至少包含一个字母

       for(int i = 0; i < s.size(); i++){
           if(isalpha(s[i]))
               newString++;
       }
       if(newString > 0)
               //将字符串转换为小写
          return toLowerCase(s);
       else
           return "";


       return 0;
}

STUDENT_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("5lb_m&ms"), "5lb_m&ms");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
    EXPECT_EQUAL(cleanToken("<<BLUE>>"), "blue");
    EXPECT_EQUAL(cleanToken("<106^B>"), "106^b");
    EXPECT_EQUAL(cleanToken(""),"");

}
// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    //以空格为分隔符，把正文文本拆分为一个个独立的子字符串
        Vector<string> independentString = stringSplit(text, " ");
        //调用辅助函数处理每个子字符串，并将结果存放到集合中
        for(const string&item : independentString)
            tokens.add(cleanToken(item));
        tokens.remove("");
    return tokens;
}

STUDENT_TEST("gatherTokens from seuss, 4 unique words") {
    Set<string> tokens = gatherTokens("milk, fish, bread, 5lb_m&ms");
    EXPECT_EQUAL(tokens.size(), 4);
    EXPECT(tokens.contains("fish"));
}

STUDENT_TEST("gatherTokens from seuss, 3 unique words, mixed case") {
    Set<string> tokens = gatherTokens("red ~green <<blue>>");
    EXPECT_EQUAL(tokens.size(), 3);
    EXPECT(tokens.contains("red"));
    EXPECT(!tokens.contains("<<blue>>"));
}
// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    //将数据从文件读入Vector的字符串表示文件的行
            ifstream fin;
            if (!openFile(fin, dbfile))
                    error("Cannot open file named " + dbfile);
            Vector<string> lines;
            readEntireFile(fin, lines);

            int page = 0;
            for(int i = 1; i < lines.size(); i++)
                if(i % 2 == 1){
                    //调用gathertokens提取网页文本中独一无二的字符串集合
                    Set<string> text = gatherTokens(lines[i]);
                    for(const string&item : text)
                      //每个单独的字符串对应相应的URL
                      index[item].add(lines[i - 1]);
                    page++;
                }
        return page;
}

STUDENT_TEST("buildIndex from website.txt") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 32);
    EXPECT(index.containsKey("week"));
    TIME_OPERATION(50000, buildIndex("res/website.txt", index));
}
// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    Vector<string> independentString = stringSplit(query, " ");
    if(independentString.size() == 1)
       result = index[cleanToken(query)];
    if(independentString.size() > 1){
        for(const string&item : independentString){
            if(startsWith(item, "+")){
                result.intersect(index[cleanToken(item)]);
            }
            else if(startsWith(item, "-")){
                result.difference(index[cleanToken(item)]);
            }else{
                result.unionWith(index[cleanToken(item)]);
            }
        }
    }
    return result;
}

STUDENT_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesFish = findQueryMatches(index, "fish");
    EXPECT_EQUAL(matchesFish.size(), 3);
    EXPECT(matchesFish.contains("www.shoppinglist.com"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesBlueOrFish = findQueryMatches(index, "blue fish");
    EXPECT_EQUAL(matchesBlueOrFish.size(), 4);
    Set<string> matchesRedAndFishOrBreadOrEat = findQueryMatches(index, "blue +fish bread eat");
    EXPECT_EQUAL(matchesRedAndFishOrBreadOrEat.size(), 3);
    Set<string> matchesRedWithoutFishAndMilk = findQueryMatches(index, "blue -fish +milk");
    EXPECT_EQUAL(matchesRedWithoutFishAndMilk.size(), 0);
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    //首先根据数据库文件内容，构建一个反向索引
    Map<string, Set<string>> index;
    int page = buildIndex(dbfile,index);
    //打印出构建反向索引用到的网页数量以及所有页面中包含了多少独一无二的单词
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << page << " pages containing " << index.size() << " unique terms" << endl;
    //进入一个循环，提示用户输入查询关键词
    while(true){
        string keyWord = getLine("Enter query sentence(RETURN/ENTER to quit): ");
        //当用户输入空字符串时，提示查询完成，程序结束。
        if(keyWord.empty()){
            break;
        }

        //对于每一个查询关键词，找出所有匹配页面并打印出 URLs
        Set<string> urls = findQueryMatches(index, keyWord);
        cout << "Found " << urls.size() << " matching pages " << endl;
        cout << urls << endl;
        cout << endl;
    }


}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
