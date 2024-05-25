#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <limits> // For numeric_limits
#include <cctype> // for character handling functions
#include <vector>
#include <ctime>

using namespace std;

class Order
{
private:
    int order_id;
    string email;
    int year, month, day;
    string time;
    int dishID;
    double price, cost;
    bool paid;

public:
    // Constructor
    Order(int orderid, const string& email, int year, int month, int day, const string& time,
        int dishID, double price, double cost, bool paid)
        : email(email), year(year), month(month), day(day), time(time),
        dishID(dishID), price(price), cost(cost), paid(paid), order_id(orderid) {}

    // Accessor methods
    int getID() const { return order_id; }
    string getEmail() const { return email; }
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }
    string getTime() const { return time; }
    int getDishID() const { return dishID; }
    double getPrice() const { return price; }
    double getCost() const { return cost; }
    bool isPaid() const { return paid; }

    // Mutator methods
    void setEmail(const string& val) { email = val; }
    void setYear(int val) { year = val; }
    void setMonth(int val) { month = val; }
    void setDay(int val) { day = val; }
    void setTime(const string& val) { time = val; }
    void setDishID(int val) { dishID = val; }
    void setPrice(double val) { price = val; }
    void setCost(double val) { cost = val; }
    void setPaid(bool val) { paid = val; }

    // Display order details
    void displayOrderDetails() const
    {
        cout << "Order Details:" << endl;
        cout << "Order ID: " << order_id << endl;
        cout << "Email: " << email << endl;
        cout << "Date: " << year << "-" << month << "-" << day << endl;
        cout << "Time: " << time << endl;
        cout << "Dish ID: " << dishID << endl;
        cout << "Price: " << price << endl;
        cout << "Cost: " << cost << endl;
        cout << "Paid: " << (paid ? "Yes" : "No") << endl;
        cout << endl;
    }

    // Write order information to file
    void writeToFile(const string& filename) const
    {
        ofstream file(filename, ios::app);
        if (!file.is_open())
        {
            cerr << "Unable to open the file for writing." << endl;
            return;
        }

        file << "Email: " << email << "\n";
        file << "ID: " << order_id << "\n";
        file << "Year: " << year << "\n";
        file << "Month: " << month << "\n";
        file << "Day: " << day << "\n";
        file << "Time: " << time << "\n";
        file << "Dish ID: " << dishID << "\n";
        file << "Price: " << price << "\n";
        file << "Cost: " << cost << "\n";
        file << "Paid: " << (paid ? "1" : "0") << "\n\n";

        file.close();
    }

    // Delete order from file based on specified ID
    void deleteOrderFromFile(const string& filename) const
    {
        vector<string> lines; // Used to store file contents
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Unable to open the file for reading." << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            if (line.find("ID: " + to_string(order_id) + ";") == string::npos)
            {
                // If the current line does not contain information about the specified ID, save it to the container
                lines.push_back(line);
            }
            else
            {
                // If the current line contains information about the specified ID, skip the complete information of the current order
                for (int i = 0; i < 8; ++i)
                {
                    getline(file, line);
                }
            }
        }
        file.close();

        // Write the modified content back to the file
        ofstream outfile(filename);
        if (!outfile.is_open())
        {
            cerr << "Unable to open the file for writing." << endl;
            return;
        }
        for (const string& l : lines)
        {
            outfile << l << "\n";
        }
        outfile.close();
    }
};


class Dish
{
private:
    string name;
    int id;
    double price;
    string ingredients;
    double cost;

public:
    Dish(string dish_name, int dish_id, double dish_price, string dish_ingredients, double dish_cost)
        : name(dish_name), id(dish_id), price(dish_price), ingredients(dish_ingredients), cost(dish_cost) {}
    double getProfit() const
    {
        return price - cost; // Selling price minus cost
    }

    void getDishInfo() const
    {
        cout << "Dish Name: " << name << endl;
        cout << "Dish ID: " << id << endl;
        cout << "Price: " << price << endl;
        cout << "Ingredients: " << ingredients << endl;
        cout << "Cost: " << cost << endl;
    }

    void writeToFile(ofstream& file) const
    {
        file << "Dish Name: " << name << endl;
        file << "Dish ID: " << id << endl;
        file << "Price: " << price << endl;
        file << "Ingredients: " << ingredients << endl;
        file << "Cost: " << cost << endl;
        file << endl;
    }

    string getName() const
    {
        return name;
    }

    int getId() const
    {
        return id;
    }

    double getPrice() const
    {
        return price;
    }

    double getCost() const
    {
        return cost;
    }
};

class User
{
protected:
    string username;
    string email;
    string userinfo_filename = "userinfo.txt";
    int age;

public:
    User(string uname, string mail, int user_age)
    {
        username = uname;
        email = mail;
        age = user_age;
    }
    virtual ~User() {}

