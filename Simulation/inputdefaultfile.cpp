
#include <QStringList>
#include "inputdefaultfile.h"


#define PARMNAMEAPPEND1 19
#define PARMNAMEAPPEND2 37

/*
 *  InputDefaultFile *inputfile = new InputDefaultFile;
    inputfile->readFile();
    inputfile->replaceValue("ra","1234567");
    inputfile->replaceValue("convert_paras","123asd4567");
    inputfile->writeFile();
 */

InputDefaultFile::InputDefaultFile()
{
    lines = QList<QString>();
}

void InputDefaultFile::setFileName(QString defaultFileName)
{
    fileName = defaultFileName;
}

bool InputDefaultFile::readFile()
{
    QFile qfile(fileName);
    if (!qfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&qfile);
    QString line = in.readLine();
    while (!line.isNull()) {
        line.append("\n");
        lines.append(line);
        line = in.readLine();
    }
    qfile.close();

    return true;
}

void InputDefaultFile::replaceValue(QString name, QString value)
{

    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines.at(i);
        if (line!=NULL && line.at(0)!='#' && line.at(0)!=' ' && line.contains(name))
        {
            QByteArray baName = name.toLatin1();
            const char *chrName = baName.data();
            QByteArray baValue = value.toLatin1();
            const char *chrValue = baValue.data();
            QString tmp;
            tmp.sprintf("%-19s%-40s", chrName, chrValue);
            int idx = line.indexOf('#');
            if(idx>0)
                tmp.append(line.mid(idx));
            else
                tmp.append("\n");
            lines.replace(i, tmp);
            break;
        }
    }
}

QString InputDefaultFile::getValue(QString name)
{
    QString rstStr = NULL;
    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines.at(i);
        if (line!=NULL && line.at(0)!='#' && line.at(0)!=' ' && line.contains(name))
        {
            QString tmp;
            int idx = line.indexOf('#');
            if(idx>0){
                tmp = line.left(idx+1);
            }else{
                tmp = line;
            }

            QStringList tslist = tmp.split(" ");
            for(int j=0; j<tslist.size(); j++){
                QString subStr = tslist.at(j);
                if(subStr!=NULL && subStr!="" && subStr!=name && subStr!="\n"){
                    rstStr = subStr;
                    break;
                }
            }
            break;
        }
    }
    return rstStr;
}

bool InputDefaultFile::writeFile()
{
    QFile qfile(fileName);
    if (!qfile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;


    for (int i = 0; i < lines.size(); ++i) {
        qfile.write(lines.at(i).toLatin1());
    }
    qfile.close();

    return true;
}
