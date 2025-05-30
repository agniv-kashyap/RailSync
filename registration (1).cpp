#include "railway_system.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <map>
#include <climits>


using namespace std;

extern vector<Station> stations;
extern map<string, map<string, int>> seatAvailability;

int registration::choice;

//  Helper function for safe integer input
int getSafeIntInput(const string& prompt, int min = INT_MIN, int max = INT_MAX) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << " Invalid input. Please enter a valid number";
            if (min != INT_MIN && max != INT_MAX)
                cout << " (" << min << "-" << max << ")";
            cout << ".\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            return value;
        }
    }
}

//  Polymorphic Base Class
class TrainService {
public:
    virtual void showAvailableTrains(const vector<Train>& trains,
                                     const map<string, map<string, int>>& seatAvailability) = 0;

    virtual Train selectTrain(const vector<Train>& trains) = 0;

    virtual ~TrainService() = default;
};

//  Concrete Implementation (Polymorphism in action)
class StandardTrainService : public TrainService {
public:
    void showAvailableTrains(const vector<Train>& trains,
                             const map<string, map<string, int>>& seatAvailability) override {
        cout << "\nAVAILABLE TRAINS\n------------------------------------------------------------\n";
        cout << "No. Train No Train Name       Dep     Arr     Dur     3AC   2AC   1AC   Avail\n";
        cout << "------------------------------------------------------------\n";

        for (size_t i = 0; i < trains.size(); i++) {
            const auto& t = trains[i];
            cout << setw(2) << i + 1 << " " << setw(8) << t.number << " "
                 << setw(18) << left << t.name.substr(0, 17) << right
                 << " " << setw(5) << t.departure << " "
                 << setw(5) << t.arrival << " " << setw(3) << t.hours << "hr "
                 << setw(5) << t.ac3Price << " " << setw(5) << t.ac2Price << " "
                 << setw(5) << t.ac1Price << " "
                 << setw(3) << seatAvailability.at(t.number).at("3AC") << "/"
                 << setw(3) << seatAvailability.at(t.number).at("2AC") << "/"
                 << setw(3) << seatAvailability.at(t.number).at("1AC") << "\n";
        }
    }

    Train selectTrain(const vector<Train>& trains) override {
        int choice = getSafeIntInput("\nSelect train (1-" + to_string(trains.size()) + "): ", 1, trains.size());
        return trains[choice - 1];
    }
};

string registration::selectBoardingStation() {
    while (true) {
        int choice = displayStationsMenu(stations);
        if (choice == -1) return "";  // Exit condition for invalid input
        return stations[choice].code;
    }
}

void registration::trains() {
    try {
        vector<Train> allTrains;
        loadDatabases(stations, allTrains);
        initializeSeatAvailability(allTrains);
        auto currentSeatAvailability = seatAvailability;

        cout << "\nSELECT BOARDING STATION\n";
        string boarding = selectBoardingStation();
        if (boarding.empty()) return;

        cout << "\nSELECT DESTINATION STATION\n";
        string destination = selectDestinationStation(boarding, stations);
        if (destination.empty()) return;

        Details::boardingStation = boarding;
        Details::destinationStation = destination;

        vector<Train> availableTrains;
        for (const auto& t : allTrains) {
            if (t.startStation == boarding && t.endStation == destination) {
                availableTrains.push_back(t);
            }
        }

        if (availableTrains.empty()) {
            cout << " No trains available for this route!\n";
            return;
        }

        //  Use polymorphism for train service
        StandardTrainService trainService;
        TrainService* service = &trainService;

        service->showAvailableTrains(availableTrains, currentSeatAvailability);
        Train selectedTrain = service->selectTrain(availableTrains);
        ticket::selectedTrainIndex = &selectedTrain - &availableTrains[0];

        cout << "\nSelect class:\n";
        cout << "1. 3AC (Rs. " << selectedTrain.ac3Price << ") - Available: "
             << currentSeatAvailability[selectedTrain.number]["3AC"] << "\n";
        cout << "2. 2AC (Rs. " << selectedTrain.ac2Price << ") - Available: "
             << currentSeatAvailability[selectedTrain.number]["2AC"] << "\n";
        if (selectedTrain.ac1Price > 0)
            cout << "3. 1AC (Rs. " << selectedTrain.ac1Price << ") - Available: "
                 << currentSeatAvailability[selectedTrain.number]["1AC"] << "\n";

        int classChoice = getSafeIntInput("Choice (1-3): ", 1, 3);

        string selectedClass;
        switch (classChoice) {
            case 1:
                ticket::charges = selectedTrain.ac3Price;
                selectedClass = "3AC";
                break;
            case 2:
                ticket::charges = selectedTrain.ac2Price;
                selectedClass = "2AC";
                break;
            case 3:
                if (selectedTrain.ac1Price <= 0) {
                    cout << " Invalid choice! 1AC not available for this train.\n";
                    return;
                }
                ticket::charges = selectedTrain.ac1Price;
                selectedClass = "1AC";
                break;
            default:
                cout << " Invalid class choice!\n";
                return;
        }

        ticket::selectedClass = selectedClass;

        cout << "\n Train " << selectedTrain.number << " " << selectedTrain.name << " booked!\n";
        cout << "Departure: " << selectedTrain.departure << " from " << boarding << "\n";
        cout << "Arrival: " << selectedTrain.arrival << " at " << destination << "\n";
        cout << "Total journey time: " << selectedTrain.hours << " hours\n";

    } catch (const exception& e) {
        cerr << " An unexpected error occurred: " << e.what() << "\n";
    } catch (...) {
        cerr << " Unknown exception caught. Please try again.\n";
    }
}
