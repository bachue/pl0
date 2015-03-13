#include<string>
#include<fstream>
#include<deque>
#include"LexicalAnalyzer.class.h"

bool LexicalAnalyzer::open(const std::string &path)
{
    int type;
	std::string word;

    if(path.size()>0)//文件操作
    {
        this->inputreader=&this->filereader;
        if(!this->filereader.open(path)) return false;
    }
    else//控制台输入操作
    {
        this->inputreader=&this->consoleinputreader;
    }
    
    if(!this->getsym(word,type)) return false;
	this->cursymtype=type;
	this->cursymword=word;
    return true;
}

void LexicalAnalyzer::next()
{
    int type;
	std::string word;
	this->prevsymtype=this->cursymtype;
	this->prevsymword=this->cursymword;
	this->getsym(word,type);
    this->cursymtype=type;
	this->cursymword=word;
}

int LexicalAnalyzer::peeksymtype() const
{
    return this->cursymtype;
}

int LexicalAnalyzer::getsymtype()
{
    if(!this->eof())
	{
		this->next();    
		return this->getprevsymtype();
	}
	else return this->peeksymtype();
}

std::string LexicalAnalyzer::peeksymword() const
{
    return this->cursymword;
}

std::string LexicalAnalyzer::getsymword()
{
	if(!this->eof())
	{
		this->next();    
		return this->getprevsymword();
	}
	else return this->peeksymword();
}

int LexicalAnalyzer::getprevsymtype() const
{
    return this->prevsymtype;
}

std::string LexicalAnalyzer::getprevsymword() const
{
    return this->prevsymword;
}

bool LexicalAnalyzer::eof() const
{
    return this->inputreader->eof();
}

int LexicalAnalyzer::get_curline() const
{
    return this->cursymline;
}

int LexicalAnalyzer::get_curchar() const
{
    return this->cursymchar;
}

int LexicalAnalyzer::get_prevline() const
{
    return this->prevsymline;
}

int LexicalAnalyzer::get_prevchar() const
{
    return this->prevsymchar;
}
