#include<algorithm>
#include<iostream>
#include<regex>
#include<sstream>
#include<string>
#include<vector>

long double CheckForValidAmount(std::string action) {
  bool is_valid_amount = false;
  long double input_amount;
  std::string str_input_amount;
  std::stringstream ss_input_amount;

  do {
    std::cout << "Enter amount to " << action << ": ";
    std::getline(std::cin, str_input_amount);
    if (!(std::regex_match(str_input_amount, std::regex("([0-9]+)\\.([0-9]){2}")))) {
      std::cout << "\nPlease enter a value less than a billion with two decimal places. Try Again.\n" << std::endl;
      continue;
    }
    ss_input_amount << str_input_amount;
    ss_input_amount >> input_amount;

    if (ss_input_amount.fail()) {
      ss_input_amount.clear();
      ss_input_amount.str("");
      std::cout << "Invalid Input, Try Again" << std::endl;
    } else {
      ss_input_amount.clear();
      ss_input_amount.str("");
      is_valid_amount = true;
    }
  } while (!is_valid_amount);

  return input_amount;
}

unsigned int CheckNumberInput(std::vector<unsigned int> valid_nums) {
  bool is_valid_num = false;
  unsigned int input_num;
  std::string str_input_num;
  std::stringstream ss_input_num;

  do {
    std::cout << "Your Entry: ";
    std::getline(std::cin, str_input_num);
    ss_input_num << str_input_num;
    ss_input_num >> input_num;

    if (ss_input_num.fail()) {
      ss_input_num.clear();
      ss_input_num.str("");
      std::cout << "Invalid Input, Try Again" << std::endl;
      continue;
    } else {
      ss_input_num.clear();
      ss_input_num.str("");
    }

    if (std::find(valid_nums.begin(), valid_nums.end(), input_num) != valid_nums.end()) {
      is_valid_num = 1;
    } else {
      std::cout << "Invalid Number, Try Again" << std::endl;
      continue;
    }
  } while (!is_valid_num);

  return input_num;
}