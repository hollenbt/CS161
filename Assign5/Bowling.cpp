/*********************************************************************
** Program Filename: Bowling.cpp
** Author: Thomas Hollenberg
** Date: 02/24/2017
** Description: Simulates games of bowling.
** Input: Number of players and player names. Press enter incessantly.
** Output: Bowling scoreboard and gameplay text.
*********************************************************************/

#include <iostream>     // for cin and cout
#include <iomanip>      // for setw(), setfill()
#include <cstring>      // for strlen()
#include <cmath>        // for ceil()
#include <cstdlib>      // for rand(), srand()
#include <ctime>        // for time()

#define INT_MAX 2147483647
#define FINAL_FRAME 9

using namespace std;

class Player {
public:
    char name[64] {};
    char bowls[21] {};
    int frame_s[10] {};
    int total_s {};
};

int game_setup(Player**);
int get_integer(const string&, int max_input = INT_MAX);
void lets_go_bowling(int, Player*);
void bowl_frame(int, Player*, int, Player*);
int bowl(const char*, char*, int, bool);
void update_score(int, Player*, int, int);
void fill_balls(Player*, bool, int, Player*);
void prompt_bowler(const char*);
void print_scoreboard(int, Player*);
void spaces(int);
char space_if_zero(char);
void print_bowls(Player*);
void print_scores(Player*);
void format_score(int, char bowl = 1);
void declare_winner(int, Player*);

/*********************************************************************
** Function: main
** Description: Seeds the random number generator and calls game_setup
**   and lets_go_bowling while the user chooses to keep playing. Handles
**   memory deallocation of Player arrays created on the heap in game_setup.
** Parameters: N/A
** Pre-Conditions: N/A
** Post-Conditions: The last game has finished and the user chose to quit.
** Return: 0
*********************************************************************/
int main() {
    int number_players;
    Player *player_list = 0;
    srand(time(NULL));

    do {
        number_players = game_setup(&player_list);
        lets_go_bowling(number_players, player_list);
        delete[] player_list;
        player_list = 0;
    }while (2 != get_integer("Would you like to play again (1: Yes, 2: Quit)? ", 2));

    return 0;
}

/*********************************************************************
** Function: game_setup
** Description: Performs pre-game setup, including determining the
**   number of players and each player's name.
** Parameters: Player **p - a pointer to a Player object pointer that will
**               point to the array of players.
** Pre-Conditions: N/A
** Post-Conditions: The pointer pointed to by p points to a dynamically
**   allocated Player array of length equal to the return value.
** Return: A positive int equal to the number of players (which is also
**   the size of the array pointed to by the pointer at the address
**   held by p).
*********************************************************************/
int game_setup(Player **p) {
    int num_p = get_integer("How many players? ");
    *p = new Player[num_p];
    for (int i = 0; i < num_p; ++i) {
        cout << "Enter name of Player " << i + 1 << ": ";
        cin.getline((*p)[i].name, 64);
    }
    return num_p;
}

/*********************************************************************
** Function: get_integer
** Description: Prompts the user and gets an integer between 1 and the
**   value passed to max_input, inclusive.
** Parameters: const string &prompt - the prompt message that will be
**               output repeatedly until the user enters an acceptable
**               value.
**             int max_input - the maximum acceptable value (the default
**               is set to INT_MAX).
** Pre-Conditions: max_input must be greater than or equal to 0.
** Post-Conditions: N/A
** Return: An integer between 1 and max_input, inclusive.
*********************************************************************/
int get_integer(const string &prompt, int max_input) {
    double x;
    while (1) {
        cout << prompt;
        cin >> x;
        if (cin.fail()) {
            cin.clear();
        }
        cin.ignore(INT_MAX, '\n');
        if ((x >= 1.0) && (x <= max_input))
			if (ceil(x) == x)
				return x;
    }
}

