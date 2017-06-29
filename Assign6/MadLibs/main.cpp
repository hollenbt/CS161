/*********************************************************************
** Program Filename: MadLibs.cpp
** Author: Tommy Hollenberg
** Date: 03/6/2017
** Description: Let's the user play Mad Libs with one of three pre-
**   programmed stories, selected by passing in a 1, 2, or 3 as the
**   sole command-line argument. Fills in the story's missing words
**   randomly with user-supplied words matching the part of speech.
** Input: Pairs consisting of parts of speech and words belonging to
**   that part of speech, space or newline delimited.
** Output: Prints out the completed story.
*********************************************************************/

#include <iostream>
#include <cstring>      // for strlen(), strcpy()
#include <cstdlib>      // for rand(), srand()
#include <ctime>        // for time()

using namespace std;

void fill_word_bank(char****);
int get_code(const char*, const char*);
void add_word(char***, const char*);
bool assign_words(const int*, char***, char***);
void print_story(const char[][102], char**);
void cleanup(char***, char****);

/*********************************************************************
** Function: main
** Description: Checks that the correct number and type of command-line
**   arguments have been passed in, seeds the random number generator,
**   defines the story array and the array of parts of speech of the
**   missing words, calls fill_word_bank() to read in words from the user,
**   calls assign_words() to randomly assign words of the correct part
**   of speech to the story blanks, calls print_story() to output the
**   completed story to the console, and calls cleanup() to free all
**   allocated memory before returning 0 to the operating system.
** Parameters: int argc - the number of command-line arguments passed in.
**             char *argv[] - array of C-style strings containing all of
**               the command-line arguments.
** Pre-Conditions: argc is 2 and argv[1][0] is '1', '2', or '3'.
** Post-Conditions: The completed story has been printed to the console
**   and all allocated memory on the heap has been freed.
** Return: 0
*********************************************************************/
int main(int argc, char *argv[]) {
    if (argc != 2 || (argv[1][0] < '1' || argv[1][0] > '3')) {
        cout << "Please pass the desired story number (1,2,3) as the sole command-line argument." << endl;
        return 0;
    }

    srand(time(NULL));
    const int story_num = argv[1][0] - '1';
    const char story[3][16][102] = {{"Story 1:\n\tMost doctors agree that bicycle ", " is a(n) ", " form of exercise.\n", " a bicycle enables you to develop your ",
                      " muscles, as well as increase\nthe rate of your ", " beat. More ", " around the world ", " bicycles than\ndrive ",
                      ". No matter what kind of ", " you ", ", always be sure to wear a(n)\n", " helmet. Make sure to have ", " reflectors too!\n", "\0"},
                      {"Story 2:\n\tYesterday, ", " and I went to the park. On our way to the ", " park,\nwe saw a(n) ", " ", " on a bike. We also saw big ",
                      " balloons tied to a(n)\n", ". Once we got to the ", " park, the sky turned ", ". It started to ", "\nand ", ". ", " and I ",
                      " all the way home. Tomorrow we will try to go to the\n", " park again and hope it doesn't ", ".\n", "\0"},
                      {"Story 3:\n\tSpring break 2017, oh how I have been waiting for you! Spring break is\nwhen you go to some ", " place to spend time with ",
                      ". Getting to ", " is\ngoing to take ", " hours. My favorite part of spring break is ", " in the\n", ". During spring break, ",
                      " and I plan to ", " all the way to ", ". After spring\nbreak, I will be ready to return to ", " and ", " hard to finish ",
                      ". Thanks\nspring break 2017!\n", "\0"}};
    const int blank_codes[3][15] = {{3, 4, 3, 0, 0, 1, 2, 1, 0, 2, 4, 4, -1},
                                   {0, 4, 4, 0, 4, 0, 4, 4, 2, 2, 0, 2, 4, 2, -1},
                                   {4, 0, 0, 4, 3, 0, 0, 2, 0, 0, 2, 0, -1}};
    char **blanks = 0, ***word_bank = 0;

    fill_word_bank(&word_bank);
    if (!assign_words(blank_codes[story_num], &blanks, word_bank))
        cout << "Some parts of speech missing." << endl;
    else print_story(story[story_num], blanks);
    cleanup(&blanks, &word_bank);

    return 0;
}

/*********************************************************************
** Function: fill_word_bank
** Description: Parses the input from the file, allocating memory on the
**   heap and adding words to the word_bank based on their part of speech
**   code as determined by get_code().
** Parameters: char ****word_bank - points to the pointer in the caller
**               that will point to the array of arrays of C-style strings
**               created in this function to hold the words from the
**               supplied word file.
** Pre-Conditions: N/A
** Post-Conditions: *word_bank points to a dynamically allocated array of
**   arrays of C-style strings holding the words from the word file.
** Return: N/A
*********************************************************************/
void fill_word_bank(char ****word_bank) {
    *word_bank = new char**[5];
    for (int i = 0; i < 5; ++i) {
        (*word_bank)[i] = new char*;
        *((*word_bank)[i]) = 0;
    }
    char PoS[10], word[30];
    cin >> PoS >> word;
    while (!cin.fail()) {
        int code = get_code(PoS, word);
        add_word(&(*word_bank)[code], word);
        cin >> PoS >> word;
    }
}

