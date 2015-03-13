#include<iostream>
#include"LexicalAnalyzer.class.h"
#include"declare.h"

int main(int num_args,char* args[])
{
    std::string word;
	LexicalAnalyzer lexical_analyzer;
	
	if(num_args<=1)
	{
	    std::cout << "没有输入需要分析的文件" << std::endl;
	    return 0;
	}
	
	if(!lexical_analyzer.open(args[1]))
	{
	    std::cout << "文件打开时出错" << std::endl;
	    return 0;
	}
	
	if((word=program(lexical_analyzer))=="")
	{
	    std::cout << "语法正确" << std::endl;
	}
	else
	{
	    std::cout << "语法错误\n"
	              << "错误行数: " << lexical_analyzer.get_prevline() << '\n'
	              << "错误内容: " << lexical_analyzer.getprevsymword() << '\n'
	              << "错误原因：" << word << std::endl;
	}
	
	return 0;
}