/*********************************************************************
** Function: let_go_bowling
** Description: Performs pre-game setup, including determining the
**   number of players and each player's name.
** Parameters: int num_p - the number of players.
**             Player *p - points to a Player object array of length num_p.
** Pre-Conditions: num_p is a positive integer, and p points to a Player
**   object array of length num_p.
** Post-Conditions: The Player objects in the array pointed to by p have
**   been updated to reflect their bowling performance.
** Return: N/A
*********************************************************************/
void lets_go_bowling(int num_p, Player *p) {
    for (int frame = 0; frame <= FINAL_FRAME; ++frame)
        for (int bowler = 0; bowler < num_p; ++bowler) {
            bowl_frame(frame, &p[bowler], num_p, p);
        }
    declare_winner(num_p, p);
}

/*********************************************************************
** Function: bowl_frame
** Description: Performs one frame of bowling for one player and updates
**   their bowls[], frame_s[], and total_s member variables, including
**   updating the frame scores of preceding frames where a strike or
**   spare was bowled.
** Parameters: int frame - the current frame number.
**             Player *bowler - the player currently bowling.
**             int num_p - the number of players.
**             Player *p_list - pointer to the Player array on the heap.
** Pre-Conditions: N/A
** Post-Conditions: The Player object's bowls[], frame_s[], and total_s
**   member variables have been updated to reflect the bowling outcomes
**   of the frame, including adding additional points to preceding strike
**   or spare frames.
** Return: N/A
*********************************************************************/
void bowl_frame(int frame, Player *bowler, int num_p, Player *p_list) {
    int pins_left = 10;
    int pins_knocked_down = bowl(bowler->name, &(bowler->bowls[2 * frame]), pins_left, true);
    update_score(frame, bowler, pins_knocked_down, 1);
    print_scoreboard(num_p, p_list);
    pins_left -= pins_knocked_down;
    if (pins_left) {
        pins_knocked_down = bowl(bowler->name, &(bowler->bowls[2 * frame + 1]), pins_left, false);
        update_score(frame, bowler, pins_knocked_down, 2);
        print_scoreboard(num_p, p_list);
    }
    if (frame == FINAL_FRAME && bowler->frame_s[frame] == 10) {
        bool strike = (bowler->bowls[2 * frame] == 'X');
        fill_balls(bowler, strike, num_p, p_list);
    }
}

/*********************************************************************
** Function: update_score
** Description: Updates player score based on the number of pins knocked
**   down, including adding additional points to preceding strike or
**   spare frames.
** Parameters: int frame - the current frame number.
**             Player *bowler - the player who just bowled.
**             int pins_knocked_down - how many pins were knocked down.
**             int ball_number - 1 if this was the first ball of the frame,
**               2 if this was the second ball of the frame, 3 if this
**               was the third ball in the final frame.
** Pre-Conditions: frame is from 0 to 9, pins_knocked_down is from 0 to
**   10, ball_number is 1, 2, or 3.
** Post-Conditions: The frame_s[] and total_s member objects have been
**   updated based on pins_knocked_down and ball_number.
** Return: N/A
*********************************************************************/
void update_score(int frame, Player *bowler, int pins_knocked_down, int ball_number) {
    bowler->frame_s[frame] += pins_knocked_down;
    bowler->total_s += pins_knocked_down;

    if (!frame)
        return;

    if (ball_number == 1) {
        if (bowler->frame_s[frame - 1] == 10) {
            bowler->frame_s[frame - 1] += pins_knocked_down;
            bowler->total_s += pins_knocked_down;
        }
        if (frame > 1 && bowler->bowls[2 * (frame - 1)] == 'X' && bowler->bowls[2 * (frame - 2)] == 'X') {
            bowler->frame_s[frame - 2] += pins_knocked_down;
            bowler->total_s += pins_knocked_down;
        }
    }
    else if (ball_number == 2 && bowler->bowls[2 * (frame - 1)] == 'X') {
        bowler->frame_s[frame - 1] += pins_knocked_down;
        bowler->total_s += pins_knocked_down;
    }
}

