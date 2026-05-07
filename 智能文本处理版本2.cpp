#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;


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


map<string, int> countFreq(const vector<string> &words) {
    map<string, int> freq;
    for (const string &w : words) {
        freq[w]++;
    }
    return freq;
}

int searchWord(const map<string, int> &freq, const string &key) {
    auto it = freq.find(key);
    if (it != freq.end())
        return it->second;
    return 0;
}

void showFreq(const map<string, int> &freq) {
    cout << "\n===== 词频统计结果 =====" << endl;
    for (auto &p : freq) {
        cout << p.first << " : " << p.second << endl;
    }
}

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
    vector<string> words;    
    map<string, int> freq;   
    string text;             
    int choice;

    while (true) {
        menu();
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
            case 1: {
                cout << "\n请输入一段文本：" << endl;
                getline(cin, text);

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
