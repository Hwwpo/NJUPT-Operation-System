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
    int id;                     // 进程id
    std::vector<int> Max;       // 进程所需的最大资源数
    std::vector<int> Allocation;// 进程已分配的资源数
    std::vector<int> Need;      // 进程还需要的资源数
    bool finish;                // 是否完成

    process(int id, const std::vector<int> &max, const std::vector<int> &allocation, const std::vector<int> &need,
            bool finish = false);
    void Display(int cell_width) const;
};
std::string vector2string(const std::vector<int> &_vector);

#endif //EXP2_PROCESS_H
