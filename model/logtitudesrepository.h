#ifndef LOGTITUDESREPOSITORY_H
#define LOGTITUDESREPOSITORY_H

#include <QQueue>

class LongtitudesRepository
{
public:
    explicit LongtitudesRepository();
    ~LongtitudesRepository();
public:
    void SetStartLongitude(int longitude);
    void SetLongtitudesList(const QString longitudesList);
    int GetNextLongtitude();
    bool HasAnyLongtitudes() const noexcept;
private:
    QQueue<int> m_logtitudesVector;
};

#endif // LOGTITUDESREPOSITORY_H
