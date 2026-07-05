#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <set>
using namespace std;

// ===================== 基础工具函数 =====================
char my_tolower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}
bool my_isalpha(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
bool my_isspace(char c)
{
    return c == ' ' || c == '\t';
}
int my_min(int a, int b)
{
    return (a < b) ? a : b;
}

// ===================== V4 BF字符串匹配 =====================
int BF(const string &text, const string &pattern)
{
    int n = text.size();
    int m = pattern.size();
    if (m == 0 || n < m)
        return -1;
    for (int i = 0; i <= n - m; ++i)
    {
        int j = 0;
        while (j < m && text[i + j] == pattern[j])
            j++;
        if (j == m)
            return i;
    }
    return -1;
}

vector<int> BF_FindAllPos(const string &text, const string &pat)
{
    vector<int> posList;
    int idx = 0;
    int n = text.size();
    int m = pat.size();
    while (idx <= n - m)
    {
        int ret = BF(text.substr(idx), pat);
        if (ret == -1)
            break;
        idx += ret;
        posList.push_back(idx);
        idx += m;
    }
    return posList;
}

string ReplaceTextBF(string text, string oldStr, string newStr)
{
    int pos = BF(text, oldStr);
    while (pos != -1)
    {
        text.replace(pos, oldStr.size(), newStr);
        pos = BF(text, oldStr);
    }
    return text;
}

void SingleSubMatch(const string &text)
{
    if (text.empty())
    {
        cout << "请先输入文本！\n";
        return;
    }
    string pat;
    cout << "请输入要匹配的字串：";
    cin >> pat;
    int pos = BF(text, pat);
    if (pos == -1)
        cout << "未匹配到该字串！\n";
    else
        cout << "字串首次匹配下标：" << pos << "\n";
}

// ===================== V1 文本预处理、分词、词频统计 =====================
string PreProcess(const string &text)
{
    string res;
    for (char ch : text)
    {
        if (my_isalpha(ch))
            res += my_tolower(ch);
        else if (my_isspace(ch))
            res += ' ';
    }
    return res;
}

vector<string> SplitWords(const string &text)
{
    vector<string> words;
    string word;
    for (char c : text)
    {
        if (c == ' ')
        {
            if (!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
        }
        else
            word += c;
    }
    if (!word.empty())
        words.push_back(word);
    return words;
}

vector<string> SplitSentences(const string &text)
{
    vector<string> sentences;
    string cur;
    for (char c : text)
    {
        cur += c;
        if (c == '.' || c == '?' || c == '!')
        {
            sentences.push_back(cur);
            cur.clear();
        }
    }
    if (!cur.empty())
        sentences.push_back(cur);
    return sentences;
}

map<string, int> CountFreq(const vector<string> &words)
{
    map<string, int> freq;
    for (const string &w : words)
    {
        freq[w]++;
    }
    return freq;
}

// 修复const map读取报错：使用at()代替[]
int SearchWord(const map<string, int> &freq, string key)
{
    for (char &c : key)
        c = my_tolower(c);
    if (freq.count(key))
        return freq.at(key);
    return 0;
}

void ShowBasicInfo(const string &text, const vector<string> &words)
{
    vector<string> sentences = SplitSentences(text);
    cout << "\n===== 基础统计 =====\n";
    cout << "总字符数：" << text.size() << "\n";
    cout << "总单词数：" << words.size() << "\n";
    cout << "总句子数：" << sentences.size() << "\n";
}

void ShowFreq(const map<string, int> &freq)
{
    cout << "\n===== 词频统计 =====\n";
    for (const auto &p : freq)
        cout << p.first << " : " << p.second << "\n";
}

void ShowTop5Key(const map<string, int> &freq)
{
    cout << "\n前5高频关键词：";
    int cnt = 0;
    for (const auto &p : freq)
    {
        if (cnt++ >= 5)
            break;
        cout << p.first << " ";
    }
    cout << "\n";
}

void ShowSummary(const string &text)
{
    vector<string> sentences = SplitSentences(text);
    cout << "\n===== 文本摘要（前两句）=====\n";
    int n = my_min(2, (int)sentences.size());
    for (int i = 0; i < n; ++i)
        cout << sentences[i] << "\n";
}

void SearchAllSub(const string &srcText)
{
    if (srcText.empty())
    {
        cout << "请先输入文本！\n";
        return;
    }
    string key;
    cout << "输入查找子串：";
    cin >> key;
    vector<int> posArr = BF_FindAllPos(srcText, key);
    if (posArr.empty())
    {
        cout << "未找到目标子串\n";
        return;
    }
    cout << "\n匹配下标列表：";
    for (int p : posArr)
        cout << p << " ";
    cout << "\n共匹配" << posArr.size() << "处\n";
}

string HighlightKey(string src, const string &key)
{
    vector<int> posList = BF_FindAllPos(src, key);
    int keylen = key.size();
    for (auto it = posList.rbegin(); it != posList.rend(); ++it)
    {
        int pos = *it;
        src = src.substr(0, pos) + "【" + key + "】" + src.substr(pos + keylen);
    }
    return src;
}

void ShowHighlight(const string &srcText)
{
    if (srcText.empty())
    {
        cout << "请先输入文本！\n";
        return;
    }
    string key;
    cout << "输入高亮关键词：";
    cin >> key;
    cout << "\n高亮结果：\n" << HighlightKey(srcText, key) << "\n";
}

// ===================== V3 语法检查 =====================
struct ErrorInfo
{
    string type;
    int lineNo;
    string content;
    string hint;
};

map<string, bool> InitDict()
{
    map<string, bool> dict;
    vector<string> base = {"i", "you", "he", "she", "is", "am", "are", "good", "book"};
    for (auto w : base)
        dict[w] = true;
    return dict;
}

vector<ErrorInfo> CheckSpelling(const vector<string> &words, map<string, bool> &dict)
{
    vector<ErrorInfo> err;
    for (int i = 0; i < words.size(); ++i)
    {
        string w = words[i];
        for (char &c : w)
            c = my_tolower(c);
        if (!dict.count(w))
        {
            err.push_back({"拼写错误", i + 1, words[i], "不在内置词典"});
        }
    }
    return err;
}

vector<ErrorInfo> CheckSenStart(const vector<string> &sen)
{
    vector<ErrorInfo> err;
    for (int i = 0; i < sen.size(); ++i)
    {
        if (!sen[i].empty() && sen[i][0] >= 'a' && sen[i][0] <= 'z')
        {
            err.push_back({"句首未大写", i + 1, sen[i], "首字母大写"});
        }
    }
    return err;
}

vector<ErrorInfo> CheckSenEnd(const vector<string> &sen)
{
    vector<ErrorInfo> err;
    for (int i = 0; i < sen.size(); ++i)
    {
        if (sen[i].empty())
            continue;
        char last = sen[i].back();
        if (last != '.' && last != '?' && last != '!')
        {
            err.push_back({"句末缺少标点", i + 1, sen[i], "结尾添加 .?! "});
        }
    }
    return err;
}

void GrammarCheck(string text, vector<string> words)
{
    auto dict = InitDict();
    auto sen = SplitSentences(text);
    auto e1 = CheckSpelling(words, dict);
    auto e2 = CheckSenStart(sen);
    auto e3 = CheckSenEnd(sen);

    vector<ErrorInfo> allErr;
    allErr.insert(allErr.end(), e1.begin(), e1.end());
    allErr.insert(allErr.end(), e2.begin(), e2.end());
    allErr.insert(allErr.end(), e3.begin(), e3.end());

    if (allErr.empty())
    {
        cout << "\n文本无语法错误\n";
        return;
    }
    cout << "\n===== 语法错误汇总 =====\n";
    for (auto &e : allErr)
    {
        cout << "[" << e.type << "] 第" << e.lineNo << "段：" << e.content << " → " << e.hint << "\n";
    }
}

// ===================== V5 哈夫曼压缩/解压缩 =====================
struct HuffNode
{
    char ch;
    int freq;
    HuffNode *lchild, *rchild;
    HuffNode(char c = 0, int f = 0) : ch(c), freq(f), lchild(nullptr), rchild(nullptr) {}
};

struct NodeCmp
{
    bool operator()(HuffNode *a, HuffNode *b)
    {
        return a->freq > b->freq;
    }
};

void GetHuffCode(HuffNode *root, string code, map<char, string> &codeTable)
{
    if (!root)
        return;
    if (!root->lchild && !root->rchild)
    {
        codeTable[root->ch] = code;
        return;
    }
    GetHuffCode(root->lchild, code + "0", codeTable);
    GetHuffCode(root->rchild, code + "1", codeTable);
}

HuffNode *BuildHuffTree(const map<char, int> &charFreq)
{
    priority_queue<HuffNode *, vector<HuffNode *>, NodeCmp> heap;
    for (auto &p : charFreq)
        heap.push(new HuffNode(p.first, p.second));
    while (heap.size() > 1)
    {
        HuffNode *l = heap.top();
        heap.pop();
        HuffNode *r = heap.top();
        heap.pop();
        HuffNode *parent = new HuffNode(0, l->freq + r->freq);
        parent->lchild = l;
        parent->rchild = r;
        heap.push(parent);
    }
    return heap.top();
}

void DeleteHuffTree(HuffNode *root)
{
    if (!root)
        return;
    DeleteHuffTree(root->lchild);
    DeleteHuffTree(root->rchild);
    delete root;
}

string CompressText(const string &raw, map<char, string> &codeTable)
{
    map<char, int> charCnt;
    for (char c : raw)
        charCnt[c]++;
    HuffNode *root = BuildHuffTree(charCnt);
    GetHuffCode(root, "", codeTable);
    string bin;
    for (char c : raw)
        bin += codeTable[c];
    DeleteHuffTree(root);
    return bin;
}

string DecompressText(const string &binStr, const map<char, string> &codeTable)
{
    map<string, char> charMap;
    for (auto &p : codeTable)
        charMap[p.second] = p.first;
    string res, temp;
    for (char bit : binStr)
    {
        temp += bit;
        if (charMap.count(temp))
        {
            res += charMap[temp];
            temp.clear();
        }
    }
    return res;
}

void FuncCompress(string src, string &outBin, map<char, string> &outCode)
{
    if (src.empty())
    {
        cout << "请先输入原始文本！\n";
        return;
    }
    outBin = CompressText(src, outCode);
    cout << "\n===== 哈夫曼压缩结果 =====\n编码表：\n";
    for (auto &p : outCode)
    {
        if (p.first == ' ')
            cout << "空格 : " << p.second << "\n";
        else
            cout << p.first << " : " << p.second << "\n";
    }
    cout << "压缩二进制串前100位：" << outBin.substr(0, 100) << "\n";
    cout << "原始字符长度：" << src.size() << " 压缩总位数：" << outBin.size() << "\n";
}

void FuncDecompress(const string &binStream, const map<char, string> &codeTable)
{
    if (binStream.empty() || codeTable.empty())
    {
        cout << "无压缩数据，请先执行压缩！\n";
        return;
    }
    string recover = DecompressText(binStream, codeTable);
    cout << "\n===== 解压缩还原文本 =====\n" << recover << "\n";
}

// ===================== V7 多维度单词排序 =====================
struct WordInfo
{
    string word;
    int count;
    int len;
    WordInfo(string w, int c) : word(w), count(c), len(w.size()) {}
};

bool cmpFreqDesc(const WordInfo &a, const WordInfo &b)
{
    if (a.count != b.count)
        return a.count > b.count;
    return a.word < b.word;
}
bool cmpLenAsc(const WordInfo &a, const WordInfo &b)
{
    if (a.len != b.len)
        return a.len < b.len;
    return a.count > b.count;
}
bool cmpDictAsc(const WordInfo &a, const WordInfo &b)
{
    return a.word < b.word;
}

vector<WordInfo> BuildWordList(const map<string, int> &freqMap)
{
    vector<WordInfo> list;
    for (auto &p : freqMap)
        list.emplace_back(p.first, p.second);
    return list;
}

void MultiSort(const map<string, int> &freqMap)
{
    if (freqMap.empty())
    {
        cout << "请先输入文本生成词频！\n";
        return;
    }
    vector<WordInfo> wordList = BuildWordList(freqMap);
    int opt;
    cout << "\n排序选项：1词频降序 2长度升序 3字典序\n请输入：";
    cin >> opt;
    switch (opt)
    {
    case 1:
        stable_sort(wordList.begin(), wordList.end(), cmpFreqDesc);
        cout << "\n【词频降序】\n";
        break;
    case 2:
        stable_sort(wordList.begin(), wordList.end(), cmpLenAsc);
        cout << "\n【单词长度升序】\n";
        break;
    case 3:
        stable_sort(wordList.begin(), wordList.end(), cmpDictAsc);
        cout << "\n【字典序升序】\n";
        break;
    default:
        cout << "无效选项\n";
        return;
    }
    cout << "单词\t词频\t长度\n-------------------------\n";
    for (auto &item : wordList)
        cout << item.word << "\t" << item.count << "\t" << item.len << "\n";
}

// ===================== V6 词共现图（全部逻辑内置，无多余拆分函数） =====================
struct Edge
{
    int to;
    int weight;
    Edge(int t, int w) : to(t), weight(w) {}
};

void WordCoOccurGraph(const string &rawText)
{
    if (rawText.empty())
    {
        cout << "请先输入文本！\n";
        return;
    }
    vector<string> sentences = SplitSentences(rawText);
    vector<vector<string>> senWordList;
    vector<string> allWords;

    for (const string &sen : sentences)
    {
        string clean = PreProcess(sen);
        vector<string> senWords = SplitWords(clean);
        if (!senWords.empty())
        {
            senWordList.push_back(senWords);
            allWords.insert(allWords.end(), senWords.begin(), senWords.end());
        }
    }
    if (allWords.empty())
    {
        cout << "文本无有效单词，无法构建共现图\n";
        return;
    }

    map<string, int> word2id;
    vector<string> id2word;
    set<string> unique(allWords.begin(), allWords.end());
    int id = 0;
    for (const string &w : unique)
    {
        word2id[w] = id;
        id2word.push_back(w);
        id++;
    }

    map<string, map<string, int>> coCount;
    for (auto &senWords : senWordList)
    {
        int n = senWords.size();
        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                string a = senWords[i];
                string b = senWords[j];
                coCount[a][b]++;
            }
        }
    }

    vector<vector<Edge>> adjGraph(id);
    for (const auto &outer : coCount)
    {
        string uWord = outer.first;
        int u = word2id[uWord];
        for (const auto &inner : outer.second)
        {
            string vWord = inner.first;
            int v = word2id[vWord];
            int cnt = inner.second;
            adjGraph[u].emplace_back(v, cnt);
            adjGraph[v].emplace_back(u, cnt);
        }
    }

    cout << "\n===== 词共现网络图（邻接表）=====\n";
    cout << "全部关键词顶点：";
    for (const string &w : id2word)
        cout << w << " ";
    cout << "\n顶点总数：" << id2word.size() << "\n\n";
    for (int u = 0; u < adjGraph.size(); ++u)
    {
        cout << "【" << id2word[u] << "】关联：";
        for (const Edge &e : adjGraph[u])
        {
            cout << id2word[e.to] << "(" << e.weight << "次) ";
        }
        cout << "\n";
    }

    cout << "\n===== BFS遍历关联网络 =====\n";
    vector<bool> visited(id, false);
    queue<int> q;
    for (int i = 0; i < id; ++i)
    {
        if (!visited[i])
        {
            cout << "起点关键词：" << id2word[i] << "\n";
            q.push(i);
            visited[i] = true;
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                for (const Edge &e : adjGraph[u])
                {
                    int v = e.to;
                    if (!visited[v])
                    {
                        visited[v] = true;
                        cout << "关联：" << id2word[v] << " 共现次数：" << e.weight << "\n";
                        q.push(v);
                    }
                }
            }
            cout << "-------------------------\n";
        }
    }
}