    virtual void getUserInfo()
    {
        cout << "Username: " << username << endl;
        cout << "Email: " << email << endl;
        cout << "Age: " << age << endl;
    }

    virtual void editUserInfo()
    {
        cout << "Enter new username: ";
        cin >> username;
        cout << "Enter new email: ";
        cin >> email;
        cout << "Enter new age: ";
        while (true) {
            cin >> age;
            if (!cin.fail()) {
                break;
            }
            else {
                cin.clear(); // Clear the error flag set by the previous extraction
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
                cout << "Please enter a valid number : ";
            }
        }
    }

    virtual string getPassword() const
    {
        return "";
    }

    // Writes user information to a file
    virtual void writeToFile(ofstream& file) = 0;

    // Check whether user information exists in the file
    bool userInfoExists(const string& filename)
    {
        ifstream infile(filename);
        if (!infile)
        {
            cerr << "Error opening file." << endl;
            return false;
        }

        string line;
        string usernameToFind = "Email: " + email;
        while (getline(infile, line))
        {
            if (line.find(usernameToFind) != string::npos)
            {
                infile.close();
                return true; // The user information already exists
            }
        }

        infile.close();
        return false; // The user information does not exist
    }
    vector<Dish> readDishesFromFile(const string& filename) const
    {
        vector<Dish> dishes;
        ifstream infile(filename);
        if (!infile)
        {
            cerr << "Error opening file." << endl;
            return dishes;
        }

        string line;
        string dish_name;
        int dish_id;
        double dish_price;
        string dish_ingredients;
        double dish_cost;
        while (getline(infile, line))
        {
            if (line.find("Dish Name: ") != string::npos)
            {
                dish_name = line.substr(11);
                getline(infile, line);
                dish_id = stoi(line.substr(9));
                getline(infile, line);
                dish_price = stod(line.substr(7));
                getline(infile, line);
                dish_ingredients = line.substr(14);
                getline(infile, line);
                dish_cost = stod(line.substr(6));

                Dish dish(dish_name, dish_id, dish_price, dish_ingredients, dish_cost);
                dishes.push_back(dish);
            }
        }

        infile.close();
        return dishes;
    }

    void print_dish_menu(const string& filename) const
    {
        vector<Dish> dishes = readDishesFromFile(filename);
        cout << "Dish Menu:" << endl;
        for (const auto& dish : dishes)
        {
            dish.getDishInfo();
            cout << endl;
        }
    }
};

class Manager : public User
{
private:
    string password;
    double totalProfit = 0.0;
    double cost;
    double price;

public:
    Manager(string uname, string pwd, string mail, int user_age) : User(uname, mail, user_age), password(pwd) {}


    void getUserInfo() override
    {
        cout << "Manager Information:" << endl;
        User::getUserInfo();
    }

    void editUserInfo() override
    {
        cout << "Editing Manager Information:" << endl;
        User::editUserInfo();
    }

    string getPassword() const override
    {
        return password;
    }
    void writeToFile(ofstream& userinfo_filename) override
    {
        userinfo_filename << "Usertype: " << "Manager" << endl;
        userinfo_filename << "Username: " << username << endl;
        userinfo_filename << "Email: " << email << endl;
        userinfo_filename << "Age: " << age << endl;
        userinfo_filename << "Password: " << password << endl;
        userinfo_filename << endl;
    }
    void viewAllOrders()
    {
        ifstream ordersFile("orders.txt");
        if (!ordersFile.is_open())
        {
            cerr << "Error opening orders file!" << endl;
            return;
        }

        string line;
        cout << "All Orders:" << endl;
        while (getline(ordersFile, line))
        {
            cout << line << endl;
        }

        ordersFile.close();
    }

    void viewChefInfo()
    {
        ifstream file("userinfo.txt");
        if (!file.is_open())
        {
            cerr << "Error opening users file!" << endl;
            return;
        }

        string line;
        bool isChef = false;
        cout << "Chef Information:" << endl;
        while (getline(file, line))
        {
            if (line.find("Usertype: Chef") != string::npos)
            {
                isChef = true;
                cout << line << endl;
            }
            else if (line.find("Usertype: ") != string::npos)
            {
                isChef = false;
            }
            else if (isChef)
            {
                cout << line << endl;
            }
        }

        file.close();
    }

    void viewCustomerInfo()
    {
        ifstream file("userinfo.txt");
        if (!file.is_open())
        {
            cerr << "Error opening users file!" << endl;
            return;
        }

        string line;
        bool isCustomer = false;
        cout << "Customer Information:" << endl;
        while (getline(file, line))
        {
            if (line.find("Usertype: Customer") != string::npos)
            {
                isCustomer = true;
                cout << line << endl;
            }
            else if (line.find("Usertype: ") != string::npos)
            {
                isCustomer = false;
            }
            else if (isCustomer)
            {
                cout << line << endl;
            }
        }

        file.close();
    }