/*********************************************************************
** Function: bowl
** Description: Prompts bowler, generates random bowl result, outputs
**   a message notifying the player of the result, and stores the result
**   in the player's bowls[] member variable char array, but does not
**   update frame or total scores.
** Parameters: const char *name - the bowler's name.
**             char* scorecard - where the result char should be stored.
**             int pins_left - the maximum number of pins that can be
**               knocked down.
**             bool new_frame - whether this is the first bowl of the
**               frame.
** Pre-Conditions: pins_left is from 1 to 10.
** Post-Conditions: The result of the bowl has been output to the console
**   and stored in the char pointed to by scorecard.
** Return: The number of pins knocked down, between 0 and pins_left.
*********************************************************************/
int bowl(const char *name, char *scorecard, int pins_left, bool new_frame) {
    prompt_bowler(name);
    int pins_knocked_down = rand() % (pins_left + 1);

    if (pins_knocked_down == pins_left) {
        if (new_frame) {
            *scorecard = 'X';
            cout << "You bowled a strike! Congratulations!\n\n";
        }
        else {
            *scorecard = '/';
            cout << "You bowled a spare! Good job!\n\n";
        }
    }
    else if (!pins_knocked_down) {
        *scorecard = '-';
        cout << "You bowled a gutter ball... This isn't bumper bowling!\n\n";
    }
    else {
        *scorecard = pins_knocked_down + '0';
        cout << "You knocked down " << pins_knocked_down << " pins.\n\n";
    }
    return pins_knocked_down;
}

/*********************************************************************
** Function: fill_balls
** Description: Allows player to bowl additional balls in frame 10 if
**   they got a spare or a strike initially.
** Parameters: Player *p - the player currently bowling.
**             bool strike - whether the player scored a strike (true)
**               or a spare (false) in the final frame.
**             int num_p - the number of players.
**             Player *p_list - pointer to the Player array on the heap.
** Pre-Conditions: N/A
** Post-Conditions: The frame_s[] and total_s member objects have been
**   updated based on the fill ball result(s).
** Return: N/A
*********************************************************************/
void fill_balls(Player *bowler, bool strike, int num_p, Player *p_list) {
    int pins_left = 10, pins_knocked_down;
    if (strike) {
        pins_knocked_down = bowl(bowler->name, &(bowler->bowls[2 * FINAL_FRAME + 1]), pins_left, true);
        update_score(FINAL_FRAME, bowler, pins_knocked_down, 2);
        print_scoreboard(num_p, p_list);
        pins_left -= pins_knocked_down;
        if (pins_left)
            pins_knocked_down = bowl(bowler->name, &(bowler->bowls[2 * FINAL_FRAME + 2]), pins_left, false);
        else pins_knocked_down = bowl(bowler->name, &(bowler->bowls[2 * FINAL_FRAME + 2]), 10, true);
        update_score(FINAL_FRAME, bowler, pins_knocked_down, 3);
    }
    else {
        pins_knocked_down = bowl(bowler->name, &(bowler->bowls[2 * FINAL_FRAME + 2]), pins_left, true);
        update_score(FINAL_FRAME, bowler, pins_knocked_down, 3);
    }
    print_scoreboard(num_p, p_list);
}

/*********************************************************************
** Function: prompt_bowler
** Description: Prompts the bowler to press enter to bowl and waits for
**   this to occur.
** Parameters: const char* name - the name of the bowler.
** Pre-Conditions: name points to an initialized char or C-style string.
** Post-Conditions: N/A
** Return: N/A
*********************************************************************/
void prompt_bowler(const char *name) {
    cout << endl << name << ", press enter to bowl.";
    char garbage[256];
    cin.getline(garbage, 256);
}

/*********************************************************************
** Function: print_scoreboard
** Description: Prints the scoreboard to the console, displaying the
**   results of each bowl thus far, as well as current frame scores and
**   total score of all players.
** Parameters: int num_p - the number of players.
**             Player *p_list - pointer to the Player array on the heap.
** Pre-Conditions: name points to an initialized char or C-style string.
** Post-Conditions: N/A
** Return: N/A
*********************************************************************/
void print_scoreboard(int num_p, Player *p_list) {
    Player *curr_p;
    cout << "Name          |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |   10  | Total";
    for (int p = 0; p < num_p; ++p) {
        curr_p = &p_list[p];
        cout << '\n' << setw(84) << setfill('-') << '\n';
        cout << curr_p->name;
        spaces(14 - strlen(curr_p->name));
        print_bowls(curr_p);
        print_scores(curr_p);
    }
    cout << endl;
}

