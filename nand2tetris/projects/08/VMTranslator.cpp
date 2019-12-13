#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <sstream>
#include<dirent.h>
using namespace std;

string line;            //Uses to get line from input files
string filename;        //Uses for handling static commands
string str[10000];      //Uses for handling call commands
string s[2],r[3];       //Uses for handling inputs
int c=0;                //Uses for handling call commands
int d=0;                //Uses for handling compare commands
int n=0;                //Uses to initial local variables of functions
int m=0;                //Uses for handling static count
bool b=true;
ifstream fin;           //Uses for reading input files
ofstream fout;          //Uses for writing output file

void In(ifstream& fin,ofstream& fout){
    //Handling inputs
    while(getline(fin,line)){
        stringstream ss(line);
        if(ss.str()[0]=='/'||ss.str()=="")
            continue;
        ss>>r[0];
        fout<<"//"<<r[0]<<" ";

        //Handling function commands
        if(r[0]=="function"){
            ss>>r[1];
            ss>>n;
            fout<<r[1]<<" "<<n<<endl;
            if(b){                  //call Sys.init function if the .vm file has function calls, uses only once
                b=false;
                fout<<"@Sys.init\n"<<"0;JMP\n";
            }
            fout<<"("<<r[1]<<")\n";
            fout<<"@SP\n"<<"D=M\n"<<"@LCL\n"<<"M=D\n"<<"@14\n"<<"M=D\n";
            for(int i=0;i<n;i++)
                fout<<"@14\n"<<"A=M\n"<<"M=0\n"<<"@14\n"<<"M=M+1\n";
            fout<<"@"<<n<<"\nD=A\n"<<"@SP\n"<<"M=M+D\n";
        }

        //Handling calling commands, saved variables before call
        else if(r[0]=="call"){
                ss>>r[1];
                ss>>n;
                str[c]=r[1]+to_string(c);
                fout<<r[1]<<" "<<n<<endl;
            fout<<"@LCL\n"<<"D=M\n"<<"@SP\n"<<"A=M+1\n"<<"M=D\n";
            fout<<"@ARG\n"<<"D=M\n"<<"@SP\n"<<"A=M+1\n"<<"A=A+1\n"<<"M=D\n";
            fout<<"@THIS\n"<<"D=M\n"<<"@SP\n"<<"A=M+1\n"<<"A=A+1\n"<<"A=A+1\n"<<"M=D\n";
            fout<<"@THAT\n"<<"D=M\n"<<"@SP\n"<<"A=M+1\n"<<"A=A+1\n"<<"A=A+1\n"<<"A=A+1\n"<<"M=D\n";
            fout<<"@"<<n<<"\nD=A\n"<<"@SP\n"<<"D=M-D\n"<<"@ARG\n"<<"M=D\n";
            fout<<"@"<<str[c]<<"\nD=A\n"<<"@SP\n"<<"A=M\n"<<"M=D\n";
            fout<<"@5\n"<<"D=A\n"<<"@SP\n"<<"M=M+D\n";
            fout<<"@"<<r[1]<<"\n0;JMP\n";
            fout<<"("<<str[c]<<")\n";
            c++;
        }

        //Handling return commands, restore all memories before return
        else if(r[0]=="return"){
            fout<<endl;
            fout<<"@5\n"<<"D=A\n"<<"@LCL\n"<<"A=M-D\n"<<"D=M\n"<<"@15\n"<<"M=D\n";
            fout<<"@SP\n"<<"A=M-1\n"<<"D=M\n"<<"@ARG\n"<<"A=M\n"<<"M=D\n";
            fout<<"@ARG\n"<<"D=M+1\n"<<"@SP\n"<<"M=D\n";
            fout<<"@3\n"<<"D=A\n"<<"@LCL\n"<<"A=M-D\n"<<"D=M\n"<<"@ARG\n"<<"M=D\n";
            fout<<"@2\n"<<"D=A\n"<<"@LCL\n"<<"A=M-D\n"<<"D=M\n"<<"@THIS\n"<<"M=D\n";
            fout<<"@1\n"<<"D=A\n"<<"@LCL\n"<<"A=M-D\n"<<"D=M\n"<<"@THAT\n"<<"M=D\n";
            fout<<"@4\n"<<"D=A\n"<<"@LCL\n"<<"A=M-D\n"<<"D=M\n"<<"@LCL\n"<<"M=D\n";
            fout<<"@15\n"<<"A=M\n"<<"0;JMP\n";
        }

        //Handling label commands
        else if(r[0]=="label"){
            ss>>r[1];
            fout<<r[1]<<endl;
            fout<<"("<<r[1]<<")\n";
        }

        //Handling goto commands
        else if(r[0]=="goto"){
            ss>>r[1];
            fout<<r[1]<<endl;
            fout<<"@"<<r[1]<<"\n"<<"0;JMP\n";
        }

        //Handling if-goto commands
        else if(r[0]=="if-goto"){
            ss>>r[1];
            fout<<r[1]<<endl;
            fout<<"@SP\n"<<"M=M-1\n"<<"A=M\n"<<"D=M\n"<<"@"<<r[1]<<"\nD;JNE\n";
        }

        //Handling push-pop commands
        else if(ss>>r[1]>>r[2]&&r[1][0]!='/'){
            fout<<r[1]<<" "<<r[2]<<endl;
            if(r[1]=="local"){ s[0]="@LCL";}
            else if(r[1]=="argument") {s[0]="@ARG";}
            else if(r[1]=="this"||r[1]=="pointer") {s[0]="@THIS";}
            else if(r[1]=="that") {s[0]="@THAT";}
            else if(r[1]=="temp") {s[0]="@5";}
            s[1]="@"+r[2];

            if(r[1]=="constant"){
                fout<<s[1]<<"\nD=A\n"<<"@SP\n"<<"A=M\n"<<"M=D\n"<<"@SP\n"<<"M=M+1\n";
            }
            else{
        //Handing push commands
            if(r[0]=="push"){
                if(r[1]=="local"||r[1]=="argument"||r[1]=="this"||r[1]=="that"){
                fout<<s[0]<<"\nD=M\n"<<s[1]<<"\nA=D+A\n";
                }
                else if(r[1]=="static"){
                    while(m<stoi(r[2])){
                        fout<<"@"<<filename<<m<<"\n";
                        m++;
                    }
                    fout<<"@"<<filename<<r[2]<<endl;
                    if(m==stoi(r[2]))
                        m++;
                    fout<<"D=A\n";
                }
                else{
                    fout<<s[0]<<"\nD=A\n"<<s[1]<<"\nA=D+A\n";
                }
                fout<<"D=M\n"<<"@SP\n"<<"A=M\n"<<"M=D\n"<<"@SP\n"<<"M=M+1\n";
            }
         //Handing pop commands
            else{
            if(r[1]=="local"||r[1]=="argument"||r[1]=="this"||r[1]=="that"){
                fout<<s[0]<<"\nD=M\n"<<s[1]<<"\nD=D+A\n"<<"@14\n"<<"M=D\n";
            }
            else if(r[1]=="static"){
                while(m<stoi(r[2])){
                    fout<<"@"<<filename<<m<<"\n";
                    m++;
                }
                fout<<"@"<<filename<<r[2]<<endl;
                if(m==stoi(r[2]))
                    m++;
                fout<<"D=A\n"<<"@14\n"<<"M=D\n";
            }
            else{
                fout<<s[0]<<"\nD=A\n"<<s[1]<<"\nD=D+A\n"<<"@14\n"<<"M=D\n";
            }
                fout<<"@SP\n"<<"M=M-1\n"<<"A=M\n"<<"D=M\n"<<"@14\n"<<"A=M\n"<<"M=D\n";
            }
            }
        }

        //Handling operator commands
       else{
            fout<<"\n";
            if(r[0]=="neg"||r[0]=="not"){
                fout<<"@SP\n"<<"A=M-1\n"<<"M=-M\n";
                if(r[0]=="not")
                    fout<<"M=M-1\n";
            }
            else{
                fout<<"@SP\n"<<"M=M-1\n"<<"A=M\n"<<"D=M\n"<<"@SP\n"<<"A=M-1\n";
                    if(r[0]=="and")fout<<"M=D&M\n";
                    else if(r[0]=="or")fout<<"M=D|M\n";
                    else if(r[0]=="add")fout<<"M=D+M\n";
                    else if(r[0]=="sub")fout<<"M=M-D\n";
                    else{
                        fout<<"D=M-D\n"<<"M=-1\n"<<"@NEXT"<<d<<"\n";
                        if(r[0]=="eq")
                            fout<<"D;JEQ\n";
                        if(r[0]=="gt")
                            fout<<"D;JGT\n";
                        if(r[0]=="lt")
                            fout<<"D;JLT\n";
                        fout<<"@SP\n"<<"A=M-1\n"<<"M=0\n"<<"(NEXT"<<d<<")\n";
                        d++;
                    }
                }
            }
        }
    }


