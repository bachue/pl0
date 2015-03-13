#include<iostream>
#include<fstream>
#include<string>
#include"InputReader.class.h"

namespace LexicalAnalyzer{int getsym(std::string&,std::string&,InputReader&,int&,int&);};

int main(int num_args,char* args[])
{
    std::string word,type;
    int value,i,num_char,num_line;
    
    ConsoleInputReader consoleinputreader;
    FileReader filereader;
    
    if(num_args>1)
    {
        filereader.open(args[1]);
    }
       
    while(true)
    {
        if(num_args>1) value=LexicalAnalyzer::getsym(word,type,filereader,num_line,num_char);
        else value=LexicalAnalyzer::getsym(word,type,consoleinputreader,num_line,num_char);
        if(value<0) break;
        std::cout << '(' << type << ',';
        if(word.size()>0) std::cout << word;
        else std::cout << value;
        std::cout << ')' << std::endl; 
    }
    
    return 0;
}

