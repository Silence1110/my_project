#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <utility>
using namespace std;

// ===================== 自定义工具函数 =====================
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

// ===================== BF算法（子串匹配） =====================
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

vector<int> BF_FindAllPos(const string &text, const string &pat) {
    vector<int> posList;
    int idx = 0;
    int n = text.size();
    int m = pat.size();
    while (idx <= n - m) {
        int ret = BF(text.substr(idx), pat);
        if (ret == -1) break;
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
        cout << "请输入有效文本\n";
        return;
    }
    string pat;
    cout << "请输入要匹配的子串：";
    cin >> pat;
    int pos = BF_SingleMatch(text, pat);
    if (pos == -1) cout << "未匹配到该子串" << endl;
    else cout << "子串首次匹配位置：" << pos << endl;
}

// ===================== 文本预处理与基础分析 =====================
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
    cout << "\n关键词（前5个）：";
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

// ===================== 子串搜索与高亮 =====================
void searchAllSub(const string &srcText) {
    if (srcText.empty()) {
        cout << "请输入有效文本\n";
        return;
    }
    string key;
    cout << "请输入要查找的子串：";
    cin >> key;
    vector<int> posArr = BF_FindAllPos(srcText, key);
    if (posArr.empty()) {
        cout << "未在文本中找到该关键词\n";
        return;
    }
    cout << "\n===== 匹配位置（下标从0开始）=====\n";
    for (int p : posArr) cout << "匹配起始下标：" << p << endl;
    cout << "总共匹配：" << posArr.size() << "次\n";
}

string highlightKey(string src, const string &key) {
    vector<int> posList = BF_FindAllPos(src, key);
    int keylen = key.size();
    for (auto it = posList.rbegin(); it != posList.rend(); ++it) {
        int pos = *it;
        string head = src.substr(0, pos);
        string mid = "【" + key + "】";
        string tail = src.substr(pos + keylen);
        src = head + mid + tail;
    }
    return src;
}

void showHighlightText(const string &srcText) {
    if (srcText.empty()) {
        cout << "请输入有效文本\n";
        return;
    }
    string key;
    cout << "请输入要高亮的关键词：";
    cin >> key;
    string out = highlightKey(srcText, key);
    cout << "\n===== 高亮文本 =====\n" << out << endl;
}

// ===================== 语法检查模块 =====================
struct ErrorInfo {
    string type;
    int lineNo;
    string content;
    string hint;
};

map<string, bool> initDict() {
    map<string, bool> dict;
    vector<string> baseWords = {"i", "you", "he", "she", "is", "am", "are", "good", "bad", "book", "school"};
    for (auto w : baseWords) dict[w] = 1;
    return dict;
}

vector<ErrorInfo> checkSpelling(const vector<string> &words, map<string, bool> &dict) {
    vector<ErrorInfo> e;
    for (int i = 0; i < words.size(); i++) {
        string w = words[i];
        for (int j = 0; j < w.size(); j++) w[j] = my_tolower(w[j]);
        if (!dict.count(w)) e.push_back({"拼写错误", i + 1, words[i], "检查单词拼写"});
    }
    return e;
}

vector<ErrorInfo> checkSentenceStart(const vector<string> &s) {
    vector<ErrorInfo> e;
    for (int i = 0; i < s.size(); i++)
        if (!s[i].empty() && s[i][0] >= 'a' && s[i][0] <= 'z')
            e.push_back({"首字母未大写", i + 1, s[i], "首字母大写"});
    return e;
}

vector<ErrorInfo> checkSentenceEnd(const vector<string> &s) {
    vector<ErrorInfo> e;
    for (int i = 0; i < s.size(); i++)
        if (!s[i].empty()) {
            char c = s[i].back();
            if (c != '.' && c != '?' && c != '!')
                e.push_back({"句末无标点", i + 1, s[i], "添加.?!结尾"});
        }
    return e;
}

void showGrammarErrors(vector<ErrorInfo> errors) {
    if (errors.empty()) {
        cout << "\n无语法错误\n";
        return;
    }
    cout << "\n===== 语法错误 =====\n";
    for (auto &e : errors)
        cout << "[" << e.type << "] " << e.lineNo << "行：" << e.content << " → " << e.hint << endl;
}

void grammarCheck(string text, vector<string> words) {
    auto dict = initDict();
    auto s = splitSentences(text);
    auto e1 = checkSpelling(words, dict), e2 = checkSentenceStart(s), e3 = checkSentenceEnd(s);
    vector<ErrorInfo> all;
    all.insert(all.end(), e1.begin(), e1.end());
    all.insert(all.end(), e2.begin(), e2.end());
    all.insert(all.end(), e3.begin(), e3.end());
    showGrammarErrors(all);
}

