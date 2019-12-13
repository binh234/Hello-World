#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <stack>
#include <dirent.h>
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
string varKind[4]={"this","static","local","argument"};
string file;
int c=0,d=0;
ofstream fout;
ifstream fin;

struct Token{
    string type;
    string value;
};

class Tokenizer{
    private:
    string line;
    int i;
    Token cur;


    Token advance();
    public:
    Tokenizer(){
        i=0;
        line="";
    }
    Token getNextToken();
    void getNextLine(){
        getline(fin,line);
        i=0;
    }
};

//Gets next token from input
Token Tokenizer::getNextToken(){
    Token tok;
    Re:
        if(i<line.length()){
            if(line[i]=='/'&&line[i+1]=='/'){
                getNextLine();
                goto Re;
            }
            else if(line[i]=='/'&&line[i+1]=='*'){
                while(line.find("*/")==std::string::npos){
                    getNextLine();
                }
                getNextLine();
                goto Re;
            }
            else if(line[i]==' '){
                while(line[i]==' ')
                    i++;
                goto Re;
            }
            else if(line[i]>='0'&&line[i]<='9'){
                cur.type=Int;
                cur.value="";
                return advance();
            }
            else if((line[i]>='&'&&line[i]<='/')||(line[i]>=';'&&line[i]<='>')||(line[i]>='{'&&line[i]<='~')||line[i]=='['||line[i]==']'){
                tok.type=Sym;
                tok.value=line[i];
            }
            else if((line[i]>='a'&&line[i]<='z')||(line[i]>='A'&&line[i]<='Z')||line[i]=='_'){
                cur.type=Idn;
                cur.value="";
                return advance();
            }
            else if(line[i]=='"'){
                cur.type=Str;
                cur.value="";
                return advance();
            }
            else{
                i++;
                goto Re;
            }
            i++;
        }
        else{
            getNextLine();
            goto Re;
        }
        return tok;
}

//Advances token
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


struct Symbol{
    string name;
    string type;
    string kind;
    int index;

    Symbol():index(0){};

};

struct SymbolTable{
    vector<Symbol> vec;
    vector<int> varCount;

    SymbolTable(){
    for(int i=0;i<4;i++)
        varCount.push_back(0);
    }

    //Gets field, static and local variables for class table and subroutine table
    void getNextSymbol(Tokenizer& parse,Token& t,int& n){
        Symbol s;
        s.kind=t.value;
        if(s.kind=="var")
            s.kind="local";
        if(s.kind=="field")
            s.kind="this";
        t=parse.getNextToken();
        s.type=t.value;
        while(t.value!=";"){
        t=parse.getNextToken();
        s.name=t.value;
        for(int i=0;i<4;i++){
            if(s.kind==varKind[i]){
                s.index=varCount[i];
                varCount[i]++;
                break;
            }
        }
        vec.push_back(s);
        n++;
        t=parse.getNextToken();
        }
    }

    //Gets argument variables for subroutine table
    void getSubroutineSymbol(Tokenizer& parse,Token& t){
        Symbol s;
        s.kind="argument";
        s.type=t.value;
        t=parse.getNextToken();
        s.name=t.value;
        s.index=varCount[3];
        varCount[3]++;
        vec.push_back(s);
    }

    //Finds symbol in symbol table
    bool findSymbol(string s,int& idx){
        for(int i=0;i<vec.size();i++){
            if(s==vec[i].name){
                idx=i;
                return true;
            }
        }
        return false;
    }

    //Clear symbol table for each subroutine table
    void clear(){
        vec.clear();
        varCount.clear();
        for(int i=0;i<4;i++)
        varCount.push_back(0);
    }
};

//Compiles .jack files and writes to .vm files
class compileEngine{
    private:
    Token token;
    Tokenizer parse;
    string className;
    SymbolTable classTable;
    SymbolTable subroutineTable;

