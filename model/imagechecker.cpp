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

ImageChecker::~ImageChecker()
{

}

void ImageChecker::SetPaths(const QStringList &dirs)
{
    m_dirs = dirs;
}

QStringList ImageChecker::StartCheckAllFiles()
{
    QStringList brokenImagesList;
    for (const QString &dir : m_dirs)
    {
        const QDir currentDir(dir);
        const QStringList allPngs = currentDir.entryList(QStringList() << "*.png");
        for (const QString &currentPng : allPngs)
        {
            const QString imagePath = currentDir.absolutePath() + '/' + currentPng;
            const QImage image(imagePath);
            if (QImage::Format_Invalid == image.format())
            {
                brokenImagesList.append(imagePath);
            }
        }
        Q_EMIT ToUpdateProgressBar();
        qApp->processEvents();
    }
    return brokenImagesList;
}

QStringList ImageChecker::StartCheckByIndex(int from, int to)
{
    QStringList brokenImagesList;
    for (const QString &dir : m_dirs)
    {
        const QDir currentDir(dir);
        qDebug() << "Формируем папку" << currentDir.absolutePath();
        for (int i = from; i <= to; ++i)
        {
            const QString imagePath = currentDir.absolutePath() + '/' + QString::number(i) + ".png";
            QFile imageFile(imagePath);
            if (imageFile.exists())
            {
                const QPixmap image(imagePath);
                if (image.isNull())
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
            QEventLoop loop;
            QFile file(saveFilePath);
            file.open(QIODevice::WriteOnly);
            QPair<QString, QString> longAndLat = GetLongAndLatFromFileName(saveFilePath);
            QUrl objectUrl("https://tile.openstreetmap.org/" + QString::number(layer) + '/' + longAndLat.first + '/' + longAndLat.second + ".png");
            request.setUrl(objectUrl);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "GET" );
            request.setRawHeader("authority", "tile.openstreetmap.org");
            request.setRawHeader("method", "GET");
            request.setRawHeader("cookie", osmTocken.toLatin1());
            request.setRawHeader("osm_session", "75def91464a5a4fc45ac277cf2a433a5");
            request.setRawHeader("sec-ch-ua",
                                 " Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Microsoft Edge\";v=\"99\"");
            request.setRawHeader("user-agent",
                                 "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36 Edg/99.0.1150.36");
            QNetworkReply *const reply = manager.get(request);
            connect(reply, &QNetworkReply::readyRead, &loop, &QEventLoop::quit);
            loop.exec();
            file.write(reply->readAll());
            file.flush();
            file.close();
            image.load(saveFilePath);
            Q_EMIT ToUpdateProgressBar();
            qApp->processEvents();
        }
        while (image.format() == QImage::Format_Invalid);
    }
}

QPair<QString, QString> ImageChecker::GetLongAndLatFromFileName(const QString &filePath) const
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

