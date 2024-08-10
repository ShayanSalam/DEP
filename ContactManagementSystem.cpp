/*
 * Contact Management System in C++
 *
 * This program is a console-based application for managing a list of contacts.
 * It allows users to perform the following actions:
 * . Add a new contact with a name, email, and phone number.
 * . Display all contacts stored in the system.
 * . View a specific contact's details by selecting from the list.
 * . Delete a contact by name.
 * . Save contacts to a file and load them when the program starts.
 *
 * Features:
 * . The Contact class stores the name, email, phone number, and the timestamp
 *   when the contact was added.
 * . Email and phone number inputs are validated using regular expressions.
 * . The program stores contacts in a vector and saves them to a file (`contacts.txt`)
 *   to persist data between sessions.
 * . The console output is enhanced with color using Windows API functions.
 *
 * SHAYAN SALAM
 */


#include <iostream>
#include <string>
#include <vector>
#include <windows.h>  // For Windows API functions
#include <ctime>      // For time functions
#include <regex>      // For regex validation
#include <fstream>    // For file handling

using namespace std;

// Class Contact
class Contact {
private:
    string name;
    string email;
    string phone;
    string timestamp;  // Storing the time when the contact is added

public:
    // Constructor
    Contact(string n, string e, string p) {
        name = n;
        email = e;
        phone = p;

        time_t now = time(0);
        struct tm timeinfo;
        char buffer[80];
        localtime_s(&timeinfo, &now);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

        timestamp = string(buffer);  // Convert formatted time to string and storing it
    }

    // Getters
    string getName() const
    {
        return name;
    }
    string getEmail() const
    {
        return email;
    }
    string getPhone() const
    {
        return phone;
    }
    string getTimestamp() const
    {
        return timestamp;
    }

    // Displaying Contact information
    void display() const {
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Added On: " << timestamp << endl;
    }

    // Saving contact to file
    void saveToFile(ofstream& file) const {
        file << name << "," << email << "," << phone << "," << timestamp << endl;
    }

    // Loading the contact from file
    static Contact loadFromFile(ifstream& file) {
        string n, e, p, t;
        getline(file, n, ',');
        getline(file, e, ',');
        getline(file, p, ',');
        getline(file, t);
        return Contact(n, e, p);
    }
};

class ContactManager {
private:
    vector<Contact> contacts;
    const string filename = "contacts.txt";  // File to store contacts

public:
    // Function to set the color of the console text
    void setColor(int color) const {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    bool validateEmail(const string& email) const {
        // Regular expression for validating the Email
        const regex pattern(R"((\w+)(\.{1}\w+)*@(\w+)(\.(\w+))+)");
        return regex_match(email, pattern);
    }

    bool validatePhone(const string& phone) const {
        // Regular expression for validating a phone number (123-456-7890 or (123) 456-7890)
        const regex pattern(R"(^(\(\d{3}\) |\d{3}-)\d{3}-\d{4}$)");
        regex pattern2("^(03[0-9]{9})$");
        return regex_match(phone, pattern) || regex_match(phone, pattern2);
    }

    void addContact() {
        system("CLS"); 
        string name, email, phone;
        cout << "Enter name: ";
        getline(cin, name);

        // Email input with validation
        while (true) {
            cout << "Enter email (e.g., name@example.com): ";
            getline(cin, email);
            if (validateEmail(email)) {
                break;
            }
            else {
                setColor(12);  
                cout << "Invalid email format. Please try again." << endl;
                setColor(7);   
            }
        }

        // Phone number input with validation
        while (true) {
            cout << "Enter phone (e.g., 03127639212 or (123) 456-7890): ";
            getline(cin, phone);
            if (validatePhone(phone)) {
                break;
            }
            else {
                setColor(12);  
                cout << "Invalid phone format. Please try again." << endl;
                setColor(7);   
            }
        }

        Contact newContact(name, email, phone);
        contacts.push_back(newContact);

        // Saving contacts to file
        saveContactsToFile();

        setColor(10);  // Set color to green
        cout << "Contact added successfully!" << endl;
        setColor(7);   // Reset to default color
        system("PAUSE");  // Pause to view the message
    }

    void displayContacts() const {
        system("CLS");  // Clear the screen
        setColor(11);  // Set color to cyan
        if (contacts.empty()) {
            cout << "No Contact Available.." << endl;
        }
        else {
            cout << "Contacts:" << endl;
            for (const auto& contact : contacts) {
                contact.display();
                cout << "------------------------" << endl;
            }
        }
        setColor(7);   // Reset to default color
        system("PAUSE");  // Pause to view the contacts
    }

    void ViewContact() const {
        system("CLS");  // Clear the screen
        if (contacts.empty()) {
            cout << "No contacts available. Please add some contacts first." << endl;
            system("PAUSE");  // Pause to view the message
            return;
        }

        vector<string> contactNames;
        for (const auto& contact : contacts) {
            contactNames.push_back(contact.getName());
        }

        cout << "Select a contact to View:" << endl;
        for (int i = 0; i < contactNames.size(); ++i) {
            cout << (i + 1) << ". " << contactNames[i] << endl;
        }

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice < 1 || choice > contactNames.size()) {
            setColor(12);  // Set color to red
            cout << "Invalid choice. Please try again." << endl;
            setColor(7);   // Reset to default color
            system("PAUSE");  // Pause to view the invalid choice message
            return;
        }

        string searchName = contactNames[choice - 1];
        bool found = false;

        for (const auto& contact : contacts) {
            if (contact.getName() == searchName) {
                setColor(10);  // Set color to green
                cout << "Contact found!" << endl;
                contact.display();
                found = true;
                break;
            }
        }

        if (!found) {
            setColor(12);  
            cout << "Contact not found!" << endl;
        }

        setColor(7);  
        system("PAUSE");  
    }

