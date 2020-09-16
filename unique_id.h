//
// Class to create a unique identifier
//

#ifndef CPP_BLOCKCHAIN_DB_UNIQUE_ID_H
#define CPP_BLOCKCHAIN_DB_UNIQUE_ID_H

#include <random>
#include <chrono>
#include <functional>
#include <string>
#include <map>

using namespace std;

class Unique_id{
    const chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();
    const int length;
    const static constexpr int default_length = 32;
    int create_seed() const;

    uniform_int_distribution<>::param_type char_num_param{0, 1};
    const int alpha_num_seed = create_seed();
    function<int()> char_num = bind(uniform_int_distribution<>{char_num_param}, default_random_engine{alpha_num_seed});

    uniform_int_distribution<>::param_type num_param{0, 9};
    const int num_seed = create_seed();
    function<int()> num = bind(uniform_int_distribution<>{num_param}, default_random_engine{num_seed});

    const int case_seed = create_seed();
    function<int()> char_case = bind(uniform_int_distribution<>{char_num_param}, default_random_engine{case_seed});

    uniform_int_distribution<>::param_type char_param{0, 25};
    const int upper_char_seed = create_seed();
    function<int()> upper_char = bind(uniform_int_distribution<>{char_param}, default_random_engine{upper_char_seed});
    const int lower_char_seed = create_seed();
    function<int()> lower_char = bind(uniform_int_distribution<>{char_param}, default_random_engine{lower_char_seed});

    map<int, string> upper_char_map {
            {0,"A"},
            {1,"B"},
            {2,"C"},
            {3,"D"},
            {4,"E"},
            {5,"F"},
            {6,"G"},
            {7,"H"},
            {8,"I"},
            {9,"J"},
            {10,"K"},
            {11,"L"},
            {12,"M"},
            {13,"N"},
            {14,"O"},
            {15,"P"},
            {16,"Q"},
            {17,"R"},
            {18,"S"},
            {19,"T"},
            {20,"U"},
            {21,"V"},
            {22,"W"},
            {23,"X"},
            {24,"Y"},
            {25,"Z"},
    };
    map<int, string> lower_char_map {
            {0,"a"},
            {1,"b"},
            {2,"c"},
            {3,"d"},
            {4,"e"},
            {5,"f"},
            {6,"g"},
            {7,"h"},
            {8,"i"},
            {9,"j"},
            {10,"k"},
            {11,"l"},
            {12,"m"},
            {13,"n"},
            {14,"o"},
            {15,"p"},
            {16,"q"},
            {17,"r"},
            {18,"s"},
            {19,"t"},
            {20,"u"},
            {21,"v"},
            {22,"w"},
            {23,"x"},
            {24,"y"},
            {25,"z"},
    };

public:
    Unique_id() : length{default_length} {};
    Unique_id(int len) : length{len} {};
    string operator()() const;
};


#endif //CPP_BLOCKCHAIN_DB_UNIQUE_ID_H
