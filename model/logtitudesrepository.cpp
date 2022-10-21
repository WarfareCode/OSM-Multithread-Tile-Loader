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

void LongtitudesRepository::SetLongtitudesList(const QString &longitudes)
{
    m_logtitudesVector.clear();
    QList<int> parcedList=ParceLongtitudesToList(longitudes);
    std::sort(parcedList.begin(), parcedList.end());
    const QList<int>::iterator newEnd=std::unique(parcedList.begin(), parcedList.end());
    std::copy(parcedList.begin(), newEnd, std::back_inserter(m_logtitudesVector));
}

int LongtitudesRepository::GetNextLongtitude()
{
    return m_logtitudesVector.dequeue();
}

bool LongtitudesRepository::HasAnyLongtitudes() const
{
    return !m_logtitudesVector.isEmpty();
}

QList<int> LongtitudesRepository::ParceLongtitudesToList(const QString &longtitutdes)
{
    Q_ASSERT(!longtitutdes.isEmpty());
    QList<int> resultList;
    const QStringList splitedByComma=longtitutdes.split(',');
    for (const QString &text : splitedByComma)
    {
        if(text.contains('-'))
        {
            const QList<int> range=ParceTextWithDiv(text);
            resultList.append(range);
        }
        else
        {
            bool canConvert=false;
            const int longtitude = text.toInt(&canConvert);
            if (canConvert)
            {
                resultList.append(longtitude);
            }
        }
    }
    return resultList;
}

QList<int> LongtitudesRepository::ParceTextWithDiv(const QString &text)
{
    QList<int> resultList;
    Q_ASSERT(text.contains('-'));
    const int indexOfSep=text.indexOf('-');
    const QString leftBorder=text.left(indexOfSep);
    const QString rightBorder=text.mid(indexOfSep+1);
    bool canParce=false;
    const int leftNum=leftBorder.toInt(&canParce);
    if(canParce)
    {
        const int rightNum=rightBorder.toInt(&canParce);
        if(canParce)
        {
            if(leftNum <=rightNum)
            {
                for (int i=leftNum; i<=rightNum; ++i)
                {
                    resultList.append(i);
                }
            }
        }
    }
    return resultList;
}
