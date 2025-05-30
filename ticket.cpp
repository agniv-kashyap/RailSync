#include "railway_system.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <limits>

using namespace std;

float ticket::charges;
int ticket::selectedTrainIndex;
string ticket::selectedClass;

extern vector<Train> allTrains;
extern map<string, map<string, int>> seatAvailability;
extern string loggedInUserId;

void ticket::Bill() {
    try {
        ofstream outf("tickets.txt", ios::app);
        if (!outf.is_open()) {
            throw runtime_error("Failed to open tickets.txt for writing.");
        }

        for (int i = 0; i < n; i++) {
            outf << loggedInUserId << "\t|" << pnr << "\t|" << cId[i] << "\t|" << name[i] << "\t|" << gender[i] << "\t|"
                 << boardingStation << "\t|" << destinationStation << "\t|" << selectedClass
                 << "\t|" << charges << "\t|" << endl;
        }
        outf.close();

        // Update seat availability
        string trainNo = allTrains[selectedTrainIndex].number;
        seatAvailability[trainNo][selectedClass] -= n;

        cout << " Total Cost: Rs. " << charges * n << endl;
    } catch (const exception& e) {
        cerr << " Error in generating bill: " << e.what() << endl;
    }
}

void ticket::dispBill() {
    try {
        ifstream ifs("tickets.txt");
        if (!ifs.is_open()) {
            throw runtime_error("Could not open tickets.txt to display bill.");
        }

        cout << "\n YOUR TICKET\nUser ID\t|PNR\t\t|CID\t|Name\t|Gender\t|From\t|To\t|Class\t|Fare\n";
        string line;
        bool found = false;

        while (getline(ifs, line)) {
            stringstream ss(line);
            string userIdFromFile;
            int currentPnr;
            getline(ss, userIdFromFile, '\t');
            ss.ignore(); 
            ss >> currentPnr;

            if (userIdFromFile == loggedInUserId && currentPnr == pnr) {
                cout << line << endl;
                found = true;
            }
        }

        if (!found) {
            cout << " No matching ticket found for current session PNR.\n";
        }

        ifs.close();
    } catch (const exception& e) {
        cerr << " Error displaying ticket: " << e.what() << endl;
    }
}

void ticket::dispDatabase() {
    try {
        ifstream ifs("tickets.txt");
        if (!ifs.is_open()) {
            throw runtime_error("Unable to open tickets.txt to display bookings.");
        }

        cout << "\n YOUR BOOKINGS\nUser ID\t|PNR\t\t|CID\t|Name\t|Gender\t|From\t|To\t|Class\t|Fare\n";
        string line;
        bool found = false;

        while (getline(ifs, line)) {
            stringstream ss(line);
            string userIdFromFile;
            getline(ss, userIdFromFile, '\t');

            if (userIdFromFile == loggedInUserId) {
                cout << line << endl;
                found = true;
            }
        }

        if (!found) {
            cout << " No bookings found under your user ID.\n";
        }

        ifs.close();
    } catch (const exception& e) {
        cerr << " Error displaying booking history: " << e.what() << endl;
    }
}

void ticket::deletedata() {
    try {
        cout << "Enter PNR to cancel: ";
        int cancelPnr;
        while (!(cin >> cancelPnr)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " Invalid input. Enter numeric PNR: ";
        }

        int passengers = 0;
        string trainNo, travelClass;

        ifstream in("tickets.txt");
        if (!in.is_open()) {
            throw runtime_error("Failed to open tickets.txt for cancellation.");
        }

        ofstream temp("temp.txt");
        if (!temp.is_open()) {
            throw runtime_error("Failed to create temporary file for cancellation.");
        }

        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string userIdFromFile;
            int currentPnr;
            getline(ss, userIdFromFile, '\t');
            ss.ignore(); 
            ss >> currentPnr;

            if (userIdFromFile == loggedInUserId && currentPnr == cancelPnr) {
                if (passengers == 0) {
                    string cid, name, gender, from, to, cls;
                    ss.ignore();
                    getline(ss, cid, '\t');
                    getline(ss, name, '\t');
                    getline(ss, gender, '\t');
                    getline(ss, from, '\t');
                    getline(ss, to, '\t');
                    getline(ss, cls, '\t');
                    travelClass = cls;

                    for (size_t i = 0; i < allTrains.size(); i++) {
                        if (allTrains[i].startStation == from && allTrains[i].endStation == to) {
                            trainNo = allTrains[i].number;
                            break;
                        }
                    }
                }
                passengers++;
            } else {
                temp << line << endl;
            }
        }

        in.close();
        temp.close();

        if (passengers > 0) {
            if (remove("tickets.txt") != 0 || rename("temp.txt", "tickets.txt") != 0) {
                throw runtime_error("Error updating tickets file during cancellation.");
            }
            seatAvailability[trainNo][travelClass] += passengers;
            cout << " Booking cancelled successfully! " << passengers << " seat(s) released.\n";
        } else {
            remove("temp.txt");
            cout << " PNR not found for your account!\n";
        }

    } catch (const exception& e) {
        cerr << " Cancellation failed: " << e.what() << endl;
    }
}
