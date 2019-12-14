import sys
import re
import time

#C-instructions table
des={'':'000','M':'001','D':'010','MD':'011','A':'100',
     'AM':'101','AD':'110','AMD':'111'}                 
jump={'':'000','JGT':'001','JEQ':'010','JGE':'011','JLT':'100',
      'JNE':'101','JLE':'110','JMP':'111'}
comp={'0':'0101010','1':'0111111','-1':'0111010','D':'0001100','A':'0110000',
      'M':'1110000','!D':'001101','!A':'0110001','!M':'1110001','-D':'0001111',
      '-A':'0110011','-M':'1110011','D+1':'0011111','A+1':'0110111','M+1':'1110111',
      'D-1':'0001110','A-1':'0110010','M-1':'1110010','D+A':'0000010','D+M':'1000010',
      'D-A':'0010011','D-M':'1010011','A-D':'0000111','M-D':'1000111','D&A':'0000000',
      'D&M':'1000000','D|A':'0010101','D|M':'1010101'}

#Symbols table
symbol={'R0':0,'R1':1,'R2':2,'R3':3,'R4':4,'R5':5,'R6':6,'R7':7,'R8':8,
            'R9':9,'R10':10,'R11':11,'R12':12,'R13':13,'R14':14,'R15':15,
            'SCREEN':16384,'KBD':24576,'SP':0,'LCL':1,'ARG':2,'THIS':3,'THAT':4}
label={}

if len(sys.argv)!=2:
    print("Inaccurate usage. Please enter in the following format: Assembler filename")
else:
    try:
        with open(sys.argv[1], 'r') as fin:         #Open file
            contents = fin.read()+'\n'
        contents=re.sub(r'(//(.)*?\n)|(/\*(.)*?\*/)','',contents,flags=re.DOTALL)    #Remove comments in .asm file
        contents=contents.strip()
        contents=re.sub(r'\n\s+','',contents)       #Remove spaces in .asm file
        contents=re.sub(r' ','',contents)           
        contents=contents.split('\n')
        out=sys.argv[1][0:-3]+'hack'
        fout=open(out,'w')
        line=0
        var=16
        parse=['','','']
    
        for s in contents:                          #Handle labels first
            if s[0]=='(':
                s=s[1:-1]
                label[s]=line
            else:
                line+=1
        for s in contents:                          #Handle other symbols and commands
            if s[0]=='@':                           #Handle A-instructions
                s=s[1:]
                if s[0].isdigit():
                    fout.write('0{0:015b}\n'.format(int(s)))
                else:
                    if s in label:
                        fout.write('0{0:015b}\n'.format(label[s]))
                    elif s in symbol:
                        fout.write('0{0:015b}\n'.format(int(symbol[s])))
                    else:
                        fout.write('0{0:015b}\n'.format(var))
                        symbol[s]=var
                        var+=1
            elif s[0]!='(':                         #Handle C-instructions
                x=s.partition(';')
                parse[2],s=x[2],x[0]
                x=s.rpartition('=')
                parse[0],parse[1]=x[0],x[2]
                fout.write('111%s%s%s\n'%(comp[parse[1]],des[parse[0]],jump[parse[2]]))
        fout.close()
    except:
        print('Could not open this file! Please make sure that you enter the right file name!')
        
