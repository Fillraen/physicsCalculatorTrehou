#ifndef UNIT_H
#define UNIT_H

#include <QString>
#include <QMap>

class unit
{
public:
    unit();

public:
    QMap<double, QString> basicUnit;
    QMap<double, QString> timeUnit;
    QMap<double, QString> speedUnit;
    QMap<double, QString> momentOfInertiaUnit;
    QMap<double, QString> longTimeUnit;
    QMap<double, QString> accelerationUnit;

public:
    float convertUnits(float, QString);
};


#endif // UNIT_H
