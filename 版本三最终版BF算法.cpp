#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


char my_tolower(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

bool my_isalpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool my_isspace(char c) {
    return c == ' ';
}

int my_min(int a, int b) {
    return a < b ? a : b;
}

// ===================== 【BF 算法：完全保留】 =====================
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

// ===================== 文本预处理（已改用手写函数） =====================
string preProcess(const string &text) {
    string res;
    for (char ch : text) {
        if (my_isalpha(ch)) {
            res += my_tolower(ch);
        } else if (my_isspace(ch)) {
            res += ' ';
        }
    }
    return res;
}

// ===================== 分词：手写实现，去掉 stringstream =====================
vector<string> splitWords(const string &text) {
    vector<string> words;
    string word;
    for (char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) words.push_back(word);
    return words;
}

// ===================== 分句：完全不变 =====================
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

// ===================== 词频统计：完全不变 =====================
map<string, int> countFreq(const vector<string> &words) {
    map<string, int> freq;
    for (const string &w : words) freq[w]++;
    return freq;
}

// ===================== 查找单词：完全不变 =====================
int searchWord(const map<string, int> &freq, const string &key) {
    for (auto &p : freq) {
        if (p.first == key) return p.second;
    }
    return 0;
}

// ===================== 基础统计：完全不变 =====================
void showBasicInfo(const string &text, const vector<string> &words) {
    vector<string> sentences = splitSentences(text);
    cout << "\n===== 基础统计 =====" << endl;
    cout << "总字符数：" << text.size() << endl;
    cout << "总单词数：" << words.size() << endl;
    cout << "总句子数：" << sentences.size() << endl;
}

// ===================== 显示词频：完全不变 =====================
void showFreq(const map<string, int> &freq) {
    cout << "\n===== 词频统计 =====" << endl;
    for (const auto &p : freq) {
        cout << p.first << " : " << p.second << endl;
    }
}

// ===================== 关键词：完全不变 =====================
void showKeyWords(const map<string, int> &freq) {
    cout << "\n关键词（前5）：";
    int count = 0;
    for (const auto &p : freq) {
        if (count++ >= 5) break;
        cout << p.first << " ";
    }
    cout << endl;
}

// ===================== 摘要：改用手写 min =====================
void showSummary(const string &text) {
    vector<string> sentences = splitSentences(text);
    cout << "\n===== 文本摘要 =====" << endl;
    int n = my_min(2, (int)sentences.size());
    for (int i = 0; i < n; i++) {
        cout << sentences[i] << endl;
    }
}

// ===================== 语法检查模块：完全不变 =====================
struct ErrorInfo {
    string type; int lineNo; string content; string hint;
};
map<string, bool> initDict() {
    map<string, bool> dict;
    vector<string> baseWords = {"i","you","he","she","is","am","are","good","bad","book","school"};
    for (auto w : baseWords) dict[w]=1;
    return dict;
}
vector<ErrorInfo> checkSpelling(const vector<string>& words, map<string, bool>& dict) {
    vector<ErrorInfo> e;
    for(int i=0;i<words.size();i++){
        string w=words[i];
        for (int j=0;j<w.size();j++) w[j] = my_tolower(w[j]);
        if(!dict.count(w)) e.push_back({"拼写错误",i+1,words[i],"不在词典"});
    }
    return e;
}
vector<ErrorInfo> checkSentenceStart(const vector<string>& s){
    vector<ErrorInfo> e;
    for(int i=0;i<s.size();i++)
        if(!s[i].empty() && s[i][0] >= 'a' && s[i][0] <= 'z')
            e.push_back({"句首未大写",i+1,s[i],"首字母大写"});
    return e;
}
vector<ErrorInfo> checkSentenceEnd(const vector<string>& s){
    vector<ErrorInfo> e;
    for(int i=0;i<s.size();i++)
        if(!s[i].empty()){
            char c=s[i].back();
            if(c!='.'&&c!='?'&&c!='!')
                e.push_back({"句末无标点",i+1,s[i],"加.?!结尾"});
        }
    return e;
}
void showGrammarErrors(vector<ErrorInfo> errors){
    if(errors.empty()){cout<<"\n? 无语法错误\n";return;}
    cout<<"\n===== 语法错误 =====\n";
    for(auto& e:errors)
        cout<<"["<<e.type<<"] "<<e.lineNo<<"："<<e.content<<" → "<<e.hint<<endl;
}
void grammarCheck(string text, vector<string> words){
    auto dict=initDict(); auto s=splitSentences(text);
    auto e1=checkSpelling(words,dict),e2=checkSentenceStart(s),e3=checkSentenceEnd(s);
    vector<ErrorInfo> all;
    all.insert(all.end(),e1.begin(),e1.end());
    all.insert(all.end(),e2.begin(),e2.end());
    all.insert(all.end(),e3.begin(),e3.end());
    showGrammarErrors(all);
}

// ===================== 菜单：完全不变 =====================
void menu() {
    cout << "\n===== 智能文本处理系统（含BF算法） =====" << endl;
    cout << "1. 输入文本" << endl;
    cout << "2. 基础统计" << endl;
    cout << "3. 分词结果" << endl;
    cout << "4. 词频统计" << endl;
    cout << "5. 关键词提取" << endl;
    cout << "6. 文本摘要" << endl;
    cout << "7. 查找单词" << endl;
    cout << "8. 查找替换" << endl;
    cout << "9. 语法检查" << endl;
    cout << "0. 退出" << endl;
    cout << "请输入选项：";
}

// ===================== 主函数：完全不变 =====================
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
                cout << "文本处理完成！" << endl;
                break;
            }
            case 2:
                if (words.empty()) cout << "请先输入文本！" << endl;
                else showBasicInfo(text, words);
                break;
            case 3:
                if (words.empty()) cout << "请先输入文本！" << endl;
                else {
                    cout << "\n===== 分词结果 =====" << endl;
                    for (const string& w : words) cout << w << " ";
                    cout << endl;
                }
                break;
            case 4:
                if (freq.empty()) cout << "请先输入文本！" << endl;
                else showFreq(freq);
                break;
            case 5:
                if (freq.empty()) cout << "请先输入文本！" << endl;
                else showKeyWords(freq);
                break;
            case 6:
                if (text.empty()) cout << "请先输入文本！" << endl;
                else showSummary(text);
                break;
            case 7: {
                if (freq.empty()) { cout << "请先输入文本！" << endl; break; }
                string key; cout << "请输入要查找的单词："; cin >> key;
                for (int i=0;i<key.size();i++) key[i] = my_tolower(key[i]);
                cout << "出现次数：" << searchWord(freq, key) << endl;
                break;
            }
            case 8: {
                if (text.empty()) { cout << "请先输入文本！" << endl; break; }
                string oldS, newS; cout << "查找："; cin >> oldS;
                cout << "替换为："; cin >> newS;
                text = replaceTextBF(text, oldS, newS);
                string clean = preProcess(text);
                words = splitWords(clean);
                freq = countFreq(words);
                cout << "替换完成！" << endl;
                break;
            }
            case 9:
                grammarCheck(text, words);
                break;
            default:
                cout << "无效选项！" << endl;
        }
    }
    return 0;
}
