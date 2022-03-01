#include"input-checkers.h"

#include<iostream>
#include<sstream>
#include<string>
#include<vector>

class Customer {
  public:
    void DepositMoney(float money) {
      bank_balance += money;
      on_hand_balance -= money;
    }
    void GetBalance() {std::cout << "Your Bank Balance: " << bank_balance << std::endl;}
    void SetBankBalance(float money) {
      bank_balance = money;
    } 
    void SetOnHandBalance(float money) {
     on_hand_balance = money;
    }
    void WithdrawMoney(float money) {
      bank_balance -= money;
      on_hand_balance += money;
    }
  private:
    float bank_balance, on_hand_balance;
};

void BalanceOptions(Customer* person) {
  unsigned int bal_option_num;
  std::string str_bal_option_num;
  std::stringstream ss_bal_option_num;
  bool isValidNum = false;

  std::cout << "Enter one of the numbers to get help on balance: " << std::endl;
  std::cout << "(1) Get Bank Balance" << std::endl;
  std::cout << "(2) Withdraw Out Bank Balance" << std::endl;
  std::cout << "(3) Deposit Into Bank Balance" << std::endl;
  std::cout << "(0) Exit" << std::endl;

  while(!isValidNum) {
    std::cout << "Your Entry: ";
    std::getline(std::cin, str_bal_option_num);
    
    ss_bal_option_num << str_bal_option_num;
    ss_bal_option_num >> bal_option_num;

    if (ss_bal_option_num.fail()) {
      ss_bal_option_num.clear();
      ss_bal_option_num.str("");
      std::cout << "Try Again" << std::endl;
      continue;
    } else {
      isValidNum = true;
      break;
    }
  }

  switch(bal_option_num) {
    case 0:
      std::cout << "Goodbye" << std::endl;
      break;
    case 1:
      person->GetBalance();
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      std::cout << "Not a Valid Option" << std::endl;
      break;
  }
}

int main() {
  unsigned int service_num;

  // Temporary creation of class until connecting db
  Customer* John = new Customer();
  John->SetBankBalance(213);
  John->SetOnHandBalance(200);

  std::cout << "Welcome to Earth Bank!" << std::endl << std::endl;
  std::cout << "Enter the one of the following numbers to be serviced:" << std::endl;
  std::cout << "(1) Balance" << std::endl;
  std::cout << "(0) Exit" << std::endl;

  std::vector<int> valid_service_nums = {0, 1};
  service_num = CheckNumberInput(valid_service_nums);

  std::cout << std::endl;

  switch (service_num) {
    case 0:
      std::cout << "Goodbye" << std::endl;
      break;
    case 1:
      std::cout << "Balance" << std::endl;
      BalanceOptions(John);
      break;
    default:
      std::cout << "Not a valid service number" << std::endl;
      break;
  }

  return 0;
}