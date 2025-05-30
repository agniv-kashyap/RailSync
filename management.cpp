#include "railway_system.h"
#include "utility.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <limits>
#include <sstream>

using namespace std;

extern string loggedInUserId;
extern map<string, map<string, int>> seatAvailability;
extern map<pair<string, int>, vector<pair<string, int>>> foodOrders;

// Forward declarations
class Details;
class registration;
class ticket;
class orderFood;

// Simple hash function (djb2)
unsigned long simpleHash(const string& str) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

// admin menu
void Management::mainMenu() {
    int lchoice;
    ticket t;
    orderFood f;

    while (true) {
        cout << "\n\t RAILSYNC Railways (ADMIN MODE)\n";
        cout << "\t__Admin Menu\n";
        cout << "\t__________________________________________\n";
        cout << "\t| Press 1 to View All Bookings           |\n";
        cout << "\t| Press 2 to View Food Orders            |\n";
        cout << "\t| Press 3 to Display Train Database      |\n";
        cout << "\t| Press 4 to Add a Train                 |\n";
        cout << "\t| Press 5 to Remove a Train              |\n";
        cout << "\t| Press 6 to Back to Main Menu           |\n";
        cout << "\t__________________________________________\n";

        lchoice = getIntInput("Enter the Choice: ");
        switch (lchoice) {
            case 1: {
                ifstream ifs("tickets.txt");
                if (!ifs.is_open()) {
                    cerr << "Error opening ticket database.\n";
                    break;
                }
                cout << "\nALL TICKETS\nUser ID|\tPNR\t|CID\t|Name\t|Gender\t|From\t|To\t|Class\t|Fare\n";
                string line;
                while (getline(ifs, line)) {
                    cout << line << "\n";
                }
                ifs.close();
                getIntInput("Press any key to go back: ");
                break;
            }
            case 2: {
                cout << "\nFOOD ORDERS\n";
                
                if (foodOrders.empty()) {
                    cout << "No food orders placed.\n";
                } else {
                    for (const auto& order : foodOrders) {
                        cout << "User ID: " << order.first.first
                             << ", PNR: " << order.first.second << "\n";
                        for (const auto& item : order.second) {
                            cout << "  " << item.first
                                 << " x " << item.second << "\n";
                        }
                    }
                }
            
                getIntInput("Press any key to go back: ");
                break;
            }
            
            case 3: {
                ifstream file("trains.txt");
                if (!file.is_open()) {
                    cerr << "Error opening trains.txt\n";
                    break;
                }

                cout << "\nTRAIN DATABASE\n";
                cout << "Train No | Name             | From | To   | Dep | Arr | Duration | 3AC  | 2AC  | 1AC  | 3AC Avail | 2AC Avail | 1AC Avail\n";
                cout << "---------------------------------------------------------------------------------------------------------------------------\n";
                string line;
                while (getline(file, line)) {
                    stringstream ss(line);
                    string field;
                    vector<string> fields;

                    while (getline(ss, field, ',')) {
                        fields.push_back(field);
                    }

                    if (fields.size() == 13) {
                        cout << fields[0] << " | "
                             << left << setw(18) << fields[1] << " | "
                             << setw(5) << fields[2] << " | "
                             << setw(4) << fields[3] << " | "
                             << setw(4) << fields[4] << " | "
                             << setw(4) << fields[5] << " | "
                             << setw(4) << fields[6] << " | "
                             << setw(5) << fields[7] << " | "
                             << setw(5) << fields[8] << " | "
                             << setw(5) << fields[9] << " | "
                             << setw(10) << fields[10] << " | "
                             << setw(10) << fields[11] << " | "
                             << setw(10) << fields[12] << "\n";
                    }
                }
                file.close();
                getIntInput("Press any key to go back: ");
                break;
            }
            case 4: {
                ofstream file("trains.txt", ios::app);
                if (!file.is_open()) {
                    cerr << "Error opening trains.txt for writing.\n";
                    break;
                }

                cout << "\nEnter Train Details:\n";
                string trainNo, name, from, to, dep, arr, days;
                int fare3ac, fare2ac, fare1ac, avail3ac, avail2ac, avail1ac;

                cout << "Train Number: ";
                cin >> trainNo;
                clearInputBuffer();
                cout << "Train Name: ";
                getline(cin, name);
                cout << "From: ";
                cin >> from;
                cout << "To: ";
                cin >> to;
                cout << "Departure Time (HH:MM): ";
                cin >> dep;
                cout << "Arrival Time (HH:MM): ";
                cin >> arr;
                cout << "Travel Days: ";
                cin >> days;
                cout << "Fare 3AC: ";
                cin >> fare3ac;
                cout << "Fare 2AC: ";
                cin >> fare2ac;
                cout << "Fare 1AC: ";
                cin >> fare1ac;
                cout << "Availability 3AC: ";
                cin >> avail3ac;
                cout << "Availability 2AC: ";
                cin >> avail2ac;
                cout << "Availability 1AC: ";
                cin >> avail1ac;

                file << trainNo << "," << name << "," << from << "," << to << "," << dep << "," << arr << ","
                     << days << "," << fare3ac << "," << fare2ac << "," << fare1ac << ","
                     << avail3ac << "," << avail2ac << "," << avail1ac << "\n";

                file.close();
                cout << " Train added successfully!\n";
                getIntInput("Press any key to go back: ");
                break;
            }
            case 5: {
                string trainNo;
                cout << "\nEnter Train Number to remove: ";
                cin >> trainNo;

                ifstream file("trains.txt");
                if (!file.is_open()) {
                    cerr << "Error opening trains.txt\n";
                    break;
                }

                ofstream temp("temp.txt");
                if (!temp.is_open()) {
                    cerr << "Error creating temp file.\n";
                    break;
                }

                string line;
                bool found = false;
                while (getline(file, line)) {
                    if (line.find(trainNo + ",") != 0) {
                        temp << line << "\n";
                    } else {
                        found = true;
                    }
                }

                file.close();
                temp.close();
                remove("trains.txt");
                rename("temp.txt", "trains.txt");

                if (found) {
                    cout << " Train removed successfully.\n";
                } else {
                    cout << " Train not found.\n";
                }
                getIntInput("Press any key to go back: ");
                break;
            }
            case 6:
                return;
            default:
                cout << "Invalid Input, Please try again!\n";
                break;
        }
    }
}

