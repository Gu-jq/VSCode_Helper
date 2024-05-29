#ifndef JSON_OPERATOR_H
#define JSON_OPERATOR_H

#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QDir>

class Json_operator{
public:
    bool write_launch(std::string path, std::string gcc_path, bool show = true, bool stop = false){
        QFile file(":/jsondoc/jsondoc/launch.json");
        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }
        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument doc(QJsonDocument::fromJson(jsonData));
        QJsonObject obj = doc.object();

        QJsonArray arr = obj["configurations"].toArray();
        QJsonObject obj1 = arr[0].toObject();
        obj1["miDebuggerPath"] = (gcc_path + "\\gdb.exe").c_str();
        obj1["externalConsole"] = show;
        obj1["stopAtEntry"] = stop;
        arr[0] = obj1;
        obj["configurations"] = arr;

        QJsonDocument o_doc(obj);
        QDir dir;
        if(!dir.mkpath((path + "\\.vscode").c_str())){
            return false;
        }
        QFile o_file((path + "\\.vscode\\launch.json").c_str());
        if (!o_file.open(QIODevice::WriteOnly)) {
            qWarning("white file launch.json failed.");
            return false;
        }
        o_file.write(o_doc.toJson());
        o_file.close();
        return true;
    }
    bool write_properties(std::string path, std::string gcc_path, int cpp_std = 17){
        QFile file(":/jsondoc/jsondoc/c_cpp_properties.json");
        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }
        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument doc(QJsonDocument::fromJson(jsonData));
        QJsonObject obj = doc.object();

        QJsonArray arr = obj["configurations"].toArray();
        QJsonObject obj1 = arr[0].toObject();
        obj1["compilerPath"] = (gcc_path + "\\g++.exe").c_str();
        std::string stdnum;
        stdnum = stdnum + (char)(cpp_std / 10 + '0');
        stdnum = stdnum + (char)(cpp_std % 10 + '0');
        obj1["cppStandard"] = ("c++" + stdnum).c_str();
        arr[0] = obj1;
        obj["configurations"] = arr;

        QJsonDocument o_doc(obj);
        QDir dir;
        if(!dir.mkpath((path + "\\.vscode").c_str())){
            return false;
        }
        QFile o_file((path + "\\.vscode\\c_cpp_properties.json").c_str());
        if (!o_file.open(QIODevice::WriteOnly)) {
            qWarning("white file c_cpp_properties.json failed.");
            return false;
        }
        o_file.write(o_doc.toJson());
        o_file.close();
        return true;
    }
    bool write_tasks(std::string path, std::string gcc_path, int cpp_std = 17, bool show = true, int optimize = 0){
        QFile file(":/jsondoc/jsondoc/tasks.json");
        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }
        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument doc(QJsonDocument::fromJson(jsonData));
        QJsonObject obj = doc.object();

        QJsonObject opt = obj["options"].toObject();
        QJsonObject env = opt["env"].toObject();
        env["Path"] = (gcc_path + ";${env:Path}").c_str();
        opt["env"] = env;
        obj["options"] = opt;

        QJsonArray arr = obj["tasks"].toArray();
        QJsonObject obj1 = arr[0].toObject();
        obj1["command"] = (gcc_path + "\\g++.exe").c_str();
        std::string stdnum;
        stdnum = stdnum + (char)(cpp_std / 10 + '0');
        stdnum = stdnum + (char)(cpp_std % 10 + '0');
        QJsonArray arg = obj1["args"].toArray();
        arg[4] = ("-std=c++" + stdnum).c_str();
        if(optimize > 0){
            std::string op;
            op = op + "-O" + (char)(optimize + '0');
            arg.insert(2, op.c_str());
        }
        obj1["args"] = arg;
        arr[0] = obj1;

        QJsonObject obj2 = arr[1].toObject();
        opt = obj2["options"].toObject();
        env = opt["env"].toObject();
        env["Path"] = (gcc_path + ";${env:Path}").c_str();
        opt["env"] = env;
        obj2["options"] = opt;
        arr[1] = obj2;
        obj["tasks"] = arr;

        QJsonDocument o_doc(obj);
        QDir dir;
        if(!dir.mkpath((path + "\\.vscode").c_str())){
            return false;
        }
        QFile o_file((path + "\\.vscode\\tasks.json").c_str());
        if (!o_file.open(QIODevice::WriteOnly)) {
            qWarning("white file tasks.json failed.");
            return false;
        }
        o_file.write(o_doc.toJson());
        o_file.close();
        return true;
    }
};

#endif // JSON_OPERATOR_H
