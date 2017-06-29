/*********************************************************************
** Program Filename: Grade_Calculator
** Author: Tommy Hollenberg
** Date: 02/1/2017
** Description: Grade calculator for CS161 and other classes.
** Input: Category weights, individual scores and point values.
** Output: Category and overall grade percentages.
*********************************************************************/

#include <iostream>
#include <iomanip>  // for setprecision()
#include <string>   // for string objects
#include <sstream>  // for stringstream objects
#include <cmath>    // for ceil()
#include <cassert>  // for assert()

// Rather than including the cfloat and climits libraries, the constants will be hardcoded as macros.
#define DBL_MAX 1.79769e+308
#define INT_MAX 2147483647

using namespace std;

/*********************************************************************
** Function: get_user_input
** Description: Get double or integer input from the user between 0 and
**     the value of the argument passed to the max_input parameter.
** Parameters: string prompt - Message to repeatedly output until good
**               good input is entered.
**             double max_input - The maximum value that is considered
**               good input (default value is DBL_MAX). If no natural
**               maximum exists and only integer values are to be
**               accepted, INT_MAX should be passed to this parameter.
**             bool int_flag - Should the function only accept integer
**               input? (Default value is false.)
** Pre-Conditions: The argument passed to max_input must be greater than
**     or equal to 0. If not, the assert statement will fail and the
**     program will stop in order to prevent an infinite loop.
** Post-Conditions: The returned value is a double from 0 to max_input,
**     inclusive. If int_flag was true, the value should also be a valid
**     integer, so conversion to int results in no loss of information.
** Return: User input value as a double.
*********************************************************************/
double get_user_input(string prompt, double max_input = DBL_MAX, bool int_flag = false) {
    assert(max_input >= 0);
    double x;
    while (1) {
        cout << prompt;
        cin >> x;
        if (cin.fail()) {
            cin.clear();
            x = -1.0;
        }
        cin.ignore(INT_MAX, '\n');
        if ((x >= 0.0) && (x <= max_input))
			if (!int_flag || (ceil(x) == x))
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
** Function: calc_simple_avg
** Description: Takes user input scores and point values and calculates
**     a simple average grade percentage (not a weighted average). The
**     result is output to the console and stored in the calling function
**     variable referenced by average.
** Parameters: double &average - Reference to the (sub)category average
**               double variable in the calling function.
**             string u_sing - Upper case singular (sub)category name.
**             string l_sing - Lower case singular (sub)category name.
**             string l_plur - Lower case plural (sub)category name.
**             bool subcat_flag - Is this a subcategory? (Default value
**               is false.) This should be true for quizzes, designs,
**               and critiques, which are subcategories of the recitation
**               category. If true, recalculation will occur without
**               prompting the user, because they have already chosen to
**               recalculate in the calc_rec_avg function.
** Pre-Conditions: N/A
** Post-Conditions: The calling function variable referenced by average
**     remains unchanged or is 100 * (sum of scores) / (sum of point values).
** Return: N/A
*********************************************************************/
void calc_simple_avg(double &average, string u_sing, string l_sing, string l_plur, bool subcat_flag = false) {
    if (!subcat_flag) {
        // Output stored value and ask user whether or not to recalculate.
        cout << "\nThe stored " + l_sing + " average is " << fixed << setprecision(1) << average << '%' << endl;
        if (get_user_input("Is this correct (No, recalculate now: 0, Yes: 1)? ", 1))
            return;
    }

    // Get scores and point values from user.
    int number;
    double score_sum = 0, point_sum = 0, current_point_value;
    number = get_user_input("\nHow many " + l_plur + "? ", INT_MAX, true);
    if (number) {
        if (get_user_input("Are the point values uniform (No: 0, Yes: 1)? ", 1)) {
            current_point_value = get_user_input("What is the point value of each " + l_sing + "? ");
            point_sum = number * current_point_value;
            for (int i = 0; i < number; ++i)
                score_sum += get_user_input(u_sing + ' ' + to_string(i + 1) + " score: ");
        }
        else {
            for (int i = 0; i < number; ++i) {
                current_point_value = get_user_input(u_sing + ' ' + to_string(i + 1) + " point value: ");
                point_sum += current_point_value;
                score_sum += get_user_input(u_sing + ' ' + to_string(i + 1) + " score: ");
            }
        }
    }

    // Calculate and output grade percentage.
    average = 100 * score_sum / (point_sum ? point_sum : 1.0);
    cout << "\nYour " + l_sing + " average is " << fixed << setprecision(1) << average << '%' << endl;
}

/*********************************************************************
** Function: calc_rec_avg
** Description: Calculates a weighted average grade percentage for the
**     recitation category, which consists of 3 subcategories: quizzes,
**     designs, and critiques. The result is output to the console and
**     stored in the calling function variable referenced by weighted_avg.
** Parameters: double &weighted_avg - Reference to the double variable
**               in the calling function holding the recitation average.
** Pre-Conditions: N/A
** Post-Conditions: The calling function variable referenced by
**     weighted_avg remains unchanged or holds the newly calculated
**     weighted recitation average.
** Return: N/A
*********************************************************************/
void calc_rec_avg(double &weighted_avg) {
    // Output stored value and ask user whether or not to recalculate.
    cout << "\nThe stored recitation average is " << fixed << setprecision(1) << weighted_avg << '%' << endl;
    if (get_user_input("Is this correct (No, recalculate now: 0, Yes: 1)? ", 1))
        return;

    double quiz_weight, design_weight, critique_weight, quiz_avg = 0, design_avg = 0, critique_avg = 0;

    // Determine subcategory weighting.
    cout << "\nEnter subcategory weights below. ";
    do {
        cout << "Weights must sum to 100." << endl;
        quiz_weight = get_user_input("Quiz weight: ", 100);
        design_weight = get_user_input("Design weight: ", 100 - quiz_weight);
        critique_weight = get_user_input("Critique weight: ", 100 - quiz_weight - design_weight);
    }while (quiz_weight + design_weight + critique_weight != 100);

    // Calculate non-zero-weighted subcategory averages.
    if (quiz_weight)
		calc_simple_avg(quiz_avg, "Quiz", "quiz", "quizzes", true);
    if (design_weight)
		calc_simple_avg(design_avg, "Design", "design", "designs", true);
    if (critique_weight)
		calc_simple_avg(critique_avg, "Critique", "critique", "critiques", true);

    // Calculate and output recitation grade percentage.
    weighted_avg = (quiz_avg * quiz_weight / 100) + (design_avg * design_weight / 100) + (critique_avg * critique_weight / 100);
    cout << "\nYour weighted recitation average is " << fixed << setprecision(1) << weighted_avg << '%' << endl;
}

/*********************************************************************
** Function: calc_total_avg
** Description: Calculates a weighted overall grade percentage for the
**     class, which consists of 4 categories: labs, assignments,
**     recitations, and tests. The result is output to the console,
**     but is not stored.
** Parameters: double &lab_a - Reference to a double variable in the
**               calling function holding the lab average.
**             double &assign_a - Reference to a double variable in the
**               calling function holding the assignment average.
**             double &rec_a - Reference to a double variable in the
**               calling function holding the recitation average.
**             double &test_a - Reference to a double variable in the
**               calling function holding the test average.
** Pre-Conditions: N/A
** Post-Conditions: The weighted overall grade percentage has been
**     output to the console.
** Return: N/A
*********************************************************************/
void calc_total_avg(double &lab_a, double &assign_a, double &rec_a, double &test_a) {
    double lab_weight, assign_weight, rec_weight, test_weight;

    // Determine category weighting.
    cout << "\nEnter category weights below. ";
    do {
        cout << "Weights must sum to 100." << endl;
        lab_weight = get_user_input("Lab weight: ", 100);
        assign_weight = get_user_input("Assignment weight: ", 100 - lab_weight);
        rec_weight = get_user_input("Recitation weight: ", 100 - lab_weight - assign_weight);
        test_weight = get_user_input("Test weight: ", 100 - lab_weight - assign_weight - rec_weight);
    }while (lab_weight + assign_weight + rec_weight + test_weight != 100);

    // Calculate non-zero-weighted category averages.
    if (lab_weight)
        calc_simple_avg(lab_a, "Lab", "lab", "labs");
    if (assign_weight)
        calc_simple_avg(assign_a, "Assignment", "assignment", "assignments");
    if (rec_weight)
        calc_rec_avg(rec_a);
    if (test_weight)
        calc_simple_avg(test_a, "Test", "test", "tests");

    // Calculate and output overall class grade percentage.
    cout << "\nYour overall weighted class average is " << fixed << setprecision(1)
         << (lab_a * lab_weight / 100) + (assign_a * assign_weight / 100) + (rec_a * rec_weight / 100) + (test_a * test_weight / 100) << '%' << endl;
}

int main() {
    int user_choice;
    double lab_avg = 0, assign_avg = 0, rec_avg = 0, test_avg = 0;
    cout << "Welcome to the CS161 Grade Calculator!" << endl;
    do {
        // Output calculator menu and perform the user's chosen action.
        cout << "\nWould you like to calculate an average for:\n\t1) Labs \t2) Assignments\t3) Recitations\n\t4) Tests\t5) Overall\t0) or Quit?\n";
        user_choice = get_user_input("Please enter a valid integer: ", 5, true);
        switch (user_choice) {
            case 0: return 0;
            case 1: calc_simple_avg(lab_avg, "Lab", "lab", "labs");
                    break;
            case 2: calc_simple_avg(assign_avg, "Assignment", "assignment", "assignments");
                    break;
            case 3: calc_rec_avg(rec_avg);
                    break;
            case 4: calc_simple_avg(test_avg, "Test", "test", "tests");
                    break;
            case 5: calc_total_avg(lab_avg, assign_avg, rec_avg, test_avg);
                    break;
        }
    }while (1);
}
