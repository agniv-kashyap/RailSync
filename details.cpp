#include "railway_system.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#include "utility.h"
#include <fstream>
#include <sstream>

using namespace std;

string Details::name[6];
string Details::gender[6];
int Details::age[6];
int Details::cId[6];
string Details::bp[6];
string Details::boardingStation;
string Details::destinationStation;
int Details::pnr;
int Details::n;

// Function to check if the generated PNR already exists in the tickets.txt file
bool Details::pnrExists(int pnr) {
    ifstream file("tickets.txt");
    if (!file) {
        cerr << "Error opening tickets.txt file.\n";
        return false;  
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string segment;
        int column = 0;

        while (getline(ss, segment, '|')) {
            if (column == 1) {  
                try {
                    long long filePnr = stoll(segment);
                    if (filePnr == pnr) {
                        return true;  
                    }
                } catch (const invalid_argument&) {
                    cerr << "Invalid PNR format in file.\n";
                }
                break;
            }
            ++column;
        }
    }

    return false;  
}

void Details::information() {
    // Input number of passengers
    cout << "\nEnter the number of passengers (1 to 6): ";
    while (true) {
        cin >> n;
        if (cin.fail() || n <= 0 || n > 6) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter a number between 1 and 6: ";
        } else {
            break;
        }
    }

    cout << "\nBoarding Station: " << boardingStation;
    cout << "\nDestination Station: " << destinationStation << endl;

    // Input passenger details
    for (int i = 0; i < n; i++) {
        cout << "\nPassenger " << i + 1 << " details:\n";

        // Customer ID input
        cout << "Enter customer ID: ";
        while (true) {
            cin >> cId[i];
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Enter a valid customer ID (number): ";
            } else {
                break;
            }
        }

        // Name input
        cout << "Enter name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name[i]);

        // Age input
        cout << "Enter age: ";
        while (true) {
            cin >> age[i];
            if (cin.fail() || age[i] <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Enter a valid age (positive number): ";
            } else {
                break;
            }
        }

        // Birth preference input
        while (true) {
            cout << "Enter birth preference (Lower, Middle, Upper): ";
            cin >> bp[i];
            clearInputBuffer();

            transform(bp[i].begin(), bp[i].end(), bp[i].begin(), ::tolower);

            if (bp[i] == "lower" || bp[i] == "middle" || bp[i] == "upper") {
                break;
            } else {
                cout << " Invalid input! Please enter Lower, Middle, or Upper.\n";
            }
        }

        // Gender input
        while (true) {
            cout << "Enter gender (Male, Female, Other): ";
            cin >> gender[i];
            clearInputBuffer();

            transform(gender[i].begin(), gender[i].end(), gender[i].begin(), ::tolower);

            if (gender[i] == "male" || gender[i] == "female" || gender[i] == "other") {
                break;
            } else {
                cout << " Invalid input! Please enter Male, Female, or Other.\n";
            }
        }
    }

    // Now generate the PNR after collecting all details
    bool uniquePnrFound = false;
    while (!uniquePnrFound) {
        srand(time(NULL));
        pnr = 1000000000 + (rand() % 900000000);

        if (!pnrExists(pnr)) {
            uniquePnrFound = true;
        }
    }

    cout << "\nYour PNR number is: " << pnr << endl;
    cout << "\nDetails saved successfully!\n";
}
