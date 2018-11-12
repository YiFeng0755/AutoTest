#ifndef WORDSGENERATOR_H
#define WORDSGENERATOR_H

/** 通用的词表生成器*/
/** 输出一个csv，txt
 */
#include <QObject>
#include <QTextStream>
#include <QFile>

void generateWords() {
    QFile file("usercids.txt" );
    if(!file.open(QIODevice::WriteOnly)) {
        return ;
    }

    int cid = 10000000 ;
    long userid = 50000000 ;
    int time = 0 ;
    QTextStream in(&file);
    while(time < 10000) {
        in << cid << '\t' << userid << '\n' ;
        ++cid ;
        ++userid ;
        ++time ;
    }

    file.close();
}
#endif // WORDSGENERATOR_H
