#include<iostream>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"

int main(int num_args,char* args[])
{
	int res,value;
    std::string word;
	LexicalAnalyzer lexical_analyzer;
	SyntacticAnalyzer syntactic_analyzer;
	
	if(num_args<=1)
	{
	    lexical_analyzer.open();
	}
	
	else if(!lexical_analyzer.open(args[1]))
	{
	    std::cout << "文件打开时出错" << std::endl;
	    return 0;
	}
	
	///仅仅给第4题用
	if(res=syntactic_analyzer.expression(lexical_analyzer,value))
	{
	    std::cout << "语法正确" << std::endl;
		if(res==2) std::cout << "他的值为  " << value << std::endl;
	}
	/*
	if(syntactic_analyzer.analyze(lexical_analyzer))
	{
	    std::cout << "语法正确" << std::endl;
	}*/
	else
	{
	    std::cout << "语法错误\n"
	              << "错误行数: " << lexical_analyzer.get_prevline() << '\n'
				  << "错误列数: " << lexical_analyzer.get_prevchar() << '\n'
	              << "错误内容: " << lexical_analyzer.getprevsymword() << '\n'
	              << "错误原因：" << syntactic_analyzer.get_error_info() << std::endl;
	}
	/*
	lexical_analyzer.reset();
	while(!lexical_analyzer.eof())
	{
	    std::cout << lexical_analyzer.peeksymtype() << " , ";
	    std::cout << lexical_analyzer.peeksymword() << " , ";
	    std::cout << lexical_analyzer.get_curline() << " , ";
	    std::cout << lexical_analyzer.get_curchar() << std::endl;
	    lexical_analyzer.next();
	}*/
	return 0;
}

