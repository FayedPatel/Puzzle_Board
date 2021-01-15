#ifndef PUZZLE_PROECT
#define PUZZLE_PROECT

#include<iostream>
#include <string>

using namespace std;

// we know its always going to be a 15 puzzle
const int BOARD_SIZE = 16;
const int BOARD_ROW = 4;
const int BOARD_COL = 4;


/* Class for the puzzle, in which we are "represeting it as
a node."

Each puzzle object will have its own 2d array int board
keeps track of the current blank spot
ability to move up,left,right,down
check if it can move into that valid direction

*/

class Puzzle_node {

private:

    // keep tracks of current blankspot
    int x_position{};
    int y_position{};

    // 2d araray of ints represeting the state of the board
    //16 pieces..so 4x4
    int current_puzzle_board [BOARD_ROW][BOARD_COL]{};

    // holds the path to reach this puzzle board u,d,l,r
    string moves ="";

    int goal_state[BOARD_ROW][BOARD_COL]{}; // store the goal state in here

public:

    // keep track number of misplace tiles
    int misplace_tiles = 0;

    // Manhattan distance
    int manhatten_dis = 0;

    // keep track of G + H for total cost of heuristic
    int heuristics = 0;

    int depth = 0; // depth starts at 0

    string moved_in_this_dir =""; // keep track of each node in which that direction it moves in

    // keep track of the parent node
    Puzzle_node *parent{};

    // keep track of how many nodes allocated
    static int node_count;

    // set up the intial state of the puzzle
    explicit Puzzle_node(int init_board[BOARD_ROW][BOARD_COL]);

    // copy constructor when we create new nodes of the puzzle board
    Puzzle_node(const Puzzle_node &new_state);

    // another constructor for which we get no solution
    explicit Puzzle_node (int x );

    void print_puzzle_board(); // debugging purposes..print the internal board

    // get positions of x,y 0 spot
    int get_x_position();
    int get_y_position();

    // boolean method to check if the blank spot can move up,down,right or left
    bool valid_direction(string direction);

    // method to move a direction...aka changing the board
    void move_direction(string direction);

    // reached goal
    bool reached_goal_state();

    // return moves which leads to getting the solution
    string get_moves();

    string convert_to_string( ); // convert the board to a string

    // calculate number of misplace tiles
    void calculate_misplace_tiles( );

    // calculate Manhattan distance
    void calculate_manhattan_distance( );



}; // class


#endif
