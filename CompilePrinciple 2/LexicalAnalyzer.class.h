#ifndef __INPUTREADER_CLASS_H
#define __INPUTREADER_CLASS_H

#include<string>
#include<fstream>
#include<deque>

class LexicalAnalyzer
{
private:
    int num_line,prev_num_line;
    std::string prev_symtype,prev_symword;
    std::deque<std::string> deq_buffer;
public:
    bool open(const std::string &path)
    {
        std::string buffer;
        std::ifstream file(path.c_str(),std::ios::in);
        if(file.fail()) return false;
        this->deq_buffer.clear();
        while(std::getline(file,buffer))
        {
            this->deq_buffer.push_back(buffer);
        }
        this->deq_buffer.push_back(buffer);
        file.close();
        this->num_line=this->prev_num_line=1;
        return true;
    }
      
    std::string peeksymtype() const
    {
        std::string row;
        int index;
        if(eof()) return "";
        row=this->deq_buffer[this->num_line-1];
        index=row.find_first_of('(')+1;
        return row.substr(index,row.find_first_of(',')-index);
    }
    
    std::string getsymtype()
    {
        std::string word=this->prev_symtype=this->peeksymtype();
        this->prev_symword=this->peeksymword();
        this->prev_num_line=this->num_line;
        ++this->num_line;
                
        return word;
    }
    
    std::string peeksymword() const
    {
        std::string row;
        int index;
        if(eof()) return "";
        row=this->deq_buffer[this->num_line-1];
        index=row.find_first_of(',')+1;
        return row.substr(index,row.find_last_of(')')-index);
    }
    
    std::string getsymword()
    {
        std::string word=this->prev_symword=this->peeksymword();
        this->prev_symtype=this->peeksymtype();
        this->prev_num_line=this->num_line;
        ++this->num_line;
        return word;
    }
    
    std::string getprevsymtype() const
    {
        return this->prev_symtype;
    }
    
    std::string getprevsymword() const
    {
        return this->prev_symword;
    }
    
    bool eof() const
    {
        return this->num_line>=this->deq_buffer.size();
    }
    
    int get_curline() const
    {
        return this->num_line;
    }
    
    int get_prevline() const
    {
        return this->prev_num_line;
    }
};

#endif

