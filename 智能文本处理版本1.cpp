#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cctype>

using namespace std;

// 转小写
string toLower(string s) {
    for (char& c : s) c = tolower(c);
    return s;
}

// 去除标点
string removePunct(const string& s) {
    string res;
    for (char c : s) {
        if (isalpha(c) || c == ' ') res += c;
    }
    return res;
}

// 分词
vector<string> splitWords(const string& s) {
    vector<string> words;
    stringstream ss(s);
    string word;
    while (ss >> word) words.push_back(word);
    return words;
}

// 分句
vector<string> splitSentences(const string& s) {
    vector<string> sentences;
    string cur;
    for (char c : s) {
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
map<string, int> countFreq(const vector<string>& words) {
    map<string, int> freq;
    for (const string& w : words) freq[w]++;
    return freq;
}

// 自定义比较函数：按词频降序（无Lambda）
bool cmpByFreqDesc(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
}

// 按词频排序（无Lambda版）
vector<pair<string, int>> sortFreq(map<string, int> freq) {
    vector<pair<string, int>> vec(freq.begin(), freq.end());
    sort(vec.begin(), vec.end(), cmpByFreqDesc);
    return vec;
}

// 基本统计信息
void showBasicInfo(const string& text) {
    int chars = text.size();
    vector<string> words = splitWords(toLower(removePunct(text)));
    vector<string> sentences = splitSentences(text);

    cout << "\n===== 基础统计 =====" << endl;
    cout << "总字符数：" << chars << endl;
    cout << "总单词数：" << words.size() << endl;
    cout << "总句子数：" << sentences.size() << endl;
}

// 显示词频
void showWordFreq(const vector<string>& words) {
    auto freq = countFreq(words);
    auto sorted = sortFreq(freq);
    cout << "\n===== 词频统计 =====" << endl;
    for (const auto& p : sorted) {
        cout << p.first << " : " << p.second << endl;
    }
}

// 提取关键词
void showKeyWords(const vector<string>& words) {
    auto freq = countFreq(words);
    auto sorted = sortFreq(freq);
    cout << "\n关键词（前5）：";
    for (int i = 0; i < min(5, (int)sorted.size()); ++i) {
        cout << sorted[i].first << " ";
    }
    cout << endl;
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

// 简单摘要
void showSummary(const string& text) {
    vector<string> sentences = splitSentences(text);
    if (sentences.empty()) return;

    cout << "\n===== 文本摘要 =====" << endl;
    int n = min(2, (int)sentences.size());
    for (int i = 0; i < n; ++i) {
        cout << sentences[i] << endl;
    }
}

// 主菜单
int main() {
    string text;
    cout << "===== 基础文本分析系统 =====" << endl;
    cout << "请输入一段文本：" << endl;
    getline(cin, text);

    string cleanText = toLower(removePunct(text));
    vector<string> words = splitWords(cleanText);

    int op;
    while (true) {
        cout << "\n===== 功能菜单 =====" << endl;
        cout << "1. 基础统计（字符/单词/句子）" << endl;
        cout << "2. 分词结果" << endl;
        cout << "3. 词频统计" << endl;
        cout << "4. 关键词提取" << endl;
        cout << "5. 文本摘要" << endl;
        cout << "6. 查找替换" << endl;
        cout << "0. 退出" << endl;
        cout << "请选择功能：";
        cin >> op;
        cin.ignore();

        if (op == 0) break;

        switch (op) {
            case 1:
                showBasicInfo(text);
                break;
            case 2:
                cout << "\n分词结果：" << endl;
                for (const string& w : words) cout << w << "  ";
                cout << endl;
                break;
            case 3:
                showWordFreq(words);
                break;
            case 4:
                showKeyWords(words);
                break;
            case 5:
                showSummary(text);
                break;
            case 6: {
                string oldS, newS;
                cout << "请输入要查找的词：";
                cin >> oldS;
                cout << "替换为：";
                cin >> newS;
                text = replaceText(text, oldS, newS);
                cout << "替换完成！" << endl;
                cleanText = toLower(removePunct(text));
                words = splitWords(cleanText);
                break;
            }
            default:
                cout << "输入错误，请重新选择！" << endl;
        }
    }
    cout << "程序结束" << endl;
    return 0;
}
