#include<iostream>
#include <vector>
#include <queue>
#include "Puzzle_node.h"
#include <time.h>
#include <sys/resource.h>
#include <stack>
#include <algorithm>

using namespace std;

// global vector to hold all the heuristics that go over the threshold
vector<int> low_values;

// create the goal,initial state
void state_creation(  int goal[BOARD_ROW][BOARD_COL], int starting_state_board[BOARD_ROW][BOARD_COL], const vector<int> &input_list) {

    // now we make the starting state
    int counter =0;

    // now starting_state_board holds the values
    for(int i=0; i < BOARD_ROW; i++) {
        for (int j=0; j < BOARD_COL; j++) {
            starting_state_board[i][j] = input_list[counter];
            counter++;
        }
    }

    // goal state should be here, but i moved it within the node class to make it easier to access

}
// iterative deepening depth first search
// we will use a iterative DFS to make it simple

Puzzle_node* star_id ( Puzzle_node* initial_state, const vector<string> &directions, int &nodes_made, int max_value, const string& flag) {

    // clear vector of heuristics
    low_values.clear();

    // create a stack to help us perform the dfs
    stack <Puzzle_node*> frontier;

    // create a vector to keep track of already same state
    vector<string> state_visited;

    // keep track of visited or not
    bool vis = false;

    clock_t tStart = clock(); // start the clock for 30 seconds

    // add the initial state to the stack
    frontier.push(initial_state);
    // keep going until the stack is empty
    while (!frontier.empty()) {

        vis = false; // rest to false

        // grab the top  element and pop it off
        Puzzle_node* p1 = frontier.top();
        frontier.pop();

        // get the string representation of the board
        string c_state = p1->convert_to_string();
        // check if we already reached this state, if we did then skip it other else store that state
        for (string s : state_visited) {
            if (s == c_state ) {
                vis = true;
            }
        }
        
        // found a matching state so we just skip it and move on
        if (vis) {
            continue;
        }
        else { // store the board
            state_visited.push_back(c_state);
        }

        // check if the goal state is reached
        if(p1->reached_goal_state()) {
            return p1;
        }

        // check if the cutoff for heuristics is over.
        // if it is, we ignore it and then store that heuristics value
        if (p1->heuristics > max_value) {
            // if it is bigger, store it in vector then find min of that vec to set new max value
            low_values.push_back(p1->heuristics);
            continue;
        }

        // loop through the direction list
        for (const auto & direction : directions) {

            // check for valid direction
            if (p1->valid_direction(direction)) {

                //make new puzzle..by coping it first
                auto* p2 = new Puzzle_node(*p1);
                // make move
                p2->move_direction(direction);
                nodes_made++;

                p2->parent = p1;

                // check if were doing misplace tiles OR Manhattan distance
                if (flag == "manhatten") {
                    p2->calculate_manhattan_distance(); // calculate Manhattan distance as well heuristics
                }
                else if (flag == "misplaceTILES") {
                    p2->calculate_misplace_tiles();  // calculate misplace tiles as well as the heuristics
                }
                frontier.push(p2);
            }
        }
    }
    
    return nullptr;
}



// intilize the static variable in puzzle node class
// not used in a*
int Puzzle_node::node_count = 0;

//========================================================================================================================
int main()
{

    cout << "Enter the board information...16 numbers" << endl;
    cout << "Example: 1 0 2 4 5 7 3 8 9 6 11 12 13 10 14 15"<< endl;
    cout << "< ";

    clock_t tStart = clock(); // start the clock

    // get the input
    int number;
    vector<int> input_list; // create vector to store the entire board

    // get all the 16 numbers
    for (int i=0; i < BOARD_SIZE; i++) {
        cin >> number;
        input_list.push_back(number);
    }

    // initialize the puzzle boards
    int starting_state_board [BOARD_ROW][BOARD_COL];
    int goal_state[BOARD_ROW][BOARD_COL];
    state_creation(goal_state, starting_state_board, input_list);

    // create a list of directions, 4 directions
    vector<string>directions;
    directions.emplace_back("UP");
    directions.emplace_back("DOWN");
    directions.emplace_back("RIGHT");
    directions.emplace_back("LEFT");

    int nodes = 1; // count how many nodes were created

    // create a puzzle object
    auto *start = new Puzzle_node(starting_state_board);

    int start_depth = 0;
    Puzzle_node *solution = star_id(start,directions,nodes, start_depth, "manhatten"); // run iddfs

    cout << "Calculating manhattan distance..." << endl;

    // continue to loop depth by depth
    while (!solution) {
        auto min_value = *min_element(low_values.begin(),low_values.end()); // grab the lowest value that we found
        start_depth = min_value;
        solution = star_id(start,directions,nodes, start_depth, "manhatten");
    }

    cout << endl << "Manhattan Distance:" << endl;

    // get all the moves which lead to the solution
    cout << "Moves: " <<  solution->get_moves() << endl;
    cout << "Number of Nodes expanded: " << nodes << endl;

    // get the total time
    auto time_so_far = (double)(clock() - tStart)/CLOCKS_PER_SEC;
    cout << "Time Taken: " << time_so_far << "s" << endl;

    // get the amount of memory used
    int mem = RUSAGE_SELF;
    struct rusage resource_usage{}; // linux get resource usage from man page
    int get_total_mem = getrusage(mem,&resource_usage);
    cout<< "Memory Used: " <<resource_usage.ru_maxrss << "kb" << endl << endl; // get the max resident size

    int nodes_created = 1;
    auto* start1 = new Puzzle_node(starting_state_board);

    //Puzzle_node* manhattan_solution = star_id(start1,directions,nodes_created, "manhatten");
    int start_depth1 = 0;
    Puzzle_node *manhattan_solution = star_id(start1,directions,nodes_created, start_depth1, "misplaceTILES"); // run iddfs

    cout << "Calculating misplace tiles..." << endl;

    // continue to loop depth by depth
    while (!manhattan_solution) {
        auto min_value1 = *min_element(low_values.begin(),low_values.end()); // grab the lowest value that we found
        start_depth1 = min_value1;
        manhattan_solution = star_id(start1,directions,nodes_created, start_depth1, "misplaceTILES");
    }

    cout << endl << "Number of misplace tiles" << endl;

    // get all the moves which lead to the solution
    cout << "Moves: " <<  manhattan_solution->get_moves() << endl;
    cout << "Number of Nodes expanded: " << nodes_created << endl;

    // get the total time
    auto time_so_far1 = (double)(clock() - tStart)/CLOCKS_PER_SEC;
    cout << "Time Taken: " << time_so_far1 << "s" << endl;

    // get the amount of memory used
    int mem1 = RUSAGE_SELF;
    struct rusage resource_usage1{}; // linux get resource usage from man page
    int get_total_mem1 = getrusage(mem1,&resource_usage1);
    cout<< "Memory Used: " <<resource_usage1.ru_maxrss << "kb" << endl << endl; // get the max resident size

    return 0;
}
