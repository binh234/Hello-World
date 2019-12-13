#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
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
ofstream fout;
ifstream fin;

struct Token{
    string type;
    string value;

    Token(){}
    Token(string t,string v){
        type=t;
        value=v;
    }

    //Writes tokens to output file
    void print(string s){
        if(type==Int)
            fout<<s<<"<"<<Int<<"> "<<value<<" </"<<Int<<">\n";
        if(type==Sym){
            if(value=="<")
                fout<<s<<"<"<<Sym<<"> &lt;"<<" </"<<Sym<<">\n";
            else if(value==">")
                fout<<s<<"<"<<Sym<<"> &gt;"<<" </"<<Sym<<">\n";
            else if(value=="&")
                fout<<s<<"<"<<Sym<<"> &amp;"<<" </"<<Sym<<">\n";
            else
            fout<<s<<"<"<<Sym<<"> "<<value<<" </"<<Sym<<">\n";
        }
        if(type==Str)
            fout<<s<<"<"<<Str<<"> "<<value<<" </"<<Str<<">\n";
        if(type==Idn)
            fout<<s<<"<"<<Idn<<"> "<<value<<" </"<<Idn<<">\n";
        if(type==Key)
            fout<<s<<"<"<<Key<<"> "<<value<<" </"<<Key<<">\n";
    }
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


//Gets next token from inputs
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


//Advance the current token
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


//Compiles .vm files and writes to .xml files
class compileEngine{
    private:
    Token token;
    Tokenizer parse;


    //Handling class declaration
    void compileClass(){
        fout<<"<class>\n";
        do{
            token=parse.getNextToken();
            token.print("  ");
        }while(token.value!="{");
        while(token.value!="}"){
        get:
        token=parse.getNextToken();
        if(token.type==Key&&(token.value=="static"||token.value=="field"||token.value=="var"))
            compileClassVarDec(1);
        else if(token.type==Key&&(token.value=="function"||token.value=="method"||token.value=="constructor")){
            compileSubroutineDec(1);
            goto get;
        }
        }
        token.print("  ");
        fout<<"</class>";
    }

    //Handling class variables declaration
    void compileClassVarDec(int n){
         string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<classVarDec>\n";
        token.print(s+ "  ");
        do{
            token=parse.getNextToken();
            token.print(s+ "  ");
        }while(token.value!=";");
        fout<<s<<"</classVarDec>\n";
    }

    //Handling subroutine declaration
    void compileSubroutineDec(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<subroutineDec>\n";
        token.print(s+ "  ");
        do{
            token=parse.getNextToken();
            token.print(s+ "  ");
        }while(token.value!="(");
        compileParameterList(n+1);
        token.print(s+ "  ");
        compileSubroutineBody(n+1);
        fout<<s<<"</subroutineDec>\n";
    }

    //Handling parameter list for subroutine functions
    void compileParameterList(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<parameterList>\n";
        while(token.value!=")"){
            token=parse.getNextToken();
            if(token.value!=")"){
            token.print(s+ "  ");
        }
        }
        fout<<s<<"</parameterList>\n";
    }

    //Handling subroutine body
    void compileSubroutineBody(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<subroutineBody>\n";
        token=parse.getNextToken();
        token.print(s+ "  ");
        while(token.value!="}"){
            token=parse.getNextToken();
            if(token.value=="}")
                break;
            if(token.value=="var"&&token.type==Key)
                compileVarDec(n+1);
            else
            compileStatements(n+1);
        }
        token.print(s+ "  ");
        fout<<s<<"</subroutineBody>\n";
    }

    //Handling var commands
    void compileVarDec(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<varDec>\n";
        token.print(s+ "  ");
        do{
            token=parse.getNextToken();
            token.print(s+ "  ");
        }while(token.value!=";");
        fout<<s<<"</varDec>\n";
    }

    //Handling all statements
    void compileStatements(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<statements>\n";
        do{
            if(token.value=="let"&&token.type==Key)
                compileLet(n+1);
            if(token.value=="do"&&token.type==Key)
                compileDo(n+1);
            if(token.value=="if"&&token.type==Key){
                compileIf(n+1);
                continue;
            }
            if(token.value=="while"&&token.type==Key)
                compileWhile(n+1);
            if(token.value=="return"&&token.type==Key){
                compileReturn(n+1);
            }
            token=parse.getNextToken();
            if(token.type==Eof){
                parse.getNextLine();
                continue;
            }
        }while(token.value!="}");
        fout<<s<<"</statements>\n";
    }