    //Handling class
    void compileClass(){
        int n=0;
        token=parse.getNextToken();
        token=parse.getNextToken();
        className=token.value;
        while(token.value!="}"){
        get:
        token=parse.getNextToken();
        if(token.type==Key&&token.value!="function"&&token.value!="method"&&token.value!="constructor")
            compileClassVarDec(n);
        else if(token.type==Key&&(token.value=="function"||token.value=="method"||token.value=="constructor")){
            compileSubroutineDec();
            subroutineTable.clear();
            c=d=0;
            goto get;
        }
        }
    }

    //Handling class variables declaration
    void compileClassVarDec(int& n){
        classTable.getNextSymbol(parse,token,n);
    }

    //Handling subroutine functions
    void compileSubroutineDec(){
        Symbol a;
        string funcName,s,funcType;
        int n=0;
        funcType=token.value;
        s=parse.getNextToken().value;
        funcName=parse.getNextToken().value;
        token=parse.getNextToken();
        if(funcType=="method"){
            a.kind="argument";
            a.type=className;
            a.name="this";
            a.index=0;
            subroutineTable.vec.push_back(a);
            subroutineTable.varCount[3]+=1;
        }
        compileParameterList();
        token=parse.getNextToken();
        while(token.value!="}"){
            token=parse.getNextToken();
            if(token.value=="}")
                break;
            if(token.value=="var"&&token.type==Key)
                compileVarDec(n);
            else{
            fout<<"function "<<className<<"."<<funcName<<" "<<n<<endl;
            if(funcType=="constructor"){
                fout<<"push constant "<<classTable.vec.size()<<endl;
                fout<<"call Memory.alloc 1\n"<<"pop pointer 0\n";
            }
            if(funcType=="method")
                fout<<"push argument 0\n"<<"pop pointer 0\n";
            compileStatements(s);
            }
        }
    }

    //Handling parameter list of subroutine functions
    void compileParameterList(){
        token=parse.getNextToken();
        while(token.value!=")"){
            subroutineTable.getSubroutineSymbol(parse,token);
            token=parse.getNextToken();
            if(token.value==")")
                break;
            token=parse.getNextToken();
        }
    }

    //Handling var commands
    void compileVarDec(int& n){
        subroutineTable.getNextSymbol(parse,token,n);
    }

    //Handling all statements
    void compileStatements(string s){
        do{
            if(token.value=="let"&&token.type==Key)
                compileLet();
            if(token.value=="do"&&token.type==Key)
                compileDo();
            if(token.value=="if"&&token.type==Key){
                compileIf(s);
                continue;
            }
            if(token.value=="while"&&token.type==Key)
                compileWhile(s);
            if(token.value=="return"&&token.type==Key)
                compileReturn(s);
            token=parse.getNextToken();
        }while(token.value!="}");
    }

    //Handling if statements
    void compileIf(string s){
        int m=0;
        token=parse.getNextToken();
        compileExpression(m);
        m=c;
        fout<<"not\n";
        fout<<"if-goto IF_FALSE"<<c++<<endl;
        token=parse.getNextToken();
        token=parse.getNextToken();
        if(token.value!="}"){
            compileStatements(s);
        }
        token=parse.getNextToken();
        fout<<"goto IF_END"<<m<<endl;
        fout<<"label IF_FALSE"<<m<<endl;
        if(token.value=="else"){
            token=parse.getNextToken();
        if(token.value!="}"){
            compileStatements(s);
        }
        token=parse.getNextToken();
        }
        fout<<"label IF_END"<<m<<endl;
    }

    //Handling while statements
    void compileWhile(string s){
        int m=0;
        fout<<"label WHILE_EXP"<<d<<endl;
        token=parse.getNextToken();
        compileExpression(m);
        m=d;
        fout<<"not\n";
        fout<<"if-goto WHILE_END"<<d++<<endl;
        token=parse.getNextToken();
        token=parse.getNextToken();
        if(token.value!="}"){
            compileStatements(s);
        }
        fout<<"goto WHILE_EXP"<<m<<endl;
        fout<<"label WHILE_END"<<m<<endl;
    }

