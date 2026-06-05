#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

int BF(const string &text, const string &pattern) {
    int n = text.size();
    int m = pattern.size();
    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }
        if (j == m) return i;
    }
    return -1;
}

string replaceTextBF(string text, string oldStr, string newStr) {
    int pos = BF(text, oldStr);
    while (pos != -1) {
        text.replace(pos, oldStr.size(), newStr);
        pos = BF(text, oldStr);
    }
    return text;
}

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

vector<string> splitWords(const string &text) {
    vector<string> words;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

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

map<string, int> countFreq(const vector<string> &words) {
    map<string, int> freq;
    for (const string &w : words) freq[w]++;
    return freq;
}

int searchWord(const map<string, int> &freq, const string &key) {
    for (const auto &p : freq) {
        if (p.first == key) {
            return p.second;
        }
    }
    return 0;
}

struct ErrorInfo {
    string type;
    int lineNo;
    string content;
    string hint;
};

map<string, bool> initDict() {
    map<string, bool> dict;
    vector<string> baseWords = {"i", "you", "he", "she", "is", "am", "are", "good", "bad", "book"};
    for (auto w : baseWords) dict[w] = 1;
    return dict;
}

vector<ErrorInfo> checkSpelling(const vector<string> &words, const map<string, bool> &dict) {
    vector<ErrorInfo> e;
    for (int i = 0; i < words.size(); i++) {
        string w = words[i];
        transform(w.begin(), w.end(), w.begin(), ::tolower);
        if (!dict.count(w)) {
            e.push_back({"拼写错误", i + 1, words[i], "单词不在词典"});
        }
    }
    return e;
}

vector<ErrorInfo> checkSentenceStart(const vector<string> &s) {
    vector<ErrorInfo> e;
    for (int i = 0; i < s.size(); i++) {
        if (!s[i].empty() && islower(s[i][0])) {
            e.push_back({"句首未大写", i + 1, s[i], "首字母应大写"});
        }
    }
    return e;
}

vector<ErrorInfo> checkSentenceEnd(const vector<string> &s) {
    vector<ErrorInfo> e;
    for (int i = 0; i < s.size(); i++) {
        if (!s[i].empty()) {
            char c = s[i].back();
            if (c != '.' && c != '?' && c != '!')
                e.push_back({"句末无标点", i + 1, s[i], "需加.?!结尾"});
        }
    }
    return e;
}

void showGrammarErrors(const vector<ErrorInfo> &err) {
    if (err.empty()) {
        cout << "? 无语法错误\n";
        return;
    }
    cout << "\n===== 语法检查 =====\n";
    for (auto &e : err)
        cout << "[" << e.type << "] 位置" << e.lineNo << "：" << e.content << " → " << e.hint << endl;
}

void grammarCheck(const string &text, const vector<string> &words) {
    auto dict = initDict();
    auto s = splitSentences(text);
    auto e1 = checkSpelling(words, dict);
    auto e2 = checkSentenceStart(s);
    auto e3 = checkSentenceEnd(s);
    vector<ErrorInfo> all;
    all.insert(all.end(), e1.begin(), e1.end());
    all.insert(all.end(), e2.begin(), e2.end());
    all.insert(all.end(), e3.begin(), e3.end());
    showGrammarErrors(all);
}

void showBasicInfo(const string &text, const vector<string> &words) {
    auto s = splitSentences(text);
    cout << "\n字符数：" << text.size() << "\n单词数：" << words.size() << "\n句子数：" << s.size() << endl;
}

void showFreq(const map<string, int> &freq) {
    cout << "\n===== 词频 =====\n";
    for (auto &p : freq) cout << p.first << "：" << p.second << endl;
}

void showKeyWords(const map<string, int> &freq) {
    cout << "\n关键词：";
    int c = 0;
    for (auto &p : freq) {
        if (c++ >= 5) break;
        cout << p.first << " ";
    }
    cout << endl;
}

void showSummary(const string &text) {
    auto s = splitSentences(text);
    cout << "\n摘要：\n";
    for (int i = 0; i < min(2, (int)s.size()); i++)
        cout << s[i] << endl;
}