    void modifyChef(string username)
    {
        vector<string> users;
        ifstream file("userinfo.txt");
        if (!file.is_open())
        {
            cerr << "Error opening users file!" << endl;
            return;
        }

        string line;
        bool isChef = false;
        bool found = false;
        while (getline(file, line))
        {
            if (line.find("Usertype: Chef") != string::npos)
            {
                isChef = true;
                users.push_back(line);
            }
            else if (line.find("Usertype: ") != string::npos)
            {
                isChef = false;
                users.push_back(line);
            }
            else if (isChef && line.find("Username: " + username) != string::npos)
            {
                found = true;
                users.push_back(line);
                cout << "Modifying chef with ID: " << username << endl;
                cout << "Enter new chef information:" << endl;
                string email1, age1;

                cout << "Enter new email: ";
                cin >> email1;
                cout << "Enter new age: ";
                while (true) {
                    cin >> age1;
                    if (!cin.fail()) {
                        break;
                    }
                    else {
                        cin.clear(); // Clear the error flag set by the previous extraction
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
                        cout << "Please enter a valid number : ";
                    }
                }
                cin.ignore();

                users.push_back("Email: " + email1);
                users.push_back("Age: " + age1);

                for (int i = 0; i < 3; ++i)
                {
                    getline(file, line);
                }
            }
            else
            {
                users.push_back(line);
            }
        }
        file.close();

        if (found)
        {
            ofstream outFile("userinfo.txt");
            for (const auto& user : users)
            {
                outFile << user << endl;
            }
            outFile.close();
            cout << "Chef information modified successfully." << endl;
        }
        else
        {
            cout << "Chef username not found." << endl;
        }
    }

    void modifyCustomer(string username)
    {

        vector<string> users;
        ifstream file("userinfo.txt");
        if (!file.is_open())
        {
            cerr << "Error opening users file!" << endl;
            return;
        }

        string line;
        bool isCustomer = false;
        bool found = false;
        while (getline(file, line))
        {
            if (line.find("Usertype: Customer") != string::npos)
            {
                isCustomer = true;
                users.push_back(line);
            }
            else if (line.find("Usertype: ") != string::npos)
            {
                isCustomer = false;
                users.push_back(line);
            }
            else if (isCustomer && line.find("Username: " + username) != string::npos)
            {
                found = true;
                users.push_back(line);
                cout << "Modifying chef with ID: " << username << endl;
                cout << "Enter new chef information:" << endl;
                string email1, age1;

                cout << "Enter new email: ";
                cin >> email1;
                cout << "Enter new age: ";
                while (true) {
                    cin >> age1;
                    if (!cin.fail()) {
                        break;
                    }
                    else {
                        cin.clear(); // Clear the error flag set by the previous extraction
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
                        cout << "Please enter a valid number : ";
                    }
                }
                cin.ignore();

                users.push_back("Email: " + email1);
                users.push_back("Age: " + age1);

                for (int i = 0; i < 3; ++i)
                {
                    getline(file, line);
                }
            }
            else
            {
                users.push_back(line);
            }
        }
        file.close();

        if (found)
        {
            ofstream outFile("userinfo.txt");
            for (const auto& user : users)
            {
                outFile << user << endl;
            }
            outFile.close();
            cout << "Chef information modified successfully." << endl;
        }
        else
        {
            cout << "Customer ID not found." << endl;
        }
    }


