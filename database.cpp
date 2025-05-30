#include "railway_system.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <limits>

using namespace std;

// Global data definitions
vector<Station> stations;
vector<Train> allTrains;
map<string, map<string, int>> seatAvailability;
map<pair<string, int>, vector<pair<string, int>>> foodOrders;
string loggedInUserId = "";

// Initialize seat availability
void initializeSeatAvailability(const vector<Train>& trains) {
    for (const auto& train : trains) {
        seatAvailability[train.number]["3AC"] = train.ac3Seats;
        seatAvailability[train.number]["2AC"] = train.ac2Seats;
        seatAvailability[train.number]["1AC"] = train.ac1Seats;
    }
}

// Load databases
void loadDatabases(vector<Station>& stationsList, vector<Train>& trainsList) {

    stationsList.clear(); 
    trainsList.clear();
    ifstream stationFile("stations.txt");
    if (!stationFile.is_open()) {
        cerr << "Error: Could not open stations.txt\n";
        return;
    }

    string scode, sname;
    while (getline(stationFile, scode, ',')) {
        if (!getline(stationFile, sname)) {
            cerr << "Warning: Malformed line in stations.txt\n";
            continue;
        }
        stationsList.push_back(Station(scode, sname));
    }
    stationFile.close();

    ifstream trainFile("trains.txt");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open trains.txt\n";
        return;
    }

    string line;
    while (getline(trainFile, line)) {
        stringstream ss(line);
        string number, name, startStation, endStation, departure, arrival;
        int hours, ac3Price, ac2Price, ac1Price, ac3Seats, ac2Seats, ac1Seats;

        if (!getline(ss, number, ',') || !getline(ss, name, ',') || !getline(ss, startStation, ',') ||
            !getline(ss, endStation, ',') || !getline(ss, departure, ',') || !getline(ss, arrival, ',')) {
            cerr << "Warning: Malformed train data line.\n";
            continue;
        }

        if (!(ss >> hours)) { cerr << "Warning: Missing or invalid duration.\n"; continue; }
        if (!ss.ignore()) continue;
        if (!(ss >> ac3Price)) { cerr << "Warning: Missing or invalid 3AC price.\n"; continue; }
        if (!ss.ignore()) continue;
        if (!(ss >> ac2Price)) { cerr << "Warning: Missing or invalid 2AC price.\n"; continue; }
        if (!ss.ignore()) continue;
        if (!(ss >> ac1Price)) { cerr << "Warning: Missing or invalid 1AC price.\n"; continue; }
        if (!ss.ignore()) continue;
        if (!(ss >> ac3Seats)) { cerr << "Warning: Missing or invalid 3AC seats.\n"; continue; }
        if (!ss.ignore()) continue;
        if (!(ss >> ac2Seats)) { cerr << "Warning: Missing or invalid 2AC seats.\n"; continue; }
        if (!ss.ignore()) continue;
        if (!(ss >> ac1Seats)) { cerr << "Warning: Missing or invalid 1AC seats.\n"; continue; }

        trainsList.push_back(Train(number, name, startStation, endStation, departure,
                                   arrival, hours, ac3Price, ac2Price, ac1Price,
                                   ac3Seats, ac2Seats, ac1Seats));
    }
    trainFile.close();
    initializeSeatAvailability(trainsList);
}

// Display stations menu with input handling
int displayStationsMenu(const vector<Station>& stationsList, bool showAll, const string& excludeCode) {
    cout << "\nAvailable Stations:\n";
    int counter = 1;
    vector<int> validIndices;
    for (size_t i = 0; i < stationsList.size(); i++) {
        if (!showAll && stationsList[i].code == excludeCode) continue;
        cout << counter << ". " << stationsList[i].code << " - " << stationsList[i].name << endl;
        validIndices.push_back(i);
        counter++;
    }

    cout << "0. Back\nSelect station: ";
    int choice;
    while (true) {
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
            continue;
        }
        if (choice == 0) return -1;
        if (choice > 0 && choice <= validIndices.size()) return validIndices[choice - 1];

        cout << "Invalid choice. Please try again: ";
    }
}

// Select destination station
string selectDestinationStation(const string& boarding, const vector<Station>& stationsList) {
    string boardingName;
    for (const auto& s : stationsList) {
        if (s.code == boarding) {
            boardingName = s.name;
            break;
        }
    }
    cout << "Boarding from: " << boardingName << " (" << boarding << ")\n\nSelect destination station:\n";

    while (true) {
        int choice = displayStationsMenu(stationsList, false, boarding);
        if (choice == -1) return "";
        return stationsList[choice].code;
    }
}

