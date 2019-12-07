#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <sstream>
using namespace std;

#define Int "integerConstant"
#define Sym "symbol"
#define Eof "End"
#define Str "stringConstant"
#define Idn "identifier"
#define Key "keyword"
#define tas "out"

string keyword[]={"class","constructor","method","field","static","var","int","char","boolean","void","true","false","null","this","let","do","if","else","while","return","function"};
stringstream ss,sss;
string line,r,t,file;
struct Token{
    string type;
    string value;

    Token(){}
    Token(string t,string v){
        type=t;
        value=v;
    }
    void print(){
        if(type==Int)
            cout<<"<"<<Int<<"> "<<value<<" </"<<Int<<">\n";
        if(type==Sym){
            if(value=="<")
                cout<<"<"<<Sym<<"> &lt;"<<" </"<<Sym<<">\n";
            else if(value==">")
                cout<<"<"<<Sym<<"> &gt;"<<value<<" </"<<Sym<<">\n";
            else
            cout<<"<"<<Sym<<"> "<<value<<" </"<<Sym<<">\n";
        }
        if(type==Str)
            cout<<"<"<<Str<<"> "<<value<<" </"<<Str<<">\n";
        if(type==Idn)
            cout<<"<"<<Idn<<"> "<<value<<" </"<<Idn<<">\n";
        if(type==Key)
            cout<<"<"<<Key<<"> "<<value<<" </"<<Key<<">\n";
    }
};

struct Tokenizer{
    string line;
    int i;
    Token cur;

    Tokenizer(){
        i=0;
        line="";
    }
    Token advance();
    Token getNextToken();
    bool clear(){
        if(getline(cin,file)){
            line=file;
            i=0;
            return 1;
        }
        return 0;
    }
};

Token Tokenizer::getNextToken(){
    Token tok;
    Re:
        if(i<line.length()){
            if(line[i]=='/'&&(line[i+1]=='/'||line[i+1]=='*')){
                tok.type=Eof;
                return tok;
            }
            if(line[i]==' '){
                i++;
                goto Re;
            }
            if(line[i]>='0'&&line[i]<='9'){
                cur.type=Int;
                cur.value="";
                return advance();
            }
            if((line[i]>='&'&&line[i]<='/')||(line[i]>=';'&&line[i]<='>')||(line[i]>='{'&&line[i]<='~')||line[i]=='['||line[i]==']'){
                tok.type=Sym;
                tok.value=line[i];
            }
            if((line[i]>='a'&&line[i]<='z')||(line[i]>='A'&&line[i]<='Z')||line[i]=='_'){
                cur.type=Idn;
                cur.value="";
                return advance();
            }
            if(line[i]=='"'){
                cur.type=Str;
                cur.value="";
                return advance();
            }
            i++;
        }
        else
        tok.type=Eof;
        return tok;
}

Token Tokenizer::advance(){
        if(cur.type==Int){
        while(i<line.length()&&line[i]>='0'&&line[i]<='9'){
            cur.value+=line[i];
            i++;
        }
        }
        if(cur.type==Idn){
        while(i<line.length()&&((line[i]>='a'&&line[i]<='z')||(line[i]>='A'&&line[i]<='Z')||(line[i]>='0'&&line[i]<='9')||line[i]=='_')){
            cur.value+=line[i];
            i++;
        }
        for(int j=0;j<22;j++){
            if(cur.value==keyword[j]){
                cur.type=Key;
            }
        }
        }
        if(cur.type==Str){
            i++;
        while(i<line.length()&&line[i]!='"'){
            cur.value+=line[i];
            i++;
        }
            i++;
        }
        return cur;
    }

    Token token;
    Tokenizer parse;