    void modifyDish(int dishId)
    {
        vector<Dish> dishes = readDishesFromFile("Dish.txt");
        bool found = false;
        ofstream tempFile("temp.txt");

        for (auto& dish : dishes)
        {
            if (dish.getId() == dishId)
            {
                found = true;
                cout << "Modifying dish with ID: " << dishId << endl;
                string newName, newIngredients;
                double newPrice, newCost;

                cout << "Enter new name: ";
                cin >> newName;
                cout << "Enter new price: ";
                while (true) {
                    cin >> newPrice;
                    if (!cin.fail()) {
                        break;
                    }
                    else {
                        cin.clear();// Clear the error flag set by the previous extraction  
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
                        cout << "Incorrect input, please restart the system!" << endl; ;
                        exit(EXIT_FAILURE); //close program
                    }
                }
                cout << "Enter new ingredients: ";

                cout << "Enter new ingredients: ";
                cin.ignore();
                getline(cin, newIngredients);
                cout << "Enter new cost: ";
                cin >> newCost;

                Dish updatedDish(newName, dishId, newPrice, newIngredients, newCost);
                updatedDish.writeToFile(tempFile);
                cout << "Dish information modified successfully." << endl;
            }
            else
            {
                dish.writeToFile(tempFile);
            }
        }

        tempFile.close();
        if (found)
        {
            remove("Dish.txt");
            rename("temp.txt", "Dish.txt");
        }
        else
        {
            cout << "Dish ID not found." << endl;
            remove("temp.txt");
        }
    }
    void addNewDish()
    {
        cout << "\nAdding New Dish:" << endl;
        string dish_name, dish_ingredients;
        double dish_price, dish_cost;
        int newDishId;

        cout << "Enter Dish ID for the new dish: ";
        while (true) {
            cin >> newDishId;
            if (!cin.fail()) {
                break;
            }
            else {
                cout << "Incorrect input, please restart the system!" << endl;
                exit(EXIT_FAILURE);

            }
        }
        cout << "Enter Dish Name: ";
        cin >> dish_name;

        cout << "Enter Price: ";

        while (true) {
            cin >> dish_price;
            if (!cin.fail()) {
                break;
            }
            else {
                cout << "Incorrect input, please restart the system!" << endl;
                exit(EXIT_FAILURE);
            }
        }
        cout << "Enter Ingredients: ";
        cout << "Enter Ingredients: ";
        cin.ignore(); // Ignore previous line breaks
        getline(cin, dish_ingredients);
        cout << "Enter Cost: ";

        while (true) {
            cin >> dish_cost;
            if (!cin.fail()) {
                break;
            }
            else {
                cout << "Incorrect input, please restart the system!" << endl;
                exit(EXIT_FAILURE);

            }
        }

        vector<Dish> dishes = readDishesFromFile("Dish.txt");
        for (const auto& dish : dishes)
        {
            if (dish.getId() == newDishId)
            {
                cout << "Dish with the same ID already exists. Not adding." << endl;
                return;
            }
        }

        ofstream dishFile("Dish.txt", ios::app); // Opens the file for append writing
        if (!dishFile.is_open())
        {
            cout << "Error opening file!" << endl;
            return;
        }

        Dish newDish(dish_name, newDishId, dish_price, dish_ingredients, dish_cost);
        newDish.writeToFile(dishFile); // Write the menu information to the file
        cout << "New Dish Added Successfully!" << endl;

        dishFile.close();
    }

    vector<Dish> readDishesFromFile(const string& filename) const
    {
        vector<Dish> dishes;
        ifstream infile(filename);
        if (!infile)
        {
            cerr << "Error opening file." << endl;
            return dishes;
        }

        string line;
        string dish_name;
        int dish_id;
        double dish_price;
        string dish_ingredients;
        double dish_cost;
        while (getline(infile, line))
        {
            if (line.find("Dish Name: ") != string::npos)
            {
                dish_name = line.substr(11);
                getline(infile, line);
                dish_id = stoi(line.substr(9));
                getline(infile, line);
                dish_price = stod(line.substr(7));
                getline(infile, line);
                dish_ingredients = line.substr(14);
                getline(infile, line);
                dish_cost = stod(line.substr(6));

                Dish dish(dish_name, dish_id, dish_price, dish_ingredients, dish_cost);
                dishes.push_back(dish);
            }
        }

        infile.close();
        return dishes;
    }

    void deleteDish(int dish_id)
    {
        vector<Dish> dishes = readDishesFromFile("Dish.txt");
        bool found = false;
        ofstream tempFile("temp.txt");
        for (const auto& dish : dishes)
        {
            if (dish.getId() == dish_id)
            {
                found = true;
                cout << "Dish deleted successfully:" << endl;
            }
            else
            {
                dish.writeToFile(tempFile); // Writes non-deleted dishes to a temporary file
            }
        }
        tempFile.close();

        if (!found)
        {
            cout << "Dish not found!" << endl;
            remove("temp.txt"); // Delete temporary files
            return;
        }

        // Delete the original file and rename the temporary file
        remove("Dish.txt");
        rename("temp.txt", "Dish.txt");
    }

    void calculateRevenue()
    {
        time_t currentTime = time(nullptr);      // Get current time
        tm* localTime = localtime(&currentTime); // Convert to local time

        double totalProfit = 0.0;

        ifstream file("orders.txt");
        if (!file.is_open())         // Not open detection
        {
            cerr << "Unable to open the file: " << endl;
            return;
        }

        string line, key, value; 
        string timeString;       
        double price, cost;      
        int year, month, day;    

        while (getline(file, line)) // Loop reads the file until the end of the file
        {
            if (line.empty())
                continue;

            size_t colonPos = line.find(':');
            if (colonPos != string::npos)
            {
                key = line.substr(0, colonPos);
                value = line.substr(colonPos + 1);
                if (!value.empty() && value.front() == ' ')
                {
                    value.erase(0, 1); // Remove leading Spaces
                }

                if (key == "Year")
                    year = stoi(value);
                else if (key == "Month")
                    month = stoi(value);
                else if (key == "Day")
                    day = stoi(value);
                else if (key == "Time")
                    timeString = value;
                else if (key == "Price")
                    price = stod(value);
                else if (key == "Cost")
                    cost = stod(value);

                if (file.peek() == '\n' || file.peek() == EOF)
                {
                    tm orderTime = { 0 }; // Initializes a time type
                    orderTime.tm_year = year - 1900;
                    orderTime.tm_mon = month - 1;
                    orderTime.tm_mday = day;

                    int hour, minute, second;
                    sscanf(timeString.c_str(), "%d:%d:%d", &hour, &minute, &second);
                    orderTime.tm_hour = hour;
                    orderTime.tm_min = minute;
                    orderTime.tm_sec = second;

                    time_t orderTimestamp = mktime(&orderTime); // Convert to a timestamp after converting to a suitable format
                    if (difftime(currentTime, orderTimestamp) < 86400)
                    {
                        totalProfit += (price - cost);
                    }
                }
            }
        }
        cout << "Profit in one day : $" << totalProfit << endl;
        file.close();
    }

