#ifndef __SYNTACTICANALYZER_CLASS_H
#define __SYNTACTICANALYZER_CLASS_H
#include"LexicalAnalyzer.class.h"
#include"keywordlist.h"
#include<string>
#include<fstream>
#include<deque>
#include<vector>
#include<set>

class SyntacticAnalyzer
{
private:
	enum IdentStackFlag{CONST,VAR,BOUND};
	enum SyntacticAnalyzerResult {ERROR=false,RIGHT=true,VALUE=2};
	enum QUERYWAY{QUERY_ALL_PROC=true,QUERY_CUR_PROC=false};
	class IdentStackNode
	{
	public:
		IdentStackFlag flag;
		std::string ident;
		int value;
	public:
		IdentStackNode(const IdentStackFlag _flag,const std::string& _ident="",const int _value=0):flag(_flag),ident(_ident),value(_value){}
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
		static bool isTempVar(const std::string& varname);
	};
	class TempPoint
	{
	private:
		int seqnum;
	public:
		TempPoint():seqnum(1){}
		std::string next();
	};
	class IntermediateCodeNode
	{
	public:
		std::string command;
		std::string oper1,oper2,oper3;
	public:
		IntermediateCodeNode(const std::string& _command="",const std::string& _oper1="",const std::string& _oper2="",const std::string& _oper3="")
			:command(_command),oper1(_oper1),oper2(_oper2),oper3(_oper3){}
	};
private:
	std::string errorinfo;
	std::deque<IdentStackNode> ident_stack;
	std::vector<std::string> procedure_vector;
	std::vector<std::deque<IntermediateCodeNode> > intermediate_code_deque_vector;
	TempVar tempvar;
	TempPoint temppoint;
	std::string cur_procedure_name;
	int cur_procedure_index;
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
    SyntacticAnalyzerResult readsym(LexicalAnalyzer&);
	SyntacticAnalyzerResult writesym(LexicalAnalyzer&);
    SyntacticAnalyzerResult ident(LexicalAnalyzer&);
    SyntacticAnalyzerResult condition(LexicalAnalyzer&,const std::string&,bool&);
    SyntacticAnalyzerResult expression(LexicalAnalyzer&,int&);
    SyntacticAnalyzerResult item(LexicalAnalyzer&,int&);
    SyntacticAnalyzerResult factor(LexicalAnalyzer&,int&);
private:
	std::deque<IdentStackNode>::const_iterator ident_stack_find(const std::string&,const QUERYWAY) const;
	bool procedure_find(const std::string&) const;
	void ident_stack_clear();
	void declare_new_var(const std::string&);
public:
	SyntacticAnalyzer():cur_procedure_name("[START]"),cur_procedure_index(0)
	{
		intermediate_code_deque_vector.push_back(std::deque<IntermediateCodeNode>());
		procedure_vector.push_back("start");
	}
	std::string get_error_info() const
	{
		return this->errorinfo;
	}
	void print_intermediate_code(std::ostream& out) const;
};

#endif