// user menu
void Management::mainMenu2() {
    int lchoice;
    static bool trainBooked = false;
    static bool customerDetailsAdded = false;
    Details d;
    registration r;
    ticket t;
    orderFood f;

    while (true) {
        cout << "\n\tRAILSYNC Railways (User: " << loggedInUserId << ")\n";
        cout << "\t__Main Menu\n";
        cout << "\t______________________________________\n";
        cout << "\t| 1. Train Booking                   |\n";
        cout << "\t| 2. Add Customer Details            |\n";
        cout << "\t| 3. Ticket and Charges              |\n";
        cout << "\t| 4. Display My Bookings             |\n";
        cout << "\t| 5. Cancel My Booking               |\n";
        cout << "\t| 6. View E-Catering Menu            |\n";
        cout << "\t| 7. Order Food                      |\n";
        cout << "\t| 8. View My Food Order              |\n";
        cout << "\t| 9. Go Back to Main Menu            |\n";  
        cout << "\t| 10. Exit                           |\n";   
        cout << "\t______________________________________\n";

        lchoice = getIntInput("Enter the Choice: ");
        switch (lchoice) {
            case 1:
                r.trains();
                trainBooked = true;
                break;
            case 2:
                if (!trainBooked) {
                    cout << " Please book a train before entering customer details.\n";
                } else {
                    d.information();
                    customerDetailsAdded = true;
                }
                break;
            case 3:
                if (!trainBooked) {
                    cout << " You must book a train first.\n";
                } else if (!customerDetailsAdded) {
                    cout << " Please enter customer details before viewing ticket.\n";
                } else {
                    t.Bill();
                    cout << "Your Ticket is printed, you can collect it\n";
                    if (getIntInput("Press 1 to display your ticket: ") == 1) {
                        t.dispBill();
                        getIntInput("Press any key to go back: ");
                    }
                }
                break;
            case 4:
                t.dispDatabase();
                getIntInput("Press any key to go back: ");
                break;
            case 5:
                t.deletedata();
                getIntInput("Press any key to go back: ");
                break;
            case 6:
                f.displayFoodDatabase();
                getIntInput("Press any key to go back: ");
                break;
            case 7:
                f.orderFoodItems(getIntInput("Enter your PNR number: "));
                getIntInput("Press any key to go back: ");
                break;
            case 8:
                f.viewFoodOrder(getIntInput("Enter your PNR number: "));
                getIntInput("Press any key to go back: ");
                break;
            case 9:
                cout << "\nReturning to Main Menu...\n";
                return; 
            case 10:
                cout << "\nThank you for using RAILSYNC Railways.\n";
                exit(0);
            default:
                cout << "Invalid Input, Please try again!\n";
                break;
        }
    }
}


