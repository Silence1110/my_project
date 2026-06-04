#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

// ===================== 1. 基础工具函数 =====================
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

// 单词查找（手动遍历，不用库函数find）
int searchWord(const map<string, int> &freq, const string &key) {
    for (const auto &p : freq) {
        if (p.first == key) {
            return p.second;
        }
    }
    return 0;
}

// ===================== 2. 版本3新增：基础语法检查 =====================
// 定义错误信息结构体（顺序存储：vector存列表）
struct ErrorInfo {
    string type;   // 错误类型：拼写错误/句首未大写/句末无标点
    int lineNo;    // 行号（这里用句子序号代替）
    string content; // 错误内容
    string hint;   // 修正建议
};

// 初始化基础词典（哈希表/字典存储正确单词）
map<string, bool> initDict() {
    map<string, bool> dict;
    // 基础英文单词示例，可自行扩展
    vector<string> baseWords = {
        "i", "you", "he", "she", "it", "we", "they",
        "a", "an", "the", "is", "am", "are", "was", "were",
        "have", "has", "had", "do", "does", "did", "go", "went",
        "good", "bad", "happy", "sad", "day", "night", "time",
        "and", "but", "or", "if", "because", "when", "what", "this",
        "that", "my", "your", "his", "her", "our", "their",
        "book", "pen", "school", "student", "teacher", "friend"
    };
    for (const string &word : baseWords) {
        dict[word] = true;
    }
    return dict;
}

// 拼写错误检测（字符串匹配：和词典比对）
vector<ErrorInfo> checkSpelling(const vector<string> &words, const map<string, bool> &dict) {
    vector<ErrorInfo> errors;
    for (int i = 0; i < words.size(); i++) {
        string wordLower = words[i];
        transform(wordLower.begin(), wordLower.end(), wordLower.begin(), ::tolower);
        if (dict.find(wordLower) == dict.end()) {
            errors.push_back({
                "拼写错误",
                i + 1,
                words[i],
                "该单词不在词典中，请检查拼写"
            });
        }
    }
    return errors;
}

// 句首大写检查（线性表遍历：逐句检查）
vector<ErrorInfo> checkSentenceStart(const vector<string> &sentences) {
    vector<ErrorInfo> errors;
    for (int i = 0; i < sentences.size(); i++) {
        const string &sent = sentences[i];
        if (sent.empty()) continue;
        
        // 找到句子中第一个字母
        size_t firstAlphaPos = 0;
        while (firstAlphaPos < sent.size() && !isalpha(sent[firstAlphaPos])) {
            firstAlphaPos++;
        }
        if (firstAlphaPos >= sent.size()) continue;
        
        if (islower(sent[firstAlphaPos])) {
            errors.push_back({
                "句首未大写",
                i + 1,
                sent.substr(0, 10) + "...",
                "句子首字母应大写"
            });
        }
    }
    return errors;
}

// 句末标点检查（线性表遍历：逐句检查）
vector<ErrorInfo> checkSentenceEnd(const vector<string> &sentences) {
    vector<ErrorInfo> errors;
    for (int i = 0; i < sentences.size(); i++) {
        const string &sent = sentences[i];
        if (sent.empty()) continue;
        
        char lastChar = sent.back();
        if (lastChar != '.' && lastChar != '?' && lastChar != '!') {
            errors.push_back({
                "句末无标点",
                i + 1,
                sent.substr(max(0, (int)sent.size() - 10)) + "...",
                "句子结尾应添加 . ? ! 之一"
            });
        }
    }
    return errors;
}

// 显示所有语法错误
void showGrammarErrors(const vector<ErrorInfo> &errors) {
    if (errors.empty()) {
        cout << "\n? 未检测到语法错误！" << endl;
        return;
    }
    
    cout << "\n===== 语法错误检测结果 =====" << endl;
    for (const auto &err : errors) {
        cout << "【" << err.type << "】位置：第" << err.lineNo << "个" 
             << (err.type == "拼写错误" ? "单词" : "句子") << endl;
        cout << "错误内容：" << err.content << endl;
        cout << "修正建议：" << err.hint << endl;
        cout << "------------------------" << endl;
    }
}

// 语法检查主函数
void grammarCheck(const string &text, const vector<string> &words) {
    if (text.empty()) {
        cout << "? 请先输入文本！" << endl;
        return;
    }
    
    // 1. 初始化词典（哈希表存储）
    map<string, bool> dict = initDict();
    
    // 2. 分句
    vector<string> sentences = splitSentences(text);
    
    // 3. 各项检查（线性表遍历）
    vector<ErrorInfo> spellingErrors = checkSpelling(words, dict);
    vector<ErrorInfo> startErrors = checkSentenceStart(sentences);
    vector<ErrorInfo> endErrors = checkSentenceEnd(sentences);
    
    // 4. 合并错误信息（顺序存储到vector）
    vector<ErrorInfo> allErrors;
    allErrors.insert(allErrors.end(), spellingErrors.begin(), spellingErrors.end());
    allErrors.insert(allErrors.end(), startErrors.begin(), startErrors.end());
    allErrors.insert(allErrors.end(), endErrors.begin(), endErrors.end());
    
    // 5. 显示结果
    showGrammarErrors(allErrors);
}

// ===================== 3. 原系统其他功能 =====================
// 基础统计
void showBasicInfo(const string &text, const vector<string> &words) {
    vector<string> sentences = splitSentences(text);
    cout << "\n===== 基础统计 =====" << endl;
    cout << "总字符数：" << text.size() << endl;
    cout << "总单词数：" << words.size() << endl;
    cout << "总句子数：" << sentences.size() << endl;
}

// 词频统计
void showFreq(const map<string, int> &freq) {
    cout << "\n===== 词频统计 =====" << endl;
    for (const auto &p : freq) {
        cout << p.first << " : " << p.second << endl;
    }
}

// 关键词提取
void showKeyWords(const map<string, int> &freq) {
    cout << "\n关键词（前5）：";
    int count = 0;
    for (const auto &p : freq) {
        if (count++ >= 5) break;
        cout << p.first << " ";
    }
    cout << endl;
}

// 文本摘要
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
    cout << "\n===== 智能文本处理系统（版本3） =====" << endl;
    cout << "1. 输入文本" << endl;
    cout << "2. 基础统计" << endl;
    cout << "3. 分词结果" << endl;
    cout << "4. 词频统计" << endl;
    cout << "5. 关键词提取" << endl;
    cout << "6. 文本摘要" << endl;
    cout << "7. 查找单词" << endl;
    cout << "8. 查找替换" << endl;
    cout << "9. 基础语法检查（版本3新增）" << endl;
    cout << "0. 退出" << endl;
    cout << "请输入选项：";
}
// ===================== 主函数 =====================
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
                string clean = preProcess(text);
                words = splitWords(clean);
                freq = countFreq(words);
                cout << "? 替换完成！" << endl;
                break;
            }
            case 9:
                grammarCheck(text, words);
                break;
            default:
                cout << "? 无效选项！" << endl;
        }
    }
    return 0;
}