// ===================== 哈夫曼压缩/解压缩（V5） =====================
struct HuffNode {
    char ch;
    int freq;
    HuffNode *lchild, *rchild;
    HuffNode(char c = 0, int f = 0) : ch(c), freq(f), lchild(nullptr), rchild(nullptr) {}
};

struct NodeCmp {
    bool operator()(HuffNode *a, HuffNode *b) {
        return a->freq > b->freq;
    }
};

void getHuffCode(HuffNode *root, string code, map<char, string> &codeTable) {
    if (!root) return;
    if (!root->lchild && !root->rchild) {
        codeTable[root->ch] = code;
        return;
    }
    getHuffCode(root->lchild, code + "0", codeTable);
    getHuffCode(root->rchild, code + "1", codeTable);
}

HuffNode *buildHuffTree(const map<char, int> &charFreq) {
    priority_queue<HuffNode *, vector<HuffNode *>, NodeCmp> minHeap;
    for (auto &p : charFreq) {
        minHeap.push(new HuffNode(p.first, p.second));
    }
    while (minHeap.size() > 1) {
        HuffNode *left = minHeap.top();
        minHeap.pop();
        HuffNode *right = minHeap.top();
        minHeap.pop();
        HuffNode *parent = new HuffNode(0, left->freq + right->freq);
        parent->lchild = left;
        parent->rchild = right;
        minHeap.push(parent);
    }
    return minHeap.top();
}

void deleteTree(HuffNode *root) {
    if (!root) return;
    deleteTree(root->lchild);
    deleteTree(root->rchild);
    delete root;
}

string compressText(const string &rawText, map<char, string> &codeTable) {
    map<char, int> charFreq;
    for (char c : rawText) charFreq[c]++;
    HuffNode *root = buildHuffTree(charFreq);
    getHuffCode(root, "", codeTable);
    string binStream;
    for (char c : rawText) binStream += codeTable[c];
    deleteTree(root);
    return binStream;
}

string decompressText(const string &binStream, const map<char, string> &codeTable) {
    map<string, char> code2char;
    for (auto &p : codeTable) code2char[p.second] = p.first;
    string rawStr, tempCode;
    for (char bit : binStream) {
        tempCode += bit;
        if (code2char.count(tempCode)) {
            rawStr += code2char[tempCode];
            tempCode.clear();
        }
    }
    return rawStr;
}

void funcCompress(string srcText, string &outBin, map<char, string> &outCode) {
    if (srcText.empty()) {
        cout << "请输入原始文本\n";
        return;
    }
    outBin = compressText(srcText, outCode);
    cout << "\n===== 哈夫曼压缩结果 =====\n";
    cout << "字符对应编码：\n";
    for (auto &p : outCode) {
        if (p.first == ' ') cout << "空格 : " << p.second << endl;
        else cout << p.first << " : " << p.second << endl;
    }
    cout << "\n压缩后的二进制流（前100位）：" << outBin.substr(0, 100) << endl;
    cout << "原始字符长度：" << srcText.size() << endl;
    cout << "压缩后二进制位数：" << outBin.size() << endl;
}

void funcDecompress(const string &binStream, const map<char, string> &codeTable) {
    if (binStream.empty() || codeTable.empty()) {
        cout << "无压缩数据，无法执行文本解压缩\n";
        return;
    }
    string recover = decompressText(binStream, codeTable);
    cout << "\n===== 解压缩还原文本 =====\n" << recover << endl;
}

// ===================== 词共现图（V6） =====================
struct Edge {
    int to;
    int weight;
    Edge(int t, int w) : to(t), weight(w) {}
};

vector<vector<Edge>> wordGraph;
map<string, int> wordToId;
vector<string> idToWord;
map<pair<string, string>, int> coExistCount;

int getWordId(const string &word) {
    if (wordToId.find(word) == wordToId.end()) {
        wordToId[word] = idToWord.size();
        idToWord.push_back(word);
        wordGraph.emplace_back();
    }
    return wordToId[word];
}

void countSentenceCoExist(const vector<string> &sentWords) {
    int len = sentWords.size();
    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            string w1 = sentWords[i];
            string w2 = sentWords[j];
            pair<string, string> key;
            if (w1 < w2)
                key = {w1, w2};
            else
                key = {w2, w1};
            coExistCount[key]++;
        }
    }
}

void buildCoExistGraph() {
    wordGraph.clear();
    wordToId.clear();
    idToWord.clear();
    for (auto &item : coExistCount) {
        string w1 = item.first.first;
        string w2 = item.first.second;
        int cnt = item.second;
        int u = getWordId(w1);
        int v = getWordId(w2);
        wordGraph[u].emplace_back(v, cnt);
        wordGraph[v].emplace_back(u, cnt);
    }
}

