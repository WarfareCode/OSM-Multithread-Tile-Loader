#include "imagechecker.h"

#include <QDir>
#include <QImage>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QUrl>

ImageChecker::ImageChecker(QObject *parent)
    : QObject{parent}
{

}
void ImageChecker::SetPaths(const QStringList &dirs)
{
    m_dirs = dirs;
}

QStringList ImageChecker::StartCheckAllFiles()
{
    QStringList brokenImagesList;
    for (const QString &dir : qAsConst(m_dirs))
    {
        const QDir currentDir(dir);
        const QStringList allPngs = currentDir.entryList(QStringList() << "*.png");
        for (const QString &currentPng : allPngs)
        {
            const QString imagePath = currentDir.absolutePath() + '/' + currentPng;
            if (IsImageBroken(imagePath))
            {
                brokenImagesList.append(imagePath);
            }
        }
        Q_EMIT ToUpdateProgressBar();
        qApp->processEvents();
    }
    return brokenImagesList;
}

QStringList ImageChecker::StartCheckByIndex(int from, int to, bool isNeedToCheckFormat)
{
    QStringList brokenImagesList;
    for (const QString &dir : qAsConst(m_dirs))
    {
        const QDir currentDir(dir);
        qDebug() << "Формируем папку" << currentDir.absolutePath();
        for (int i = from; i <= to; ++i)
        {
            const QString imagePath = currentDir.absolutePath() + QDir::separator() + QString::number(i) + ".png";
            if (QFile::exists(imagePath))
            {
                if (isNeedToCheckFormat && IsImageBroken(imagePath))
                {
                    brokenImagesList.append(imagePath);
                }
            }
            else
            {
                brokenImagesList.append(imagePath);
            }
        }
        Q_EMIT ToUpdateProgressBar();
        qApp->processEvents();
    }
    return brokenImagesList;
}

void ImageChecker::LoadImages(int layer, const QString &osmTocken, const QStringList &imagesPaths)
{
    for (const QString &saveFilePath : imagesPaths)
    {
        QImage image;
        do
        {
            QNetworkAccessManager  manager;
            QNetworkRequest request;
            QFile file(saveFilePath);
            file.open(QIODevice::ReadWrite);
            file.resize(0);
            const QPair<QString, QString> longAndLat = GetLongAndLatFromFileName(saveFilePath);
            const QString objectUrl("https://b.tile.geofabrik.de/549e80f319af070f8ea8d0f149a149c2/" + QString::number(layer) + '/' + longAndLat.first + '/' + longAndLat.second + ".png");
            request.setUrl(objectUrl);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "GET" );
            request.setRawHeader("authority", "tile.openstreetmap.org");
            request.setRawHeader("method", "GET");
            request.setRawHeader("cookie", osmTocken.toLatin1());
            request.setRawHeader("osm_session", "150f3119a1517547d825735a88f56569");
            request.setRawHeader("sec-ch-ua",
                                 " Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Microsoft Edge\";v=\"99\"");
            request.setRawHeader("user-agent",
                                 "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36 Edg/99.0.1150.36");
            QEventLoop loop;
            QNetworkReply *const reply = manager.get(request);
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            const QByteArray arr = reply->readAll();
            file.write(arr);
            file.flush();
            file.close();
            reply->deleteLater();
            image.load(saveFilePath);
            qDebug() << saveFilePath;
            Q_EMIT ToUpdateProgressBar();
            qApp->processEvents();
        }
        while (image.format() == QImage::Format_Invalid);
    }
}

QPair<QString, QString> ImageChecker::GetLongAndLatFromFileName(const QString &filePath)
{
    const QRegularExpression re("\\d+");
    QRegularExpressionMatchIterator iter = re.globalMatch(filePath);
    QStringList list;
    while (iter.hasNext())
    {
        const QRegularExpressionMatch match = iter.next();
        list << match.captured();
    }
    if (list.count() >= 2)
    {
        return qMakePair(list.at(list.count() - 2), list.last());
    }
    else
    {
        qFatal("Лист имеет недопустимое колличество элементов");
    }
}

bool ImageChecker::IsImageBroken(const QString &path)
{
    const QImage image(path);
    return QImage::Format_Invalid == image.format();

}

