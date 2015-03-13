#include<string>
#include<ctype.h>
#include<map>
#include"LexicalAnalyzer.class.h"
#include"keywordlist.h"
#define KEYWORD_LENGTH 13

std::string LexicalAnalyzer::getKeyword(std::string str) const
{
	static const std::pair<std::string,std::string> KEYWORDS_ARRAY[]={ std::make_pair("if",IFSYM),
                                                             std::make_pair("then",THENSYM),
                                                             std::make_pair("while",WHILESYM),
                                                             std::make_pair("do",DOSYM),
                                                             std::make_pair("read",READSYM),
                                                             std::make_pair("write",WRITESYM),
                                                             std::make_pair("call",CALLSYM),
                                                             std::make_pair("begin",BEGINSYM),
                                                             std::make_pair("end",ENDSYM),
                                                             std::make_pair("const",CONSTSYM),
                                                             std::make_pair("var",VARSYM),
                                                             std::make_pair("procedure",PROCSYM),
                                                             std::make_pair("odd",ODDSYM)};

	static const std::map<std::string,std::string> KEYWORDS(KEYWORDS_ARRAY,KEYWORDS_ARRAY+KEYWORD_LENGTH);

    for(int i=0;i<str.size();++i) str[i]=tolower(str[i]);
    std::map<std::string,std::string>::const_iterator iter=KEYWORDS.find(str);
    if(iter!=KEYWORDS.end()) return iter->second;
    else return "";
}
