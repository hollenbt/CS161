/*********************************************************************
** Program: adventure.cpp
** Author: Thomas Hollenberg
** Date: 1/20/2017
** Description: Text-based adventure game.
** Input: User must enter 1 or 2 to make decisions.
** Output: Text narration of gameplay.
*********************************************************************/

#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Obstacle {
    string description;
    int difficulty;
    string prompt_text;
    string failure_text;
};

struct Platform {
    int number;
    Platform *straight;
    Platform *side;
    const Obstacle *obs_straight;
    const Obstacle *obs_side;
};

/*********************************************************************
** Function: create_dungeon_map
** Description: Assigns values to the straight and side pointers of each
**              Platform in the dungeon_map array, thus constructing the
**              dungeon layout.
** Parameters: Platform *d_map
** Pre-Conditions: d_map is an array containing at least 10 elements
**                 (additional elements will not be assigned).
** Post-Conditions: number, straight, and side variables of each Platform
**                  object in d_map have been correctly assigned.
** Return: N/A
*********************************************************************/
void create_dungeon_map(Platform *d_map) {
    // Platform 0 (the entrance) is a special  case.
    d_map[0].number = 0;
    d_map[0].straight = &d_map[2];
    d_map[0].side = &d_map[1];

    for (int i = 1; i < 8; ++i) {
        d_map[i].number = i;
        d_map[i].straight = &d_map[i+2];
        if (i % 2)
            d_map[i].side = &d_map[i+1];
        else d_map[i].side = &d_map[i-1];
    }
    // Platforms 8 and 9 (the treasure) are special cases. Platform 9 has no connections because the game is over when it is reached.
    d_map[8].number = 8;
    d_map[8].straight = &d_map[9];
    d_map[8].side = &d_map[7];
    d_map[9].number = 9;
    d_map[9].straight = NULL;
    d_map[9].side = NULL;
}

/*********************************************************************
** Function: shuffle_obstacles
** Description: Randomizes the element ordering of the obstacle_numbering
**              array so that each playthrough of the game is unique.
** Parameters: int *obs_numbering
** Pre-Conditions: obs_numbering is an array containing at least 14 elements
**                 (additional elements will not be shuffled).
** Post-Conditions: The elements of obs_numbering have been permuted.
** Return: N/A
*********************************************************************/
void shuffle_obstacles(int *obs_numbering) {
    int temp, rand_num;
    for (int i = 0; i < 14; ++i) {
        rand_num = rand() % 14;
        temp = obs_numbering[i];
        obs_numbering[i] = obs_numbering[rand_num];
        obs_numbering[rand_num] = temp;
    }
}

/*********************************************************************
** Function: assign_obstacles
** Description: Assigns Obstacles from the OBSTACLE_LIBRARY array to the
**              obs_side and obs_straight pointers of each Platform object
**              in the dungeon_map array based on the values in the
**              obstacle_numbering array.
** Parameters: Platform *d_map, int *obs_list, const Obstacle *OBS_LIBRARY
** Pre-Conditions: d_map, obs_numbering, and OBS_LIBRARY are arrays containing
**                 at least 10, 14, and 14 elements, respectively.
** Post-Conditions: obs_side and obs_straight pointers of each Platform
**                  object in d_map have been assigned.
** Return: N/A
*********************************************************************/
void assign_obstacles(Platform *d_map, int *obs_numbering, const Obstacle *OBS_LIBRARY) {
    // Platform 0 (the entrance) is a special case.
    d_map[0].obs_side = &OBS_LIBRARY[obs_numbering[0]];
    d_map[0].obs_straight = &OBS_LIBRARY[obs_numbering[1]];
    for (int i = 0; i < 4; ++i) {
        d_map[2 * i + 1].obs_side = &OBS_LIBRARY[obs_numbering[3 * i + 2]];
        d_map[2 * i + 1].obs_straight = &OBS_LIBRARY[obs_numbering[3 * i + 3]];
        d_map[2 * i + 2].obs_side = &OBS_LIBRARY[obs_numbering[3 * i + 2]];
        d_map[2 * i + 2].obs_straight = &OBS_LIBRARY[obs_numbering[3 * i + 4]];
    }
    // Platform 9 (the treasure) is a special case.
    d_map[9].obs_side = NULL;
    d_map[9].obs_straight = NULL;
}

