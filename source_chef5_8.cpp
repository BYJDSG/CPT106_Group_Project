#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Dish {
private:
    string name;
    int id;
    double price;
    string ingredients;
    double cost;

public:
    Dish(string dish_name, int dish_id, double dish_price, string dish_ingredients, double dish_cost)
        : name(dish_name), id(dish_id), price(dish_price), ingredients(dish_ingredients), cost(dish_cost) {}

    void getDishInfo() const {
        cout << "Dish Name: " << name << endl;
        cout << "Dish ID: " << id << endl;
        cout << "Price: " << price << endl;
        cout << "Ingredients: " << ingredients << endl;
        cout << "Cost: " << cost << endl;
    }

    void writeToFile(ofstream& file) const {
        file << "Dish Name: " << name << endl;
        file << "Dish ID: " << id << endl;
        file << "Price: " << price << endl;
        file << "Ingredients: " << ingredients << endl;
        file << "Cost: " << cost << endl;
        file << endl;
    }

    string getName() const {
        return name;
    }

    int getId() const {
        return id;
    }
};

//����
class User {
protected:
    string username;
    string email;
    int age;

public:
    User(string uname, string mail, int user_age) {
        username = uname;
        email = mail;
        age = user_age;
    }
    //����û��������Ϣ
    virtual void getUserInfo() const {
        cout << "Username: " << username << endl;
        cout << "Email: " << email << endl;
        cout << "Age: " << age << endl;
    }
    //�û����������Ϣ
    virtual void editUserInfo() {
        cout << "Enter new username: ";
        cin >> username;
        cout << "Enter new email: ";
        cin >> email;
        cout << "Enter new age: ";
        cin >> age;
    }

    virtual string getPassword() const {
        return ""; // Ĭ�Ϸ��ؿ��ַ���
    }

    // ���û���Ϣд���ļ�
    void writeToFile(ofstream& file) const {
        file << "Username: " << username << endl;
        file << "Email: " << email << endl;
        file << "Age: " << age << endl;
        file << endl;
    }

    // ����û���Ϣ�Ƿ�������ļ���
    bool userInfoExists(const string& filename) const {
        ifstream infile(filename);
        if (!infile) {
            cerr << "Error opening file." << endl;
            return false;
        }

        string line;
        string usernameToFind = "Username: " + username;
        while (getline(infile, line)) {
            if (line.find(usernameToFind) != string::npos) {
                infile.close();
                return true; // �û���Ϣ�Ѵ���
            }
        }

        infile.close();
        return false; // �û���Ϣ������
    }
};

class Manager : public User {
private:
    string password; // �����������

public:
    Manager(string uname, string pwd, string mail, int user_age) : User(uname, mail, user_age), password(pwd) {}

    void getUserInfo() const override {
        cout << "Manager Information:" << endl;
        User::getUserInfo();
    }
    //�޸ĸ�����Ϣ
    void editUserInfo() override {
        cout << "Editing Manager Information:" << endl;
        User::editUserInfo();
    }

    string getPassword() const override {
        return password;
    }
};

class Chef : public User {
private:
    string password; // �����������

public:
    Chef(string uname, string pwd, string mail, int user_age) : User(uname, mail, user_age), password(pwd) {}

    void getUserInfo() const override {
        cout << "Chef Information:" << endl;
        User::getUserInfo();
    }

    void editUserInfo() override {
        cout << "Editing Chef Information:" << endl;
        User::editUserInfo();
    }

    string getPassword() const override {
        return password;
    }

    void addNewDish(int dish_id) {
        cout << "\nAdding New Dish:" << endl;
        string dish_name, dish_ingredients;
        double dish_price, dish_cost;

        cout << "Enter Dish Name: ";
        cin >> dish_name;
        cout << "Enter Price: ";
        cin >> dish_price;
        cout << "Enter Ingredients: ";
        cin.ignore(); // ����֮ǰ�Ļ��з�
        getline(cin, dish_ingredients);
        cout << "Enter Cost: ";
        cin >> dish_cost;

        vector<Dish> dishes = readDishesFromFile("Dish.txt");
        for (const auto& dish : dishes) {
            if (dish.getId() == dish_id) {
                cout << "Dish with the same ID already exists. Not adding." << endl;
                return;
            }
        }

        ofstream dishFile("Dish.txt", ios::app); // ���ļ���׷�ӷ�ʽд��
        if (!dishFile.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }

        Dish newDish(dish_name, dish_id, dish_price, dish_ingredients, dish_cost);
        newDish.writeToFile(dishFile); // ����Ʒ��Ϣд���ļ�
        cout << "New Dish Added Successfully!" << endl;

        dishFile.close(); // �ر��ļ�
    }

