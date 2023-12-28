#include <iostream>
#include <vector>
#include "process.h"
#include <queue>
#include <algorithm>
#include <iomanip>

int n;
int p;
std::vector<process> processList;
std::queue<process> RRQueue;
std::vector<process> finishedProcess;

void input() {
    std::cout << "请输入进程个数:";
    std::cin >> n;
    for(int i = 0; i < n; i++)
        processList.push_back(process(i + 65));

    std::cout << "请输入到达时间:";
    for(int i = 0; i < n; i++)
        std::cin >> processList[i].ArrivalTime;

    std::cout << "请输入服务时间:";
    for(int i = 0; i < n; i++)
        std::cin >> processList[i].ServerTime;

    std::cout << "请输入时间片q:";
    std::cin >> p;

    std::sort(processList.begin(), processList.end());
}

void printRow(int currentTime, char name, std::string text){
    std::cout << "时刻" << currentTime << ":" << name << text << std::endl;
}

void RR() {
    int currentTime = 0;
    int i = 0;
    for(auto item : processList)    // 按照 FCFS 顺序加入队列
        RRQueue.push(item);
    // 进程没有运行完时
    while(!RRQueue.empty()) {
        // 如果已经到达
        if(RRQueue.front().ArrivalTime <= currentTime) {
            // 如果一个时间片运行不完
            if (RRQueue.front().ServerTime > p) {
                printRow(currentTime, RRQueue.front().name, "开始运行");
                currentTime += p;
                RRQueue.front().ServerTime -= p;
                RRQueue.push(RRQueue.front());
                RRQueue.pop();
            } else {// 运行结束
                currentTime += RRQueue.front().ServerTime;
                RRQueue.front().ServerTime = 0;
                printRow(currentTime, RRQueue.front().name, "运行结束");
                RRQueue.front().StopTime = currentTime;
                finishedProcess.push_back(RRQueue.front());
                RRQueue.pop();
            }
        } else {
            RRQueue.push(RRQueue.front());
            RRQueue.pop();
        }
    }
}

void output() {
    int cell_width = 20;
    std::sort(finishedProcess.begin(), finishedProcess.end());
    double averageTurnaroundTime = 0;
    double averageTurnaroundTimeWithWeight = 0;
    for(int i = 0; i < n; i++) {
        finishedProcess[i].ServerTime = processList[i].ServerTime;
        finishedProcess[i].turnaroundTime = finishedProcess[i].StopTime - finishedProcess[i].ArrivalTime;
        averageTurnaroundTime += finishedProcess[i].turnaroundTime;
        finishedProcess[i].TurnaroundTimeWithWeight = finishedProcess[i].turnaroundTime * 1.0 / finishedProcess[i].ServerTime;
        averageTurnaroundTimeWithWeight += finishedProcess[i].TurnaroundTimeWithWeight;
    }

    std::cout << std::left << std::setfill(' ');
    std::cout << std::setw(cell_width) << "ProcessName"
              << std::setw(cell_width) << "ArrivalTime"
              << std::setw(cell_width) << "ServeTime"
              << std::setw(cell_width) << "EndTime"
              << std::setw(cell_width) << "TurnaroundTime"
              << std::setw(cell_width) << "TurnaroundTimeWithWeight" << std::endl;
    for(auto item : finishedProcess) {
        std::cout << std::setw(cell_width) << item.name
                  << std::setw(cell_width) << item.ArrivalTime
                  << std::setw(cell_width) << item.ServerTime
                  << std::setw(cell_width) << item.StopTime
                  << std::setw(cell_width) << item.turnaroundTime
                  << std::setw(cell_width) << item.TurnaroundTimeWithWeight
                  << std::endl;
    }
    std::cout << "平均周转时间：" << averageTurnaroundTime / n << std::endl;
    std::cout << "平均带权周转时间：" << averageTurnaroundTimeWithWeight / n;
}

int main() {
    input();
    RR();
    output();
    return 0;
}
