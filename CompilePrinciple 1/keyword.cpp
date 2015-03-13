#include<string>
#include<ctype.h>
#include<map>
#define KEYWORD_LENGTH 13

const std::pair<std::string,std::string> KEYWORDS_ARRAY[]={ std::make_pair("if","ifsym"),
                                                             std::make_pair("then","thensym"),
                                                             std::make_pair("while","whilesym"),
                                                             std::make_pair("do","dosym"),
                                                             std::make_pair("read","readsym"),
                                                             std::make_pair("write","writesym"),
                                                             std::make_pair("call","callsym"),
                                                             std::make_pair("begin","beginsym"),
                                                             std::make_pair("end","endsym"),
                                                             std::make_pair("const","constsym"),
                                                             std::make_pair("var","varsym"),
                                                             std::make_pair("procedure","procsym"),
                                                             std::make_pair("odd","oddsym")};

const std::map<std::string,std::string> KEYWORDS(KEYWORDS_ARRAY,KEYWORDS_ARRAY+KEYWORD_LENGTH);

std::string getKeyword(std::string str)
{
    for(int i=0;i<str.size();++i) str[i]=tolower(str[i]);
    std::map<std::string,std::string>::const_iterator iter=KEYWORDS.find(str);
    if(iter!=KEYWORDS.end()) return iter->second;
    else return "";
}