    //Handling if statements
    void compileIf(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<ifStatement>\n";
        token.print(s+ "  ");
        token=parse.getNextToken();
        token.print(s+ "  ");
        compileExpression(n+1);
        token.print(s+ "  ");
        token=parse.getNextToken();
        token.print(s+ "  ");
        token=parse.getNextToken();
        if(token.value!="}"){
            compileStatements(n+1);
        }
        token.print(s+ "  ");
        token=parse.getNextToken();
        if(token.value=="else"){
            token.print(s+ "  ");
            token=parse.getNextToken();
            token.print(s+ "  ");
        if(token.value!="}"){
            compileStatements(n+1);
        }
        token.print(s+ "  ");
        token=parse.getNextToken();
        }
        fout<<s<<"</ifStatement>\n";
    }

    //Handling while statements
    void compileWhile(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<whileStatement>\n";
        token.print(s+ "  ");
        token=parse.getNextToken();
        token.print(s+ "  ");
        compileExpression(n+1);
        token.print(s+ "  ");
        token=parse.getNextToken();
        token.print(s+ "  ");
        token=parse.getNextToken();
        if(token.value!="}"){
            compileStatements(n+1);
        }
        token.print(s+ "  ");
        fout<<s<<"</whileStatement>\n";
    }

    //Handling let statements
    void compileLet(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<letStatement>\n";
        token.print(s+ "  ");
        do{
            token=parse.getNextToken();
            token.print(s+ "  ");
            if(token.value=="["){
                compileExpression(n+1);
                token.print(s+ "  ");
            }
        }while(token.value!="=");
        compileExpression(n+1);
        token.print(s+ "  ");
        fout<<s<<"</letStatement>\n";
    }

    //Handling do statements
    void compileDo(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<doStatement>\n";
        token.print(s+ "  ");
        do{
            token=parse.getNextToken();
            token.print(s+ "  ");
        }while(token.value!="(");
        compileExpressionList(n+1);
        token.print(s+ "  ");
        token=parse.getNextToken();
        token.print(s+ "  ");
        fout<<s<<"</doStatement>\n";
    }

    //Handling return statements
    void compileReturn(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<returnStatement>\n";
        token.print(s+ "  ");
        compileExpression(n+1);
        token.print(s+ "  ");
        fout<<s<<"</returnStatement>\n";
    }

    //Handling expression list for function calls
    void compileExpressionList(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<expressionList>\n";
        while(token.value!=")"){
            if(token.value!=")"){
                compileExpression(n+1);
                if(token.value!=")"){
                    token.print(s+ "  ");
                }
            }
        }
        fout<<s<<"</expressionList>\n";
    }

    //Handling expressions
    void compileExpression(int n){
        token=parse.getNextToken();
        if(token.value==")"||token.value==";")
            return;
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<expression>\n";
        compileTerm(n+1);
        while(token.type==Sym&&token.value!=";"&&token.value!="]"&&token.value!=")"&&token.value!=","){
            token.print(s+ "  ");
            token=parse.getNextToken();
            compileTerm(n+1);
        }
        fout<<s<<"</expression>\n";
    }

    //Handling terms
    void compileTerm(int n){
        string s="";
        for(int i=0;i<n;i++)
            s+="  ";
        fout<<s<<"<term>\n";
            token.print(s+ "  ");
            if(token.type==Sym&&token.value=="("){
                compileExpression(n+1);
                token.print(s+ "  ");
            }
            if(token.type==Sym&&(token.value=="-"||token.value=="~")){
                token=parse.getNextToken();
                compileTerm(n+1);
                fout<<s<<"</term>\n";
                return;
            }
            token=parse.getNextToken();
            if(token.type==Sym&&token.value=="("){
                token.print(s+ "  ");
                compileExpressionList(n+1);
                    token.print(s+ "  ");
                token=parse.getNextToken();
            }
            if(token.type==Sym&&token.value=="["){
                token.print(s+ "  ");
                compileExpression(n+1);
                    token.print(s+ "  ");
                token=parse.getNextToken();
            }
            if(token.type==Sym&&token.value=="."){
                token.print(s+ "  ");
                token=parse.getNextToken();
                token.print(s+ "  ");
                token=parse.getNextToken();
                token.print(s+ "  ");
                compileExpressionList(n+1);
                token.print(s+ "  ");
                token=parse.getNextToken();
            }
        fout<<s<<"</term>\n";
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
    string str=c;
    string out=str.substr(0,str.length()-5)+".xml";
    fout.open(out,ios::out);
    compileEngine comp;
    comp.compileAll();
    fin.close();
    fout.close();
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
                string out=file.substr(0,file.length()-5)+".xml";
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
}

int main(int argc, char* argv[]){
    try{
    if(argc!=2)
        throw "Inaccurate usage. Please enter in the following format: JackAnalyzer (directory/filename)";
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
