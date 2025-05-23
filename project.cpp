#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <sstream>
using namespace std;

struct Transaction {
    string date;
    string type;
    string category;
    float amount;
    Transaction* next;

    Transaction(string d, string t, string c, float a) {
        date = d;
        type = t;
        category = c;
        amount = a;
        next = NULL; 
    }
};

struct Investment {
    string name;
    float minAmount;
    float returnPercent;
    Investment* left;
    Investment* right;

    Investment(string n, float m, float r) {
        name = n;
        minAmount = m;
        returnPercent = r;
        left = NULL; 
        right = NULL; 
    }
};

void addTransaction(Transaction*& head, string date, string type, string category, float amount) {
    Transaction* newTransaction = new Transaction(date, type, category, amount);
    newTransaction->next = head;
    head = newTransaction;
}

float calculateSavings(Transaction* head) {
    float totalIncome = 0;
    float totalExpense = 0;
    Transaction* temp = head;
    while (temp != NULL) { 
        if (temp->type == "Income") {
            totalIncome += temp->amount;
        } else {
            totalExpense += temp->amount;
        }
        temp = temp->next;
    }
    return totalIncome - totalExpense;
}

void viewTransactions(Transaction* head) {
    Transaction* temp = head;
    cout << "----------TRANSACTIONS-----------------\n";
    cout << "+-------------------+------------+------------+----------+\n";
    cout << "|       DATE        |    TYPE    |  CATEGORY  |  AMOUNT  |\n";
    cout << "+-------------------+------------+------------+----------+\n";
    while (temp != NULL) {
        cout << "| " << setw(17) << left << temp->date
             << "| " << setw(10) << left << temp->type
             << "| " << setw(10) << left << temp->category
             << "| " << setw(8) << right << fixed << setprecision(2) << temp->amount << " |\n";
        temp = temp->next;
    }
    cout << "+-------------------+------------+------------+----------+\n";
}

void addInvestments(Investment*& root, string name, float minAmount, float returnPercent) {
    if (root == NULL) {
        root = new Investment(name, minAmount, returnPercent);
        return;
    }
    if (minAmount <= root->minAmount) {
        addInvestments(root->left, name, minAmount, returnPercent);
    } else {
        addInvestments(root->right, name, minAmount, returnPercent);
    }
}

void suggestInvestments(Investment* root, float savings) {
    if (root == NULL) { 
        return;
    }
    if (root->minAmount <= savings) {
        cout << "INVESTMENT OFFER: " << root->name
             << " | MIN_AMOUNT: " << root->minAmount
             << " | RETURN PERCENT: " << root->returnPercent << "%\n";
    }
    suggestInvestments(root->left, savings);
    suggestInvestments(root->right, savings);
}

void openWebsite(string investmentName) {
    string url;

    if (investmentName == "Mutual Funds") {
        url = "https://www.mutualfunds.com";
    } else if (investmentName == "Fixed Deposit") {
        url = "https://www.fixeddeposit.com";
    } else if (investmentName == "Stock Market") {
        url = "https://www.stockmarket.com";
    } else {
        cout << "No website available for this investment option.\n";
        return;
    }

    system(("start " + url).c_str());
}

void loadFile(Transaction*& head) {
    ifstream file("Transactions.txt");
    if (!file) {
        cout << "No existing transaction data found. Starting a new database.\n";
        return;
    }
    string line;
    while (getline(file, line)) {
        string date, type, category;
        float amount;
        stringstream ss(line);
        ss >> date >> type >> category >> amount;
        addTransaction(head, date, type, category, amount);
    }
    file.close();
}

void save(Transaction* head) {
    ofstream file("Transactions.txt");
    Transaction* temp = head;
    while (temp != NULL) {
        file << temp->date << " " << temp->type << " " << temp->category << " " << temp->amount << endl;
        temp = temp->next;
    }
    file.close();
}

void menu() {
    Transaction* head = NULL;
    Investment* root = NULL;

    loadFile(head);

    addInvestments(root, "Mutual Funds", 5000, 10);
    addInvestments(root, "Fixed Deposit", 10000, 7);
    addInvestments(root, "Stock Market", 20000, 15);

    int choice;
    do {
        cout << "--- Personal Finance Management System ---" << endl;
        cout << "1. Add Income" << endl;
        cout << "2. Add Expense" << endl;
        cout << "3. View Transactions" << endl;
        cout << "4. View Savings" << endl;
        cout << "5. Suggest Investments" << endl;
        cout << "6. Open a website for investment" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string date, category = "None";
            float amount;
            cout << "Enter date (dd/mm/yyyy): ";
            cin >> date;
            cout << "Enter amount: ";
            cin >> amount;
            addTransaction(head, date, "Income", category, amount);
            cout << "Income added successfully!"<<endl;
            break;
        }
        case 2: {
            string date, category;
            float amount;
            cout << "Enter date (dd/mm/yyyy): ";
            cin >> date;
            cout << "Enter category: ";
            cin >> category;
            cout << "Enter amount: ";
            cin >> amount;
            addTransaction(head, date, "Expense", category, amount);
            cout << "Expense added successfully!"<<endl;
            break;
        }
        case 3: {
            viewTransactions(head);
            break;
        }
        case 4: {
            float savings = calculateSavings(head);
            cout << "YOUR SAVINGS: " << savings << endl;
            break;
        }
        case 5: {
            float savings = calculateSavings(head);
            cout << "AVAILABLE INVESTMENT OPTIONS BASED ON YOUR SAVINGS (" << savings << "):"<<endl;
            suggestInvestments(root, savings);
            break;
        }
        case 6: {
            string investmentName;
            cout << "Enter the name of the investment option (e.g., 'Mutual Funds', 'Fixed Deposit', 'Stock Market'): ";
            cin.ignore();
            getline(cin, investmentName);
            openWebsite(investmentName);
            break;
        }
        case 7: {
            save(head);
            cout << "Exiting... BYE BYE -.- ";
            break;
        }
        default: {
            cout << "WRONG CHOICE OPEN YOUR EYES -_- ";
            break;
        }
        }
    } while (choice != 7);
}

int main() {
    menu();
    return 0;
}

