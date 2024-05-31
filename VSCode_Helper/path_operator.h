#ifndef PATH_OPERATOR_H
#define PATH_OPERATOR_H

#include <vector>
#include <string>
#include <QFile>

bool path_check(std::string aim_array[], int aim_num, std::vector<std::string>& path_array, std::string &path){
    for(std::string s : path_array){
        int cnt = 0;
        for (int j = 0; j < aim_num; j++){
            if(s.size() > 0 && s[s.size() - 1] != '\\' && s[s.size() - 1] != '/')
                s = s + '\\';
            if(QFile::exists((s + aim_array[j]).c_str()))
                cnt++;
            else
                break;
            if(cnt == aim_num){
                path = s;
                return true;
            }
        }
    }
    return false;
}

#endif // PATH_OPERATOR_H