// ===================== 菜单打印 =====================
void PrintMenu()
{
    cout << "\n======== 文本处理综合系统 V7 ========\n";
    cout << "1. 输入文本\n";
    cout << "2. 基础统计\n";
    cout << "3. 单词拆分查看\n";
    cout << "4. 词频统计\n";
    cout << "5. 提取前5高频关键词\n";
    cout << "6. 文本摘要（前两句）\n";
    cout << "7. 查询单词出现次数\n";
    cout << "8. BF查找替换文本\n";
    cout << "9. 语法错误检测\n";
    cout << "10. 全文批量查找子串\n";
    cout << "11. 关键词高亮显示\n";
    cout << "12. 子串首次匹配\n";
    cout << "13. 哈夫曼文本压缩\n";
    cout << "14. 哈夫曼解压缩还原\n";
    cout << "15. V6生成词共现关系图\n";
    cout << "16. V7单词多维度排序\n";
    cout << "0. 退出程序\n";
    cout << "请输入功能序号：";
}

// ===================== 主函数 =====================
int main()
{
    string inputText;
    vector<string> wordList;
    map<string, int> wordFreq;
    string compressBinData;
    map<char, string> huffCodeTable;

    cout << "===== 整合V1/V3/V4/V5/V6/V7 完整无错文本系统 =====\n";
    int op;
    while (true)
    {
        PrintMenu();
        cin >> op;
        cin.ignore(1024, '\n');
        if (op == 0)
        {
            cout << "程序正常退出\n";
            break;
        }
        switch (op)
        {
        case 1:
        {
            cout << "\n请输入完整文本：\n";
            getline(cin, inputText);
            string cleanTxt = PreProcess(inputText);
            wordList = SplitWords(cleanTxt);
            wordFreq = CountFreq(wordList);
            compressBinData.clear();
            huffCodeTable.clear();
            cout << "文本加载完成，缓存已重置\n";
            break;
        }
        case 2:
            ShowBasicInfo(inputText, wordList);
            break;
        case 3:
        {
            if (wordList.empty())
            {
                cout << "请先输入文本\n";
                break;
            }
            cout << "\n分词结果：";
            for (auto &w : wordList)
                cout << w << " ";
            cout << "\n";
            break;
        }
        case 4:
            ShowFreq(wordFreq);
            break;
        case 5:
            ShowTop5Key(wordFreq);
            break;
        case 6:
            ShowSummary(inputText);
            break;
        case 7:
        {
            if (wordFreq.empty())
            {
                cout << "请先输入文本\n";
                break;
            }
            string key;
            cout << "请输入要查询的单词：";
            cin >> key;
            cout << "该单词出现次数：" << SearchWord(wordFreq, key) << "\n";
            break;
        }
        case 8:
        {
            if (inputText.empty())
            {
                cout << "请先输入文本\n";
                break;
            }
            string oldS, newS;
            cout << "查找原串：";
            cin >> oldS;
            cout << "替换为：";
            cin >> newS;
            inputText = ReplaceTextBF(inputText, oldS, newS);
            string clean = PreProcess(inputText);
            wordList = SplitWords(clean);
            wordFreq = CountFreq(wordList);
            compressBinData.clear();
            huffCodeTable.clear();
            cout << "替换完成，数据已更新\n";
            break;
        }
        case 9:
            GrammarCheck(inputText, wordList);
            break;
        case 10:
            SearchAllSub(inputText);
            break;
        case 11:
            ShowHighlight(inputText);
            break;
        case 12:
            SingleSubMatch(inputText);
            break;
        case 13:
            FuncCompress(inputText, compressBinData, huffCodeTable);
            break;
        case 14:
            FuncDecompress(compressBinData, huffCodeTable);
            break;
        case 15:
            WordCoOccurGraph(inputText);
            break;
        case 16:
            MultiSort(wordFreq);
            break;
        default:
            cout << "输入序号无效，请重新输入\n";
        }
    }
    return 0;
}
