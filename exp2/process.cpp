//
// Created by 潘鹏宇 on 2023/12/28.
//

#include "process.h"

process::process(int id, const std::vector<int> &max, const std::vector<int> &allocation, const std::vector<int> &need,
                 bool finish) : id(id), Max(max), Allocation(allocation), Need(need), finish(finish) {}
