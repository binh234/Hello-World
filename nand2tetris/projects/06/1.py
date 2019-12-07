import sys
import re

des={'':'000','M':'001','D':'010','MD':'011','A':'100','AM':'101','AD':'110','AMD':'111'}
jump={'':'000','JGT':'001','JEQ':'010','JGE':'011','JLT':'100','JNE':'101','JLE':'110','JMP':'111'}
comp={'0':'0101010','1':'0111111','-1':'0111010','D':'0001100','A':'0110000',
      'M':'1110000','!D':'001101','!A':'0110001','!M':'1110001','-D':'0001111',
      '-A':'0110011','-M':'1110011','D+1':'0011111','A+1':'0110111','M+1':'1110111',
      'D-1':'0001110','A-1':'0110010','M-1':'1110010','D+A':'0000010','D+M':'1000010',
      'D-A':'0010011','D-M':'1010011','A-D':'0000111','M-D':'1000111','D&A':'0000000',
      'D&M':'1000000','D|A':'0010101','D|M':'1010101'}


if len(sys.argv)!=2:
    print("No")
else:
    with open(sys.argv[1], 'r') as fin:
        contents = fin.read()
    contents=re.sub(r'(//)(.)+','',contents)
    contents=contents.strip()
    contents=re.sub(r' ','',contents)
    contents=contents.split('\n')
    out=sys.argv[1][0:-3]+'hack'
    fout=open(out,'w')
    line=0
    var=16
    label={}
    symbol={}
    parse=['','','']
    
    for s in contents:
        if s[0]=='(':
            s=s[1:-1]
            label[s]=line
        else:
            line+=1
    for s in contents:
        if s[0]=='@':
            s=s[1:]
            if s[0].isdigit():
                fout.write('0{0:015b}\n'.format(int(s)))
            else:
                if s in label:
                    fout.write('0{0:015b}\n'.format(label[s]))
                else:
                    fout.write('0{0:015b}\n'.format(var))
                    var+=1
        else:
            x=s.partition(';')
            parse[2],s=x[2],x[0]
            x=s.rpartition('=')
            parse[0],parse[1]=x[0],x[2]
            fout.write('111%s%s%s\n'%(comp[parse[1]],des[parse[0]],jump[parse[2]]))
    fout.close()         
    
        
