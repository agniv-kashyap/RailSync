#include "railway_system.h"
#include <stdexcept>
#include <iostream>

using namespace std;

Train::Train(string num, string nm, string start, string end,
             string dep, string arr, int hrs, int ac3, int ac2,
             int ac1, int ac3s, int ac2s, int ac1s) {
    try {
        // Validate input
        if (num.empty() || nm.empty() || start.empty() || end.empty() || dep.empty() || arr.empty()) {
            throw invalid_argument("Train details (number, name, start station, end station, departure, arrival) cannot be empty.");
        }

        if (hrs <= 0) {
            throw invalid_argument("Journey hours must be greater than 0.");
        }

        if (ac3 < 0 || ac2 < 0 || ac1 < 0) {
            throw invalid_argument("Seat prices cannot be negative.");
        }

        if (ac3s < 0 || ac2s < 0 || ac1s < 0) {
            throw invalid_argument("Seat availability cannot be negative.");
        }

        // Initialize class members
        number = num;
        name = nm;
        startStation = start;
        endStation = end;
        departure = dep;
        arrival = arr;
        hours = hrs;
        ac3Price = ac3;
        ac2Price = ac2;
        ac1Price = ac1;
        ac3Seats = ac3s;
        ac2Seats = ac2s;
        ac1Seats = ac1s;

    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        
    }
}
