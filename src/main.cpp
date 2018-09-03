#include <iostream>
#include <string>
#include <limits>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <ctype.h>
#include <map>

#define PLAYER_CHAR 'X'
#define AI_CHAR 'O'
#define BOARD_INDEX(answer) answer - '0' - 1

#define PLAYER_WIN 1
#define AI_WIN -1

#define ABS(n) (n * (((n > 0) << 1) - 1))

using namespace std;

map<char, int> CHAR_TO_INT;

char board[9];

void game();

int main() {
    CHAR_TO_INT[PLAYER_CHAR] = PLAYER_WIN;
    CHAR_TO_INT[AI_CHAR] = AI_WIN;
    srand(time(NULL));
    game();   
    return 0;
}

void clear_screen() {
    // Hacky but simple.
    system("clear");
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

#define CHECK_WIN_ROW(index, step) (board[index] == board[index + step] && board[index + step] == board[index + (step * 2)])

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

#define GAME_STATE_CONTINUE 0
#define GAME_STATE_TIE 1
#define GAME_STATE_WON 2 

int game_state() {
    if (is_winner())
        return GAME_STATE_WON;
    if (is_board_full())
        return GAME_STATE_TIE;
    return GAME_STATE_CONTINUE;
}

bool make_move(int index, int step) {
    int total = 0;
    int adj_index;

    for (int i = 0; i < 3; i++)
       total += CHAR_TO_INT[board[index + (step * i)]];

    if (ABS(total) == 2) {
        for (int i = 0; i < 3; i++) {
            adj_index = index + (step * i);
            if (is_char_numeric(board[adj_index])) {
                board[adj_index] = AI_CHAR;
                return true;
            }
        }
    }
    return false;
}

void ai_move() {
    int move;

    for(size_t i = 0; i < 3; i++)
    {
        if (make_move(i, 3)) return;
        if (make_move(i * 3, 1)) return;
    }

    if (make_move(0, 4)) return;
    if (make_move(2, 2)) return;
    
    do {
        move = rand() % 9;
    } while (!is_char_numeric(board[move]));

    board[move] = AI_CHAR;
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