void Management::firstPage() {
    int choice;
    string eid, pass;

    while (true) {
        cout << "\n____________________________________________________________________________________\n";
        cout << "_______________________________WELCOME TO RAILSYNC____________________________________\n";
        cout << "\n\t\t\t\t| 1. Admin Mode |\n";
        cout << "\t\t\t\t| 2. User Mode  |\n";
        cout << "\t\t\t\t| 3. Exit       |\n";

        choice = getIntInput("Select: ");
        if (choice == 3) {
            cout << "Thanks for using RAILSYNC !\n";
            exit(0);
        }

        ifstream f("user.txt");
        if (!f.is_open()) {
            cerr << "Error opening user.txt\n";
            exit(1);
        }

        switch (choice) {
            case 1: {
                cout << "\nEnter Admin Username: ";
                cin >> eid;
                cout << "Enter Admin Password: ";
                cin >> pass;

                bool adminFound = false;
                string fileUid, fileHash, fileRole;

                while (f >> fileUid >> fileHash >> fileRole) {
                    if (fileRole == "admin" &&
                        fileUid == eid &&
                        fileHash == to_string(simpleHash(pass))) {
                        adminFound = true;
                        break;
                    }
                }

                if (adminFound) {
                    mainMenu();
                } else {
                    cout << "Invalid admin credentials. Try again.\n";
                }
                break;
            }

            case 2: {
                cout << "\nWELCOME TO USER MODE\n";
                cout << "  1. Create New Account\n";
                cout << "  2. Login using existing account\n";

                int sub = getIntInput("Select: ");
                if (sub == 1) {
                    string uid, upass;
                    cout << "Enter New ID: ";
                    cin >> uid;
                    cout << "Enter Password: ";
                    cin >> upass;

                    ifstream chk("user.txt");
                    string eu, eh, er;
                    bool exists = false;
                    while (chk >> eu >> eh >> er) {
                        if (eu == uid) { exists = true; break; }
                    }
                    if (exists) {
                        cout << "User ID already exists!\n";
                        break;
                    }

                    ofstream app("user.txt", ios::app);
                    app << uid << " " << simpleHash(upass) << " user\n";
                    cout << "User ID created successfully!\n";
                }
                else if (sub == 2) {
                    string uid, upass;
                    cout << "Enter ID: ";
                    cin >> uid;
                    cout << "Enter Password: ";
                    cin >> upass;

                    ifstream uf("user.txt");
                    string cuid, role;
                    unsigned long cupass;
                    bool found = false;

                    while (uf >> cuid >> cupass >> role) {
                        if (role != "user") continue;
                        if (cuid == uid && simpleHash(upass) == cupass) {
                            loggedInUserId = uid;
                            found = true;
                            mainMenu2();
                            break;
                        }
                    }

                    if (!found) {
                        cout << "Incorrect login details.\n";
                    }
                }
                else {
                    cout << "Invalid choice.\n";
                }
                break;
            }

            default:
                cout << "Invalid choice, try again.\n";
                break;
        }
    }
}

Management::Management() {
    firstPage();
}
