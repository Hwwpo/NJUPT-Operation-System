//
// Created by 潘鹏宇 on 2023/12/28.
//

#include "process.h"

// 将vector中的内容化为string
std::string vector2string(const std::vector<int> &_vector) {
    std::string str;
    for(auto item : _vector) {
        str += std::to_string(item);
        str += "  ";
    }
    return str;
}

process::process(int id, const std::vector<int> &max, const std::vector<int> &allocation, const std::vector<int> &need,
                 bool finish) : id(id), Max(max), Allocation(allocation), Need(need), finish(finish) {}

void process::Display(int cell_width) const {
    std::cout << std::setw(cell_width) << this->id
              << std::setw(cell_width) << vector2string(this->Max)
              << std::setw(cell_width) << vector2string(this->Allocation)
              << std::setw(cell_width) << vector2string(this->Need);
}
