#include "readconf.h"
#include <QTextStream>
#include <QDebug>
#include <QStringRef>
#include <QChar>



ReadConf::ReadConf(QFile* file)
{
    if(!file->open(QFile::ReadOnly))
        qDebug() << "file cannot open!";
    //read data
    QTextStream out(file);
    QString str;
    while(!out.atEnd()){
        str=out.readLine();
        strVec.push_back(str);
    }

}

void ReadConf::matchValue(QString key)
{
    QString value;
    QRegExp exp(QString("(")+key+QString("\\s*=\\s*)"));
    QVector<QString>::iterator it=strVec.begin();
    for(;it!=strVec.end();++it){
        int pos=0, posTemp=0;
        while((pos=exp.indexIn(*it,pos))>-1){
            pos += exp.matchedLength();
            posTemp=pos;
            //extract the value of key
            while((*it)[pos].isNumber()||(*it)[pos].isLetter())
                ++pos;
            QStringRef subStr(it,posTemp,pos-posTemp);
            QString mathedStr=subStr.toString();
            strMatch.push_back(mathedStr);
        }

    }
}

void ReadConf::matchCvs(std::map<std::string,std::string>& m)
{
    QString value;
    QRegExp exp(QString("(\\s*,\\s*)"));
    QVector<QString>::iterator it=strVec.begin();
    for(;it!=strVec.end();++it){
        int pos=0;
        if((pos=exp.indexIn(*it,pos))>-1){
            QStringRef subStr1(it,0,pos);
            pos += exp.matchedLength();
            int len=it->length();
            QStringRef subStr2(it,pos,len-pos);
            m[subStr1.toString().toStdString()]=subStr2.toString().toStdString();
        }
    }
}

void ReadConf::printStr()
{
    QVector<QString>::iterator it=strMatch.begin();
    for(;it!=strMatch.end();++it)
        qDebug() << *it;
}