    //Handling let statements
    void compileLet(){
        string s;
        int idx=0;
        bool isArray=0;
        token=parse.getNextToken();
        s=token.value;
        token=parse.getNextToken();
        if(token.value=="["){
            isArray=1;
            if(subroutineTable.findSymbol(s,idx)){
                fout<<"push "<<subroutineTable.vec[idx].kind<<" "<<subroutineTable.vec[idx].index<<endl;
            }
            else if(classTable.findSymbol(s,idx)){
                fout<<"push "<<classTable.vec[idx].kind<<" "<<classTable.vec[idx].index<<endl;
            }
            compileExpression(idx);
            token=parse.getNextToken();
            fout<<"add\n";
        }
        compileExpression(idx);
        if(isArray){
            fout<<"pop temp 0\n"<<"pop pointer 1\n"<<"push temp 0\n"<<"pop that 0\n";
        }
        else if(subroutineTable.findSymbol(s,idx)){
            fout<<"pop "<<subroutineTable.vec[idx].kind<<" "<<subroutineTable.vec[idx].index<<endl;
        }
        else if(classTable.findSymbol(s,idx)){
            fout<<"pop "<<classTable.vec[idx].kind<<" "<<classTable.vec[idx].index<<endl;
        }
    }

    //Handling do statements
    void compileDo(){
        int n=0,idx=0;
        string funcName;
        do{
            token=parse.getNextToken();
            if (token.value!="(")
                funcName+=token.value;
        }while(token.value!="(");
        if(funcName.find(".")==std::string::npos){
            fout<<"push pointer 0\n";
            funcName=className + "." + funcName;
            n++;
        }
        else{
            size_t f=funcName.find(".");
            string s=funcName.substr(0,f);
            if(subroutineTable.findSymbol(s,idx)){
                fout<<"push "<<subroutineTable.vec[idx].kind<<" "<<subroutineTable.vec[idx].index<<endl;
                funcName=subroutineTable.vec[idx].type+funcName.substr(f);
                n++;
            }
            else if(classTable.findSymbol(s,idx)){
                fout<<"push "<<classTable.vec[idx].kind<<" "<<classTable.vec[idx].index<<endl;
                funcName=classTable.vec[idx].type+funcName.substr(f);
                n++;
            }
        }
        compileExpressionList(n);
        fout<<"call "<<funcName<<" "<<n<<endl;
        fout<<"pop temp 0\n";
    }

    //Handling return statements
    void compileReturn(string s){
        int n=0;
        if(s=="void")
            fout<<"push constant 0\n";
        compileExpression(n);
        fout<<"return\n";
    }

    //Handling expression list for function calls
    void compileExpressionList(int& n){
        while(token.value!=")"){
            compileExpression(n);
        }
    }

    //Handling expressions
    void compileExpression(int& n){
        token=parse.getNextToken();
        if(token.value==")"||token.value==";")
            return;
        compileTerm();
        n++;
        while(token.type==Sym&&token.value!=";"&&token.value!="]"&&token.value!=")"&&token.value!=","){
            string s=token.value;
            token=parse.getNextToken();
            compileTerm();
            compileOp(s);
        }
    }

