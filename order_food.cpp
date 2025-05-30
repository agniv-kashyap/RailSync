
#include "railway_system.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include "utility.h" 

using namespace std;

extern string loggedInUserId;
extern map<pair<string, int>, vector<pair<string, int>>> foodOrders;

orderFood::orderFood() : menu({
    {1, {"Veg Meal", 120}},
    {2, {"Non-Veg Meal", 150}},
    {3, {"Special Thali", 200}}
}) {}

void orderFood::displayFoodDatabase() {
    cout << "\nE-CATERING MENU\n";
    for (const auto& item : menu) {
        cout << item.first << ". " << item.second.first << " - Rs. " << item.second.second << endl;
    }
}

// Function to check if the PNR exists in tickets.txt
bool orderFood::pnrExists(int pnr) {
    ifstream file("tickets.txt");
    if (!file) {
        cerr << " Error opening tickets.txt file.\n";
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
                    cerr << " Invalid PNR format in file.\n";
                }
                break;
            }
            ++column;
        }
    }

    return false;
}

void orderFood::orderFoodItems(int pnr) {
    if (!pnrExists(pnr)) {
        cout << " Invalid PNR! No such booking found.\n";
        return;
    }

    displayFoodDatabase();
    vector<pair<string, int>> order;
    int choice, quantity;
    char more;

    do {
        choice = getIntInput("Enter item number to order (0 to finish): "); 

        if (choice == 0) break;

        if (menu.find(choice) == menu.end()) {
            cout << " Invalid item number! Please try again.\n";
            continue;
        }

        quantity = getIntInput("Enter quantity: "); 
        if (quantity <= 0) {
            cout << " Quantity must be a positive number!\n";
            continue;
        }

        order.push_back({menu[choice].first, quantity});

        cout << "Add more items? (y/n): ";
        cin >> more;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (more == 'y' || more == 'Y');

    if (!order.empty()) {
        foodOrders[{loggedInUserId, pnr}] = order;
        cout << " Food order placed successfully for PNR: " << pnr << " under user: " << loggedInUserId << endl;
    } else {
        cout << " No items ordered.\n";
    }
}

void orderFood::viewFoodOrder(int pnr) {
    if (foodOrders.find({loggedInUserId, pnr}) == foodOrders.end()) {
        cout << " No food orders found for PNR: " << pnr << " under your account.\n";
        return;
    }

    cout << "\nFOOD ORDER DETAILS FOR PNR: " << pnr << " (User: " << loggedInUserId << ")\n";
    cout << "Item\t\t     Quantity\tPrice\n";
    float total = 0;

    for (const auto& item : foodOrders[{loggedInUserId, pnr}]) {
        float price = 0;
        for (const auto& m : menu) {
            if (m.second.first == item.first) {
                price = m.second.second;
                break;
            }
        }
        cout << item.first << "\t\t" << item.second << "\t\t" << price * item.second << endl;
        total += price * item.second;
    }

    cout << "TOTAL: Rs. " << total << endl;
}
