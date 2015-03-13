#include"CompilePrincipleInterpreter.class.h"
#include<iostream>

int main(int argc,char* argv[])
{
	CompilePrincipleInterpreter interpreter;
	if(argc<=1)
	{
		std::cerr << "必须指定输入文件" << std::endl;
		return 1;
	}
	interpreter.interpreter(argv[1]);
	return 0;
}
