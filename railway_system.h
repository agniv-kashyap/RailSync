

#ifndef RAILWAY_SYSTEM_H
#define RAILWAY_SYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

// Global variable
extern std::string loggedInUserId;

// Station class declaration
class Station {
public:
    std::string code;
    std::string name;
    Station(std::string c = "", std::string n = "");
};

// Train class declaration
class Train {
public:
    std::string number;
    std::string name;
    std::string startStation;
    std::string endStation;
    std::string departure;
    std::string arrival;
    int hours;
    int ac3Price;
    int ac2Price;
    int ac1Price;
    int ac3Seats;
    int ac2Seats;
    int ac1Seats;
    Train(std::string num = "", std::string nm = "", std::string start = "", std::string end = "",
          std::string dep = "", std::string arr = "", int hrs = 0, int ac3 = 0,
          int ac2 = 0, int ac1 = 0, int ac3s = 0, int ac2s = 0, int ac1s = 0);
};

// Details class declaration
class Details {
public:
    static std::string name[6], gender[6];
    static int age[6], cId[6];
    static std::string bp[6];
    static std::string boardingStation, destinationStation;
    static int pnr, n;
    char arr[100];
    void information();
    bool pnrExists(int pnr);
};

// Ticket class declaration
class ticket : public Details {
public:
    static float charges;
    static int selectedTrainIndex;
    static std::string selectedClass;
    void Bill();
    void dispBill();
    void dispDatabase();
    void deletedata();
};

// orderFood class declaration

class orderFood {
    private:
        std::map<int, std::pair<std::string, float>> menu;
    
        // Helper function to check if PNR exists in tickets.txt
        bool pnrExists(int pnr);
    
    public:
        orderFood(); 
        void displayFoodDatabase(); 
        void orderFoodItems(int pnr); 
        void viewFoodOrder(int pnr);
    };

// registration class declaration
class registration {
public:
    static int choice;
    int choice1;
    std::string selectBoardingStation();
    void trains();  // First option: Train Booking
    void addCustomerDetails();  // Second option: Add Customer Details
};

// Management class declaration
class Management {
private:
    void mainMenu();
    void mainMenu2();
    void firstPage();
public:
    Management();
};

// Function declarations
void initializeSeatAvailability(const std::vector<Train>&);
void loadDatabases(std::vector<Station>&, std::vector<Train>&);
int displayStationsMenu(const std::vector<Station>&, bool = true, const std::string& = "");
std::string selectDestinationStation(const std::string&, const std::vector<Station>&);




#endif // RAILWAY_SYSTEM_H
