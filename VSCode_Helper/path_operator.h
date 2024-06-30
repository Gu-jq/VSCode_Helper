#ifndef PATH_OPERATOR_H
#define PATH_OPERATOR_H

#include <vector>
#include <string>
#include <QProcess>
#include <QRegularExpression>

class Path_operator{
public:
    void get_version(std::vector<std::string>* name, std::vector<std::string>* path, std::vector<std::string>* version, std::string conda_path){
        QProcess process;
        process.start((conda_path + "\\conda.exe").c_str(), QStringList() << "env" << "list");
        process.waitForFinished();
        QString out = process.readAllStandardOutput();
        static const QRegularExpression nospace("\\s+");
        QStringList list = out.split(nospace);
        qDebug() << list;
        int cnt = 0;
        for(QString& s : list){
            if(s.size() == 0)
                continue;
            if(s != "#" && s != "conda" && s != "environments:" && s[1] != ':'){
                cnt = 4;
            }
            if(cnt < 4){
                cnt++;
                continue;
            }
            if(cnt == 4 && (s == "#" || s == "conda" || s == "environments:" || s[1] == ':')){
                cnt--;
                continue;
            }
            if(cnt == 4){
                name->push_back(s.toStdString());
                cnt = 5;
            }
            else if(cnt == 5){
                path->push_back(s.toStdString());
                QProcess process2;
                process2.start("cmd", QStringList() << "/c" << (s + "\\python.exe") << "--version");
                process2.waitForFinished();
                QString ver = process2.readAllStandardOutput().trimmed();
                version->push_back(ver.toStdString());
                cnt = 4;
            }
        }
    }
};

#endif // PATH_OPERATOR_H