void printWordGraph() {
    if (wordGraph.empty()) {
        cout << "无单词共现关系（请先输入文本）\n";
        return;
    }
    cout << "\n===== 词共现关系图（邻接表）=====\n";
    for (int u = 0; u < wordGraph.size(); u++) {
        string wordU = idToWord[u];
        cout << "节点" << u << "（单词：" << wordU << "） 关联：";
        for (Edge &e : wordGraph[u]) {
            string wordV = idToWord[e.to];
            cout << wordV << "(共现" << e.weight << "次)  ";
        }
        cout << "\n";
    }
}

void createCoExistGraph(const string &srcText) {
    if (srcText.empty()) {
        cout << "请输入有效文本\n";
        return;
    }
    coExistCount.clear();
    vector<string> sentenceList = splitSentences(srcText);
    for (string sen : sentenceList) {
        string cleanSen = preProcess(sen);
        vector<string> senWords = splitWords(cleanSen);
        countSentenceCoExist(senWords);
    }
    buildCoExistGraph();
    printWordGraph();
}

// ===================== 菜单与主函数 =====================
void menu() {
    cout << "\n===== 智能文本分析系统 V6（整合压缩+词共现） =====" << endl;
    cout << "1. 输入文本" << endl;
    cout << "2. 基础统计" << endl;
    cout << "3. 单词拆分" << endl;
    cout << "4. 词频统计" << endl;
    cout << "5. 关键词提取" << endl;
    cout << "6. 文本摘要" << endl;
    cout << "7. 单词搜索" << endl;
    cout << "8. 文本替换" << endl;
    cout << "9. 语法检查" << endl;
    cout << "10. 全量子串搜索(BF)" << endl;
    cout << "11. 关键词高亮" << endl;
    cout << "12. 子串单次匹配(BF)" << endl;
    cout << "13. 哈夫曼文本压缩" << endl;
    cout << "14. 哈夫曼解压缩还原" << endl;
    cout << "15. 生成词共现关系图" << endl;
    cout << "0. 退出" << endl;
    cout << "请选择操作：";
}

int main() {
    string text;
    vector<string> words;
    map<string, int> freq;
    // 哈夫曼压缩全局变量
    string compressBin;
    map<char, string> huffCodeTable;

    cout << "===== 欢迎使用智能文本分析系统 V6 =====" << endl;
    while (true) {
        menu();
        int op;
        cin >> op;
        cin.ignore();

        if (op == 0) {
            cout << "程序退出" << endl;
            break;
        }

        switch (op) {
            case 1: {
                cout << "\n请输入文本：" << endl;
                getline(cin, text);
                string clean = preProcess(text);
                words = splitWords(clean);
                freq = countFreq(words);
                compressBin.clear();
                huffCodeTable.clear();
                cout << "文本输入完成，已重置压缩缓存" << endl;
                break;
            }
                case 2:
				    if (words.empty())
				    {
				        cout << "请先输入文本\n";
				    }
				    else
				    {
				        showBasicInfo(text, words);
				    }
				    break;
            case 3:
                if (words.empty()) cout << "请先输入文本\n";
                else {
                    cout << "\n===== 单词拆分 =====" << endl;
                    for (const string &w : words) cout << w << " ";
                    cout << endl;
                }
                break;
case 4:
    if (freq.empty())
    {
        cout << "请先输入文本\n";
    }
    else
    {
        showFreq(freq);
    }
    break;
          case 5:
		      if (freq.empty())
		      {
		          cout << "请先输入文本\n";
		      }
		      else
		      {
		          showKeyWords(freq);
		      }
		      break;
          case 6:
		      if (text.empty())
		      {
		          cout << "请先输入文本\n";
		      }
		      else
		      {
		          showSummary(text);
		      }
		      break;
            case 7: {
                if (freq.empty()) {
                    cout << "请先输入文本\n";
                    break;
                }
                string key;
                cout << "请输入要搜索的单词：";
                cin >> key;
                for (int i = 0; i < key.size(); i++) key[i] = my_tolower(key[i]);
                cout << "该单词出现次数：" << searchWord(freq, key) << endl;
                break;
            }
            case 8: {
                if (text.empty()) {
                    cout << "请先输入文本\n";
                    break;
                }
                string oldS, newS;
                cout << "替换原串：";
                cin >> oldS;
                cout << "替换为：";
                cin >> newS;
                text = replaceTextBF(text, oldS, newS);
                string clean = preProcess(text);
                words = splitWords(clean);
                freq = countFreq(words);
                compressBin.clear();
                huffCodeTable.clear();
                cout << "替换完成，已重置压缩缓存" << endl;
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
            case 13:
                funcCompress(text, compressBin, huffCodeTable);
                break;
            case 14:
                funcDecompress(compressBin, huffCodeTable);
                break;
            case 15:
                createCoExistGraph(text);
                break;
            default:
                cout << "无效选择，请重新输入\n";
        }
    }
    return 0;
}
