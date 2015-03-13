#include<string>
#include<stdio.h>
#include"func_lib.h"
#include"LexicalAnalyzer.class.h"
#include"wordlist.h"

bool LexicalAnalyzer::getsym(std::string &word,int& word_type)
{
    char c;
    int c_type;
    const int WORD_MAX_LENGTH=10;
    const int NUM_MAX_LENGTH=14;
    word="";
	word_type=0;
        
    do
    {
        if((c=this->inputreader->getchar())<=0) return false;
        c_type=judge_char_type(c);
    }
    while(c_type==CHAR_NULL);

	this->prevsymchar=this->cursymchar;
	this->prevsymline=this->cursymline;
	this->cursymchar=this->inputreader->get_curchar();
	this->cursymline=this->inputreader->get_curline();

    if(c_type==CHAR_CHARACTER)
    {
        word=c;
        while(word.size()<WORD_MAX_LENGTH)
        {
            c=this->inputreader->peekchar();
            c_type=judge_char_type(c);
            if(c_type!=CHAR_CHARACTER && c_type!=CHAR_NUMBER) break;
            c=this->inputreader->getchar();
            word+=c;
        }
            
        word_type=get_keyword(word);
        if(word_type==0)
        {
            word_type=IDENTTYPE;
        }
    }
    else if(c_type==CHAR_NUMBER)
    {
        word=c;
        while(word.size()<NUM_MAX_LENGTH)
        {
            c=this->inputreader->peekchar();
            c_type=judge_char_type(c);
            if(c_type!=CHAR_NUMBER) break;
            c=this->inputreader->getchar();
            word+=c;
        }
        word_type=NUMBERTYPE;
    }
    else/*if(c_type==CHAR_OTHER)*/
    {
        word=c;
        switch(c)
        {
        case ':':
        case '>':
        case '<':
            if(this->inputreader->peekchar()=='=')
            {
                word+=this->inputreader->getchar();
            }
            break;
        }

		word_type=get_operator(word);
        //if(word_type==0) error
    }
    return true;
}
