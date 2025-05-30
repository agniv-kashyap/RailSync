/* 
Team member 1 : Agniv Kashyap
scholar ID : 2312180
Team member 2 : Sanjogita Bhagowati
scholar ID : 2312087
group number : 23 
*/



#include "railway_system.h"
#include <vector>
#include <iostream>
#include <exception>

using namespace std;

// Global data
extern vector<Station> stations;
extern vector<Train> allTrains;
extern map<string, map<string, int>> seatAvailability;
extern map<pair<string, int>, vector<pair<string, int>>> foodOrders;
extern string loggedInUserId;

int main() {
    try {
        loadDatabases(stations, allTrains);
        Management system;
    } catch (const exception& ex) {
        cerr << "Exception caught: " << ex.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown error occurred!" << endl;
        return 1;
    }
    return 0;
}

