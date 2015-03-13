#ifndef COMPILEPRINCIPLEINTERPRETER_CLASS_H
#define	COMPILEPRINCIPLEINTERPRETER_CLASS_H
#include<map>
#include<string>
#include<deque>

class CompilePrincipleInterpreter
{
private:
	class IntermediateCodeNode
	{
	public:
		std::string command;
		std::string oper1,oper2,oper3;
	public:
		IntermediateCodeNode(const std::string& _command,const std::string& _oper1,const std::string& _oper2,const std::string& _oper3)
			:command(_command),oper1(_oper1),oper2(_oper2),oper3(_oper3){}
	};
	class RuntimeStackNode
	{
	public:
		int ip;
		std::map<std::string,int> var_map;
	public:
		RuntimeStackNode(const int _ip):ip(_ip){}
	};
private:
	std::map<std::string,int> point_map;
	std::deque<IntermediateCodeNode> code_stack;
	std::deque<RuntimeStackNode> runtime_stack;
	int ip;//程序计数器
public:
	void interpreter(const std::string&);
private:
	void command(const std::string&,const std::string&,const std::string&,const std::string&);
	void collect_point(const std::string&,const int);
	void var(const std::string&);
	void becomes(const std::string&,const std::string&);
	void read(const std::string&);
	void write(const std::string&);
	void plus(const std::string&,const std::string&,const std::string&);
	void minus(const std::string&,const std::string&,const std::string&);
	void times(const std::string&,const std::string&,const std::string&);
	void slash(const std::string&,const std::string&,const std::string&);
	void odd(const std::string&,const std::string&);
	void jump(const std::string&);
	void geq(const std::string&,const std::string&,const std::string&);
	void gtr(const std::string&,const std::string&,const std::string&);
	void leq(const std::string&,const std::string&,const std::string&);
	void lss(const std::string&,const std::string&,const std::string&);
	void eql(const std::string&,const std::string&,const std::string&);
	void neq(const std::string&,const std::string&,const std::string&);
	void call(const std::string&);
	void ret();
public:
	CompilePrincipleInterpreter():ip(0)
	{
		this->runtime_stack.push_back(RuntimeStackNode(0));
	}
private:
	int get_var(const std::string&) const;
	void set_var(const std::string&,const int);
	void create_var(const std::string&);
};

#endif	/* COMPILEPRINCIPLEINTERPRETER_CLASS_H */

