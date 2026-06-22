#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

struct StrArr{
    char**data;int size,cap;
    StrArr(){cap=4;size=0;data=new char*[cap];}
    ~StrArr(){for(int i=0;i<size;i++)delete[]data[i];delete[]data;}
    void expand(){cap*=2;char**t=new char*[cap];for(int i=0;i<size;i++)t[i]=data[i];delete[]data;data=t;}
    void push(const char*s){if(size>=cap)expand();int l=strlen(s);data[size]=new char[l+1];strcpy(data[size++],s);}
    const char*operator[](int i)const{return data[i];}
};

#define HASH_MOD 101
struct KV{char key[64];int val;bool empty;KV(){empty=1;val=0;memset(key,0,64);}};
struct HashTable{
    KV table[HASH_MOD];
    int getHash(const char*s){int r=0;for(;*s;s++)r=(r*131+*s)%HASH_MOD;return r;}
    void insert(const char*s){
        int p=getHash(s);
        while(!table[p].empty&&strcmp(table[p].key,s))p=(p+1)%HASH_MOD;
        if(table[p].empty){strcpy(table[p].key,s);table[p].empty=0;table[p].val=1;}
        else table[p].val++;
    }
    int findVal(const char*s){
        int p=getHash(s),st=p;
        while(!table[p].empty){
            if(!strcmp(table[p].key,s))return table[p].val;
            p=(p+1)%HASH_MOD;if(p==st)break;
        }
        return 0;
    }
    bool exist(const char*s){return findVal(s)>0;}
    void getAll(StrArr&k,StrArr&v){char buf[20];for(int i=0;i<HASH_MOD;i++)if(!table[i].empty){k.push(table[i].key);sprintf(buf,"%d",table[i].val);v.push(buf);}}
};
char* preProcess(const char*t){
    int l=strlen(t),p=0;char*res=new char[l+1];
    for(int i=0;t[i];i++){
        if(isalpha(t[i]))res[p++]=tolower(t[i]);
        else if(isspace(t[i]))res[p++]=' ';
    }
    res[p]=0;return res;
}
StrArr splitWords(const char*t){
    StrArr arr;char buf[256]={0},idx=0;
    for(int i=0;t[i];i++){
        if(t[i]==' '){if(idx){buf[idx]=0;arr.push(buf);idx=0;memset(buf,0,256);}}
        else buf[idx++]=t[i];
    }
    if(idx){buf[idx]=0;arr.push(buf);}
    return arr;
}
StrArr splitSentences(const char*t){
    StrArr arr;char cur[512]={0},p=0;
    for(int i=0;t[i];i++){
        cur[p++]=t[i];
        if(t[i]=='.'||t[i]=='?'||t[i]=='!'){cur[p]=0;arr.push(cur);p=0;memset(cur,0,512);}
    }
    if(p){cur[p]=0;arr.push(cur);}
    return arr;
}
HashTable countFreq(StrArr&w){HashTable ht;for(int i=0;i<w.size;i++)ht.insert(w[i]);return ht;}
HashTable initDict(){
    HashTable d;
    const char*w[]={"i","you","he","she","is","are","the","a","an"};
    int n=sizeof(w)/sizeof(w[0]);
    for(int i=0;i<n;i++)d.insert(w[i]);
    return d;
}
void grammarCheck(const char*txt,StrArr&words){
    HashTable dict=initDict();
    StrArr sen=splitSentences(txt);
    cout<<"¡¾Æ´Ð´´íÎó¡¿\n";
    for(int i=0;i<words.size;i++)if(!dict.exist(words[i]))cout<<words[i]<<" ";
    cout<<"\n¡¾¾äÊ×´óÐ´¼ì²é¡¿\n";
    for(int i=0;i<sen.size;i++){
        const char*s=sen[i];int pos=0;
        while(s[pos]&&!isalpha(s[pos]))pos++;
        if(s[pos]&&islower(s[pos]))cout<<sen[i]<<endl;
    }
}
char* replaceTxt(char*src,const char*old,const char*nw){
    char tmp[1024]={0},*p=src;
    int ol=strlen(old),nwl=strlen(nw);
    while((p=strstr(p,old))){
        strncat(tmp,src,(int)(p-src));
        strcat(tmp,nw);
        src=p+ol;
    }
    strcat(tmp,src);
    char*out=new char[strlen(tmp)+1];strcpy(out,tmp);
    return out;
}
