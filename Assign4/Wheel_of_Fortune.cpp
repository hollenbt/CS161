/*********************************************************************
** Program Filename: Wheel_of_Fortune.cpp
** Author: Thomas Hollenberg
** Date: 02/8/2017
** Description: Phrase guessing game based on the television game show
**   Wheel of Fortune.
** Input: N/A
** Output: Gameplay text.
*********************************************************************/

#include <iostream>
#include <string>       // for string objects
#include <sstream>      // for stringstream objects
#include <cassert>      // for assert()
#include <cmath>        // for ceil()
#include <cstdlib>      // for rand(), srand(), system()
#include <ctime>        // for time()
#include <algorithm>    // for swap()

#define INT_MAX 2147483647
#define LETTERS_IN_ALPHABET 26
#define NUM_VOWELS 5

using namespace std;

struct Player {
    static int counter;
    int number;
    int total_score ;
    int round_score;

    Player() {
        number = ++counter;
        total_score = 0;
        round_score = 0;
    }
};

int Player::counter = 0;

/** Function Prototypes **/
void game_setup(int*, int*, Player**, string**);
int get_integer(const string&, int max_input = INT_MAX);
string to_string(int);
string get_phrase(const string&);
bool check_phrase_validity(const string&);
bool is_alphabetic(char);

void play_game(int, int, Player*, string*);
int censure_phrase(string&);
bool take_turn(Player&, string&, const string&, int*, int&, int&);
bool spin_wheel(Player&, string&, const string&, int*, int&);
bool random_spin(Player&, int&);
bool solve_puzzle(const string&);
bool case_insensitive_compare(const string&, const string&);
void buy_vowel(Player&, string&, const string&, int*, int&);
char get_letter(int*, bool);
bool is_vowel(int);
int decode_phrase(string&, const string&, char);
void print_guess_result(Player&, int, char);
void reset_guess_history(int*, int&, int&);

void declare_winner(int, Player*);
int print_standings(int, Player*, bool);
void sort_by_total_score(int, Player**);


int main() {
    int num_players, num_rounds;
    Player *player;
    string *phrase;

    srand(time(NULL));

    game_setup(&num_players, &num_rounds, &player, &phrase);
    play_game(num_players, num_rounds, player, phrase);
    declare_winner(num_players, player);

    // Deallocate memory.
    delete[] phrase;
    delete[] player;

    return 0;
}

