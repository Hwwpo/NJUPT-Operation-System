#include <iostream>
#include <iomanip>
#include <limits>
#include "process.h"

static int N;
static int M;

void printMenu() {
    int cell_width = 20;
    std::cout << std::left <<std::setfill('-');
    std::cout << std::endl << std::setw(cell_width) << "" << std::endl;
    std::cout << std::setw(cell_width) << "0.Exit" << std::endl;
    std::cout << std::setw(cell_width) << "1.Initialize Resource" << std::endl;
    std::cout << std::setw(cell_width) << "2.Show resource usage" << std::endl;
}

bool checkInput(int _input, int floor, int ceiling = std::numeric_limits<int>::max()) {
    if(_input >= floor && _input <= ceiling)
        return true;
    return false;
}

void Input(const std::string& str, int &n, int floor, int ceiling = std::numeric_limits<int>::max()) {
    std::cout << str;
    std::cin >> n;
    while (!checkInput(n, floor, ceiling)) {
        std::cout << "Input error!" << std::endl;
        std::cout << str;
        std::cin >> n;
    }
}

void initialize_Resource(std::vector<process> &A, std::vector<int> &B) {
    Input("Please enter the number of task:", N, 0);
    Input("Please enter the number of resource:", M, 0);
    for(int i = 0; i < N; i++) {
        std::cout << "Please Input Max, Allocation and Need of process " << i << std::endl;
        std::vector<int> Max, Allocation, Need;
        for(int j = 0; j < M; j++) {
            int x; std::cin >> x;
            Max.push_back(x);
        }
        for(int j = 0; j < M; j++) {
            int x; std::cin >> x;
            Allocation.push_back(x);
        }
        for(int j = 0; j < M; j++) {
            int x = Max[i] - Allocation[i];
            Need.push_back(x);
        }
        process tmp(i, Max, Allocation, Need);
        A.push_back(tmp);
    }
    std::cout << "Please enter the number of Available resource:";
    for(int i = 0; i < M; i++) {
        int x; std::cin >> x;
        B.push_back(x);
    }
}

void run() {
    int input;
    std::vector<process> processes;
    std::vector<int> available;
    while(true) {
        printMenu();
        Input("Please choose the function:", input, 0, 2);
        switch (input) {
            // 结束
            case 0:
                return;
            // 初始化
            case 1:
                initialize_Resource(processes, available);
                break;
            // TODO: 打印进程资源信息
            case 2:

            default:
                break;
        }
    }
}

int main() {
    run();
    return 0;
}
