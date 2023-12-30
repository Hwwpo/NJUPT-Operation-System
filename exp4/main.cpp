#include <iostream>
#include <vector>

void input(int &pos, std::vector<int> &vector1, int &length);

void FCFS(int pos, std::vector<int> vector1, int length);

void SSTF(int &pos, std::vector<int> &vector1, int &length);

int findClosestElement(int &pos, std::vector<int> &vector1);

int main() {
    int currentPos = 0;
    int requestQueueLength;
    std::vector<int> requestQueue;
    input(currentPos, requestQueue, requestQueueLength);
    FCFS(currentPos, requestQueue, requestQueueLength);
    SSTF(currentPos, requestQueue, requestQueueLength);
    return 0;
}

void SSTF(int &pos, std::vector<int> &vector1, int &length) {
    int seekCount = 0;  // 寻道数
    int index = 0;
    std::vector<int> seq;   // 寻道序列
    seq.reserve(vector1.size() + 1);
    seq.push_back(pos);
    // 当请求队列不为空
    while (!vector1.empty()) {
        // 找到最近的队列
        index = findClosestElement(pos, vector1);
        // 访问
        seq.push_back(vector1[index]);
        seekCount += abs(pos - vector1[index]);
        pos = vector1[index];
        // 移除队列
        vector1.erase(vector1.begin() + index);
    }
    std::cout << "SSTF算法:" << std::endl;
    std::cout << "寻道路径：" << seq[0];
    for(size_t i = 1; i < seq.size(); i++)
        std::cout << " -> " << seq[i];
    std::cout << std::endl << "寻道数: " << seekCount << std::endl;
}

int findClosestElement(int &pos, std::vector<int> &vector1) {
    // 遍历寻找
    int closest = vector1[0];
    int index = 0;
    int minDiff = abs(pos - closest);

    for(int i = 1; i < vector1.size(); i++) {
        int currentDiff = abs(pos - vector1[i]);
        if(currentDiff < minDiff) {
            minDiff = currentDiff;
            closest = vector1[i];
            index = i;
        }
    }
    return index;
}

void FCFS(int pos, std::vector<int> vector1, int length) {
    int seekCount = 0;
    std::vector<int> seq;
    seq.reserve(vector1.size() + 1);
    seq.push_back(pos);
    // 按顺序寻找
    for(auto item:vector1) {
        seq.push_back(item);
        seekCount += abs(pos - item);
        pos = item;
    }
    std::cout << "FCFS算法:" << std::endl;
    std::cout << "寻道路径：" << seq[0];
    for(size_t i = 1; i < seq.size(); i++)
        std::cout << " -> " << seq[i];
    std::cout << std::endl << "寻道数: " << seekCount << std::endl;
}

void input(int &pos, std::vector<int> &vector1, int &length) {
    std::cout << "请输入当前磁头所在的柱面" << std::endl;
    std::cin >> pos;

    std::cout << "请输入请求队列的长度:" << std::endl;
    std::cin >> length;

    std::cout << "请输入请求队列:" << std::endl;
    for(int i = 0; i < length; i++) {
        int x; std::cin >> x;
        vector1.push_back(x);
    }
}