    //Handling terms
    void compileTerm(){
        string s=token.value;
        string funcName;
        int idx=0;
        if(token.type==Str){
            fout<<"push constant "<<token.value.length()<<endl;
            fout<<"call String.new 1\n";
            for(int i=0;i<token.value.length();i++){
                fout<<"push constant "<<int(token.value[i])<<endl;
                fout<<"call String.appendChar 2\n";
            }
        }
        else if(isdigit(s[0]))
            fout<<"push constant "<<s<<endl;
        else if(s=="this")
            fout<<"push pointer 0\n";
        else if(subroutineTable.findSymbol(s,idx)){
            fout<<"push "<<subroutineTable.vec[idx].kind<<" "<<subroutineTable.vec[idx].index<<endl;
        }
        else if(classTable.findSymbol(s,idx)){
            fout<<"push "<<classTable.vec[idx].kind<<" "<<classTable.vec[idx].index<<endl;
        }
        else if(s=="null"||s=="true"||s=="false"){
            fout<<"push constant 0\n";
            if(s=="true")
                fout<<"not\n";
        }
        int n=0;
        if(token.type==Sym&&token.value=="("){
            compileExpression(idx);
        }
            if(token.type==Sym&&(token.value=="-"||token.value=="~")){
                s=token.value;
                token=parse.getNextToken();
                compileTerm();
                if(s=="-")
                    fout<<"neg\n";
                else
                    fout<<"not\n";
                return;
            }
            token=parse.getNextToken();
            if(token.type==Sym&&token.value=="("){
                fout<<"push pointer 0\n";
                compileExpressionList(n);
                n++;
                funcName=className + "." + s;
                fout<<"call "<<funcName<<" "<<n<<endl;
                token=parse.getNextToken();
            }
            if(token.type==Sym&&token.value=="["){
                compileExpression(idx);
                fout<<"add\n"<<"pop pointer 1\n"<<"push that 0\n";
                token=parse.getNextToken();
            }
            if(token.value=="."){
                funcName="."+parse.getNextToken().value;
                parse.getNextToken();
                if(subroutineTable.findSymbol(s,idx)){
                    funcName=subroutineTable.vec[idx].type+funcName;
                    n++;
                }
                else if(classTable.findSymbol(s,idx)){
                    funcName=classTable.vec[idx].type+funcName;
                    n++;
                }
                else
                    funcName=s+funcName;
                compileExpressionList(n);
                fout<<"call "<<funcName<<" "<<n<<endl;
                token=parse.getNextToken();
            }
    }

    //Handling operators
    void compileOp(string s){
        if(s=="+")
            fout<<"add\n";
        if(s=="-")
            fout<<"sub\n";
        if(s=="*")
            fout<<"call Math.multiply 2\n";
        if(s=="/")
            fout<<"call Math.divide 2\n";
        if(s==">")
            fout<<"gt\n";
        if(s=="<")
            fout<<"lt\n";
        if(s=="=")
            fout<<"eq\n";
        if(s=="&")
            fout<<"and\n";
        if(s=="|")
            fout<<"or\n";
    }

    public:
        compileEngine(){
        parse.getNextLine();
        }
        void compileAll(){
        compileClass();
        }
};

void CompileFile(const char* c){
    fin.open(c,ios::in);
    if(fin.good()){
    string str=c;
    string out=str.substr(0,str.length()-5)+".vm";
    fout.open(out,ios::out);
    compileEngine comp;
    comp.compileAll();
    fout.close();
    fin.close();
    }
    else{
        throw "Can't find such file name or directory, please make sure that you enter the right file name or directory";
    }
}

void CompileDirectory(const char* c){
    DIR *pDIR;
    struct dirent *entry;
    string str=c;
    if( pDIR=opendir(c)){
        while(entry = readdir(pDIR)){
            string file=entry->d_name;
            if(file.find(".jack")!=std::string::npos){
                file=str+"/"+file;
                string out=file.substr(0,file.length()-5)+".vm";
                fin.open(file,ios::in);
                fout.open(out,ios::out);
                compileEngine comp;
                comp.compileAll();
                fin.close();
                fout.close();
            }
        }
            closedir(pDIR);
    }
    else{
        throw "Can't find such file name or directory, please make sure that you enter the right file name or directory";
    }
}

int main(int argc, char* argv[]){
    try{
    if(argc!=2)
        throw "Inaccurate usage. Please enter in the following format: JackCompiler (directory/filename)";
    else{
        string file=argv[1];
    if(file.find(".")==std::string::npos)
        CompileDirectory(argv[1]);
    else if(file.find(".jack")!=std::string::npos)
        CompileFile(argv[1]);
    else
        throw "Not the correct file type. Please enter a .jack file or a directory containing .jack files.";
    }
    }
    catch (const char* msg) {
     cerr << msg << endl;
    }
    return 0;
}
