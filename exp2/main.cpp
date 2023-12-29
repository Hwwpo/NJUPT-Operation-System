#include <iostream>
#include <iomanip>
#include <limits>
#include "process.h"

static int N;   // 进程数
static int M;   // 资源数

// 暂停
void pause() {
    std::cout << "Press any key to continue..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// 打印菜单
void printMenu() {
    int cell_width = 30;
    std::cout << std::right <<std::setfill('-');
    std::cout << std::setw(cell_width) << "0.Exit" << std::endl;
    std::cout << std::setw(cell_width) << "1.Initialize Resource" << std::endl;
    std::cout << std::setw(cell_width) << "2.Show resource usage" << std::endl;
    std::cout << std::setw(cell_width) << "3.Allocate resource" << std::endl;
    std::cout << std::setw(cell_width) << "4.Undo Job" << std::endl;
}

// 检查输入，非法则提示并重新输入
bool checkInput(int _input, int floor, int ceiling = std::numeric_limits<int>::max()) {
    if(_input >= floor && _input <= ceiling)
        return true;
    return false;
}

// 输入内容
void Input(const std::string& str, int &n, int floor, int ceiling = std::numeric_limits<int>::max()) {
    std::cout << str;
    std::cin >> n;
    while (!checkInput(n, floor, ceiling)) {
        std::cout << "Input error!" << std::endl;
        std::cout << str;
        std::cin >> n;
    }
}

// 初始化资源
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
            int x = Max[j] - Allocation[j];
            Need.push_back(x);
        }
        // 新建进程
        process tmp(i, Max, Allocation, Need);
        A.push_back(tmp);
    }
    std::cout << "Please enter the number of Available resource:" << std::endl;
    for(int i = 0; i < M; i++) {
        int x; std::cin >> x;
        B.push_back(x);
    }
}

// 打印系统占用资源与空闲资源
void printResource(const std::vector<process> &A, const std::vector<int> &B) {
    int cell_width = 15;
    std::cout << std::left << std::setfill(' ') << std::endl;
    std::cout << std::setw(cell_width) << ""
              << std::setw(cell_width) << "Max"
              << std::setw(cell_width) << "Allocation"
              << std::setw(cell_width) << "Need"
              << std::setw(cell_width) << "Available";
    std::cout << std::endl << std::setw(cell_width) << "Process";
    std::string str;
    for(int i = 0; i < M; i++) {
        str += char(i + 65);
        str += "  ";
    }
    for(int i = 0; i < 4; i++) {
        std::cout << std::setw(cell_width) << str;
    }
    std::cout << std::endl;
    int flag = 1;
    for(const auto& item:A) {
        item.Display(cell_width);
        if(flag-- == 1)
            std::cout << std::setw(cell_width) << vector2string(B);
        std::cout << std::endl;
    }
    pause();
}

// 根据id获取进程
process &get_process(std::vector<process> &A, int id) {
    for(auto &item:A)
        if(item.id == id)
            return item;
}

// 比较vector
bool cp_Need_Available(const std::vector<int> &_need, const std::vector<int> &_available) {
    for(int i = 0; i < _need.size(); i++)
        if(_need[i] > _available[i])
            return false;
    return true;
}

// vector加法
void ad_Allocation_Available(std::vector<int> &_allocation, std::vector<int> &_available) {
    for(int i = 0; i < _available.size(); i++)
        _available[i] += _allocation[i];
}

// vector减法
void minus_Allocation_Available(std::vector<int> &_allocation, std::vector<int> &_available) {
    for(int i = 0; i < _available.size(); i++)
        _available[i] -= _allocation[i];
}

// DFS求解银行家算法
bool banker(std::vector<process> &A, std::vector<int> Available, int layer) {
    if (layer != N) {
        // 递归层数还未到结果层
        for (int i = 0; i < N; i++) {   // 遍历每一个进程
            // 如果可以分配并且还未分配
            if (cp_Need_Available(A[i].Need, Available) && !A[i].finish) {
                // 假设分配资源
                ad_Allocation_Available(A[i].Allocation, Available);
                A[i].finish = true;
                // DFS
                if (banker(A, Available, layer + 1))
                    return true;
                // 还原现场
                minus_Allocation_Available(A[i].Allocation, Available);
                A[i].finish = false;
            }
        }
        return false;
    } else {
        bool ans = true;
        // 当所有进程的finish都为true则安全
        for (auto &i: A)
            ans &= i.finish;
        return ans;
    }
}

// 分配资源功能
void allocate_Resource(std::vector<process> &A, std::vector<int> &B) {
    int input;  // 用户输入的进程id
    std::vector<int> request;   // 进程请求的资源

    // 输入资源
    Input("Please input the process id:\n", input, 0, N-1);
    std::cout << "Please input the request resource:\n";
    for(int i = 0; i < M; i++) {
        int x; std::cin >> x;
        request.push_back(x);
    }

    // 获取进程
    process &p = get_process(A, input);
    // 判断所申请的资源大于其所需资源或大于系统此时的可利用资源
    if(cp_Need_Available(p.Need, request)
        || cp_Need_Available(B, request)) {
        std::cout << "Unreasonable allocation" << std::endl;
        return;
    }
    else {
        // 尝试分配
        minus_Allocation_Available(request, B);
        ad_Allocation_Available(request, p.Allocation);
        minus_Allocation_Available(request, p.Need);
        // 检查安全性
        if(banker(A, B, 0)) {
            std::cout << "Allocate successfully!" << std::endl;
            printResource(A, B);
        }
        else {
            // 不安全则给出提示并还原分配
            std::cout << "Unsecure allocation" << std::endl;
            ad_Allocation_Available(request, B);
            minus_Allocation_Available(request, p.Allocation);
            ad_Allocation_Available(request, p.Need);
        }
    }
}
void undoJob(std::vector<process> &A, std::vector<int> &B) {
    int input;
    Input("Please input the process id:\n", input, 0, N-1);
    process &p = get_process(A, input);
    ad_Allocation_Available(p.Allocation, B);
    auto it = std::find_if(A.begin(), A.end(), [input](const process& p) {
        return p.id == input;
    });
    if (it != A.end()) {
        A.erase(it);
    }
}

void run() {
    int input;
    std::vector<process> processes;
    std::vector<int> available;
    while(true) {
        printMenu();
        Input("Please choose the function:", input, 0, 4);
        switch (input) {
            // 结束
            case 0:
                return;
            // 初始化
            case 1:
                initialize_Resource(processes, available);
                break;
            // 打印进程资源信息
            case 2:
                printResource(processes, available);
                break;
            // 分配资源
            case 3:
                allocate_Resource(processes, available);
                break;
            // 撤销资源
            case 4:
                undoJob(processes, available);
                break;
            default:
                break;
        }
    }
}

int main() {
    run();
    return 0;
}
