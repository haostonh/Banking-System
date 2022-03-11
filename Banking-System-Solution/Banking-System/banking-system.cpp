#include"input-checkers.h"

#include<iostream>
#include<sstream>
#include<string>
#include<vector>

#include"mysql_driver.h"
#include"mysql_connection.h"
#include<cppconn/driver.h>
#include<cppconn/exception.h>
#include<cppconn/statement.h>
#include<cppconn/prepared_statement.h>
#include<cppconn/datatype.h>

class Customer {
public:
    Customer() {
    }
    Customer(std::string f_n, std::string l_n, long double b_b, long double o_n_b): 
        first_name(f_n), last_name(l_n), bank_balance(b_b), on_hand_balance(o_n_b) {
    }
    void DepositMoney(long double money) {
        bank_balance += money;
        on_hand_balance -= money;
    }
    long double GetBankBalance() { return bank_balance; }
    void GetBankBalancePrompt() { std::cout << "Your Bank Balance: " << bank_balance << "\n"; }
    long double GetOnHandBalance() { return on_hand_balance; }
    void GetOnHandBalancePrompt() {
        std::cout << "Your On Hand Balance: " << on_hand_balance << "\n";
    }
    void SetBankBalance(long double money) {
        bank_balance = money;
    }
    void SetOnHandBalance(long double money) {
        on_hand_balance = money;
    }
    void WithdrawMoney(long double money) {
        bank_balance -= money;
        on_hand_balance += money;
    }
private:
    long double bank_balance = 0;
    long double on_hand_balance = 0;
    std::string first_name, last_name;
};

void DepositFromBankBalance(Customer* person) {
    bool is_valid_deposit = false;
    long double deposit_amount = 0;

    while (!is_valid_deposit) {
        deposit_amount = CheckForValidAmount("Deposit");

        if (deposit_amount > person->GetOnHandBalance()) {
            std::cout << "Cannot Deposit Amount Specified, Try Again" << std::endl;
            continue;
        }
        else if (deposit_amount < 0) {
            std::cout << "Cannot Deposit a Negative Amount, Try Again" << std::endl;
            continue;
        }
        else {
            person->DepositMoney(deposit_amount);
            std::cout << "Money Deposited Successfully!" << std::endl;
            person->GetOnHandBalancePrompt();
            person->GetBankBalancePrompt();
            is_valid_deposit = true;
        }
    }
}

void WithdrawFromBankBalance(Customer* person) {
    bool is_valid_withdrawal = false;
    long double withdraw_amount = 0;

    while (!is_valid_withdrawal) {
        withdraw_amount = CheckForValidAmount("Withdraw");

        if (withdraw_amount > person->GetBankBalance()) {
            std::cout << "Cannot Withdraw Amount Specified, Try Again" << std::endl;
            continue;
        }
        else if (withdraw_amount < 0) {
            std::cout << "Cannot Withdraw a Negative Amount, Try Again" << std::endl;
            continue;
        }
        else {
            person->WithdrawMoney(withdraw_amount);
            std::cout << "Money Withdrew Successfully!" << std::endl;
            person->GetOnHandBalancePrompt();
            person->GetBankBalancePrompt();
            is_valid_withdrawal = true;
        }
    }
}

void BalanceOptions(Customer* person) {
    unsigned int bal_option_num;

    std::cout << "Enter one of the numbers to get help on balance:\n";
    std::cout << "(1) Get Bank Balance\n";
    std::cout << "(2) Deposit Into Bank Balance\n";
    std::cout << "(3) Withdraw Out Bank Balance\n";
    std::cout << "(0) Exit\n";

    std::vector<unsigned int> valid_bal_nums = { 0, 1, 2, 3 };
    bal_option_num = CheckNumberInput(valid_bal_nums);

    switch (bal_option_num) {
    case 0:
        std::cout << "Goodbye\n";
        break;
    case 1:
        person->GetBankBalancePrompt();
        break;
    case 2:
        DepositFromBankBalance(person);
        break;
    case 3:
        WithdrawFromBankBalance(person);
        break;
    default:
        std::cout << "Not a Valid Option\n";
        break;
    }
}

int main(int argc, const char **argv) {
    unsigned int service_num;
    sql::Connection* con;
    sql::Driver* driver;
    sql::Statement* statement;
    sql::ResultSet* result_set;
    
    // Make the Connection with MySQL Server
    try {
        driver = get_driver_instance();
        con = driver->connect(argv[1], argv[2], argv[3]);
        std::cout << "Connection Successful!\n" ;
    } catch (sql::SQLException& e) {
        std::cout << "Could not connect to server. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // Created schema ahead of time in MySQL Workbench
    try {
        con->setSchema("earth_bank");
    } catch (sql::SQLException& e) {
        std::cout << "Could not find schema. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // Create table of customer information
    statement = con->createStatement();
    try {
        statement->execute("CREATE TABLE IF NOT EXISTS customer_information(customer_id INT PRIMARY KEY, first_name VARCHAR(20), last_name VARCHAR(20), on_hand_balance DECIMAL(10,2), bank_balance DECIMAL(10,2));");
    } catch (sql::SQLException& e) {
        std::cout << "Creation of table failed. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    // Insert customer information
    try {
        statement->execute("INSERT INTO customer_information VALUES(1,'John','Smith',213.73,510.67);");
    }
    catch (sql::SQLException& e) {
        std::cout << "Insertion into table failed. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // Retreive balances from table
    long double bank_balance = 0;
    long double on_hand_balance = 0;

    try {
        result_set = statement->executeQuery("SELECT on_hand_balance,bank_balance FROM customer_information WHERE customer_id = 1;");
    }
    catch (sql::SQLException& e) {
        std::cout << "Cound not retreive on_hand_balance and bank_balance. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    while (result_set->next()) {
        on_hand_balance = result_set->getDouble("on_hand_balance");
        bank_balance = result_set->getDouble("bank_balance");
    }

    Customer* John = new Customer();
    John->SetBankBalance(bank_balance);
    John->SetOnHandBalance(on_hand_balance);

    std::cout << "Welcome to Earth Bank!\n\n" ;
    std::cout << "Enter the one of the following numbers to be serviced:\n" ;
    std::cout << "(1) Balance\n";
    std::cout << "(0) Exit\n";

    std::vector<unsigned int> valid_service_nums = { 0, 1 };
    service_num = CheckNumberInput(valid_service_nums);

    std::cout << "\n";

    switch (service_num) {
    case 0:
        std::cout << "Goodbye\n";
        break;
    case 1:
        std::cout << "Balance\n";
        BalanceOptions(John);
        break;
    default:
        std::cout << "Not a valid service number\n";
        break;
    }

    delete result_set;
    delete statement;
    delete con;
    return EXIT_SUCCESS;
}