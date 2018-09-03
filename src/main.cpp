#include <iostream>
#include <string>
#include <limits>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <ctype.h>
#include <map>
#include <vector>
#include <algorithm>
#include <random> 

#define CAN_CLEAR_SCREEN 1

#ifdef _WIN32
    #define CLEAR_SCREEN_CMD "cls"
#elif __APPLE__ || __unix__
    #define CLEAR_SCREEN_CMD "clear"
#else
    #define CAN_CLEAR_SCREEN 0
#endif

#define PLAYER_CHAR 'X'
#define AI_CHAR 'O'
#define BOARD_INDEX(answer) answer - '0' - 1
#define PLAYER_WIN 1
#define AI_WIN -1
#define CHECK_WIN_ROW(index, step) (board[index] == board[index + step] && board[index + step] == board[index + (step * 2)])
#define GAME_STATE_CONTINUE 0
#define GAME_STATE_TIE 1
#define GAME_STATE_WON 2 

using namespace std;

map<char, int> CHAR_TO_INT;
int seed = time(NULL);

//  2 winning move => player 
// -2 winning move => ai
 
struct ai_move_t
{
    int index;
    int score;
    int priority;
    vector<int> moves;
};

char board[9];

void game();

int main() {
    CHAR_TO_INT[PLAYER_CHAR] = PLAYER_WIN;
    CHAR_TO_INT[AI_CHAR] = AI_WIN;
    srand(seed);
    if (!CAN_CLEAR_SCREEN) {
        cout << "I was unable to detect your platform so the screen clear fuction has been dissabled." << endl;
    }
    game();   
    return 0;
}

void clear_screen() {
    // Hacky but simple.
    if (CAN_CLEAR_SCREEN)
        system(CLEAR_SCREEN_CMD);
}

bool ask_yn(string q) {
    char a;

    cout << q << " (Y/n) :: ";
    cin >> noskipws >> a;
    return a == '\n' || tolower(a) == 'y';
}

void reset_board() {
    string tiles = "123456789";
    char* b = board + 9;
    while (b-- != board)
        *b = tiles[b - board];
}

int randft(int from, int to) {
    return (rand() % to) + from;
}

void print_board() {
    unsigned char i;

    i = 0;
    cout << endl << " |";
    cout << string(11, '-');
    cout << "| " << endl << " | ";
    while (board[i]){
        cout << board[i++] << " | ";
        if (!(i % 3)) {
            cout << endl << " |";
            cout << string(11, '-');
            cout << "| " << endl;
            if (i != 9) {
                cout << " | ";
            }
        }
            
    }
    cout << endl;
}

bool is_char_numeric(char c) {
    return (c >= '0' && c <= '9');
}

bool is_board_full() {
    int i;

    i = 9;
    while (i-- > 0 && !is_char_numeric(board[i]));
    return !++i;
}

char get_char(istream &stream, char& holder) {
    stream.get(holder);
    stream.clear();
    stream.ignore(numeric_limits<streamsize>::max(), '\n');
    return holder;
}

int is_winner() {
    return (
        // rows
        CHECK_WIN_ROW(0, 1) ||
        CHECK_WIN_ROW(3, 1) ||
        CHECK_WIN_ROW(6, 1) ||
        // cols
        CHECK_WIN_ROW(0, 3) ||
        CHECK_WIN_ROW(1, 3) ||
        CHECK_WIN_ROW(2, 3) ||
        // verticals
        CHECK_WIN_ROW(0, 4) ||
        CHECK_WIN_ROW(2, 2)
    );
}

int game_state() {
    if (is_winner())
        return GAME_STATE_WON;
    if (is_board_full())
        return GAME_STATE_TIE;
    return GAME_STATE_CONTINUE;
}

ai_move_t make_move(ai_move_t move, int index, int step) {
    int adj_index;

    move.priority = 2; 
    move.score = 0;
    for (int i = 0; i < 3; i++) {
        adj_index = index + (step * i);
        if (is_char_numeric(board[adj_index])) {
            move.moves.push_back(adj_index);
        } else {
            move.score += CHAR_TO_INT[board[adj_index]];
        }
        
    }
       

    if (move.score == -2) {
        move.priority = 0; // ai can win
    } else if (move.score == 2) {
        move.priority = 1; // user can win
    } else if (move.score == 0) {
        move.index = move.moves[rand() % distance(move.moves.begin(), move.moves.end())];
        return move; 
    }
    for (int i = 0; i < 3; i++) {
        move.index = index + (step * i);
        if (is_char_numeric(board[move.index])) {
            return move;
        }
    }

    return move;
}

bool sort_moves(ai_move_t x, ai_move_t y) {
    return x.priority < y.priority;
}

void ai_move() {
    ai_move_t move;
    vector<ai_move_t> ai_moves;

    for(size_t i = 0; i < 3; i++)
    {
        ai_moves.push_back(make_move(move, i, 3));
        ai_moves.push_back(make_move(move, i * 3, 1));
    }

    ai_moves.push_back(make_move(move, 0, 4));
    ai_moves.push_back(make_move(move, 2, 2));

    // ensures it dosn't sort equivalent priorities in the same order ever turn
    auto gen = default_random_engine(seed);
    shuffle(ai_moves.begin(), ai_moves.end(), gen);

    sort(ai_moves.begin(), ai_moves.end(), sort_moves);
    
    board[ai_moves[0].index] = AI_CHAR;
}

void game() {
    char answer;
    int turn = rand() % 2;
    int state;
    bool game_continue = true;

    clear_screen();
    reset_board();
    
    cout << "Welcome to a clasic game of Tick Tac Toe!" << endl;

    while (game_continue) {
        cout << endl;
        if (turn % 2) { // player turn
            if (turn < 2) cout << "You get to go first!" << endl;
            print_board();
            cout << "Pick a number to commandeer a tile." << endl;
            get_char(cin, answer);
            clear_screen();
            // Becasue if answer == '0': '0' - '0' == 0 == false
            if (
                !(answer - '0') 
            ||  !is_char_numeric(answer) 
            ||  !is_char_numeric(board[BOARD_INDEX(answer)])) {
            cout << "That doesn't seem to be a valid answer..." << endl;
                continue;
            }

            board[BOARD_INDEX(answer)] = PLAYER_CHAR;
        } else { // ai turn
            if (turn < 2) {
                cout << "The AI will take the first turn. " << endl;
                turn += 2; // so the player doesn't get the same message next turn
            }
            ai_move();
        }
        
        state = game_state();
        if (state == GAME_STATE_CONTINUE) {
            turn++;
            continue;
        }
        clear_screen();
        cout << endl;
        if (state == GAME_STATE_TIE)
            cout << "Well that was anticlimactic... You tied..." << endl;
        if (state == GAME_STATE_WON) {
            if ((turn %= 2)) { // player turn
                cout << "Congratulations! You won!" << endl;
            } else { // ai turn
                cout << "You got whooped! You lost..." << endl;
            }
        }
        print_board();
        if ((game_continue = ask_yn("Would you like to play again?"))) 
            reset_board();
        clear_screen();
    }
    cout << "Good bye!" << endl;
}