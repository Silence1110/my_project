#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;

// ===================== 手写基础工具函数 =====================
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

// ===================== V4原有BF串匹配模块 =====================
int BF(const string &text, const string &pattern) {
    int n = text.size();
    int m = pattern.size();
    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) j++;
        if (j == m) return i;
    }
    return -1;
}
vector<int> BF_FindAllPos(const string &text, const string &pat)
{
    vector<int> posList;
    int idx = 0;
    int n = text.size();
    int m = pat.size();
    while(idx <= n - m)
    {
        int ret = BF(text.substr(idx), pat);
        if(ret == -1) break;
        idx += ret;
        posList.push_back(idx);
        idx += m;
    }
    return posList;
}
string replaceTextBF(string text, string oldStr, string newStr) {
    int pos = BF(text, oldStr);
    while (pos != -1) {
        text.replace(pos, oldStr.size(), newStr);
        pos = BF(text, oldStr);
    }
    return text;
}
int BF_SingleMatch(const string &text, const string &pattern) {
    int n = text.size();
    int m = pattern.size();
    if (m == 0 || n < m) return -1;
    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) j++;
        if (j == m) return i;
    }
    return -1;
}
void singleSubStringMatch(const string &text) {
    if (text.empty()) {
        cout << "请先输入文本！\n";
        return;
    }
    string pat;
    cout << "请输入要匹配的字串：";
    cin >> pat;
    int pos = BF_SingleMatch(text, pat);
    if (pos == -1) cout << "未匹配到该字串！" << endl;
    else cout << "字串首次匹配位置：" << pos << endl;
}

// ===================== 文本基础处理函数（V3保留） =====================
string preProcess(const string &text) {
    string res;
    for (char ch : text) {
        if (my_isalpha(ch)) res += my_tolower(ch);
        else if (my_isspace(ch)) res += ' ';
    }
    return res;
}
vector<string> splitWords(const string &text) {
    vector<string> words;
    string word;
    for (char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else word += c;
    }
    if (!word.empty()) words.push_back(word);
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
    for (auto &p : freq)
        if (p.first == key) return p.second;
    return 0;
}
void showBasicInfo(const string &text, const vector<string> &words) {
    vector<string> sentences = splitSentences(text);
    cout << "\n===== 基础统计 =====" << endl;
    cout << "总字符数：" << text.size() << endl;
    cout << "总单词数：" << words.size() << endl;
    cout << "总句子数：" << sentences.size() << endl;
}
void showFreq(const map<string, int> &freq) {
    cout << "\n===== 词频统计 =====" << endl;
    for (const auto &p : freq) cout << p.first << " : " << p.second << endl;
}
void showKeyWords(const map<string, int> &freq) {
    cout << "\n关键词（前5）：";
    int count = 0;
    for (const auto &p : freq) {
        if (count++ >= 5) break;
        cout << p.first << " ";
    }
    cout << endl;
}
void showSummary(const string &text) {
    vector<string> sentences = splitSentences(text);
    cout << "\n===== 文本摘要 =====" << endl;
    int n = my_min(2, (int)sentences.size());
    for (int i = 0; i < n; i++) cout << sentences[i] << endl;
}
void searchAllSub(const string &srcText)
{
    if(srcText.empty()){
        cout<<"请先输入文本！\n";
        return;
    }
    string key;
    cout<<"请输入需要查找的子串：";
    cin>>key;
    vector<int> posArr = BF_FindAllPos(srcText, key);
    if(posArr.empty()){
        cout<<"未在文本中找到该关键词\n";
        return;
    }
    cout<<"\n===== 匹配位置（下标从0开始）=====\n";
    for(int p : posArr) cout<<"匹配起始下标："<<p<<endl;
    cout<<"总共匹配："<<posArr.size()<<"处\n";
}
string highlightKey(string src, const string &key)
{
    vector<int> posList = BF_FindAllPos(src, key);
    int keylen = key.size();
    for(auto it = posList.rbegin(); it != posList.rend(); ++it)
    {
        int pos = *it;
        string head = src.substr(0, pos);
        string mid = "【" + key + "】";
        string tail = src.substr(pos + keylen);
        src = head + mid + tail;
    }
    return src;
}
void showHighlightText(const string &srcText)
{
    if(srcText.empty()){
        cout<<"请先输入文本！\n";
        return;
    }
    string key;
    cout<<"输入需要高亮的关键词：";
    cin>>key;
    string out = highlightKey(srcText, key);
    cout<<"\n===== 高亮结果 =====\n"<<out<<endl;
}
// 语法检查模块
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
    if(errors.empty()){cout<<"\n无语法错误\n";return;}
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

