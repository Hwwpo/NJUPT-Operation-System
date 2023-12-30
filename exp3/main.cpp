#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>


class Page {
public:
    int id;                     // 页面id
    int entryTime;              // 进入内存时间
    int lastUsedTime;           // 最后被使用时间
    int futureLastUsedTime{};   // 未来第一次被使用时间

public:
    // 构造函数
    Page(int id, int entryTime, int lastUsedTime) : id(id), entryTime(entryTime), lastUsedTime(lastUsedTime) {
    }
    // 两个页面只要id相等则他们相等
    bool operator==(const Page &other) const {
        return id == other.id;
    }
};
// 为Page编写hash函数
namespace std {
    template<>
    struct hash<Page> {
        size_t operator()(const Page &page) const {
            // 使用 std::hash 来哈希 id 字段
            return std::hash<int>()(page.id);
        }
    };
}

// 用户输入
void input(int &blocks, int &referenceString, std::vector<int> &vector1);
// 模拟置换步骤
void pageReplaceAlgorithm(int &numberPhysicalBlocks, std::vector<int> &referenceString, int algorithm);
// 向结果表中添加当前步骤的结果
void addBlock(const std::vector<Page> &_blocks, std::vector<std::vector<char>> &table, int blocksNumber, bool isMissing);
// OPT页面置换算法
int OPT(std::vector<Page> &blocks, std::vector<int> &referenceString, int pos);
// FIFO页面置换算法
int FIFO(std::vector<Page> &_blocks);
// LRU页面置换算法
int LRU(std::vector<Page> &_blocks);
// 打印结果表
void printTable(const std::vector<std::vector<char>> &table, const std::vector<int> &referenceString);

int main() {
    int numberPhysicalBlocks;   // 物理块数量
    int lengthPageNumberReferenceString;    // 页面引用串长度
    std::vector<int> pageNumberReferenceString; // 页面引用串内容
    input(numberPhysicalBlocks, lengthPageNumberReferenceString, pageNumberReferenceString);
    for(int i = 1; i < 4; i++)
        // 分别调用不同算法
        pageReplaceAlgorithm(numberPhysicalBlocks, pageNumberReferenceString, i);
    return 0;
}

void pageReplaceAlgorithm(int &numberPhysicalBlocks, std::vector<int> &referenceString, int algorithm) {
    int count = 1;  // 步骤计数器
    int missCount = 0;  // 缺页计数器
    std::vector<Page> blocks;   // 物理块模拟
    std::vector<std::vector<char>> table(numberPhysicalBlocks + 1); // 结果表
    // 打印算法标题
    switch (algorithm) {
        case 1:
            std::cout << "OPT算法：" << std::endl;
            break;
        case 2:
            std::cout << "FIFO算法：" << std::endl;
            break;
        case 3:
            std::cout << "LRU算法：" << std::endl;
            break;
        default:
            break;
    }
    // 开始模拟
    for(int i = 0; i < referenceString.size(); i++, count++) {
        // 进入一个新页面
        int pageId = referenceString[i];
        bool isExist = false;

        // 如果在内存中，直接访问
        for(auto &page : blocks)
            if(page.id == pageId) {
                page.lastUsedTime = count;
                isExist = true;
                break;
            }
        if (isExist) {
            addBlock(blocks, table, numberPhysicalBlocks, false);
            continue;
        }

        // 如果不存在且还未填满
        if(blocks.size() < numberPhysicalBlocks) {
            Page page(pageId, count, count);
            blocks.push_back(page);
            missCount++;
            addBlock(blocks, table, numberPhysicalBlocks, true);
            continue;
        }
        // 如果不存在且已填满
        int replacePos;
        // 调用算法获取置换位置
        switch (algorithm) {
            case 1:
                replacePos = OPT(blocks, referenceString, i + 1);
                break;
            case 2:
                replacePos = FIFO(blocks);
                break;
            case 3:
                replacePos = LRU(blocks);
                break;
            default:
                replacePos = 0;
                break;
        }
        Page page(pageId, count, count);
        blocks[replacePos] = page;
        missCount++;
        addBlock(blocks, table, numberPhysicalBlocks, true);
    }
    // 打印结果
    printTable(table, referenceString);
    std::cout << "缺页次数:" << missCount << "  缺页率:" << missCount * 1.0 / referenceString.size() << std::endl;
}

int LRU(std::vector<Page> &_blocks) {
    // 寻找最小使用时间即可
    auto min_it = std::min_element(_blocks.begin(), _blocks.end(),
                                   [](const Page &A, const Page &B) {
                                       return A.lastUsedTime < B.lastUsedTime;
    });
    return std::distance(_blocks.begin(), min_it);
}

int FIFO(std::vector<Page> &_blocks) {
    // 寻找最小的进入时间
    auto min_it = std::min_element(_blocks.begin(), _blocks.end(),
                                   [](const Page &A, const Page &B) {
                                       return A.entryTime < B.entryTime;
    });
    return std::distance(_blocks.begin(), min_it);
}

void printTable(const std::vector<std::vector<char>> &table, const std::vector<int> &referenceString) {
    int cell_width = 3;
    // 打印表头
    for(auto item:referenceString)
        std::cout << std::setw(cell_width) << item;
    std::cout << std::endl;
    // 打印内容
    for(const auto& row:table) {
        for (auto item: row)
            std::cout << std::setw(cell_width) << item;
        std::cout << std::endl;
    }
}

int OPT(std::vector<Page> &blocks, std::vector<int> &referenceString, int pos) {
    std::unordered_map<Page, bool> flag;
    // 遍历之后的每一个页面
    for(int i = pos; i < referenceString.size(); i++) {
        int pageId = referenceString[i];
        auto it = std::find_if(blocks.begin(), blocks.end(), [pageId](const Page &page) {
            return page.id == pageId;
        });
        // 如果该页面在内存中，更新使用时间
        if(it != blocks.end()) {
            Page &page = *it;
            if(!flag[page]){
                // 只找第一次使用的
                page.futureLastUsedTime = i;
                flag[page] = true;
            }
        }
    }
    // 如果以后再也没有使用，就假设在引用串最后一项结束后使用
    for(auto &item:blocks) {
        if(!flag[item])
            item.futureLastUsedTime = int(referenceString.size());
    }
    // 寻找最大的第一次使用时间
    auto max_it = std::max_element(blocks.begin(), blocks.end(),
                                   [](const Page &A, const Page &B) {
        return A.futureLastUsedTime < B.futureLastUsedTime;
    });

    return int(std::distance(blocks.begin(), max_it));
}

void addBlock(const std::vector<Page> &_blocks, std::vector<std::vector<char>> &table, int blocksNumber, bool isMissing) {
    int i = 0;
    // 缺页标记
    if(isMissing)
        table[i].push_back('*');
    else
        table[i].push_back(' ');
    for(i = 1; i <= _blocks.size(); i++)
        table[i].push_back(char(_blocks[i-1].id + 48));
    while(i <= blocksNumber)
        table[i++].push_back(char(64));
}

void input(int &blocks, int &referenceString, std::vector<int> &vector1) {
    std::cout << "请输入物理块的数量:" << std::endl;
    std::cin >> blocks;

    std::cout << "请输入页面引用串的长度:" << std::endl;
    std::cin >> referenceString;

    std::cout << "请输入页面引用串(使用空格隔开):" << std::endl;
    for(int i = 0; i < referenceString; i++) {
        int x; std::cin >> x;
        vector1.push_back(x);
    }
}
