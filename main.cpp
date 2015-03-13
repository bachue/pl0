#include<iostream>
#include<cstring>
#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"

int main(int argc,char* argv[])
{
	LexicalAnalyzer lexical_analyzer;
	SyntacticAnalyzer syntactic_analyzer;
	std::string outputfilename="",inputfilename="";
	int i;

	for(i=1;i<argc;++i)
	{
		if(!strcmp(argv[i],"-o"))
		{
			if(i+1<argc)
			{
				outputfilename=argv[i+1];
				++i;
			}
			else
			{
				std::cerr << "没有输出文件" << std::endl;
				return 1;
			}
		}
		else if(inputfilename=="")
		{
			inputfilename=argv[i];
		}
		else
		{
			std::cerr << argv[i] << ": 多余的参数" << std::endl;
			return 1;
		}
	}
	
	if(inputfilename=="")
	{
	    lexical_analyzer.open();
	}
	
	else if(!lexical_analyzer.open(inputfilename))
	{
	    std::cerr << inputfilename << ": 文件打开时出错" << std::endl;
	    return 1;
	}
	
	if(syntactic_analyzer.analyze(lexical_analyzer))
	{
	    std::cout << "语法正确" << std::endl;
		if(outputfilename!="")
		{
			std::ofstream file(outputfilename.c_str(),std::ios::out);
			if(file.fail())
			{
				std::cerr << outputfilename << ": 文件打开时出错" << std::endl;
				return 1;
			}
			syntactic_analyzer.print_intermediate_code(file);
			file.close();
		}
		else
		{
			syntactic_analyzer.print_intermediate_code(std::cout);
		}
	}
	else
	{
	    std::cerr << "语法错误\n"
	              << "错误行数: " << lexical_analyzer.get_prevline() << '\n'
				  << "错误列数: " << lexical_analyzer.get_prevchar() << '\n'
	              << "错误内容: " << lexical_analyzer.getprevsymword() << '\n'
	              << "错误原因：" << syntactic_analyzer.get_error_info() << std::endl;
		return 1;
	}
	return 0;
}

