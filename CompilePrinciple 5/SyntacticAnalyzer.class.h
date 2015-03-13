#ifndef __SYNTACTICANALYZER_CLASS_H
#define __SYNTACTICANALYZER_CLASS_H
#include"LexicalAnalyzer.class.h"
#include<string>
#include<fstream>
#include<deque>
#include<set>

#define TEMP_VARNAME_PREFIX "tempvar_"

class SyntacticAnalyzer
{
private:
	enum IdentStackFlag{CONST,VAR,BOUND};
	enum SyntacticAnalyzerResult {ERROR=false,RIGHT=true,VALUE=2};
	class IdentStackNode
	{
	public:
		IdentStackFlag flag;
		std::string ident;
		int value;
	public:
		IdentStackNode(const IdentStackFlag _flag,const std::string _ident="",const int _value=0):flag(_flag),ident(_ident),value(_value){}
	};
	class TempVar
	{
	public:
		std::string prev_temp_varname;
	private:
		int seqnum;
	public:
		TempVar():seqnum(1){}
		std::string next();
		static bool isTempVar(const std::string varname);
	};
	class IntermediateCodeNode
	{
	public:
		std::string command;
		std::string oper1,oper2,oper3;
	};
private:
	std::string errorinfo;
	std::deque<IdentStackNode> ident_stack;
	std::set<std::string> procedure_set;
	std::deque<IntermediateCodeNode> intermediate_code_deque;
	TempVar tempvar;
public:
    SyntacticAnalyzerResult analyze(LexicalAnalyzer&);
private:
    SyntacticAnalyzerResult subprogram(LexicalAnalyzer&);
    SyntacticAnalyzerResult constsym(LexicalAnalyzer&);
    SyntacticAnalyzerResult varsym(LexicalAnalyzer&);
    SyntacticAnalyzerResult procsym(LexicalAnalyzer&);
    SyntacticAnalyzerResult statement(LexicalAnalyzer&);
    SyntacticAnalyzerResult callsym(LexicalAnalyzer&);
    SyntacticAnalyzerResult beginsym(LexicalAnalyzer&);
    SyntacticAnalyzerResult ifsym(LexicalAnalyzer&);
    SyntacticAnalyzerResult whilesym(LexicalAnalyzer&);
    SyntacticAnalyzerResult readwritesym(LexicalAnalyzer&);
    SyntacticAnalyzerResult ident(LexicalAnalyzer&);
    SyntacticAnalyzerResult condition(LexicalAnalyzer&,bool&);
    SyntacticAnalyzerResult expression(LexicalAnalyzer&,int&);
    SyntacticAnalyzerResult item(LexicalAnalyzer&,int&);
    SyntacticAnalyzerResult factor(LexicalAnalyzer&,int&);
private:
	std::deque<IdentStackNode>::const_iterator ident_stack_find(const std::string _ident);
	void ident_stack_clear();
public:
	std::string get_error_info() const
	{
		return this->errorinfo;
	}
	void print_intermediate_code(std::ostream& out) const
	{
		for(std::deque<IntermediateCodeNode>::const_iterator iter=this->intermediate_code_deque.begin();
												iter!=this->intermediate_code_deque.end();++iter)
		{
			out << '(' << iter->command << ' ' << iter->oper1 << ' ' << iter->oper2 << ' ' << iter->oper3 << ')' << std::endl;
		}
	}
};

#endif

