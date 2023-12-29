//
// Created by 潘鹏宇 on 2023/12/28.
//

#ifndef EXP2_PROCESS_H
#define EXP2_PROCESS_H
#include <vector>
#include <iomanip>
#include <iostream>

class process {
public:
    int id;
    std::vector<int> Max;
    std::vector<int> Allocation;
    std::vector<int> Need;
    bool finish;

    process(int id, const std::vector<int> &max, const std::vector<int> &allocation, const std::vector<int> &need,
            bool finish = false);
    void Display(int cell_width);
};
std::string vector2string(const std::vector<int> &_vector);

#endif //EXP2_PROCESS_H