    void deleteContact() {
        system("CLS");
        string deleteName;
        cout << "Enter name to delete: ";
        getline(cin, deleteName);
        bool found = false;

        for (auto it = contacts.begin(); it != contacts.end(); ++it) {
            if (it->getName() == deleteName) {
                contacts.erase(it);
                setColor(10);  // Set color to green
                cout << "Contact deleted successfully!" << endl;
                found = true;
                break;
            }
        }

        if (!found) {
            setColor(12);  // Set color to red
            cout << "Contact not found!" << endl;
        }

        // Save contacts to file
        saveContactsToFile();

        setColor(7);   // Reset to default color
        system("PAUSE");  // Pause to view the delete result
    }

    // Load contacts from file
    void loadContactsFromFile() {
        ifstream file(filename);
        if (file.is_open()) {
            string n, e, p, t;
            while (getline(file, n, ',')) {
                getline(file, e, ',');
                getline(file, p, ',');
                getline(file, t);
                Contact contact(n, e, p);
                contacts.push_back(contact);
            }
            file.close();
        }
    }

    // Save contacts to file
    void saveContactsToFile() const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& contact : contacts) {
                contact.saveToFile(file);
            }
            file.close();
        }
    }
};

int main() {
    ContactManager manager;
    manager.loadContactsFromFile();  

    int choice;

    while (true) {
        system("CLS"); 
        manager.setColor(14);  
        cout << "---------------------------" << endl;
        cout << "Contact Management System" << endl;
        cout << "---------------------------" << endl;

        manager.setColor(3);
        cout << "1. Add Contact" << endl;
        manager.setColor(5);
        cout << "2. Display Contacts" << endl;
        manager.setColor(10);
        cout << "3. View Contact" << endl;
        manager.setColor(11);
        cout << "4. Delete Contact" << endl;
        manager.setColor(12);
        cout << "5. Exit" << endl;
        manager.setColor(1);
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        manager.setColor(7); 

        switch (choice) {
        case 1:
            manager.addContact();
            break;
        case 2:
            manager.displayContacts();
            break;
        case 3:
            manager.ViewContact();
            break;
        case 4:
            manager.deleteContact();
            break;
        case 5:
            manager.setColor(13);  
            cout << "Exiting..." << endl;
            manager.setColor(7);  
            return 0;
        default:
            manager.setColor(12);  
            cout << "Invalid choice. Please try again." << endl;
            manager.setColor(7);  
            system("PAUSE"); 
        }
    }

    return 0;
}