    void sellDish(int dishId, int quantitySold)
    {
        vector<Dish> dishes = readDishesFromFile("Dish.txt");
        bool found = false;
        for (const auto& dish : dishes)
        {
            if (dish.getId() == dishId)
            {
                double profit = dish.getProfit() * quantitySold;
                totalProfit += profit; // Cumulative profit
                cout << "Sold " << quantitySold << " units of " << dish.getName() << ". Profit: $" << profit << endl;
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Dish ID " << dishId << " not found." << endl;
        }
    }
};
void ChefprintMenu()
{
    cout << "\nMenu:" << endl;
    cout << "1. Edit User Info" << endl;
    cout << "2. Delete Dish" << endl;
    cout << "3. Add New Dish" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}
class Chef : public User
{
private:
    string password;

public:
    Chef(string uname, string pwd, string mail, int user_age) : User(uname, mail, user_age), password(pwd) {}

    void getUserInfo() override
    {
        cout << "Chef Information:" << endl;
        User::getUserInfo();
    }

    void editUserInfo() override
    {
        cout << "Editing Chef Information:" << endl;
        User::editUserInfo();
    }

    string getPassword() const override
    {
        return password;
    }
    void writeToFile(ofstream& userinfo_filename)
    {
        userinfo_filename << "Usertype: " << "Chef" << endl;
        userinfo_filename << "Username: " << username << endl;
        userinfo_filename << "Email: " << email << endl;
        userinfo_filename << "Age: " << age << endl;
        userinfo_filename << "Password: " << password << endl;
        userinfo_filename << endl;
    }
    void addNewDish(int dish_id)
    {
        cout << "\nAdding New Dish:" << endl;
        string dish_name, dish_ingredients;
        double dish_price, dish_cost;
        cout << "warning,if you put incorrect data here, the programme will be quited " << endl;
        cout << "Enter Dish Name: ";
        cin >> dish_name;
        cout << "Enter Price: ";

        while (true) {
            cin >> dish_price;
            if (!cin.fail()) {
                break;
            }
            else {
                cout << "Incorrect input, please restart the system!" << endl;
                exit(EXIT_FAILURE);
            }
        }
        cout << "Enter Ingredients: ";
        cin.ignore();
        getline(cin, dish_ingredients);
        cout << "Enter Cost: ";

        while (true) {
            cin >> dish_cost;
            if (!cin.fail()) {
                break;
            }
            else {
                cout << "Incorrect input, please restart the system!" << endl;
                exit(EXIT_FAILURE);
            }
        }
        vector<Dish> dishes = readDishesFromFile("Dish.txt");
        for (const auto& dish : dishes)
        {
            if (dish.getId() == dish_id)
            {
                cout << "Dish with the same ID already exists. Not adding." << endl;
                return;
            }
        }

        ofstream dishFile("Dish.txt", ios::app);
        if (!dishFile.is_open())
        {
            cout << "Error opening file!" << endl;
            return;
        }

        Dish newDish(dish_name, dish_id, dish_price, dish_ingredients, dish_cost);
        newDish.writeToFile(dishFile);
        cout << "New Dish Added Successfully!" << endl;

        dishFile.close();
    }

    void deleteDish(int dish_id)
    {
        vector<Dish> dishes = readDishesFromFile("Dish.txt");
        bool found = false;
        ofstream tempFile("temp.txt");
        for (const auto& dish : dishes)
        {
            if (dish.getId() == dish_id)
            {
                found = true;
                cout << "Dish deleted successfully:" << endl;
            }
            else
            {
                dish.writeToFile(tempFile);
            }
        }
        tempFile.close();

        if (!found)
        {
            cout << "Dish not found!" << endl;
            remove("temp.txt");
            return;
        }

        remove("Dish.txt");
        rename("temp.txt", "Dish.txt");
    }
};

void updateMaxOrderIDFile(int newMaxOrderID)
{
    ofstream file("MaxOrderID.txt");
    if (file.is_open())
    {
        file << newMaxOrderID; // Write the new maximum OrderID
        file.close();
    }
    else
    {
        cerr << "Unable to open MaxOrderID.txt for writing." << endl;
    }
}

int getMaxOrderID()
{
    ifstream file("MaxOrderID.txt");
    int maxOrderID = 0;
    if (file.is_open())
    {
        file >> maxOrderID;
        file.close();
    }
    return maxOrderID;
}

class Customer : public User
{
private:
    vector<Order> orders;

