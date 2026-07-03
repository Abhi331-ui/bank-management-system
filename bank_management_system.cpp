#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<ctime>
#include<limits>

using namespace std;

class account;
bool account_exists(int accNo);

void logTransaction(int accNo, string type, float amount) {
    ofstream logFile("transactions.txt", ios::app);
    if (!logFile) return;

    time_t now = time(0);
    string dt_str = ctime(&now);
    if (!dt_str.empty() && dt_str.back() == '\n') {
        dt_str.pop_back();
    }

    logFile << "AccNo: " << accNo
            << " | " << type
            << " | Amount: " << amount
            << " | Time: " << dt_str << "\n";

    logFile.close();
}

class account{
    private:
        int account_number;
        char name[50];
        float balance;
        int PIN;

    public:
        void create_account(){
            cout << "Enter Account Number: ";
            while (true) {
                if (!(cin >> account_number) || account_number <= 0) {
                    cout << "Invalid Account Number. Please enter a positive integer: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else if (account_exists(account_number)) {
                    cout << "Account number already exists. Enter a different Account Number: ";
                } else {
                    break;
                }
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter User Name: ";
            cin.getline(name, sizeof(name));
            
            cout << "Enter Balance: ";
            while (!(cin >> balance) || balance < 0) {
                cout << "Invalid Balance. Please enter a non-negative number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            
            cout << "Set PIN: ";
            while (!(cin >> PIN) || PIN < 0) {
                cout << "Invalid PIN. Please enter a positive integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        void display_account(){
            cout << "\n Account Number: " << account_number;
            cout << "\n Name: " << name;
            cout << "\n Balance: " << balance;
        }

        void deposit(float amount){
            balance = balance + amount;
        }

        bool withdraw(float amount){
            if(amount <= balance){
                balance = balance - amount;
                return true;
            }

            cout << "\nInsufficient Balance";
            return false;
        }

        bool check_pin(int input_pin){
            return input_pin == PIN;
        }

        int get_account_number(){
            return account_number;
        }
};

bool account_exists(int accNo) {
    ifstream file("accounts.dat", ios::binary);
    if (!file) return false;
    account temp;
    while (file.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
        if (temp.get_account_number() == accNo) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}


int main(){
    account acc;
    int choice = 0, pin = 0;
    float amount = 0.0f;
    cout << "\n======================";
    cout << "\n BANK MANAGEMENT SYSTEM";
    cout << "\n======================\n";
       

    do{
        
        cout << "\n....Services....";        
        cout << "\n1.Create Account";
        cout << "\n2.Deposit";
        cout << "\n3.Withdraw";
        cout << "\n4.Display";
        cout << "\n5.Delete Account";
        cout << "\n6.View Transaction History";
        cout << "\n7.Delete All Accounts";
        cout << "\n8.Exit\n";
        
        
        cout << "\nEnter your Choice: \n";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
        case 1:{
            acc.create_account();

            ofstream outFile("accounts.dat", ios::binary|ios::app);
            if (!outFile) {
                cout << "\nError: Unable to open database for writing.\n";
                break;
            }
            outFile.write(reinterpret_cast<char*>(&acc), sizeof(acc));
            outFile.close();
            cout << "\nAccount created successfully.\n";
            break;
        }
        case 2:{
            int accNo;
            bool found = false;
            cout << "Enter Account Number: ";
            if (!(cin >> accNo)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Account Number.\n";
                break;
            }

            fstream file("accounts.dat", ios::binary | ios::in | ios::out);
            if (!file) {
                cout << "\nError: No accounts exist or database file cannot be opened.\n";
                break;
            }
            account tempAcc;

            while (file.read(reinterpret_cast<char*>(&tempAcc), sizeof(tempAcc))) {
                if (tempAcc.get_account_number() == accNo) {
                    found = true;

                    cout << "Enter Your PIN: ";
                    if (!(cin >> pin)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid PIN format.\n";
                        break;
                    }

                    if(tempAcc.check_pin(pin)){
                        cout << "Enter Amount: ";
                        while (!(cin >> amount) || amount <= 0) {
                            cout << "Invalid amount. Enter a positive number: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        tempAcc.deposit(amount);

                        streampos pos = file.tellg();
                        file.seekp(pos - static_cast<streamoff>(sizeof(tempAcc)));
                        file.write(reinterpret_cast<char*>(&tempAcc), sizeof(tempAcc));

                        cout << "\nDeposit Successful\n";
                        logTransaction(tempAcc.get_account_number(), "DEPOSIT", amount);
                    }
                    else{
                        cout << "\nWrong PIN\n";
                    }
                    break;
                }
            }
            file.close();

            if (!found) {
                cout << "\nAccount not found\n";
            }
            break;
        }

        case 3:{
            int accNo;
            bool found = false;
            cout << "Enter Account Number: ";
            if (!(cin >> accNo)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Account Number.\n";
                break;
            }

            fstream file("accounts.dat", ios::binary | ios::in | ios::out);
            if (!file) {
                cout << "\nError: No accounts exist or database file cannot be opened.\n";
                break;
            }
            account tempAcc;

            while (file.read(reinterpret_cast<char*>(&tempAcc), sizeof(tempAcc))) {
                if (tempAcc.get_account_number() == accNo) {
                    found = true;

                    cout << "Enter Your PIN: ";
                    if (!(cin >> pin)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid PIN format.\n";
                        break;
                    }

                    if(tempAcc.check_pin(pin)){
                        cout << "Enter Amount: ";
                        while (!(cin >> amount) || amount <= 0) {
                            cout << "Invalid amount. Enter a positive number: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }

                        if (tempAcc.withdraw(amount)) {
                            streampos pos = file.tellg();
                            file.seekp(pos - static_cast<streamoff>(sizeof(tempAcc)));
                            file.write(reinterpret_cast<char*>(&tempAcc), sizeof(tempAcc));

                            cout << "\nWithdraw Successful\n";
                            logTransaction(tempAcc.get_account_number(), "WITHDRAW", amount);
                        }
                    }
                    else{
                        cout << "\nWrong PIN\n";
                    }
                    break;
                }
            }
            file.close();

            if (!found) {
                cout << "\nAccount not found\n";
            }
            break;
        }

        case 4:{
            ifstream inFile("accounts.dat", ios::binary);
            if (!inFile) {
                cout << "\nNo accounts found.\n";
                break;
            }
            account tempAcc;
            bool hasAccounts = false;

            while (inFile.read(reinterpret_cast<char*>(&tempAcc), sizeof(tempAcc))) {
                tempAcc.display_account();
                cout << "\n----------------------";
                hasAccounts = true;
            }
            inFile.close();
            if (!hasAccounts) {
                cout << "\nNo accounts found.\n";
            } else {
                cout << endl;
            }
            break;
        }

        case 5: {
            int accNo;
            cout << "Enter Account Number to delete: ";
            if (!(cin >> accNo)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Account Number.\n";
                break;
            }

            ifstream inFile("accounts.dat", ios::binary);
            if (!inFile) {
                cout << "\nError: No accounts exist or database file cannot be opened.\n";
                break;
            }
            ofstream outFile("temp.dat", ios::binary);
            if (!outFile) {
                inFile.close();
                cout << "\nError: Unable to create temporary file.\n";
                break;
            }

            account tempAcc;
            bool found = false;

            while (inFile.read(reinterpret_cast<char*>(&tempAcc), sizeof(tempAcc))) {
                if (tempAcc.get_account_number() == accNo) {
                    found = true;
                    cout << "\nAccount Deleted Successfully\n";
                } else {
                    outFile.write(reinterpret_cast<char*>(&tempAcc), sizeof(tempAcc));
                }
            }

            inFile.close();
            outFile.close();

            remove("accounts.dat");           // delete original file
            rename("temp.dat", "accounts.dat"); // rename temp to original

            if (!found) {
                cout << "Account not found\n";
            }

            break;
        }

        case 6: {
            int accNo;
            cout << "Enter Account Number: ";
            if (!(cin >> accNo)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Account Number.\n";
                break;
            }

            ifstream logFile("transactions.txt");
            if (!logFile) {
                cout << "\nNo transaction history found.\n";
                break;
            }

            string line;
            bool found = false;

            cout << "\n--- Transaction History ---\n";

            while (getline(logFile, line)) {
                if (line.find("AccNo: " + to_string(accNo)) != string::npos) {
                    cout << line << endl;
                    found=true;
                }
            }

            logFile.close();
            if (!found) {
                cout << "No transactions found for this account.\n";
            }
            break;
        }

        case 7: {
            char confirm;

            cout << "Are you sure you want to delete ALL accounts? (y/n): ";
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                if (remove("accounts.dat") == 0) {
                    cout << "All accounts deleted successfully.\n";
                } else {
                    cout << "No accounts found or error deleting file.\n";
                }
            } else {
                cout << "Operation cancelled.\n";
            }

            break;
        }

        case 8:{
            cout << "Exiting...\n";
            break;
        }

        default:{
            cout << "Invalid choice\n";
        }
        }
    }while (choice != 8);
    
    return 0;
}
