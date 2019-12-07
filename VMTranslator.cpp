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

string comp[18][3];
string line,str[10000];
string s[2];
string r[3];
int c=0,d=0,n,m=0;
string filename;

void In(ifstream& fin,ofstream& fout){
    while(getline(fin,line)){
        stringstream ss(line);
        if(ss.str()[0]=='/'||ss.str()=="")
            continue;
        ss>>r[0];
        fout<<"//"<<r[0]<<" ";
        if(r[0]=="function"){
            ss>>r[1];
            ss>>n;
            fout<<r[1]<<" "<<n<<endl;
            fout<<"("<<r[1]<<")\n";
            fout<<"@SP\n"<<"D=M\n"<<"@LCL\n"<<"M=D\n"<<"@14\n"<<"M=D\n";
            for(int i=0;i<n;i++)
                fout<<"@14\n"<<"A=M\n"<<"M=0\n"<<"@14\n"<<"M=M+1\n";
            fout<<"@"<<to_string(n)<<"\nD=A\n"<<"@SP\n"<<"M=M+D\n";
        }
        else if(r[0]=="call"){
                ss>>r[1];
                ss>>n;
                str[c]=r[1]+to_string(c);
                fout<<r[1]<<" "<<n<<endl;
            fout<<"@LCL\n"<<"D=M\n"<<"@SP\n"<<"A=M+1\n"<<"M=D\n";
            fout<<"@ARG\n"<<"D=M\n"<<"@SP\n"<<"A=M+1\n"<<"A=A+1\n"<<"M=D\n";
            fout<<"@THIS\n"<<"D=M\n"<<"@SP\n"<<"A=M+1\n"<<"A=A+1\n"<<"A=A+1\n"<<"M=D\n";
            fout<<"@THAT\n"<<"D=M\n"<<"@SP\n"<<"A=M+1\n"<<"A=A+1\n"<<"A=A+1\n"<<"A=A+1\n"<<"M=D\n";
            fout<<"@"<<to_string(n)<<"\nD=A\n"<<"@SP\n"<<"D=M-D\n"<<"@ARG\n"<<"M=D\n";
            fout<<"@"<<str[c]<<"\nD=A\n"<<"@SP\n"<<"A=M\n"<<"M=D\n";
            fout<<"@5\n"<<"D=A\n"<<"@SP\n"<<"M=M+D\n";
            fout<<"@"<<r[1]<<"\n0;JMP\n";
            fout<<"("<<str[c]<<")\n";
            c++;
        }
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
        else if(r[0]=="label"){
            ss>>r[1];
            fout<<r[1]<<endl;
            fout<<"("<<r[1]<<")\n";
        }
        else if(r[0]=="goto"){
            ss>>r[1];
            fout<<r[1]<<endl;
            fout<<"@"<<r[1]<<"\n"<<"0;JMP\n";
        }
        else if(r[0]=="if-goto"){
            ss>>r[1];
            fout<<r[1]<<endl;
            fout<<"@SP\n"<<"M=M-1\n"<<"A=M\n"<<"D=M\n"<<"@"<<r[1]<<"\nD;JNE\n";
        }
        else if(ss>>r[1]>>r[2]&&r[1][0]!='/'){
            fout<<r[1]<<" "<<r[2]<<endl;
            if(r[1]=="local"){ s[0]="@LCL";s[1]="@"+r[2];}
            else if(r[1]=="argument") {s[0]="@ARG";s[1]="@"+r[2];}
            else if(r[1]=="this") {s[0]="@THIS";s[1]="@"+r[2];}
            else if(r[1]=="that") {s[0]="@THAT";s[1]="@"+r[2];}
            else if(r[1]=="temp") {s[0]="@5";s[1]="@"+r[2];}
            else if(r[1]=="constant") {s[1]="@"+r[2];}
            else if(r[1]=="pointer") {
                    s[1]=r[2];
                    s[0]="@THIS";
            }
            if(r[1]=="constant"){
                fout<<s[1]<<"\nD=A\n"<<"@SP\n"<<"A=M\n"<<"M=D\n"<<"@SP\n"<<"M=M+1\n";
            }
            else{
            if(r[0]=="push"){
                if(r[1]=="local"||r[1]=="argument"||r[1]=="this"||r[1]=="that"){
                fout<<s[0]<<"\nD=M\n"<<s[1]<<"\nA=D+A\n";
                }
                else if(r[1]=="static"){
                    while(m<stoi(r[2])){
                        fout<<"@"<<filename<<to_string(m)<<"\n";
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
            else{
            if(r[1]=="local"||r[1]=="argument"||r[1]=="this"||r[1]=="that"){
                fout<<s[0]<<"\nD=M\n"<<s[1]<<"\nD=D+A\n"<<"@14\n"<<"M=D\n";
            }
            else if(r[1]=="static"){
                while(m<stoi(r[2])){
                    fout<<"@"<<filename<<to_string(m)<<"\n";
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
                        fout<<"D=M-D\n"<<"M=-1\n"<<"@NEXT"<<to_string(d)<<"\n";
                        if(r[0]=="eq")
                            fout<<"D;JEQ\n";
                        if(r[0]=="gt")
                            fout<<"D;JGT\n";
                        if(r[0]=="lt")
                            fout<<"D;JLT\n";
                        fout<<"@SP\n"<<"A=M-1\n"<<"M=0\n"<<"(NEXT"<<to_string(d)<<")\n";
                        d++;
                    }
                }
            }
        }
    }

void Translator(const char* c){
    ifstream fin;
    ofstream fout;
    DIR *pDIR;
    struct dirent *entry;
    fin.open(c,ios::in);
    filename=c;
    string str=c;
    size_t f = str.find_last_of("/\\");
    string dir=str.substr(0,f);
    string sourcefile=str.substr(f+1);
    f = dir.find_last_of("/\\");
    string out=dir.substr(f+1)+".asm";
    fout.open(out,ios::out);
    In(fin,fout);
    fin.close();
    if( pDIR=opendir(dir.c_str()) ){
		while(entry = readdir(pDIR)){
			if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){
                filename=entry->d_name;
				if(filename.find(".vm")!=std::string::npos&&filename!=sourcefile){
                    fin.open(entry->d_name,ios::in);
                    m=0;
                    In(fin,fout);
                    fin.close();
				}
			}
			}
			closedir(pDIR);
		}
    fout.close();
}

int main(int argc, char* argv[]){
    Translator(argv[1]);
    return 0;
}
