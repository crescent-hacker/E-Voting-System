#ifndef READCONF_H
#define READCONF_H
#include <QFile>
#include <QString>
#include <QRegExp>
#include <QVector>
#include <map>

class ReadConf
{
public:
    ReadConf(QFile*);


    void matchValue(QString key);
    void matchCvs(std::map<std::string,std::string>& m);
    void printStr();
private:
    QVector<QString> strVec;
    QVector<QString> strMatch;
};

#endif // READCONF_H
