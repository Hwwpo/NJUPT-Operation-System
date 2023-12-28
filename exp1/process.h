//
// Created by 潘鹏宇 on 2023/12/26.
//

#ifndef EXP1_PROCESS_H
#define EXP1_PROCESS_H
#include <iostream>

class process {
public:
    explicit process(const char &name);

    bool operator<(const process &rhs) const;

    bool operator>(const process &rhs) const;

    bool operator<=(const process &rhs) const;

    bool operator>=(const process &rhs) const;

public:
    char name;      // 进程名
    int ArrivalTime;// 到达时间
    int ServerTime; // 服务时间
    int StopTime;   // 结束时间
    int turnaroundTime; // 周转时间
    double TurnaroundTimeWithWeight;    // 平均周转时间
};


#endif //EXP1_PROCESS_H
