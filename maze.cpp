// 使用ADT表示迷宫并找到入口到出口的路径
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* 编写generateValidMoves（）函数
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    GridLocation next;

    for(int i=0; i < 4; i++){
       int dx[4] = {1, -1, 0, 0};//东西南北
       int dy[4] = {0, 0, -1, 1};
       int neighborsx = cur.row + dx[i];
       int neighborsy = cur.col + dy[i];
       next = GridLocation(neighborsx, neighborsy);
       //使用ADT接口inBounds
       if(maze.inBounds(next) && maze[next])
           neighbors.add(next);
       }
    return neighbors;
}

STUDENT_TEST("generateValidMoves on location in the center of 3x3 grid with  wall") {
    Grid<bool> maze = {{false, false, false},
                       {false, true, false},
                       {false, false, false}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves on location on the corner of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 0};
    Set<GridLocation> expected = {{0,1}, {1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {false, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

/* TODO: 编写validatePath()函数；
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    //判断path顶端是否等于exit
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    /* TODO: 验证所给路线是否可以顺利走出迷宫. */

    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
    //定义一个ADT存放visited
    Set<GridLocation> visited;
    Set<GridLocation> neighbors;
    GridLocation next;
    GridLocation cur = path.pop();
    //循环操作
    while(!path.isEmpty()){

       //取出栈顶
       next = path.pop();
       //计算generateValidMoves
       neighbors = generateValidMoves(maze, cur);
       //判断peek是否在generateValidMoves
       if(!neighbors.contains(next)){
           error("没有按照规则移动");
       }
       if(!visited.contains(cur)){
           visited.add(cur);
       }else{
           error("走了回头路");
       }
       cur = next;

    }
    //判断起点
    GridLocation start = {0, 0};
    if(cur != start){
        error("路径不是从起点开始的");
    }

}
STUDENT_TEST("边界测试") {
    Grid<bool> maze = {{true}};
    Stack<GridLocation> soln = {{0, 0}};

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("回头") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    Stack<GridLocation> soln = {{0,0}, {1,0},{1, 0},{2,0},{2,1},{2,2}};


    EXPECT_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("回到原点") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    Stack<GridLocation> soln = {{0,0}, {1,0},{2,0},{2,1},{2,2},{1,2},{0,2},{0,1},{0,0}};


    EXPECT_ERROR(validatePath(maze, soln));
}


//构造一个已经走过路径的集合
Set<GridLocation> gatherVisited(Stack<GridLocation> path){
    Set<GridLocation> visited;
    while(!path.isEmpty()){
        visited.add(path.pop());
    }
    return visited;
}
/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */
        //创建一个空的队列
        Queue<Stack<GridLocation>> solution;
        GridLocation start = {0,0};
        GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
        Set<GridLocation> visited;
        Set<GridLocation> next;
        //创建一个仅包含入口的路径并添加到队列中
        Stack<GridLocation> init = {start};
        solution.enqueue(init);

        while(!solution.isEmpty()){
            MazeGraphics::highlightPath(path, "red", 100);
            path = solution.dequeue();
            GridLocation cur = path.peek();

            visited = gatherVisited(path);
            if(cur == mazeExit){
                break;
            }

            next = generateValidMoves(maze, cur);
            for(const GridLocation& item : next){
                if(!visited.contains(item)){
                    Stack<GridLocation> copy = path;
                    copy.push(item);
                    solution.enqueue(copy);

                }
            }

        }


    return path;
}

STUDENT_TEST("solveMaze on file 25x33") {
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
