#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

// 文本预处理：转小写 + 去除标点
string preProcess(const string &text) {
    string res;
    for (char ch : text) {
        if (isalpha(ch)) {
            res += tolower(ch);
        } else if (isspace(ch)) {
            res += ' ';
        }
    }
    return res;
}

// 分词：按空格分割
vector<string> splitWords(const string &text) {
    vector<string> words;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

// 分句：按 . ? ! 分割
vector<string> splitSentences(const string &text) {
    vector<string> sentences;
    string cur;
    for (char c : text) {
        cur += c;
        if (c == '.' || c == '?' || c == '!') {
            sentences.push_back(cur);
            cur.clear();
        }
    }
    if (!cur.empty()) sentences.push_back(cur);
    return sentences;
}

// 词频统计
map<string, int> countFreq(const vector<string> &words) {
    map<string, int> freq;
    for (const string &w : words) freq[w]++;
    return freq;
}

// 查找单词出现次数
int searchWord(const map<string, int> &freq, const string &key) {
    auto it = freq.find(key);
    return (it != freq.end()) ? it->second : 0;
}

// 基础统计：字符数、单词数、句子数
void showBasicInfo(const string &text, const vector<string> &words) {
    vector<string> sentences = splitSentences(text);
    cout << "\n===== 基础统计 =====" << endl;
    cout << "总字符数：" << text.size() << endl;
    cout << "总单词数：" << words.size() << endl;
    cout << "总句子数：" << sentences.size() << endl;
}

// 显示词频
void showFreq(const map<string, int> &freq) {
    cout << "\n===== 词频统计 =====" << endl;
    for (const auto &p : freq) {
        cout << p.first << " : " << p.second << endl;
    }
}

// 显示关键词（前5个）
void showKeyWords(const map<string, int> &freq) {
    cout << "\n关键词（前5）：";
    int count = 0;
    for (const auto &p : freq) {
        if (count++ >= 5) break;
        cout << p.first << " ";
    }
    cout << endl;
}

// 文本摘要（前两句）
void showSummary(const string &text) {
    vector<string> sentences = splitSentences(text);
    cout << "\n===== 文本摘要 =====" << endl;
    int n = min(2, (int)sentences.size());
    for (int i = 0; i < n; i++) {
        cout << sentences[i] << endl;
    }
}

// 查找替换
string replaceText(string text, string oldStr, string newStr) {
    size_t pos = 0;
    while ((pos = text.find(oldStr, pos)) != string::npos) {
        text.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
    return text;
}

// 菜单
void menu() {
    cout << "\n===== 智能文本处理系统 =====" << endl;
    cout << "1. 输入文本" << endl;
    cout << "2. 基础统计" << endl;
    cout << "3. 分词结果" << endl;
    cout << "4. 词频统计" << endl;
    cout << "5. 关键词提取" << endl;
    cout << "6. 文本摘要" << endl;
    cout << "7. 查找单词" << endl;
    cout << "8. 查找替换" << endl;
    cout << "0. 退出" << endl;
    cout << "请输入选项：";
}

int main() {
    string text;
    vector<string> words;
    map<string, int> freq;

    cout << "===== 欢迎使用文本处理系统 =====" << endl;

    while (true) {
        menu();
        int op;
        cin >> op;
        cin.ignore();

        if (op == 0) {
            cout << "程序结束" << endl;
            break;
        }

        switch (op) {
            case 1: {
                cout << "\n请输入文本：" << endl;
                getline(cin, text);
                string clean = preProcess(text);
                words = splitWords(clean);
                freq = countFreq(words);
                cout << "? 文本处理完成！" << endl;
                break;
            }
            case 2:
                if (words.empty()) cout << "? 请先输入文本！" << endl;
                else showBasicInfo(text, words);
                break;
            case 3:
                if (words.empty()) cout << "? 请先输入文本！" << endl;
                else {
                    cout << "\n===== 分词结果 =====" << endl;
                    for (const string &w : words) cout << w << " ";
                    cout << endl;
                }
                break;
            case 4:
                if (freq.empty()) cout << "? 请先输入文本！" << endl;
                else showFreq(freq);
                break;
            case 5:
                if (freq.empty()) cout << "? 请先输入文本！" << endl;
                else showKeyWords(freq);
                break;
            case 6:
                if (text.empty()) cout << "? 请先输入文本！" << endl;
                else showSummary(text);
                break;
            case 7: {
                if (freq.empty()) {
                    cout << "? 请先输入文本！" << endl;
                    break;
                }
                string key;
                cout << "请输入要查找的单词：";
                cin >> key;
                transform(key.begin(), key.end(), key.begin(), ::tolower);
                cout << "出现次数：" << searchWord(freq, key) << endl;
                break;
            }
            case 8: {
                if (text.empty()) {
                    cout << "? 请先输入文本！" << endl;
                    break;
                }
                string oldS, newS;
                cout << "查找：";
                cin >> oldS;
                cout << "替换为：";
                cin >> newS;
                text = replaceText(text, oldS, newS);
                // 更新预处理后的数据
                string clean = preProcess(text);
                words = splitWords(clean);
                freq = countFreq(words);
                cout << "? 替换完成！" << endl;
                break;
            }
            default:
                cout << "? 无效选项！" << endl;
        }
    }
    return 0;
}
