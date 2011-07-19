#include "utils.h"

QDateTime fromOleTime(double time)
{
    if (abs(time) < 1.e-3)
        return QDateTime();
    QDateTime dateTime = QDateTime(QDate(1899, 12, 30));
    return dateTime.addDays(time);
}

double toOleTime(QDateTime time)
{
    if (!time.isValid())
        return 0.0;
    return QDateTime(QDate(1900, 1, 1)).daysTo(time) + 2;
}

QColor fromRawColor(QString col)
{
    // 0xBbGgRr
    // 0xF00F00
    int val = col.toInt();
    int r = val & 0x0000ff;
    int g = (val & 0x00ff00) >> 8;
    int b = (val & 0xff0000) >> 16;
    return QColor::fromRgb(r, g, b);
}

QString toRawColor(QColor col)
{
    int r = col.red();
    int g = col.green();
    int b = col.blue();

    int val = r | (g << 8) | (b << 16);

    return QString("%1").arg(val);
}