/*********************************************************************
** Function: spaces
** Description: Outputs num_spaces ' ' characters to the console.
** Parameters: int num_spaces - the number of spaces to be output.
** Pre-Conditions: num_spaces must be non-negative.
** Post-Conditions: num_spaces ' ' characters have been output to the
**   console.
** Return: N/A
*********************************************************************/
void spaces(int num_spaces) {
    for (int i = 0; i < num_spaces; ++i)
        cout << ' ';
}

/*********************************************************************
** Function: space_if_zero
** Description: Returns a ' ' is the passed character is equal to 0, and
**   returns the character otherwise.
** Parameters: char c - the character to be checked.
** Pre-Conditions: N/A
** Post-Conditions: N/A
** Return: A space or the passed character.
*********************************************************************/
char space_if_zero(char c) {
    return (c ? c : ' ');
}

/*********************************************************************
** Function: print_bowls
** Description: Prints the bowling results.
** Parameters: Player *curr_p - the player whose results are being
**               printed.
** Pre-Conditions: N/A
** Post-Conditions: The bowling results have been printed to the console.
** Return: N/A
*********************************************************************/
void print_bowls(Player *curr_p) {
    cout << "| ";
    for (int frame = 0; frame <= FINAL_FRAME; ++frame) {
        cout << space_if_zero(curr_p->bowls[2 * frame]) << ' ' << space_if_zero(curr_p->bowls[2 * frame + 1]);
        if (frame == FINAL_FRAME)
            cout << ' ' << space_if_zero(curr_p->bowls[2 * frame + 2]);
        cout << " | ";
    }
}

/*********************************************************************
** Function: print_scores
** Description: Prints the frame scores and total scores.
** Parameters: Player *curr_p - the player whose scores are being
**               printed.
** Pre-Conditions: N/A
** Post-Conditions: The scores have been printed to the console.
** Return: N/A
*********************************************************************/
void print_scores(Player *curr_p) {
    spaces(4);
    format_score(curr_p->total_s);
    cout << endl;
    spaces(14);
    cout << "|   ";
    for (int frame = 0; frame <= FINAL_FRAME; ++frame) {
        if (frame == FINAL_FRAME)
            spaces(2);
        format_score(curr_p->frame_s[frame], curr_p->bowls[2 * frame]);
        cout << " |   ";
    }
}

/*********************************************************************
** Function: format_score
** Description: Prints the score if score and bowl are not both
**   zero, otherwise prints a space. If the score is 10 or greater,
**   the most recent character printed to the console will be erased.
**   If the score is 100 or greater, a second character will also be
**   erased.
** Parameters: int score - the score to be output.
**             char bowl - the result of the first bowl of the frame,
**               default value is 1.
** Pre-Conditions: score is non-negative.
** Post-Conditions: The score has been output to the console properly.
** Return: N/A
*********************************************************************/
void format_score(int score, char bowl) {
    if (!score && !bowl)
        cout << ' ';
    else {
        if (score >= 10)
            cout << '\b';
        if (score >= 100)
            cout << '\b';
        cout << score;
    }
}

/*********************************************************************
** Function: declare_winner
** Description: Determines which player had the highest total score
**   and prints out their name, declaring them the winner.
** Parameters: int num_p - the number of players.
**             Player *p_list - pointer to the Player array on the heap.
** Pre-Conditions: The length of the array pointed to by p_list is equal
**   to num_p, which must be a positive integer.
** Post-Conditions: The winner has been declared.
** Return: N/A
*********************************************************************/
void declare_winner(int num_p, Player *p_list) {
    char winner[256];
    int max_score = -1;
    bool tie = false;

    for (int i = 0; i < num_p; ++i) {
        if (p_list[i].total_s == max_score)
            tie = true;
        else if (p_list[i].total_s > max_score) {
            strcpy(winner, p_list[i].name);
            max_score = p_list[i].total_s;
            tie = false;
        }
    }
    if (tie)
        cout << "\nIt was a tie!" << endl;
    else cout << '\n' << winner << " won the game!" << endl;
}