/*********************************************************************
** Function: get_user_input
** Description: Prompt the user for input, and continue outputting the
**              prompt until a 1 or 2 has been input.
** Parameters: string prompt
** Pre-Conditions: N/A
** Post-Conditions: The user has entered a 1 or a 2.
** Return: x (the user input value)
*********************************************************************/
int get_user_input(string prompt) {
    int x;
    while (1) {
        cout << prompt;
        cin >> x;
        if (cin.fail())
            cin.clear();
        cin.ignore(32767, '\n');
        if ((x == 1) || (x == 2))
            return x;
    }
}

/*********************************************************************
** Function: play_game
** Description: Outputs text narrating game events and prompts the user for
**              input to make decisions.
** Parameters: Platform *d_map
** Pre-Conditions: d_map is an array containing at least 10 elements and
**                 all of the Platform and const Obstacle pointers of each
**                 Platform object in d_map have been correctly assigned.
** Post-Conditions: The user has won the game or given up.
** Return: N/A
*********************************************************************/
void play_game(Platform *d_map) {
    int user_choice, user_luck;
    Platform *const entrance = &d_map[0];
    Platform *const treasure = &d_map[9];
    Platform *current_platform = entrance;

    cout << "\nYou enter the cave.\n";
    while (current_platform != treasure) {
        user_luck = rand() % 100;

		if (current_platform == entrance) {
			cout << "\nYou orient yourself towards the back of the cave.\nTo your right you see " << current_platform->obs_straight->description << endl;
			cout << "To your left you see " << current_platform->obs_side->description << endl;
		}
		else {
			cout << "\nYou orient yourself towards the back of the cave.\nStraight ahead you see " << current_platform->obs_straight->description << endl;
			cout << "To your " << ((current_platform->number % 2)?"right":"left") << " you see " << current_platform->obs_side->description << endl;
		}
		user_choice = get_user_input("Do you " + current_platform->obs_straight->prompt_text + " (1) or " + current_platform->obs_side->prompt_text + " (2)? ");

        // User chose to go straight
        if (user_choice == 1) {
            if (user_luck >= current_platform->obs_straight->difficulty) {
                cout << "You " << current_platform->obs_straight->prompt_text << " safely to the platform on the other side!\n";
                current_platform = current_platform->straight;
            }
            else {
                cout << current_platform->obs_straight->failure_text;
                cout << "\n\nYou wash ashore and find yourself at the cave entrance.\n";
                current_platform = entrance;
                if (get_user_input("Would you like to exit the cave and quit this game (Keep playing: 1, Quit: 2)? ") == 2)
                    return;
            }
        }
        // User chose to go to the side.
        else {
            if (user_luck >= current_platform->obs_side->difficulty) {
                cout << "You " << current_platform->obs_side->prompt_text << " safely to the platform on the other side!\n";
                current_platform = current_platform->side;
            }
            else {
                cout << current_platform->obs_side->failure_text;
                cout << "\n\nYou wash ashore and find yourself at the cave entrance.\n";
                current_platform = entrance;
                if (get_user_input("Would you like to exit the cave and quit this game (Keep playing: 1, Quit: 2)? ") == 2)
                    return;
		    }
		}
	}

    cout << "\nYou found the treasure! You're set for life!\n"
         << "        ______________________ _\n"
         << "       | \\                    | \\\n"
         << "       |  |                   |  |\n"
         << "       |  |                   |  |\n"
         << "       | /____________________|_/\n"
         << "       /$ $ $ $ $ $ $ $ $ $ /|\n"
         << "      / $ $ $ $ $ $ $ $ $ $/ |\n"
         << "     /_$_$_$_$_$_$_$_$_$_$/  |\n"
         << "    |                     |  |\n"
         << "    |                     |  /\n"
         << "    |                     | /\n"
         << "    |_____________________|/\n\n";
}

