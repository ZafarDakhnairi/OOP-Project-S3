#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;

void ScreenClearer() {
    system("cls");
}

struct Item {
    string name;
    double buyPrice;
    double sellPrice;
    int quantity;
};

struct Transaction {
    string type; // "Add" or "Sell"
    string itemName;
    int quantity;
    double totalAmount;
};

class GroceryStore {
private:
    vector<Item> stock;
    vector<Transaction> transactions;
    double money;
    const string logFile = "transaction_log.txt";

    void logTransaction(const Transaction &transaction) {
        ofstream file(logFile, ios::app); // Open in append mode
        if (file.is_open()) {
            file << transaction.type << ", " << transaction.itemName << ", "
                 << transaction.quantity << ", " << transaction.totalAmount << endl;
            file.close();
        } else {
            cerr << "Unable to open log file." << endl;
        }
    }

    void loadTransactionLog() {
        ifstream file(logFile);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string type, itemName;
                int quantity;
                double totalAmount;
                char delimiter;

                ss >> type >> delimiter;
                getline(ss, itemName, ','); // Get itemName until next comma
                ss >> quantity >> delimiter >> totalAmount;

                Transaction transaction = {type, itemName, quantity, totalAmount};
                transactions.push_back(transaction);
            }
            file.close();
        }
    }

public:
    GroceryStore(double initialMoney) : money(initialMoney) {
        loadTransactionLog(); // Load existing transactions
    }

    void addItem(const string &name, double buyPrice, double sellPrice) {
        Item item = {name, buyPrice, sellPrice, 0};
        stock.push_back(item);
    }

    void addStock(const string &itemName, int quantity) {
        for (size_t i = 0; i < stock.size(); ++i) {
            if (stock[i].name == itemName) {
                double cost = stock[i].buyPrice * quantity;
                if (money >= cost) {
                    stock[i].quantity += quantity;
                    money -= cost;
                    Transaction transaction = {"Add", itemName, quantity, cost};
                    transactions.push_back(transaction);
                    logTransaction(transaction);
                    cout << quantity << " units of " << itemName << " added to stock.\n";
                } else {
                    cout << "Insufficient funds to add stock.\n";
                }
                return;
            }
        }
        cout << "Item not found in stock.\n";
    }

    void sellItem(const string &itemName, int quantity) {
        for (size_t i = 0; i < stock.size(); ++i) {
            if (stock[i].name == itemName) {
                if (stock[i].quantity >= quantity) {
                    double revenue = stock[i].sellPrice * quantity;
                    stock[i].quantity -= quantity;
                    money += revenue;
                    Transaction transaction = {"Sell", itemName, quantity, revenue};
                    transactions.push_back(transaction);
                    logTransaction(transaction);
                    cout << quantity << " units of " << itemName << " sold.\n";
                } else {
                    cout << "Insufficient stock to complete the sale.\n";
                }
                return;
            }
        }
        cout << "Item not found in stock.\n";
    }

    void viewStock() const {
        cout << "\nCurrent Stock:\n";
        cout << setw(20) << "Item" << setw(20) << "Quantity" << setw(20) << "Buy Price" << setw(20) << "Sell Price" << "\n";
        for (int i = 0; i < 100; i++) {
            cout << "-";
        }
        cout << endl;
        for (size_t i = 0; i < stock.size(); ++i) {
            cout << setw(20) << stock[i].name << setw(20) << stock[i].quantity
                 << setw(20) << stock[i].buyPrice << setw(20) << stock[i].sellPrice << "\n";
        }
    }

    void viewTransactions() const {
        cout << "\nTransaction Record:\n";
        cout << setw(10) << "Type" << setw(15) << "Item" << setw(10) << "Quantity" << setw(10) << "Amount" << "\n";
        for (size_t i = 0; i < transactions.size(); ++i) {
            cout << setw(10) << transactions[i].type << setw(15) << transactions[i].itemName
                 << setw(10) << transactions[i].quantity << setw(10) << transactions[i].totalAmount << "\n";
        }
    }

    void viewProfit() const {
        double totalProfit = 0;
        for (size_t i = 0; i < transactions.size(); ++i) {
            if (transactions[i].type == "Sell") {
                for (size_t j = 0; j < stock.size(); ++j) {
                    if (stock[j].name == transactions[i].itemName) {
                        double profit = transactions[i].totalAmount - (stock[j].buyPrice * transactions[i].quantity);
                        totalProfit += profit;
                        break;
                    }
                }
            }
        }
        cout << "\nTotal Profit: " << totalProfit << "\n";
    }

    void viewMoney() const {
        cout << "\nCurrent Money: " << money << "\n";
    }
};

int main() {
    GroceryStore store(1000.0); // Initial money: 1000

    // Add items to the store
    store.addItem("Apple", 2.0, 3.0);
    store.addItem("Banana", 1.5, 2.5);
    store.addItem("Milk", 10.0, 12.0);
    store.addItem("Bread", 5.0, 7.0);
    store.addItem("Eggs", 0.5, 1.0);

    int choice;
    do {
        cout << "\n---------------------------------------- Grocery Store Menu ----------------------------------------\n";
        cout << "1. View Stock\n2. Add Stock\n3. Sell Item\n4. View Transactions\n5. View Profit\n6. View Money\n7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                store.viewStock();
                break;
            case 2: {
                string itemName;
                int quantity;
                cout << "Enter item name: ";
                cin >> itemName;
                cout << "Enter quantity: ";
                cin >> quantity;
                store.addStock(itemName, quantity);
                break;
            }
            case 3: {
                string itemName;
                int quantity;
                cout << "Enter item name: ";
                cin >> itemName;
                cout << "Enter quantity: ";
                cin >> quantity;
                store.sellItem(itemName, quantity);
                break;
            }
            case 4:
                store.viewTransactions();
                break;
            case 5:
                store.viewProfit();
                break;
            case 6:
                store.viewMoney();
                break;
            case 7:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
        cin.ignore();
        cout << "\nPress Enter to continue...";
        cin.get();
        ScreenClearer();
    } while (choice != 7);

    return 0;
}
