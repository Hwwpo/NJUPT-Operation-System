#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>


class Page {
public:
    int id;
    int entryTime;
    int lastUsedTime;
    int futureLastUsedTime{};

public:
    Page(int id, int entryTime, int lastUsedTime) : id(id), entryTime(entryTime), lastUsedTime(lastUsedTime) {
    }
    bool operator==(const Page &other) const {
        return id == other.id;
    }
};
namespace std {
    template<>
    struct hash<Page> {
        size_t operator()(const Page &page) const {
            // 使用 std::hash 来哈希 id 字段
            return std::hash<int>()(page.id);
        }
    };
}


void input(int &blocks, int &referenceString, std::vector<int> &vector1);

void pageReplaceAlgorithm(int &numberPhysicalBlocks, std::vector<int> &referenceString, int algorithm);

void addBlock(const std::vector<Page> &_blocks, std::vector<std::vector<char>> &table, int blocksNumber, bool isMissing);

int OPT(std::vector<Page> &blocks, std::vector<int> &referenceString, int pos);

void printTable(const std::vector<std::vector<char>> &table, const std::vector<int> &referenceString);

int FIFO(std::vector<Page> &_blocks);

int LRU(std::vector<Page> &_blocks);

int main() {
    int numberPhysicalBlocks;
    int lengthPageNumberReferenceString;
    std::vector<int> pageNumberReferenceString;
    input(numberPhysicalBlocks, lengthPageNumberReferenceString, pageNumberReferenceString);
    for(int i = 1; i < 4; i++)
        pageReplaceAlgorithm(numberPhysicalBlocks, pageNumberReferenceString, i);
    return 0;
}

void pageReplaceAlgorithm(int &numberPhysicalBlocks, std::vector<int> &referenceString, int algorithm) {
    int count = 1;
    int missCount = 0;
    std::vector<Page> blocks;
    std::vector<std::vector<char>> table(numberPhysicalBlocks + 1);
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
    for(int i = 0; i < referenceString.size(); i++, count++) {
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
        int replacePos;
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
//        int replacePos = OPT(blocks, referenceString, i + 1);
//        int replacePos = FIFO(blocks);
        Page page(pageId, count, count);
        blocks[replacePos] = page;
        missCount++;
        addBlock(blocks, table, numberPhysicalBlocks, true);
    }
    printTable(table, referenceString);
    std::cout << "缺页次数:" << missCount << "  缺页率:" << missCount * 1.0 / referenceString.size() << std::endl;
}

int LRU(std::vector<Page> &_blocks) {
    auto min_it = std::min_element(_blocks.begin(), _blocks.end(),
                                   [](const Page &A, const Page &B) {
                                       return A.lastUsedTime < B.lastUsedTime;
    });
    return std::distance(_blocks.begin(), min_it);
}

int FIFO(std::vector<Page> &_blocks) {
    auto min_it = std::min_element(_blocks.begin(), _blocks.end(),
                                   [](const Page &A, const Page &B) {
                                       return A.entryTime < B.entryTime;
    });
    return std::distance(_blocks.begin(), min_it);
}

void printTable(const std::vector<std::vector<char>> &table, const std::vector<int> &referenceString) {
    int cell_width = 3;
    for(auto item:referenceString)
        std::cout << std::setw(cell_width) << item;
    std::cout << std::endl;
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
                page.futureLastUsedTime = i;
                flag[page] = true;
            }
        }
    }
    for(auto &item:blocks) {
        if(!flag[item])
            item.futureLastUsedTime = int(referenceString.size());
    }
    auto max_it = std::max_element(blocks.begin(), blocks.end(),
                                   [](const Page &A, const Page &B) {
        return A.futureLastUsedTime < B.futureLastUsedTime;
    });

    return int(std::distance(blocks.begin(), max_it));
}

void addBlock(const std::vector<Page> &_blocks, std::vector<std::vector<char>> &table, int blocksNumber, bool isMissing) {
    int i = 0;
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