/*********************************************************************
** Function: get_code
** Description: Determines the part of speech code of the word based on
**   the part of speech label and word ending.
** Parameters: const char *PoS - C-style string containing the part of
**               speech label.
**             const char *word - C-style string containing the word
**               itself.
** Pre-Conditions: PoS and word point to C-style strings.
** Post-Conditions: N/A
** Return: Returns the part of speech code of the word. If no match is
**   found, returns -1.
*********************************************************************/
int get_code(const char *PoS, const char *word) {
    int len = strlen(word);
    if (!strcmp(PoS, "noun")) {
        if (word[len - 1] == 's')
            return 1; //plural noun
        return 0; //singular noun
    }
    if (!strcmp(PoS, "verb")) {
        if (word[len - 3] == 'i' && word[len - 2] == 'n' && word[len - 1] == 'g')
            return 3; //-ing verb
        return 2; //verb
    }
    if (!strcmp(PoS, "adjective"))
        return 4; // adjective
    return -1;
}

/*********************************************************************
** Function: add_word
** Description: Allocates memory for an additional word in the word_list
**   array. Handles deallocation of the old, shorter array.
** Parameters: char ***word_list - points to a pointer in the caller that
**               points to the array of C-style strings containing the
**               words of a particular part of speech.
**             const char *word - C-style string containing the word
**               itself.
** Pre-Conditions: The part of speech of *word_list and word match.
**   *word_list is terminated with a null pointer.
** Post-Conditions: word has been added to *word_list. *word_list has
**   been increased in size by one, and is still null terminated.
** Return: N/A
*********************************************************************/
void add_word(char ***word_list, const char *word) {
    char **temp = *word_list;
    int list_size = -1;
    while ((*word_list)[++list_size]) {}
    *word_list = new char*[list_size + 2];
    for (int i = 0; i < list_size; ++i)
        (*word_list)[i] = temp[i];
    (*word_list)[list_size] = new char[strlen(word) + 1];
    strcpy((*word_list)[list_size], word);
    (*word_list)[list_size + 1] = 0;
    delete[] temp;
}

/*********************************************************************
** Function: assign_words
** Description: Allocates memory on the heap for an array of character
**   pointers that will randomly be assigned to words from the word
**   bank that match the part of speech of each missing word in the
**   story.
** Parameters: const int *blank_codes - an array of the part of speech
**               codes of the required missing words.
**             char ***blanks - points to the pointer in the caller that
**               will point to the array of missing words that will be
**               created in this function.
**             char ***word_bank - points to the array of arrays of
**               C-style strings holding the words from the word file.
** Pre-Conditions: blank_codes points to an integer array terminated by
**   -1. word_bank points to an array of five C-style string arrays that
**   are terminated with a null C-style string.
** Post-Conditions: *blanks points to a dynamically allocated array of
**   C-style strings holding the missing words for the story.
** Return: Returns false if there were no words in the word_bank for one
**   of the necessary parts of speech. Returns true if successful.
*********************************************************************/
bool assign_words(const int *blank_codes, char ***blanks, char ***word_bank) {
    int num_words = -1;
    while (blank_codes[++num_words] != -1) {}
    *blanks = new char*[num_words];
    for (int i = 0; i < num_words; ++i) {
        int num_in_bank = -1;
        while (word_bank[blank_codes[i]][++num_in_bank]) {}
        if (!num_in_bank)
            return false;
        (*blanks)[i] = word_bank[blank_codes[i]][rand() % num_in_bank];
    }
    return true;
}

/*********************************************************************
** Function: print_story
** Description: Prints the story, complete with the missing words
**   supplied by the user.
** Parameters: const char story[][102] - the array of C-style strings
**               holding the paragraph fragments between the missing
**               words.
**             char **blanks - points to the array of C-style strings
**               holding the words selected to fill in the blanks in
**               the story.
** Pre-Conditions: the story array is terminated with a C-style string
**   consisting only of the null terminator character. The length of
**   blanks is two less than the length of story (including the
**   aforementioned terminating element).
** Post-Conditions: The completed story has been printed to the console.
** Return: N/A
*********************************************************************/
void print_story(const char story[][102], char **blanks) {
    int i = 0;
    cout << endl;
    while (story[i + 1][0]) {
        cout << story[i] << blanks[i];
        ++i;
    }
    cout << story[i] << endl;
}

/*********************************************************************
** Function: cleanup
** Description: Frees the memory occupied on the heap by the arrays of
**   words from the word file.
** Parameters: char ***blanks - points to the pointer in the caller that
**               points to the array of C-style strings holding the
**               words selected to fill in the blanks in the story.
**             char ****word_bank - points to the pointer in the caller
**               that points to the array of arrays of C-style strings
**               holding the words from the supplied word file.
** Pre-Conditions: *word_bank points to an array of five C-style string
**   arrays that are terminated with a null pointer
** Post-Conditions: All memory occupied for word storage has been freed.
** Return: N/A
*********************************************************************/
void cleanup(char ***blanks, char ****word_bank) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; (*word_bank)[i][j]; ++j) {
            delete[] (*word_bank)[i][j];
            (*word_bank)[i][j] = 0;
        }
        delete[] (*word_bank)[i];
        (*word_bank)[i] = 0;
    }
    delete[] *word_bank;
    *word_bank = 0;
    delete[] *blanks;
    *blanks = 0;
}
