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
string line,r,t;
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
            ss<<"<"<<Int<<"> "<<value<<" </"<<Int<<">\n";
        if(type==Sym){
            if(value=="<")
                ss<<"<"<<Sym<<"> &lt;"<<" </"<<Sym<<">\n";
            else if(value==">")
                ss<<"<"<<Sym<<"> &gt;"<<value<<" </"<<Sym<<">\n";
            else
            ss<<"<"<<Sym<<"> "<<value<<" </"<<Sym<<">\n";
        }
        if(type==Str)
            ss<<"<"<<Str<<"> "<<value<<" </"<<Str<<">\n";
        if(type==Idn)
            ss<<"<"<<Idn<<"> "<<value<<" </"<<Idn<<">\n";
        if(type==Key)
            ss<<"<"<<Key<<"> "<<value<<" </"<<Key<<">\n";
        if(value!="")
        sss<<value<<endl;
    }
};

struct Tokenizer{
    string line;
    Token cur;
    int i;
    char c;

    Tokenizer(){}
    Tokenizer(string l){
        line=l;
        i=0;
    }
    Token advance();
    Token getNextToken();
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

struct compileEngine{
    void compileClass(){
        cout<<"<class>\n";
        do{
            getline(ss,r);
            cout<<"  "<<r<<endl;
            getline(sss,line);
        }while(line!="{");
        do{
        getline(sss,line);
        getline(ss,r);
        stringstream a(r);
        a>>t;
        if(t=="<keyword>"&&(line=="static"||line=="field"||line=="var")){
            compileClassVarDec(1);
        }
        else if(t=="<keyword>"&&line=="function")
            compileSubroutineDec(1);
        }while(line!="}");
        getline(ss,line);
        cout<<"  "<<r<<endl;
        cout<<"</class>";
    }
    void compileClassVarDec(int n){
         string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<classVarDec>\n";
        cout<<s<<"  "<<r<<endl;
        do{
            getline(ss,r);
            cout<<s<<"  "<<r<<endl;
            getline(sss,line);
        }while(line!=";");
        cout<<s<<"</classVarDec>\n";
    }
    void compileSubroutineDec(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<subroutineDec>\n";
        cout<<s<<"  "<<r<<endl;
        do{
            getline(ss,r);
            cout<<s<<"  "<<r<<endl;
            getline(sss,line);
        }while(line!="(");
        compileParameterList(n+1);
        getline(ss,line);
        cout<<s<<"  "<<line<<endl;
        compileSubroutineBody(n+1);
        cout<<s<<"</subroutineDec>\n";
    }
    void compileParameterList(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<parameterList>\n";
        getline(sss,line);
        while(line!=")"){
            getline(ss,r);
            cout<<s<<"  "<<r<<endl;
            getline(sss,line);
        }
        cout<<s<<"</parameterList>\n";
    }
    void compileSubroutineBody(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<subroutineBody>\n";
        getline(sss,line);
        getline(ss,line);
        cout<<s<<"  "<<line<<endl;
        getline(sss,line);
        while(line!="}"){
            getline(ss,r);
            stringstream a(r);
            a>>t;
            if(line=="var"&&t=="<keyword>")
                compileVarDec(n+1);
            else
            compileStatements(n+1);
            getline(sss,line);
        }
        getline(ss,line);
        cout<<s<<"  "<<line<<endl;
        cout<<s<<"</subroutineBody>\n";
    }
    void compileStatements(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<statements>\n";
        do{
            if(line=="let"&&t=="<keyword>")
                compileLet(n+1);
            if(line=="do"&&t=="<keyword>")
                compileDo(n+1);
            if(line=="if"&&t=="<keyword>")
                compileIf(n+1);
            if(line=="while"&&t=="<keyword>")
                compileWhile(n+1);
            if(line=="else"&&t=="<keyword>")
                compileElse(n+1);
            if(line=="return"&&t=="<keyword>"){
                compileReturn(n+1);
                break;
            }
            getline(sss,line);
            getline(ss,r);
            stringstream a(r);
            a>>t;
        }while(line!="}");
        cout<<s<<"</statements>\n";
    }
    void compileElse(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<elseStatement>\n";
        cout<<s<<"  "<<r<<endl;
        getline(ss,r);
        cout<<s<<"  "<<r<<endl;
        getline(sss,line);
        getline(sss,line);
        if(line!="}"){
            getline(ss,r);
            stringstream a(r);
            a>>t;
            compileStatements(n+1);
        }
        cout<<s<<"  "<<r<<endl;
        cout<<s<<"</elseStatement>\n";
    }
    void compileIf(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<ifStatement>\n";
        cout<<s<<"  "<<r<<endl;
        getline(ss,r);
        cout<<s<<"  "<<r<<endl;
        getline(sss,line);
        compileExpressonList(n+1);
        cout<<s<<"  "<<r<<endl;
        getline(sss,line);
        getline(ss,line);
        cout<<s<<"  "<<line<<endl;
        getline(sss,line);
        if(line!="}"){
            getline(ss,r);
            stringstream a(r);
            a>>t;
            compileStatements(n+1);
        }
        cout<<s<<"  "<<r<<endl;
        cout<<s<<"</ifStatement>\n";
    }
    void compileWhile(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<whileStatement>\n";
        cout<<s<<"  "<<r<<endl;
        getline(ss,r);
        cout<<s<<"  "<<r<<endl;
        getline(sss,line);
        compileExpressonList(n+1);
        cout<<s<<"  "<<r<<endl;
        getline(sss,line);
        getline(ss,line);
        cout<<s<<"  "<<line<<endl;
        getline(sss,line);
        if(line!="}"){
            getline(ss,r);
            stringstream a(r);
            a>>t;
            compileStatements(n+1);
        }
        cout<<s<<"  "<<r<<endl;
        cout<<s<<"</whileStatement>\n";
    }
    void compileLet(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<letStatement>\n";
        cout<<s<<"  "<<r<<endl;
        do{
            getline(ss,r);
            getline(sss,line);
            cout<<s<<"  "<<r<<endl;
            if(line=="["){
                compileExpression(n+1);
                cout<<s<<"  "<<r<<endl;
            }
        }while(line!="=");
        compileExpression(n+1);
        cout<<s<<"  "<<r<<endl;
        cout<<s<<"</letStatement>\n";
    }
    void compileReturn(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<returnStatement>\n";
        cout<<s<<"  "<<r<<endl;
        do{
            getline(ss,r);
            cout<<s<<"  "<<r<<endl;
            getline(sss,line);
        }while(line!=";");
        cout<<s<<"</returnStatement>\n";
    }
    void compileDo(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<doStatement>\n";
        cout<<s<<"  "<<r<<endl;
        do{
            getline(ss,r);
            cout<<s<<"  "<<r<<endl;
            getline(sss,line);
        }while(line!="(");
        compileExpressonList(n+1);
        cout<<s<<"  "<<r<<endl;
        getline(sss,line);
        getline(ss,line);
        cout<<s<<"  "<<line<<endl;
        cout<<s<<"</doStatement>\n";
    }
    void compileExpressonList(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<expressionList>\n";
        while(line!=")"){
            if(line!=")"){
                compileExpression(n+1);
                if(line!=")")
                    cout<<s<<"  "<<r<<endl;
            }
        }
        cout<<s<<"</expressionList>\n";
    }
    void compileExpression(int n){
        getline(sss,line);
        getline(ss,r);
        if(line==")")
            return;
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<expression>\n";
        compileTerm(n+1);
        if(t=="<symbol>"&&line!=";"&&line!="]"&&line!=")"&&line!=","){
            cout<<s<<"  "<<r<<endl;
            getline(sss,line);
            getline(ss,r);
            compileTerm(n+1);
        }
        cout<<s<<"</expression>\n";
    }
    void compileTerm(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<term>\n";
            cout<<s<<"  "<<r<<endl;
            stringstream a(r);
            a>>t;
            if(t=="<symbol>"&&line=="("){
                compileExpression(n+1);
                cout<<s<<"  "<<r<<endl;
            }
            if(t=="<symbol>"&&(line=="-"||line=="~")){
                getline(sss,line);
                getline(ss,r);
                compileTerm(n+1);
                cout<<s<<"</term>\n";
                return;
            }
            getline(sss,line);
            getline(ss,r);
            stringstream b(r);
            b>>t;
            if(t=="<symbol>"&&line=="["){
                cout<<s<<"  "<<r<<endl;
                compileExpression(n+1);
                    cout<<s<<"  "<<r<<endl;
                getline(sss,line);
                getline(ss,r);
                stringstream c(r);
                c>>t;
            }
            if(t=="<symbol>"&&line=="."){
                cout<<s<<"  "<<r<<endl;
                getline(sss,line);
                getline(ss,r);
                cout<<s<<"  "<<r<<endl;
                getline(sss,line);
                getline(ss,r);
                cout<<s<<"  "<<r<<endl;
                compileExpressonList(n+1);
                cout<<s<<"  "<<r<<endl;
                getline(sss,line);
                getline(ss,r);
                stringstream c(r);
                c>>t;
            }
        cout<<s<<"</term>\n";
    }
    void compileVarDec(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        cout<<s<<"<varDec>\n";
        cout<<s<<"  "<<r<<endl;
        do{
            getline(ss,r);
            cout<<s<<"  "<<r<<endl;
            getline(sss,line);
        }while(line!=";");
        cout<<s<<"</varDec>\n";
    }
};

void In(){
    while(cin.good()){
    getline(cin,line);
    if(line[0]!='/'&& line!=""){
    Tokenizer Analyzer(line);
    Token c=Analyzer.getNextToken();
    while(c.type!=Eof){
        c.print();
        c=Analyzer.getNextToken();
    }
    }
    }
}

void Compiler(const char* c){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    freopen(c,"r",stdin);
    string str=c;
    size_t f = str.find(".jack");
    string out=str.substr(0,f)+".xml";
    freopen(out.c_str(),"w",stdout);
    In();
    compileEngine comp;
    comp.compileClass();
}

int main(int argc, char* argv[]){
    Compiler(argv[1]);
    return 0;
}
