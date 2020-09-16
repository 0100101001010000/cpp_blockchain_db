//
// Created by asus-lubuntu on 10/09/2020.
//

#include "unique_id.h"

int Unique_id::create_seed() const {
    auto current_time = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> time_difference = (current_time - start_time) * 1000000;
    return time_difference.count();
}

string Unique_id::operator()() const {
    string id;
    for (int i = 0; i < length; ++i){
        int char_num_choice = char_num();
        if (char_num_choice == 0){
            //choose a number
            id += to_string(num());
        }
        else if (char_num_choice == 1){
            //choose case for char
            int case_choice = char_case();
            if (case_choice == 0){
                //choose upper case character
                int up = upper_char();
                id += upper_char_map.at(up);
            }
            else if (case_choice == 1){
                //choose lower case character
                int low = lower_char();
                id += lower_char_map.at(low);
            }
            else {
                throw;
            }
        }
        else {
            throw;
        }
    }
    return id;
}