#ifndef UTILS_H
#define UTILS_H

#include <QDateTime>
#include <QColor>
#include <QString>

QDateTime fromOleTime(double time);
double toOleTime(QDateTime time);

QColor fromRawColor(QString col);
QString toRawColor(QColor col);

int fixIntToRange(int min, int max, int current);

#endif // UTILS_H