    // Load orders from file
    void loadOrdersFromFile(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Unable to open the file: " << filename << endl;
            ofstream outfile(filename);
        }

        string line, key, value, email, time_;
        int orderID, year, month, day, dishID;
        double price, cost;
        bool paid;

        while (getline(file, line))
        {
            if (line.empty())
                continue; // Skip empty lines

            size_t colonPos = line.find(':');
            if (colonPos != string::npos)
            {
                key = line.substr(0, colonPos);
                value = line.substr(colonPos + 1);
                if (!value.empty() && value.front() == ' ')
                {
                    value.erase(0, 1); // Remove leading space
                }
                if (!value.empty() && value.front() == ' ')
                    value.erase(0, 1); // Remove leading space

                if (key == "Email")
                    email = value;
                else if (key == "ID")
                    orderID = stoi(value);
                else if (key == "Year")
                    year = stoi(value);
                else if (key == "Month")
                    month = stoi(value);
                else if (key == "Day")
                    day = stoi(value);
                else if (key == "Time")
                    time_ = value;
                else if (key == "Dish ID")
                    dishID = stoi(value);
                else if (key == "Price")
                    price = stod(value);
                else if (key == "Cost")
                    cost = stod(value);
                else if (key == "Paid")
                    paid = stoi(value);

                if (file.peek() == '\n' || file.peek() == EOF)
                { // Check for end of order
                    if (this->email == email)
                    { // Only load orders for the specified customer
                        orders.push_back(Order(orderID, email, year, month, day, time_, dishID, price, cost, paid));
                    }
                }
            }
        }
    }

public:
    Customer(string uname, string mail, int user_age) : User(uname, mail, user_age)
    {
        if (!userInfoExists(userinfo_filename))
        {
            ofstream ofile(userinfo_filename, ios_base::app);
            if (!ofile)
            {
                cerr << "Error opening file." << endl;
            }
            else
            {
                writeToFile(ofile);
            }
        }
        loadOrdersFromFile("orders.txt");
    }

    // Get user information
    void getUserInfo() override
    {
        cout << "Customer Information:" << endl;
        User::getUserInfo();
    }

    // Edit user information
    void editUserInfo() override
    {
        cout << "Editing Customer Information:" << endl;
        User::editUserInfo();
    }

    // Write user information to file
    void writeToFile(ofstream& userinfo_filename) override
    {
        userinfo_filename << "Usertype: " << "Customer" << endl;
        userinfo_filename << "Username: " << username << endl;
        userinfo_filename << "Email: " << email << endl;
        userinfo_filename << "Age: " << age << endl;
        userinfo_filename << endl;
    }

    // Order a dish
    void orderdish(const string& filename, int dish_ID)
    {
        vector<Dish> dishes = readDishesFromFile(filename);
        for (const Dish& dish : dishes)
        {
            if (dish.getId() == dish_ID)
            {
                time_t curtime = time(nullptr);
                tm* nowtime = localtime(&curtime);
                if (nowtime == nullptr)
                {
                    cerr << "Local time could not be retrieved." << endl;
                    return;
                }
                int currentMaxOrderID = getMaxOrderID();
                int newOrderID = currentMaxOrderID + 1;
                orders.push_back(Order(newOrderID, email, 1900 + nowtime->tm_year, 1 + nowtime->tm_mon, nowtime->tm_mday, to_string(nowtime->tm_hour) + ':' + to_string(nowtime->tm_min) + ':' + to_string(nowtime->tm_sec), dish_ID, dish.getPrice(), dish.getCost(), false));
                updateMaxOrderIDFile(newOrderID);
                for (const Order& order : orders)
                {
                    order.deleteOrderFromFile("orders.txt");
                    order.writeToFile("orders.txt");
                }
                cout << "Dish ordered successfully." << endl;
                return;
            }
        }
        cout << "Dish not found" << endl;
    }

    // Show orders
    void showOrders(bool showOnlyNotPaid)
    {
        cout << "Displaying " << (showOnlyNotPaid ? "unpaid" : "all") << " orders:" << endl;
        for (const Order& order : orders)
        {
            if (showOnlyNotPaid && order.isPaid())
            {
                continue; // Skip paid orders if only unpaid orders are to be displayed
            }
            order.displayOrderDetails();
            cout << endl; // Add a blank line between orders for readability
        }
    }

