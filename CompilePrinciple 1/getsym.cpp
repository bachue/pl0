#include<string>
#include<stdio.h>
#include"InputReader.class.h"
#include"func_lib.h"

std::string getKeyword(std::string);
std::string get_operator_name(const std::string&);

namespace LexicalAnalyzer
{
    int getsym(std::string &word,std::string &word_type,InputReader& inputreader,int &num_line,int& num_char)
    {
        char c;
        int c_type,value=0;
        const int WORD_MAX_LENGTH=10;
        const int NUM_MAX_LENGTH=14;
        word="";
        
        do
        {
            num_line=inputreader.get_curline();
            num_char=inputreader.get_curchar();
            if((c=inputreader.getchar())<=0) return -1;
            c_type=judge_char_type(c);
        }
        while(c_type==CHAR_NULL);

        if(c_type==CHAR_CHARACTER)
        {
            word+=c;
            while(word.size()<WORD_MAX_LENGTH)
            {
                c=inputreader.peekchar();
                c_type=judge_char_type(c);
                if(c_type!=CHAR_CHARACTER && c_type!=CHAR_NUMBER) break;
                c=inputreader.getchar();
                word+=c;
            }
            
            word_type=getKeyword(word);
            if(word_type.size()==0)
            {
                word_type="ident";
            }
        }
        else if(c_type==CHAR_NUMBER)
        {
            word+=c;
            while(word.size()<NUM_MAX_LENGTH)
            {
                c=inputreader.peekchar();
                c_type=judge_char_type(c);
                if(c_type!=CHAR_NUMBER) break;
                c=inputreader.getchar();
                word+=c;
            }
            word_type="number";
            sscanf(word.c_str(),"%d",&value);
        }
        else/*if(c_type==CHAR_OTHER)*/
        {
            word=c;
            switch(c)
            {
            case ':':
            case '>':
            case '<':
                if(inputreader.peekchar()=='=')
                {
                    word+=inputreader.getchar();
                }
                break;
            }
            
            word_type=get_operator_name(word);
            //if(word_type.size()==0) error
        }
        return value;
    }
};
