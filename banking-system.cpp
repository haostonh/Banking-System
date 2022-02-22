#include<iostream>
#include<string>

class Customer {
  public:
    void DepositMoney(float money) {
      bank_balance += money;
      on_hand_balance -= money;
    }
    void GetBalance() {std::cout << bank_balance << std::endl;}
    void WithdrawMoney(float money) {
      bank_balance -= money;
      on_hand_balance += money;
    }
  private:
    float bank_balance, on_hand_balance;
};

void BalanceOptions() {

}

int main() {
  unsigned int service_num;

  std::cout << "Welcome to Earth Bank!" << std::endl << std::endl;
  std::cout << "Enter the one of the following numbers to be serviced:" << std::endl;
  std::cout << "(1) Balance" << std::endl;
  std::cout << "(0) Exit" << std::endl;
  std::cout << "Your Entry: ";

  std::cin >> service_num;

  switch (service_num) {
    case 0:
      break;
    case 1:
      std::cout << "Balance" << std::endl;
      BalanceOptions();
      break;
    default:
      std::cout << "Not a valid service number" << std::endl;
      break;
  }

  return 0;
}