#ifndef __INPUTREADER_CLASS_H
#define __INPUTREADER_CLASS_H

#include<iostream>
#include<fstream>
#include<string>

class InputReader
{
protected:
    int curline,curchar,curindex;
public:
    InputReader()
    {
        reset();
    }
    virtual char peekchar() const=0;
    virtual char getchar()=0;
    virtual void reset()
    {
        this->curline=this->curchar=1;
        this->curindex=0;
    }
    virtual int get_curline() const
    {
        return this->curline;
    }
    virtual int get_curchar() const
    {
        return this->curchar;
    }
    virtual int get_curindex() const
    {
        return this->curindex;
    }
    virtual bool eof() const=0;
};

class ConsoleInputReader:public InputReader
{
public:
    char peekchar() const
    {
        if(eof()) return -1;
        return std::cin.peek();
    }
    char getchar()
    {
        if(eof()) return -1;
        char c=std::cin.get();
        ++this->curindex;
        if(c=='\n')
        {
            ++this->curline;
            this->curchar=1;
        }
        else ++this->curchar;        
        return c;
    }
    bool eof() const
    {
        return std::cin.eof();    
    }
};

class FileReader:public InputReader
{
private:
    std::string buffer;
public:
    void open(const std::string& path)
    {
        std::ifstream file(path.c_str(),std::ios::in);
        std::getline(file,this->buffer,(char)-1);
        file.close();
        reset();
    }
    char peekchar() const
    {
        if(eof()) return -1;
        return this->buffer[this->curindex];
    }
    char getchar()
    {
        char c=peekchar();
        ++this->curindex;
        if(c=='\n')
        {
            ++this->curline;
            this->curchar=1;
        }
        else ++this->curchar;
        return c;
    }
    bool eof() const
    {
        return this->curindex >= this->buffer.size();
    }
};

#endif


