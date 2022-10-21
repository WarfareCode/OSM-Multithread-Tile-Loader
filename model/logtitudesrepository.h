#ifndef LOGTITUDESREPOSITORY_H
#define LOGTITUDESREPOSITORY_H

#include <QQueue>
#include <algorithm>

class LongtitudesRepository
{
public:
    explicit LongtitudesRepository();
    ~LongtitudesRepository();
public:
    void SetStartLongitude(int longitude);
    void SetLongtitudesList(const QString &longitudes);
    int GetNextLongtitude();
    bool HasAnyLongtitudes() const;
private:
    static QList<int> ParceLongtitudesToList(const QString &longtitutdes);
    static QList<int> ParceTextWithDiv(const QString &text);
private:
    QQueue<int> m_logtitudesVector;
};

#endif // LOGTITUDESREPOSITORY_H
