#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

// 1. 文本预处理：转小写 + 去除标点
string preProcess(const string &text) {
    string res;
    for (char ch : text) {
        if (isalpha(ch)) {
            // 字母转小写
            res += tolower(ch);
        } else if (isspace(ch)) {
            // 保留空格用于分词
            res += ' ';
        }
        // 其余符号直接过滤
    }
    return res;
}

// 2. 分词：按空格分割
vector<string> splitWords(const string &text) {
    vector<string> words;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

// 3. 词频统计
map<string, int> countFreq(const vector<string> &words) {
    map<string, int> freq;
    for (const string &w : words) {
        freq[w]++;
    }
    return freq;
}

// 4. 查找关键词出现次数
int searchWord(const map<string, int> &freq, const string &key) {
    auto it = freq.find(key);
    if (it != freq.end())
        return it->second;
    return 0;
}

// 5. 显示词频结果
void showFreq(const map<string, int> &freq) {
    cout << "\n===== 词频统计结果 =====" << endl;
    for (auto &p : freq) {
        cout << p.first << " : " << p.second << endl;
    }
}

// 主菜单
void menu() {
    cout << "\n===== 智能文本处理系统 =====" << endl;
    cout << "1. 输入文本并处理" << endl;
    cout << "2. 查看分词结果" << endl;
    cout << "3. 查看词频统计" << endl;
    cout << "4. 查找关键词" << endl;
    cout << "0. 退出" << endl;
    cout << "请输入选项：";
}

int main() {
    vector<string> words;    // 分词结果
    map<string, int> freq;   // 词频
    string text;             // 原始文本
    int choice;

    while (true) {
        menu();
        cin >> choice;
        cin.ignore(); // 吸收换行

        switch (choice) {
            case 1: {
                cout << "\n请输入一段文本：" << endl;
                getline(cin, text);

                // 预处理 + 分词
                string clean = preProcess(text);
                words = splitWords(clean);
                freq = countFreq(words);

                cout << "? 文本预处理与分词完成！" << endl;
                break;
            }

            case 2: {
                if (words.empty()) {
                    cout << "? 请先输入文本！" << endl;
                    break;
                }
                cout << "\n===== 分词结果 =====" << endl;
                for (int i = 0; i < words.size(); i++) {
                    cout << i + 1 << ". " << words[i] << endl;
                }
                break;
            }

            case 3: {
                if (freq.empty()) {
                    cout << "? 请先输入文本！" << endl;
                    break;
                }
                showFreq(freq);
                break;
            }

            case 4: {
                if (freq.empty()) {
                    cout << "? 请先输入文本！" << endl;
                    break;
                }
                string key;
                cout << "请输入要查找的单词：";
                cin >> key;
                // 转小写统一匹配
                transform(key.begin(), key.end(), key.begin(), ::tolower);
                int cnt = searchWord(freq, key);
                cout << "单词 \"" << key << "\" 出现次数：" << cnt << endl;
                break;
            }

            case 0:
                cout << "?? 退出系统！" << endl;
                return 0;

            default:
                cout << "? 无效选项！" << endl;
        }
    }
}