/*********************************************************************
** Function: game_setup
** Description: Performs pre-game setup, including determining the
**   number of players and rounds, allocating memory for the player and
**   secret phrase arrays, and determining the secret phrases to be used
**   in each round.
** Parameters: int *num_p - a pointer to an integer in the caller that
**               will hold the number of players.
**             int *num_r - a pointer to an integer in the caller that
**               will hold the number of rounds.
**             Player **p - a pointer to a Player object pointer that will
**               point to the array of players.
**             string **r - a pointer to a string pointer that will point
**               to the array of secret phrases.
** Pre-Conditions: N/A
** Post-Conditions: num_p points to an integer from 1-3, num_r
**   points to a positive integer, p points to a Player pointer pointing
**   to a Player array of length num_p with each element's total_score
**   and round_score initialized to 0, and r points to a string pointer
**   pointing to a string array of length num_r containing valid secret
**   phrases.
** Return: N/A
*********************************************************************/
void game_setup(int *num_p, int *num_r, Player **p, string **r) {
    system("clear");
    cout << "Wheel of Fortune Game Setup:\n";
    *num_p = get_integer("How many players (1-3)? ", 3);
    *num_r = get_integer("How many rounds? ");

    *p = new Player[*num_p];
    *r = new string[*num_r];

    for (int i = 0; i < *num_r; ++i)
        (*r)[i] = get_phrase("Enter round " + to_string(i + 1) + " phrase: ");
    system("clear");
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
    assert(max_input >= 0);
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
** Function: to_string
** Description: Converts an integer to a string (reproducing the
**     functionality of the overloaded C++11 function std::to_string
**     for int arguments).
** Parameters: int x - The value to be converted to a string.
** Pre-Conditions: N/A
** Post-Conditions: N/A
** Return: The integer value as a string.
*********************************************************************/
string to_string(int x) {
    stringstream ss;
    ss << x;
    return ss.str();
}

/*********************************************************************
** Function: get_phrase
** Description: Prompts the user and gets a valid phrase as determined
**   by the check_validity function.
** Parameters: const string &prompt - the prompt that will be output
**               repeatedly until the user enters a valid phrase.
** Pre-Conditions: N/A
** Post-Conditions: N/A
** Return: A string containing a valid phrase.
*********************************************************************/
string get_phrase(const string &prompt) {
    string s;
    do {
        cout << prompt;
        getline(cin, s);
    }while (!check_phrase_validity(s));
    return s;
}

/*********************************************************************
** Function: check_phrase_validity
** Description: Determines whether the passed phrase contains only
**   letters, spaces, and certain punctuation marks (.!?,;:-'). Every
**   phrase must contain at least one letter.
** Parameters: const string &s - the phrase to be checked.
** Pre-Conditions: N/A
** Post-Conditions: N/A
** Return: True if the phrase is valid, false otherwise.
*********************************************************************/
bool check_phrase_validity(const string &s) {
    bool letter = false;
    for (int i = 0; i < s.length(); ++i) {
        if (is_alphabetic(s[i]) || s[i] == ' ') {
            if (s[i] != ' ')
                letter = true;
            continue;
        }
        if (i && is_alphabetic(s[i-1])) {
            if (i == s.length() - 1 && (s[i] == '.' || s[i] == '!' || s[i] == '?'))
                continue;
            if (i != s.length() - 1 && s[i+1] == ' ' && (s[i] == ',' || s[i] == ';' || s[i] == ':'))
                continue;
            if (i != s.length() - 1 && s[i] == '-' && is_alphabetic(s[i+1]))
                continue;
        }
        if (s[i] == '\'' && ((i && is_alphabetic(s[i-1])) || (i != s.length() - 1 && is_alphabetic(s[i+1]))))
            continue;

        return false;
    }
    return letter;
}

/*********************************************************************
** Function: is_alphabetic
** Description: Determines whether or not the character is a letter of
**   the alphabet.
** Parameters: char c - the letter to  check.
** Pre-Conditions: N/A
** Post-Conditions: N/A
** Return: true if the letter is alphabetic, false otherwise.
*********************************************************************/
bool is_alphabetic(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/*********************************************************************
** Function: play_game
** Description: Orchestrates the wheel of fortune game, determines when
**   each round is over, and keeps track of player scores.
** Parameters: int num_p - the number of players.
**             int num_r - the number of rounds to be played.
**             Player *p - a pointer to a Player array of length num_p.
**             string *r - a pointer to the string array of length
**               num_r containing the secret phrases.
** Pre-Conditions: num_p is from 1-3, num_r is a positive
**   integer, p is a Player array of length num_p, and r is an array
**   of valid phrases of length num_r.
** Post-Conditions: The total_score variable of every Player in p has
**   been updated.
** Return: N/A
*********************************************************************/
void play_game(int num_p, int num_r, Player *p, string *r) {
    int turn = -1, alphabet[LETTERS_IN_ALPHABET] = {}, c_guessed = 0, v_guessed = 0;
    bool solved;
    string board_state;

    cout << "Let's play, Wheel of Fortune!";
    for (int i = 0; i < num_r; ++i) {
        solved = false;
		reset_guess_history(alphabet, c_guessed, v_guessed);
        board_state = r[i];
        censure_phrase(board_state);
        for (int j = 0; j < num_p; ++j)
            p[j].round_score = 0;

        cout << "\n\nRound " << (i + 1) << ":\n\n";
        while (!solved) {
            turn++;
            solved = take_turn(p[turn % num_p], board_state, r[i], alphabet, c_guessed, v_guessed);
        }
        cout << "Player " << p[turn % num_p].number << " won Round " << (i + 1) << ", accumulating " << p[turn % num_p].round_score << " points.\n";
        p[turn % num_p].total_score += p[turn % num_p].round_score;
        if (i != num_r - 1)
            print_standings(num_p, p, false);
    }
}

/*********************************************************************
** Function: censure_phrase
** Description: Censures the phrase by replacing all alphabetic
**   characters with underscores, and returns how many characters where
**   replaced.
** Parameters: string s - the phrase to be censured.
** Pre-Conditions: N/A
** Post-Conditions: All alphabetic characters in s have been replaced
**   with the '_' character.
** Return: The number of characters that were replaced, an integer
**   greater than or equal to 0.
*********************************************************************/
int censure_phrase(string &s) {
    int letters = 0;
    for (int i = 0; i < s.length(); ++i)
        if (is_alphabetic(s[i])) {
            s[i] = '_';
            letters++;
        }
    return letters;
}

/*********************************************************************
** Function: take_turn
** Description: Conducts one turn for the current player.
** Parameters: Player &p - the player whose turn it is.
**             string &board - the current state of the board.
**             const string &answer - the secret phrase being guessed.
**             int *alphabet - a pointer to an integer array keeping
**               track of which letters have been guessed already.
**             int &c_guessed - the number of consonants that have
**               already been guessed.
**             int &v_guessed - the number of vowels that have already
**               been bought.
** Pre-Conditions: N/A
** Post-Conditions: The board state, player score, and letters guessed
**   have all been updated
** Return: A boolean variable indicating whether or not the player
**   correctly guessed the phrase during their turn.
*********************************************************************/
bool take_turn(Player &p, string &board, const string &answer, int *alphabet, int &c_guessed, int &v_guessed) {
    bool solved = false, end_turn = false;
    int choice;
    cout << "Player " << p.number << ':';
    while (!end_turn) {
        cout << endl << board << "\n\n";
        choice = get_integer("Do you want to spin the wheel(1), solve the puzzle(2), or buy a vowel(3)? ", 3);
        if (choice == 1)
            end_turn = spin_wheel(p, board, answer, alphabet, c_guessed);
        else if (choice == 2) {
            solved = solve_puzzle(answer);
            end_turn = true;
        }
        else buy_vowel(p, board, answer, alphabet, v_guessed);

        if (case_insensitive_compare(answer, board)) {
            end_turn = true;
            solved = true;
            cout << endl << board << "\n\n";
        }
    }
    return solved;
}

/*********************************************************************
** Function: spin_wheel
** Description: Allows the player to spin the wheel and, if they don't
**   roll a 0 or 21, guess a consonant. It also updates the player's
**   score and the board.
** Parameters: Player &p - the player spinning the wheel.
**             string &board - the current state of the board.
**             const string &answer - the secret phrase being guessed.
**             int *alphabet - a pointer to an integer array keeping
**               track of which letters have been guessed already.
**             int &c_guessed - the number of consonants that have
**               already been guessed.
** Pre-Conditions: board is a censured version of answer, based on the
**   censure_phrase function, alphabet is an integer array of length
**   LETTERS_IN_ALPHABET
** Post-Conditions: p.round_score has been updated based on the outcome
**   of the wheel spin and the guessed consonant, and the board has been
**   updated to reveal any correctly guessed consonants.
** Return: True if the player's turn has ended, false otherwise.
*********************************************************************/
bool spin_wheel(Player &p, string &board, const string &answer, int *alphabet, int &c_guessed) {
    if (c_guessed >= LETTERS_IN_ALPHABET - NUM_VOWELS) {
		cout << "\nAll of the consonants have already been guessed.\n";
		return false;
	}

	int spin;
	if (random_spin(p, spin))
        return true;

    ++c_guessed;
    char consonant = get_letter(alphabet, false);
    int num_in_phrase = decode_phrase(board, answer, consonant);

    p.round_score += num_in_phrase * spin;
    print_guess_result(p, num_in_phrase, consonant);
    if (!num_in_phrase) {
        cout << "Your turn is over.\n\n";
        return true;
    }
    return false;
}

/*********************************************************************
** Function: random_spin
** Description: Generates a random spin of the wheel and determines
**   whether the player lost all of their round score and/or lost their
**   turn.
** Parameters: Player &p - the playing spinning the wheel.
**             int &spin - the variable that will hold the result.
** Pre-Conditions: N/A
** Post-Conditions: If the player spun a 0, p.round_score is set to 0.
**   spin holds the result of the random spin.
** Return: True if the player lost their turn, false otherwise.
*********************************************************************/
bool random_spin(Player &p, int &spin) {
    spin = rand() % 22;
	cout << "\nYou spun a(n) " << spin << "!\n";
	switch (spin) {
        case 0:  p.round_score = 0;
                 cout << "You lose all of your points. ";
        case 21: cout << "Your turn is over.\n\n";
                 return true;
	}
	return false;
}

/*********************************************************************
** Function: solve_puzzle
** Description: Prompts the user to input a puzzle solution and checks
**   whether the solution was correct.
** Parameters: const string &answer - the correct solution.
** Pre-Conditions: N/A
** Post-Conditions: N/A
** Return: A boolean value indicating whether or not the puzzle was
**   correctly solved.
*********************************************************************/
bool solve_puzzle(const string &answer) {
    string s;
    bool correct;

    cout << "\nEnter the phrase: ";
    getline(cin, s);

    correct = case_insensitive_compare(answer, s);
    if (correct) {
        cout << "Correct!\n";
    }
    else cout << "That is incorrect.\n\n";

    return correct;
}

/*********************************************************************
** Function: case_insensitive_compare
** Description: Compares two equal length strings to see if they are
**   equal, without case sensitivity.
** Parameters: const string &s1 - the first string.
               const string &s2 - the second string,
** Pre-Conditions: The strings are of equal length.
** Post-Conditions: N/A
** Return: True if the strings are equal (ignoring uppercase vs lower-
**   case discrepancies), false otherwise.
*********************************************************************/
bool case_insensitive_compare(const string &s1, const string &s2) {
    for (int i = 0; i < s1.length(); ++i) {
        if (s1[i] != s2[i]) {
            if (is_alphabetic(s1[i]) && is_alphabetic(s2[i]))
                if (s1[i] + 32 == s2[i] || s1[i] - 32 == s2[i])
                    continue;
            return false;
        }
    }
    return true;
}

/*********************************************************************
** Function: buy_vowel
** Description: Handles vowel buying, including ensuring the player can
**   afford it and that not all vowels have already been bought.
** Parameters: Player &p - the player attempting to buy a vowel.
**             string &board - the current state of the board.
**             const string &answer - the secret phrase being guessed.
**             int *alphabet - a pointer to an integer array keeping
**               track of which letters have been guessed already.
**             int &v_guessed - the number of vowels that have already
**               been bought.
** Pre-Conditions: board is a censured version of answer, based on the
**   censure_phrase function, alphabet is an integer array of length
**   LETTERS_IN_ALPHABET
** Post-Conditions: 10 points have been deducted from p.round_score,
**   and the board is updated to reveal any correctly guessed vowels.
** Return: N/A
*********************************************************************/
void buy_vowel(Player &p, string &board, const string &answer, int *alphabet, int &v_guessed) {
    if (p.round_score < 10) {
        cout << "\nYou don't have enough points to buy a vowel!\n";
        return;
    }
	if (v_guessed >= NUM_VOWELS) {
		cout << "\nAll of the vowels have already been bought.\n";
		return;
	}
    cout << endl;
	p.round_score -= 10;
	++v_guessed;
    char vowel = get_letter(alphabet, true);
	int num_in_phrase = decode_phrase(board, answer, vowel);
    print_guess_result(p, num_in_phrase, vowel);
}

/*********************************************************************
** Function: get_letter
** Description: Prompts the user for a vowel or consonant, depending on
**   the value of vowel_flag, and ensures that it has not already been
**   guessed this round.
** Parameters: int *alphabet - an integer array in which the i-th
**               element is zero if the i-th letter of the alphabet has
**               not been guessed yet this round and one otherwise.
**             bool vowel_flag - Defines acceptable input to consist of
**               the vowels (true) or the consonants (false).
** Pre-Conditions: alphabet points to an integer array of length
**   LETTERS_IN_ALPHABET in which the i-th element is zero if the i-th
**   letter of the alphabet has not been guessed yet this round and one
**   otherwise.
** Post-Conditions: alphabet has been updated to reflect the letter that
**   was guessed by the user.
** Return: A previously unguessed lowercase letter, that is a vowel if
**   vowel_flag is true, and a consonant if vowel_flag is false.
*********************************************************************/
char get_letter(int *alphabet, bool vowel_flag){
    int pos;
    char letter;

    while (1) {
        cout << (vowel_flag ? "Pick a vowel: " : "Pick a consonant: ");
        cin >> letter;
        cin.ignore(INT_MAX, '\n');
        if (letter - 'A' < LETTERS_IN_ALPHABET)
            letter += 32;
        pos = letter - 'a';
        if ((pos >= 0) && (pos < LETTERS_IN_ALPHABET)) {
            if (is_vowel(pos) == vowel_flag) {
                if(!alphabet[pos]){
                    ++alphabet[pos];
                    return letter;
                }
                else cout << "That letter has already been chosen this round." << endl;
            }
        }
    }
}

/*********************************************************************
** Function: is_vowel
** Description: Checks whether the letter occupying specified position
**   in the alphabet is a vowel.
** Parameters: int pos - the position in the alphabet to be checked.
** Pre-Conditions: pos is [0, 25].
** Post-Conditions: N/A
** Return: True if pos represents a vowel, false otherwise.
*********************************************************************/
bool is_vowel(int pos) {
    const int vowels[] = {0, 4, 8, 14, 20};
    for (int i = 0; i < NUM_VOWELS; ++i)
        if (pos == vowels[i])
            return true;
    return false;
}

/*********************************************************************
** Function: decode_phrase
** Description: Searches the phrase for a certain letter, reveals
**   the locations of any instances, and returns the number found.
** Parameters: string &board - a string containing the current state
**               of the board.
**             const string &answer - the secret phrase being guessed.
**             char letter - the letter to search for in answer.
** Pre-Conditions: board and answer are the same length
** Post-Conditions: Any instances of the letter (or its uppercase
**   version) found in the answer string will be copied into the
**   corresponding element of the board string.
** Return: The non-negative number of instances found.
*********************************************************************/
int decode_phrase(string &board, const string &answer, char letter) {
    int instances = 0;
    for (int i = 0; i < answer.length(); ++i) {
        if ((letter == answer[i]) || (letter - 32 == answer[i])) {
            board[i] = answer[i];
            ++instances;
        }
    }
    return instances;
}

/*********************************************************************
** Function: print_guess_result
** Description: Prints how many of the guessed letter were found and
**   the player's updated score.
** Parameters: Player &p - the player whose action is being summarized.
**             int num - how many of the guessed letter were found.
**             char letter - the guessed letter.
** Pre-Conditions: N/A
** Post-Conditions: N/A
** Return: N/A
*********************************************************************/
void print_guess_result(Player &p, int num, char letter) {
    if (num == 1)
        cout << "There is 1 " << letter;
    else cout << "There are " << num << ' ' << letter << "'s";
    cout << " in the phrase.\nScore: " << p.round_score << endl;
}

/*********************************************************************
** Function: reset_guess_history
** Description: Resets the previously guessed letters from the previous
**   round.
** Parameters: int *ab - points to an integer array of length
**               LETTERS_IN_ALPHABET that keeps track of which letters
**               have alrady been guessed.
**             int &c - reference to an integer in the caller holding
**               the number of consonants that have been guessed.
**             int &v - reference to an integer in the caller holding
**               the number of vowels that have been guessed.
** Pre-Conditions: ab has LETTERS_IN_ALPHABET elements.
** Post-Conditions: Every element in ab is zero, and c and v are zero.
** Return: N/A
*********************************************************************/
void reset_guess_history(int *ab, int &c, int &v) {
	for (int i = 0; i < LETTERS_IN_ALPHABET; ++i)
		ab[i] = 0;
	c = 0;
	v = 0;
}

/*********************************************************************
** Function: declare_winner
** Description: Prints the final standings and declares the winner.
** Parameters: int num_p - an integer from 1-3 containing the number of
**               of players.
**             Player *p -  an array of Player objects of length num_p.
** Pre-Conditions: num_p is an integer from 1-3 and p is a Player
**   object array of length num_p.
** Post-Conditions: The final standings have been output to the console
**   and the winner has been declared.
** Return: N/A
*********************************************************************/
void declare_winner(int num_p, Player *p) {
	int winner = print_standings(num_p, p, true);
	if (!winner)
		cout << "\nIt was a tie!\n";
	else cout << "\nPlayer " << winner << " is the winner!\n\n";
}

/*********************************************************************
** Function: print_standings
** Description: Sorts the players by total score, prints the standings,
**   and returns the number of the winning player.
** Parameters: int num_p - an integer from 1-3 containing the number of
**               of players.
**             Player *p -  an array of Player objects of length num_p.
**             bool final_score - whether or not these are the final
**               standings.
** Pre-Conditions: num_p is an integer from 1-3 and p is a Player
**   object array of length num_p.
** Post-Conditions: The standings have been output to the console.
** Return: The number of the winning player. If two or more players
**   share the highest score, 0 is returned.
*********************************************************************/
int print_standings(int num_p, Player *p, bool final_score) {
    Player *standings[num_p];
    for (int i = 0; i < num_p; ++i)
        standings[i] = &p[i];

    sort_by_total_score(num_p, standings);

    cout << endl << (final_score ? "Final" : "Current") << " Standings:\n";
    for (int i = 0; i < num_p; ++i)
        cout << "   Player " << standings[i]->number << "   " << standings[i]->total_score << endl;

	if (num_p > 1 && standings[0]->total_score == standings[1]->total_score)
        return 0;
    else return standings[0]->number;
}

/*********************************************************************
** Function: sort_by_total_score
** Description: Sorts the players by total score in descending order.
** Parameters: int num_p - an integer from 1-3 containing the number of
**               of players.
**             Player **standings -  an array of pointers to Player
**               objects of length num_p to be sorted by total_score.
** Pre-Conditions: num_p is an integer from 1-3 and standings is an
**   array of pointers to Player objects of length num_p.
** Post-Conditions: standings has been sorted by total_score in
**   descending order.
** Return: N/A
*********************************************************************/
void sort_by_total_score(int num_p, Player **standings) {
    switch (num_p) {
        case 3: if (standings[0]->total_score < standings[1]->total_score)
                    swap(standings[0], standings[1]);
                if (standings[1]->total_score < standings[2]->total_score)
                    swap(standings[1], standings[2]);
        case 2: if (standings[0]->total_score < standings[1]->total_score)
                    swap(standings[0], standings[1]);
        case 1: break;
    }
}
