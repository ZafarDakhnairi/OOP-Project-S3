#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <conio.h>
#include <cstdio>

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
        ofstream file(logFile, ios::app);
        if (file.is_open()) {
            file << transaction.type << ", " << transaction.itemName << ", "
                 << transaction.quantity << ", " << transaction.totalAmount << endl;
            file.close();
        } else {
            cerr << "Unable to open log file." << endl;
        }
    }

public:
    GroceryStore(double initialMoney) : money(initialMoney) {}

    void addItem(const string &name, double buyPrice, double sellPrice) {
        stock.push_back({name, buyPrice, sellPrice, 0});
    }

    void addStock(const string &itemName, int quantity) {
        for (auto &item : stock) {
            if (item.name == itemName) { // Ensure this matches user input
                double cost = item.buyPrice * quantity;
                if (money >= cost) {
                    item.quantity += quantity;
                    money -= cost;
                    Transaction transaction = {"Add", itemName, quantity, cost};
                    transactions.push_back(transaction);
                    logTransaction(transaction);
                    cout << quantity << " units of " << itemName << " added to stock.\n";
                } else {
                    cout << "Insufficient funds to add stock.\n";
                }
                return; // Exit once the item is found and processed
            }
        }
        cout << "Item not found in stock. Please check the item name.\n";
    }

    void sellItem(const string &itemName, int quantity) {
        for (auto &item : stock) {
            if (item.name == itemName) {
                if (item.quantity >= quantity) {
                    double revenue = item.sellPrice * quantity;
                    item.quantity -= quantity;
                    money += revenue;
                    Transaction transaction = {"Sell", itemName, quantity, revenue};
                    transactions.push_back(transaction);
                    logTransaction(transaction);
                    cout << quantity << " units of " << itemName << " sold.\n";
                } else {
                    cout << "Insufficient stock to complete the sale.\n";
                }
                return; // Exit once the item is found and processed
            }
        }
        cout << "Item not found in stock. Please check the item name.\n";
    }

    void viewStock() const {
        cout << "\nCurrent Stock:\n";
        cout << setw(20) << "Item" << setw(20) << "Quantity" << setw(20) << "Buy Price" << setw(20) << "Sell Price" << "\n";
        for (int i = 0; i < 100; i++){
            cout << "-";
        } cout << endl;
        for (const auto &item : stock) {
            cout << setw(20) << item.name << setw(20) << item.quantity
                      << setw(20) << item.buyPrice << setw(20) << item.sellPrice << "\n";
        }
    }

    void viewTransactions() const {
        cout << "\nTransaction Record:\n";
        cout << setw(10) << "Type" << setw(15) << "Item" << setw(10) << "Quantity" << setw(10) << "Amount" << "\n";
        for (const auto &transaction : transactions) {
            cout << setw(10) << transaction.type << setw(15) << transaction.itemName
                      << setw(10) << transaction.quantity << setw(10) << transaction.totalAmount << "\n";
        }
    }

    void viewProfit() const {
        double totalProfit = 0;
        for (const auto &transaction : transactions) {
            if (transaction.type == "Sell") {
                for (const auto &item : stock) {
                    if (item.name == transaction.itemName) {
                        double profit = transaction.totalAmount - (item.buyPrice * transaction.quantity);
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

    ~GroceryStore() {
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
    store.addItem("Burger", 10, 15);

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

    // // Delete the transaction log file
    // if (remove("transaction_log.txt") == 0) {
    //     cout << "Transaction log file deleted successfully.\n";
    // } else {
    //     cerr << "Error deleting the transaction log file.\n";
    // }

    cout << "Exiting program. Goodbye!\n";
    return 0;
}