    // Process payment
    void pay()
    {
        double totalDue = 0;
        vector<Order*> unpaidOrders;

        // Calculate total due for unpaid orders
        for (Order& order : orders)
        {
            if (!order.isPaid())
            {
                totalDue += order.getPrice();
                unpaidOrders.push_back(&order);
            }
        }

        cout << "Total amount due: $" << totalDue << endl;
        if (totalDue == 0)
        {
            cout << "No outstanding payments." << endl;
            return;
        }

        cout << "Do you want to proceed with payment? (Y/N): ";
        char confirmation;
        cin >> confirmation;

        if (confirmation == 'Y' || confirmation == 'y')
        {
            for (Order* order : unpaidOrders)
            {
                order->deleteOrderFromFile("orders.txt");
                order->setPaid(true);
                order->writeToFile("orders.txt");
            }
            cout << "Payment successful." << endl;
        }
        else
        {
            cout << "Payment cancelled." << endl;
        }
    }
};


void printCustomerMenu()
{
    cout << "\nMenu:" << endl;
    cout << "1. show dish menu" << endl;
    cout << "2. order" << endl;
    cout << "3. display orders not paid" << endl;
    cout << "4. display all orders" << endl;
    cout << "5. pay" << endl; // add new choice
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
}

void customerMenu(User* user)
{
    int menuChoice;
    do
    {
        printCustomerMenu();
        cin >> menuChoice;
        switch (menuChoice)
        {
        case 1:
        {
            user->print_dish_menu("Dish.txt");
            break;
        }
        case 2:
        {
            int dishId;
            cout << "Enter Dish ID to order: ";
            cin >> dishId;
            dynamic_cast<Customer*>(user)->orderdish("Dish.txt", dishId);
            break;
        }
        case 3:
        {
            dynamic_cast<Customer*>(user)->showOrders(1);
            break;
        }
        case 4:
        {
            dynamic_cast<Customer*>(user)->showOrders(0);
            break;
        }
        case 5:
        {
            dynamic_cast<Customer*>(user)->pay();
            break;
        }
        case 6:
        {
            cout << "Exiting Customer UI..." << endl;
            break;
        }
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (menuChoice != 6);
}

void customer_logging(User* user)
{
    string input_name;
    string customor_email;
    int age = 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
    while (true)
    {
        cout << "Please enter your name: ";
        getline(cin, input_name);

        // Check if the name is empty
        if (input_name.empty())
        {
            cout << "Please enter a valid name.\n";
            continue; // Prompt the user to enter a name again
        }

        // Check if the name contains only alphabets and spaces
        bool valid = true;
        for (char c : input_name)
        {
            if (!isalpha(c) && !isspace(c))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            cout << "Name should contain only alphabets and spaces.\n";
            continue; // Prompt the user to enter a name again
        }

        // If the name is valid, break the loop
        break;
    }
    cout << "Enter Your Email: ";
    while (!(cin >> customor_email) || (customor_email.find('@') == string::npos))
    {
        cin.clear(); // Clear the error flag
        cerr << "Invalid input. Please enter a right email." << endl;
        cout << "Enter Your Email: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
    }
    cout << "Enter Your age: ";
    while (!(cin >> age) || age <= 0)
    {
        cin.clear(); // Clear the error flag
        cerr << "Invalid input. Please enter a right age." << endl;
        cout << "Enter Your age: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
    }
    user = new Customer(input_name, customor_email, age); // from here start
    customerMenu(user);
    system("cls");
}

void printChefMenu()
{
    cout << "\nMenu:" << endl;
    cout << "1. Edit User Info" << endl;
    cout << "2. Delete Dish" << endl;
    cout << "3. Add New Dish" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

void ChefMenu(User* user)
{
    int menuChoice;
    do
    {
        printChefMenu();
        cin >> menuChoice;
        switch (menuChoice)
        {
        case 1:
            user->editUserInfo(); // Modifying User Information
            cout << "Updated User Info:" << endl;
            user->getUserInfo();
            break;
        case 2:
        {
            int dishId;
            cout << "Enter Dish ID to delete: ";
            while (true) {
                cin >> dishId;
                if (!cin.fail()) {
                    break;
                }
                else {
                    exit(EXIT_FAILURE);
                }
            }
            dynamic_cast<Chef*>(user)->deleteDish(dishId);
            break;
        }
        case 3:
            int newDishId;
            cout << "Enter Dish ID for the new dish: ";
            cin >> newDishId;
            dynamic_cast<Chef*>(user)->addNewDish(newDishId);
            break;
        case 4:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (menuChoice != 4);
}

void ManagerPrintMenu()
{
    cout << "\nManager Menu:" << endl;
    cout << "1. View All Orders" << endl;
    cout << "2. View Chef Info" << endl;
    cout << "3. View Customer Info" << endl;
    cout << "4. Modify Chef" << endl;
    cout << "5. Delete Dish" << endl;
    cout << "6. Add dish" << endl;
    cout << "7. Calculate Revenue" << endl;
    cout << "8. Sell Dish" << endl;
    cout << "9. Exit" << endl;
    cout << "Enter your choice: ";
}

void ManagerMenu(Manager* manager)
{
    int ManagerMenuChoice;
    do
    {
        ManagerPrintMenu();
        cin >> ManagerMenuChoice;
        switch (ManagerMenuChoice)
        {
        case 1:
            manager->viewAllOrders();
            break;
        case 2:
            manager->viewChefInfo();
            break;
        case 3:
            manager->viewCustomerInfo();
            break;
        case 4:
        {
            string username;
            cout << "Enter Chef uername to modify: ";
            cin >> username;
            manager->modifyChef(username);
            break;
        }
        case 5:
        {
            int dishId;
            cout << "Enter Dish ID to delete: ";
            while (true) {
                cin >> dishId;
                if (!cin.fail()) {
                    break;
                }
                else {
                    cin.clear(); // Clear the error flag set by the previous extraction
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
                    cout << "Please enter a valid number : ";
                }
            }
            manager->deleteDish(dishId);
            break;
        }
        case 6:
        {
            manager->addNewDish();
            break;
        }

        case 7:
            manager->calculateRevenue(); // Invoke the function to calculate revenue
            break;
        case 8:
        {
            int dishId, quantity;
            cout << "Enter Dish ID to sell: ";
            while (true) {
                cin >> dishId;
                if (!cin.fail()) {
                    break;
                }
                else {
                    cin.clear(); // Clear the error flag set by the previous extraction
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
                    cout << "Please enter a valid number : ";
                }
            }
            cout << "Enter quantity sold: ";

            while (true) {
                cin >> quantity;
                if (!cin.fail()) {
                    break;
                }
                else {
                    cin.clear(); // Clear the error flag set by the previous extraction
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
                    cout << "Please enter a valid number : ";
                }
            }
            manager->sellDish(dishId, quantity);
            break;
        }
        case 9:
            cout << "Exiting program..." << endl;
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (ManagerMenuChoice != 9);
}
bool authenticate(User* user)
{
    // Only Manager and Chef require password authentication
    if (dynamic_cast<Manager*>(user) || dynamic_cast<Chef*>(user))
    {
        string correctPassword = user->getPassword();
        string inputPassword;
        int attempts = 0;
        while (attempts < 3)
        {
            cout << "Enter your password: ";
            cin >> inputPassword;
            if (inputPassword == correctPassword)
            {
                return true;
            }
            else
            {
                attempts++;
                cout << "Incorrect password. Please try again." << endl;
            }
        }
        cout << "Too many incorrect attempts. Exiting..." << endl;
        return false;
    }
    return true; // Customer does not require password authentication
}

void chef_logging(User* user)
{
    user = new Chef("chef123", "654321", "chef@example.com", 30);
    if (authenticate(user))
    {
        cout << "Access granted." << endl;
        user->editUserInfo();
        cout << "Your User Info:" << endl;
        user->getUserInfo();

        // Check whether the user information exists
        if (!user->userInfoExists("userinfo.txt"))
        {
            ofstream outFile("userinfo.txt", ios::app);
            if (!outFile)
            {
                cerr << "Error opening file." << endl;
                delete user;
            }
            user->writeToFile(outFile);
            outFile.close();
            cout << "User Info added to file." << endl;
        }
        else
        {
            cout << "User Info already exists in file." << endl;
        }
        ChefMenu(user);
    }
    else
    {
        cout << "Access denied." << endl;
    }
}


void managerloger(User* user)
{
    user = new Manager("manager123", "123456", "manager@example.com", 35);
    if (authenticate(user))
    {
        cout << "Access granted." << endl;
        user->editUserInfo();
        cout << "Your User Info:" << endl;
        user->getUserInfo();

        if (!user->userInfoExists("userinfo.txt"))
        {
            ofstream outFile("userinfo.txt", ios::app);
            if (!outFile)
            {
                cerr << "Error opening file." << endl;
                delete user;
            }
            user->writeToFile(outFile);
            outFile.close();
            cout << "User Info added to file." << endl;
        }
        else
        {
            cout << "User Info already exists in file." << endl;
        }

        Manager* manager = dynamic_cast<Manager*>(user);
        ManagerMenu(manager);

    }
    else
    {
        cout << "Access denied." << endl;
    }
}


int main()
{
    int choice;
    cout << "Select your role:" << endl;
    cout << "1. Manager" << endl;
    cout << "2. Chef" << endl;
    cout << "3. Customer" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    User* user = nullptr;

    switch (choice)
    {
    case 1:
        managerloger(user);
        break;
    case 2:
        chef_logging(user);
        break;
    case 3:
    {
        customer_logging(user);
        break;
    }
    default:
        cout << "Invalid choice. Exiting..." << endl;
        return 0;
    }

    delete user; // free the memory

    return 0;
}
