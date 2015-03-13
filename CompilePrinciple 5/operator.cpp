#include<string>
#include<map>
#include"LexicalAnalyzer.class.h"
#include"operatorlist.h"
#define OPERATOR_LENGTH 16

std::string LexicalAnalyzer::get_operator_name(const std::string& str) const
{
	static const std::pair<std::string,std::string> OPERATORS_ARRAY[]={ std::make_pair(BECOMESOPER,BECOMES),
                                                              std::make_pair(GEQOPER,GEQ),
                                                              std::make_pair(GTROPER,GTR),
                                                              std::make_pair(LEQOPER,LEQ),
                                                              std::make_pair(LSSOPER,LSS),
                                                              std::make_pair(PLUSOPER,PLUS),
                                                              std::make_pair(MINUSOPER,MINUS),
                                                              std::make_pair(TIMESOPER,TIMES),
                                                              std::make_pair(SLASHOPER,SLASH),
                                                              std::make_pair(EQLOPER,EQL),
                                                              std::make_pair(NEQOPER,NEQ),
                                                              std::make_pair(COMMAOPER,COMMA),
                                                              std::make_pair(PERIODOPER,PERIOD),
                                                              std::make_pair(SEMICOLONOPER,SEMICOLON),
                                                              std::make_pair(LPARANOPER,LPAREN),
                                                              std::make_pair(RPARANOPER,RPAREN)};

	static const std::map<std::string,std::string> OPERATORS(OPERATORS_ARRAY,OPERATORS_ARRAY+OPERATOR_LENGTH);

    std::map<std::string,std::string>::const_iterator iter=OPERATORS.find(str);
    if(iter!=OPERATORS.end()) return iter->second;
    else return "";
}
