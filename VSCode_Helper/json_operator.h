#ifndef JSON_OPERATOR_H
#define JSON_OPERATOR_H

#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QStandardPaths>

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
        obj1["miDebuggerPath"] = (gcc_path + "/gdb.exe").c_str();
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
    bool write_properties(std::string path, std::string gcc_path, std::string cpp_std = "c++17"){
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
        obj1["compilerPath"] = (gcc_path + "/g++.exe").c_str();
        obj1["cppStandard"] = cpp_std.c_str();
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
    bool write_tasks(std::string path, std::string gcc_path, std::string cpp_std = "c++17", bool show = true, int optimize = 0, int warn_level = 0, bool warn_err = false){
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
        obj1["command"] = (gcc_path + "/g++.exe").c_str();
        QJsonArray arg = obj1["args"].toArray();
        arg[4] = ("-std=" + cpp_std).c_str();
        if(optimize > 0){
            std::string op;
            op = op + "-O" + (char)(optimize + '0');
            arg.insert(2, op.c_str());
        }
        if(warn_err){
            arg.insert(2, "-Werror");
        }
        switch(warn_level){
        case 0:
            break;
        case 1:
            arg.insert(2, "-Wall");
            break;
        case 2:
            arg.insert(2, "-Wall");
            arg.insert(3, "-Wextra");
            arg.insert(4, "-Wpedantic");
            break;
        case 3:{
            std::string arg_list[17] = {"-Wall", "-Wextra", "-Wpedantic", "-Wformat=2", "-Wswitch-default", "-Wuseless-cast", "-Wuninitialized", "-Wstrict-overflow=4",
                                        "-Warith-conversion", "-Wduplicated-branches", "-Wduplicated-cond", "-Wfloat-equal", "-Wshadow", "-Wcast-align", "-Wconversion",
                                        "-Wdangling-else", "-Wredundant-decls"};
            for(int i = 0; i < 17; i++){
                arg.insert(i + 2, arg_list[i].c_str());
            }
            break;
        }
        case 4:
            arg.insert(2, "-w");
            break;
        }
        obj1["args"] = arg;
        arr[0] = obj1;

        QJsonObject obj2 = arr[1].toObject();
        opt = obj2["options"].toObject();
        env = opt["env"].toObject();
        env["Path"] = (gcc_path + ";${env:Path}").c_str();
        opt["env"] = env;
        obj2["options"] = opt;
        if(!show){
            obj2.remove("type");
        }
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
    bool create_key_bind(std::string key, std::string arg, std::string cmd, std::string lang){
        QJsonObject key_bind;
        key_bind["key"] = key.c_str();
        key_bind["args"] = arg.c_str();
        key_bind["command"] = cmd.c_str();
        key_bind["when"] = ("editorTextFocus && editorLangId == '" + lang + '\'').c_str();

        QString q_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir roaming(q_path);
        roaming.cdUp();
        std::string path = roaming.absolutePath().toStdString() + "\\Code\\User\\keybindings.json";
        qDebug() << path;
        QJsonArray arr;
        if(QFile::exists(path.c_str())){
            QFile file(path.c_str());
            if (!file.open(QIODevice::ReadOnly)) {
                return false;
            }
            QByteArray jsonData = file.readAll();
            file.close();

            QJsonDocument doc(QJsonDocument::fromJson(jsonData));
            arr = doc.array();
        }
        arr.append(key_bind);
        QJsonDocument o_doc(arr);

        QFile o_file(path.c_str());
        if (!o_file.open(QIODevice::WriteOnly)) {
            qWarning("white file keybindings.json failed.");
            return false;
        }
        o_file.write(o_doc.toJson());
        o_file.close();
        return true;
    }
};

#endif // JSON_OPERATOR_H