struct compileEngine{
    void compileClass(){
        cout<<"<class>\n";
        do{
            token=parse.getNextToken();
            if(token.type==Eof){
                parse.clear();
                continue;
            }
            cout<<"  ";
            token.print();
        }while(token.value!="{");
        do{
        token=parse.getNextToken();
        if(token.type==Eof){
                parse.clear();
                continue;
            }
        if(token.type==Key&&(token.value=="static"||token.value=="field"||token.value=="var"))
            compileClassVarDec(1);
        else if(token.type==Key&&token.value=="function"){
            compileSubroutineDec(1);
            token=parse.getNextToken();
            if(token.type==Eof){
                parse.clear();
            }
        }
        }while(token.value!="}");
        cout<<"  ";
        token.print();
        cout<<"</class>";
    }
    void compileClassVarDec(int n){
         string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<classVarDec>\n";
        cout<<s<<"  ";
        token.print();
        do{
            token=parse.getNextToken();
            cout<<s<<"  ";
            token.print();
        }while(token.value!=";");
        cout<<s<<"</classVarDec>\n";
    }
    void compileSubroutineDec(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<subroutineDec>\n";
        cout<<s<<"  ";
        token.print();
        do{
            token=parse.getNextToken();
            cout<<s<<"  ";
            token.print();
        }while(token.value!="(");
        compileParameterList(n+1);
        cout<<s<<"  ";
        token.print();
        compileSubroutineBody(n+1);
        cout<<s<<"</subroutineDec>\n";
    }
    void compileParameterList(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<parameterList>\n";
        while(token.value!=")"){
            token=parse.getNextToken();
            if(token.value!=")"){
            cout<<s<<"  ";
            token.print();
        }
        }
        cout<<s<<"</parameterList>\n";
    }
    void compileSubroutineBody(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<subroutineBody>\n";
        token=parse.getNextToken();
        while(token.type==Eof){
                parse.clear();
                token=parse.getNextToken();
            }
        cout<<s<<"  ";
        token.print();
        while(token.value!="}"){
            token=parse.getNextToken();
            if(token.value=="}")
                break;
            if(token.type==Eof){
                parse.clear();
                continue;
            }
            if(token.value=="var"&&token.type==Key)
                compileVarDec(n+1);
            else
            compileStatements(n+1);
        }
        cout<<s<<"  ";
        token.print();
        cout<<s<<"</subroutineBody>\n";
    }
    void compileStatements(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<statements>\n";
        do{
            if(token.value=="let"&&token.type==Key)
                compileLet(n+1);
            if(token.value=="do"&&token.type==Key)
                compileDo(n+1);
            if(token.value=="if"&&token.type==Key)
                compileIf(n+1);
            if(token.value=="while"&&token.type==Key)
                compileWhile(n+1);
            if(token.value=="else"&&token.type==Key)
                compileElse(n+1);
            if(token.value=="return"&&token.type==Key){
                compileReturn(n+1);
                break;
            }
            token=parse.getNextToken();
            if(token.type==Eof){
                parse.clear();
                continue;
            }
        }while(token.value!="}");
        cout<<s<<"</statements>\n";
    }
    void compileElse(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<elseStatement>\n";
        cout<<s<<"  ";
        token.print();
        token=parse.getNextToken();
        while(token.type==Eof){
                parse.clear();
                token=parse.getNextToken();
            }
        cout<<s<<"  ";
        token.print();
        token=parse.getNextToken();
        while(token.type==Eof){
                parse.clear();
                token=parse.getNextToken();
            }
        if(token.value!="}"){
            compileStatements(n+1);
        }
        cout<<s<<"  ";
        token.print();
        cout<<s<<"</elseStatement>\n";
    }
    void compileIf(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<ifStatement>\n";
        cout<<s<<"  ";
        token.print();
        token=parse.getNextToken();
        cout<<s<<"  ";
        token.print();
        compileExpressonList(n+1);
        cout<<s<<"  ";
        token.print();
        token=parse.getNextToken();
        while(token.type==Eof){
                parse.clear();
                token=parse.getNextToken();
            }
        cout<<s<<"  ";
        token.print();
        token=parse.getNextToken();
        while(token.type==Eof){
                parse.clear();
                token=parse.getNextToken();
            }
        if(token.value!="}"){
            compileStatements(n+1);
        }
        cout<<s<<"  ";
        token.print();
        cout<<s<<"</ifStatement>\n";
    }
    void compileWhile(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<whileStatement>\n";
        cout<<s<<"  ";
        token.print();
        token=parse.getNextToken();
        cout<<s<<"  ";
        token.print();
        compileExpressonList(n+1);
        cout<<s<<"  ";
        token.print();
        token=parse.getNextToken();
        while(token.type==Eof){
                parse.clear();
                token=parse.getNextToken();
            }
        cout<<s<<"  ";
        token.print();
        token=parse.getNextToken();
        while(token.type==Eof){
                parse.clear();
                token=parse.getNextToken();
            }
        if(token.value!="}"){
            compileStatements(n+1);
        }
        cout<<s<<"  ";
        token.print();
        cout<<s<<"</whileStatement>\n";
    }
    void compileLet(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<letStatement>\n";
        cout<<s<<"  ";
        token.print();
        do{
            token=parse.getNextToken();
            cout<<s<<"  ";
            token.print();
            if(token.value=="["){
                compileExpression(n+1);
                cout<<s<<"  ";
                token.print();
            }
        }while(token.value!="=");
        compileExpression(n+1);
        cout<<s<<"  ";
        token.print();
        cout<<s<<"</letStatement>\n";
    }
    void compileReturn(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<returnStatement>\n";
        cout<<s<<"  ";
        token.print();
        do{
            token=parse.getNextToken();
            cout<<s<<"  ";
            token.print();
        }while(token.value!=";");
        cout<<s<<"</returnStatement>\n";
    }
    void compileDo(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<doStatement>\n";
        cout<<s<<"  ";
        token.print();
        do{
            token=parse.getNextToken();
            cout<<s<<"  ";
            token.print();
        }while(token.value!="(");
        compileExpressonList(n+1);
        cout<<s<<"  ";
        token.print();
        token=parse.getNextToken();
        cout<<s<<"  ";
        token.print();
        cout<<s<<"</doStatement>\n";
    }
    void compileExpressonList(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<expressionList>\n";
        while(token.value!=")"){
            if(token.value!=")"){
                compileExpression(n+1);
                if(token.value!=")"){
                    cout<<s<<"  ";
                    token.print();
                }
            }
        }
        cout<<s<<"</expressionList>\n";
    }
    void compileExpression(int n){
        token=parse.getNextToken();
        if(token.value==")")
            return;
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<expression>\n";
        compileTerm(n+1);
        while(token.type==Sym&&token.value!=";"&&token.value!="]"&&token.value!=")"&&token.value!=","){
            cout<<s<<"  ";
            token.print();
            token=parse.getNextToken();
            compileTerm(n+1);
        }
        cout<<s<<"</expression>\n";
    }
    void compileTerm(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<term>\n";
            cout<<s<<"  ";
            token.print();
            if(token.type==Sym&&token.value=="("){
                compileExpression(n+1);
                cout<<s<<"  ";
                token.print();
            }
            if(token.type==Sym&&(token.value=="-"||token.value=="~")){
                token=parse.getNextToken();
                compileTerm(n+1);
                cout<<s<<"</term>\n";
                return;
            }
            token=parse.getNextToken();
            while(token.type==Eof){
                parse.clear();
                token=parse.getNextToken();
            }
            if(token.type==Sym&&token.value=="["){
                cout<<s<<"  ";
                token.print();
                compileExpression(n+1);
                    cout<<s<<"  ";
                    token.print();
                token=parse.getNextToken();
            }
            if(token.type==Sym&&token.value=="."){
                cout<<s<<"  ";
                token.print();
                token=parse.getNextToken();
                cout<<s<<"  ";
                token.print();
                token=parse.getNextToken();
                cout<<s<<"  ";
                token.print();
                compileExpressonList(n+1);
                cout<<s<<"  ";
                token.print();
                token=parse.getNextToken();
            }
        cout<<s<<"</term>\n";
    }
    void compileVarDec(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<varDec>\n";
        cout<<s<<"  ";
        token.print();
        do{
            token=parse.getNextToken();
            cout<<s<<"  ";
            token.print();
        }while(token.value!=";");
        cout<<s<<"</varDec>\n";
    }
};

void Compiler(const char* c){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    freopen(c,"r",stdin);
    string str=c;
    size_t f = str.find(".jack");
    string out=str.substr(0,f)+".xmp";
    freopen(out.c_str(),"w",stdout);
    getline(cin,file);
    parse.line=file;
    compileEngine comp;
    comp.compileClass();
}

int main(int argc, char* argv[]){
    Compiler(argv[1]);
    return 0;
}
