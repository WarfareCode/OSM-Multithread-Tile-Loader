#include "logtitudesrepository.h"

LongtitudesRepository::LongtitudesRepository()
{

}

LongtitudesRepository::~LongtitudesRepository()
{

}

void LongtitudesRepository::SetStartLongitude(int longitude)
{
    m_logtitudesVector.clear();
    for (int i = 0; i < 10000; ++i)
    {
        m_logtitudesVector.enqueue(longitude);
        longitude++;
    }
}

void LongtitudesRepository::SetLongtitudesList(const QString longitudesList)
{
    m_logtitudesVector.clear();
    const QStringList list = longitudesList.split(',');
    for (const QString &numStr : list)
    {
        const QString newString = numStr.trimmed();
        bool canConvert;
        const int longtitude = newString.toInt(&canConvert);
        if (canConvert)
        {
            m_logtitudesVector.enqueue(longtitude);
        }
    }
}

int LongtitudesRepository::GetNextLongtitude()
{
    return m_logtitudesVector.dequeue();
}

bool LongtitudesRepository::HasAnyLongtitudes() const noexcept
{
    return !m_logtitudesVector.isEmpty();
}