    vector<Dish> readDishesFromFile(const string& filename) const {
        vector<Dish> dishes;
        ifstream infile(filename);
        if (!infile) {
            cerr << "Error opening file." << endl;
            return dishes;
        }

        string line;
        string dish_name;
        int dish_id;
        double dish_price;
        string dish_ingredients;
        double dish_cost;
        while (getline(infile, line)) {
            if (line.find("Dish Name: ") != string::npos) {
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

    void deleteDish(int dish_id) {
        vector<Dish> dishes = readDishesFromFile("Dish.txt");
        bool found = false;
        ofstream tempFile("temp.txt");
        for (const auto& dish : dishes) {
            if (dish.getId() == dish_id) {
                found = true;
                cout << "Dish deleted successfully:" << endl;
            }
            else {
                dish.writeToFile(tempFile); // ���Ǳ�ɾ���Ĳ�Ʒд����ʱ�ļ�
            }
        }
        tempFile.close();

        if (!found) {
            cout << "Dish not found!" << endl;
            remove("temp.txt"); // ɾ����ʱ�ļ�
            return;
        }

        // ɾ��ԭ�ļ�����������ʱ�ļ�
        remove("Dish.txt");
        rename("temp.txt", "Dish.txt");
    }
};

class Customer : public User {
public:
    Customer(string uname, string mail, int user_age) : User(uname, mail, user_age) {}

    void getUserInfo() const override {
        cout << "Customer Information:" << endl;
        User::getUserInfo();
    }

    void editUserInfo() override {
        cout << "Editing Customer Information:" << endl;
        User::editUserInfo();
    }
};

//������غ���
bool authenticate(User* user) {
    // ֻ��Manager��Chef��Ҫ������֤
    if (dynamic_cast<Manager*>(user) || dynamic_cast<Chef*>(user)) {
        string correctPassword = user->getPassword();
        string inputPassword;
        int attempts = 0;

        //��������3��ϵͳֱ���˳�����
        while (attempts < 3) {
            cout << "Enter your password: ";
            cin >> inputPassword;
            if (inputPassword == correctPassword) {
                return true;
            }
            else {
                attempts++;
                cout << "Incorrect password. Please try again." << endl;
            }
        }
        cout << "Too many incorrect attempts. Exiting..." << endl;
        return false;
    }
    return true; // Customer����Ҫ������֤
}

//��ӡ��ʦ�˵�ҳ��
void ChefprintMenu() {
    cout << "\nMenu:" << endl;
    cout << "1. Edit User Info" << endl;
    cout << "2. Delete Dish" << endl;
    cout << "3. Add New Dish" << endl; // �����ѡ��
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    int choice;
    cout << "Select your role:" << endl;
    cout << "1. Manager" << endl;
    cout << "2. Chef" << endl;
    cout << "3. Customer" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    User* user = nullptr;

    switch (choice) {
    case 1:
        user = new Manager("manager123", "123456", "manager@example.com", 35);//��������ʼ���˹�������Ϣ��������123456
        break;
    case 2:
        user = new Chef("chef123", "654321", "chef@example.com", 30);//ͬ�ϣ���ʦ������654321
        break;
    case 3:
        user = new Customer("customer123", "customer@example.com", 25);//������
        break;
    default:
        cout << "Invalid choice. Exiting..." << endl;
        return 0;
    }

    if (authenticate(user)) {
        cout << "Access granted." << endl;
        user->editUserInfo(); // �û����������Ϣ
        cout << "Your User Info:" << endl;
        user->getUserInfo(); // ��ӡ�û���Ϣ

        // ����û���Ϣ�Ƿ��Ѵ��ڣ����ֻ�е�������Ϣ��һ�²����û����ڣ��о��е��������������õģ�������뷨��Ⱥ������
        if (!user->userInfoExists("Person_Inf.txt")) {
            ofstream outFile("Person_Inf.txt", ios::app);
            if (!outFile) {
                cerr << "Error opening file." << endl;
                delete user;
                return 1;
            }
            user->writeToFile(outFile); // ���û���Ϣд���ļ�
            outFile.close();
            cout << "User Info added to file." << endl;
        }
        else {
            cout << "User Info already exists in file." << endl;
        }

        int ChefmenuChoice;
        do {
            //Ŀǰֻ�г�ʦ�Ĳ˵�ҳ�棬��Ҫ�Լ��������û�ҳ��,���Լ�д���룬�����Կ���д�ɺ���
            ChefprintMenu();

            cin >> ChefmenuChoice;
            switch (ChefmenuChoice) {
            case 1:
                user->editUserInfo(); // �޸��û���Ϣ
                cout << "Updated User Info:" << endl;
                user->getUserInfo(); // ��ӡ���º���û���Ϣ
                break;
            case 2: {
                int dishId;
                cout << "Enter Dish ID to delete: ";
                cin >> dishId;
                dynamic_cast<Chef*>(user)->deleteDish(dishId); // ɾ����Ʒ
                break;
            }
            case 3:
                int newDishId;
                cout << "Enter Dish ID for the new dish: ";
                cin >> newDishId;
                dynamic_cast<Chef*>(user)->addNewDish(newDishId); // ��Ӳ�Ʒ
                break;
            case 4:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (ChefmenuChoice != 4);
    }
    else {
        cout << "Access denied." << endl;
    }

    delete user; // �ͷ��ڴ�

    return 0;
}