// ========================== 版本5 新增：哈夫曼文本压缩解压缩模块 ==========================
// 哈夫曼树二叉树节点
struct HuffNode {
    char ch;
    int freq;
    HuffNode *lchild, *rchild;
    HuffNode(char c = 0, int f = 0) : ch(c), freq(f), lchild(nullptr), rchild(nullptr) {}
};
// 优先队列排序规则：权值小的优先出队（最小堆）
struct NodeCmp {
    bool operator()(HuffNode* a, HuffNode* b) {
        return a->freq > b->freq;
    }
};
// 递归遍历哈夫曼树，生成字符编码哈希表
void getHuffCode(HuffNode* root, string code, map<char, string>& codeTable) {
    if (!root) return;
    // 叶子节点，保存编码
    if (!root->lchild && !root->rchild) {
        codeTable[root->ch] = code;
        return;
    }
    getHuffCode(root->lchild, code + "0", codeTable);
    getHuffCode(root->rchild, code + "1", codeTable);
}
// 构建哈夫曼树（贪心算法+最小堆优先队列）
HuffNode* buildHuffTree(const map<char, int>& charFreq) {
    priority_queue<HuffNode*, vector<HuffNode*>, NodeCmp> minHeap;
    // 所有字符入堆
    for (auto& p : charFreq) {
        minHeap.push(new HuffNode(p.first, p.second));
    }
    // 不断合并最小两个节点
    while (minHeap.size() > 1) {
        HuffNode* left = minHeap.top(); minHeap.pop();
        HuffNode* right = minHeap.top(); minHeap.pop();
        HuffNode* parent = new HuffNode(0, left->freq + right->freq);
        parent->lchild = left;
        parent->rchild = right;
        minHeap.push(parent);
    }
    return minHeap.top();
}
// 递归释放哈夫曼树内存
void deleteTree(HuffNode* root) {
    if (!root) return;
    deleteTree(root->lchild);
    deleteTree(root->rchild);
    delete root;
}
// 文本压缩：输出二进制编码串、编码表
string compressText(const string& rawText, map<char, string>& codeTable) {
    // 1.统计每个字符出现频次
    map<char, int> charFreq;
    for (char c : rawText) charFreq[c]++;
    // 2.构建哈夫曼树
    HuffNode* root = buildHuffTree(charFreq);
    // 3.遍历生成编码表
    getHuffCode(root, "", codeTable);
    // 4.原始文本转为二进制01串（线性表存储压缩流）
    string binStream;
    for (char c : rawText) binStream += codeTable[c];
    deleteTree(root);
    return binStream;
}
// 二进制流解压缩，还原原始文本
string decompressText(const string& binStream, const map<char, string>& codeTable) {
    // 反转编码表：编码→字符
    map<string, char> code2char;
    for (auto& p : codeTable) code2char[p.second] = p.first;
    string rawStr, tempCode;
    for (char bit : binStream) {
        tempCode += bit;
        // 匹配到完整编码则输出字符
        if (code2char.count(tempCode)) {
            rawStr += code2char[tempCode];
            tempCode.clear();
        }
    }
    return rawStr;
}
// 菜单功能：文本压缩
void funcCompress(string srcText, string& outBin, map<char, string>& outCode) {
    if (srcText.empty()) {
        cout << "请先输入原始文本！\n";
        return;
    }
    outBin = compressText(srcText, outCode);
    cout << "\n===== 哈夫曼压缩完成 =====\n";
    cout << "字符哈夫曼编码表：\n";
    for (auto& p : outCode) {
        if (p.first == ' ') cout << "空格 : " << p.second << endl;
        else cout << p.first << " : " << p.second << endl;
    }
    cout << "\n压缩二进制流(01串)前100位：" << outBin.substr(0,100) << endl;
    cout << "原始字符长度：" << srcText.size() << endl;
    cout << "压缩二进制总位数：" << outBin.size() << endl;
}
// 菜单功能：解压缩
void funcDecompress(const string& binStream, const map<char, string>& codeTable) {
    if (binStream.empty() || codeTable.empty()) {
        cout << "无压缩数据，请先执行文本压缩！\n";
        return;
    }
    string recover = decompressText(binStream, codeTable);
    cout << "\n===== 解压缩还原文本 =====\n" << recover << endl;
}

// ===================== 更新菜单（新增13压缩、14解压缩） =====================
void menu() {
    cout << "\n===== 智能文本处理系统 V5（含哈夫曼压缩） =====" << endl;
    cout << "1. 输入文本" << endl;
    cout << "2. 基础统计" << endl;
    cout << "3. 分词结果" << endl;
    cout << "4. 词频统计" << endl;
    cout << "5. 关键词提取" << endl;
    cout << "6. 文本摘要" << endl;
    cout << "7. 查找单词" << endl;
    cout << "8. 查找替换" << endl;
    cout << "9. 语法检查" << endl;
    cout << "10. 全文子串查找(BF)" << endl;
    cout << "11. 关键词高亮" << endl;
    cout << "12. 字串首次匹配(BF)" << endl;
    cout << "13. V5-哈夫曼文本压缩" << endl;
    cout << "14. V5-哈夫曼解压缩还原" << endl;
    cout << "0. 退出" << endl;
    cout << "请输入选项：";
}

// ===================== 主函数全局存储压缩数据 =====================
int main() {
    string text;
    vector<string> words;
    map<string, int> freq;
    // V5新增全局缓存：压缩二进制流、字符编码表
    string compressBin;
    map<char, string> huffCodeTable;

    cout << "===== 欢迎使用文本处理系统 V5 =====" << endl;
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
                // 输入新文本后清空旧压缩缓存
                compressBin.clear();
                huffCodeTable.clear();
                cout << "文本处理完成！已清空历史压缩数据" << endl;
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
                compressBin.clear();
                huffCodeTable.clear();
                cout << "替换完成！压缩缓存已重置" << endl;
                break;
            }
            case 9:
                grammarCheck(text, words);
                break;
            case 10:
                searchAllSub(text);
                break;
            case 11:
                showHighlightText(text);
                break;
            case 12:
                singleSubStringMatch(text);
                break;
            // V5哈夫曼压缩
            case 13:
                funcCompress(text, compressBin, huffCodeTable);
                break;
            // V5解压缩
            case 14:
                funcDecompress(compressBin, huffCodeTable);
                break;
            default:
                cout << "无效选项！" << endl;
        }
    }
    return 0;
}