int main() {
	// Contains the predefined obstacle set.
	const Obstacle OBSTACLE_LIBRARY[] = {
		{ "a rope hanging from the ceiling. It looks like you could swing across, if you had to.", 55,
		"use the rope swing", "You don't swing far enough and plunge into the water below." },
		{ "a sturdy-looking wooden footbridge.", 5,
		"cross the footbridge", "You stub your toe on a protruding nail and, blinded by pain, stumble into the river." },
		{ "a wet log extending over the opening. It looks quite slippery.", 45,
		"walk across the log", "The log is too slippery. You lose your footing and fall in." },
		{ "mossy stepping stones spaced a bit farther apart than you'd like.", 35,
		"use the stepping stones", "You make a wrong step and walk right into the water." },
		{ "a narrow ledge along the wall of the cave. If you're careful you could walk across.", 30,
		"sidle across the ledge", "You panic and lose your balance, plummeting into the river below." },
		{ "handholds and footholds protruding from the wall. A skilled climber could make it across.", 50,
		"climb the rock wall", "You get stuck halfway across. Your arms fatigue and you have no choice but to let go and dive into the water below." },
		{ "a giant eagle beckoning for your to climb onto its back.", 20,
		"ride the eagle", "You accidentally pull out one of its feathers, and the great bird bucks you off into the river." },
		{ "a hang glider. There is probably enough room to get a good running start.", 25,
		"take the hang glider", "You don't pick up enough speed and come up pathetically short." },
		{ "a zipline securely fastened at both ends.", 10,
		"ride the zipline", "You lose your grip and plunge into the water." },
		{ "a large tree with several long branches extending across the opening.", 40,
		"climb the tree branch", "As you're inching your way across, you hear a loud crack. Both you and the branch tumble down into the river." },
		{ "a narrow gap. You could probably jump across.", 15,
		"make the jump", "It has been longer since your track and field days than you remembered. You take a leap and go right into the river." },
		{ "a canoe resting on the shore. Hopefully it doesn't leak.", 20,
		"paddle across in the canoe", "You hit a rock and the canoe capsizes." },
		{ "a set of rusted monkey bars leading across the opening.", 15,
		"swing across the monkey bars", "Halfway through you miss a bar and fall in." },
		{ "a slackline stretched across the river.", 50,
		"walk across the slackline", "You lose your balance and plunge into the river below." }
	};
    int obstacle_numbering[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

    srand(time(NULL)); // seed random number generator

	Platform dungeon_map[10];
    create_dungeon_map(dungeon_map);

    cout << "***********************************************************************************************************************\n"
         << "**                                        Welcome to \"Cave Explorer\"!                                                **\n"
         << "**                                                                                                                   **\n"
         << "**  You are an intrepid treasure hunter in search of the lost treasure of an ancient civilization of cave dwellers.  **\n"
         << "**  Finally, you have tracked down the very cave where the treasure is hidden. The cave consists of a series of      **\n"
         << "**  platforms (some high and some low) above a roaring underground river. Each platform gives access to two other    **\n"
         << "**  platforms through obstacles of varying difficulty. Choose your path carefully, because if you fail an obstacle,  **\n"
		 << "**  the river brings you all the way back to the cave entrance. Going straight moves you towards the back of the     **\n"
         << "**  cave, one step closer to the treasure. Moving side to side does not, but allows you to circumnavigate dangerous  **\n"
         << "**  obstacles you may find in your path.                                                                             **\n"
         << "**                                                                                                                   **\n"
         << "***********************************************************************************************************************\n";

    do {
        shuffle_obstacles(obstacle_numbering);
        assign_obstacles(dungeon_map, obstacle_numbering, OBSTACLE_LIBRARY);
        play_game(dungeon_map);
    } while (get_user_input("Would you like to play again (Play Again: 1, Quit: 2)? ") != 2);

    return 0;
}