//Translate a .vm file to .asm file
void TranslateFile(const char* c){
    fin.open(c,ios::in);
    if(fin.good()){
    string str=c;
    string out=str.substr(0,str.length()-3)+".asm";
    fout.open(out,ios::out);
    In(fin,fout);
    fout.close();
    fin.close();
    }
    else{
        throw "Can't find such file name or directory, please make sure that you enter the right file name or directory";
    }
}

//Translate directory contains .vm files into one .asm file
void TranslateDirectory(const char* c){
    DIR *pDIR;
    struct dirent *entry;
    string str=c;
    if( pDIR=opendir(c)){
        size_t f = str.find_last_of("/\\");
        string out=str+"/"+str.substr(f+1)+".asm";
        fout.open(out,ios::out);
        while(entry = readdir(pDIR)){
            filename=entry->d_name;
            if(filename.find(".vm")!=std::string::npos){
                string file=str+"/"+filename;
                fin.open(file,ios::in);
                m=0;
                In(fin,fout);
                fin.close();
            }
        }
            fout.close();
            closedir(pDIR);
    }
    else{
        throw "Can't find such file name or directory, please make sure that you enter the right file name or directory";
    }
}

int main(int argc, char* argv[]){
    try{
    if(argc!=2)
        throw "Inaccurate usage. Please enter in the following format: VMTranslator (directory/filename)";
    else{
        string file=argv[1];
    if(file.find(".")==std::string::npos)
        TranslateDirectory(argv[1]);
    else if(file.find(".vm")!=std::string::npos)
        TranslateFile(argv[1]);
    else
        throw "Not the correct file type. Please enter a .vm file or a directory containing .vm files.";
    }
    }
    catch (const char* msg) {
     cerr << msg << endl;
    }
    return 0;
}
