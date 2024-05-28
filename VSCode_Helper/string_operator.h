#ifndef STRING_OPERATOR_H
#define STRING_OPERATOR_H

#include<string>
#include<ctype.h>
#include <QDebug>


class Stringoperator{

public:
    bool check_asc(std::string s){
        for(auto e :s){
            if(!isascii(e)){
                return false;
            }
        }

        return true;
    }

    std::string delete_quot(std::string s){
        std::string news = s;
        if(s[0]=='"'&&s[s.size()-1]=='"'){
            news =s.substr(1,s.size()-2);
        }
        return news;
    }

    bool check_suffix(std::string s,std::string suffix){
        int position = s.find(suffix);
        if(position != s.npos){
            return true;
        }
        return false;
    }

};



#endif // STRING_OPERATOR_H
