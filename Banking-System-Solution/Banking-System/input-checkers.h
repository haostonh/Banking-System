#include<algorithm>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>

int CheckNumberInput(std::vector<int> valid_nums) {
    bool is_valid_num = false;
    int input_num;
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
        }
        else {
            ss_input_num.clear();
            ss_input_num.str("");
        }

        if (std::find(valid_nums.begin(), valid_nums.end(), input_num) != valid_nums.end()) {
            is_valid_num = 1;
        }
        else {
            std::cout << "Invalid Number, Try Again" << std::endl;
            continue;
        }
    } while (!is_valid_num);

    return input_num;
}