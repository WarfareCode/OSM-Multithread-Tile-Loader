#include "imagedowloader.h"

ImageDownloader::ImageDownloader(const QString &folderToSavePath, const QString &osmTocken, const QString &layer, int latitudeBottom, int latitudeTop, LongtitudesRepository *rep, QObject *parent)
    : QObject(parent)
    , m_url("https://c.tile.geofabrik.de/549e80f319af070f8ea8d0f149a149c2/" + layer + '/')
    , m_manager(new QNetworkAccessManager(this))
    , m_osmTocken("_osm_totp_token=" + osmTocken)
    , m_folderToSavePath(folderToSavePath)
    , m_latitudeBottom(latitudeBottom)
    , m_latitudeTop(latitudeTop)
    , m_repository(rep)
{
}

ImageDownloader::~ImageDownloader()
{
    delete m_manager;
}

void ImageDownloader::OnImageRead()
{
    m_file.write(m_reply->readAll());
    m_file.flush();
}

void ImageDownloader::OnMakeQueue()
{
    if (m_repository->HasAnyLongtitudes())
    {
        m_currentLongtitude = m_repository->GetNextLongtitude();
        Q_EMIT ToResetBar(m_currentLongtitude, m_latitudeBottom - m_latitudeTop);
        const QString folderPath = m_folderToSavePath + '/' +
                                   QString::number(m_currentLongtitude);

        for (int i = m_latitudeTop; i <= m_latitudeBottom; ++i)
        {
            const QString fileName = folderPath + '/' + QString::number(i) + m_type;
            const QUrl finalyUrl(m_url + QString::number(m_currentLongtitude) + '/' +
                                 QString::number(i) + m_type);
            m_tilesList.push_front(qMakePair(finalyUrl, fileName));
        }

        QDir dir;

        if (!dir.exists(folderPath))
        {
            dir.mkdir(folderPath);
        }

        Next();
    }
    else
    {
        Q_EMIT ToFinishThread();
    }
}

void ImageDownloader::OnFinishedImageLoad()
{
    switch (m_reply->error())
    {
    case QNetworkReply::NoError:
    {
        m_file.close();
        m_tilesList.pop_front();
        if (m_tilesList.empty())
        {
            qDebug() << "FULL Loaded  " << m_currentLongtitude;
            OnMakeQueue();
        }
        else
        {
            Q_EMIT ToUpdateProgressBar();
            qDebug() << m_file.fileName() << " Loaded";
            Next();
        }

        break;
    }

    default:
    {
        qDebug() << m_reply->errorString().toLatin1();
        delete m_reply;
        QTimer::singleShot(5000, [ = ]()
        {
            qDebug() << "run again";
            Next();
        });
        break;
    }
    }

}

void ImageDownloader::MakeRequest(const QUrl &fileUrl, const QString &fileName)
{
    const QString saveFilePath(fileName);
    QNetworkRequest request;
    request.setUrl(fileUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "GET" );
    request.setRawHeader("authority", "tile.openstreetmap.org");
    request.setRawHeader("method", "GET");
    request.setRawHeader("cookie", m_osmTocken.toLatin1());
    request.setRawHeader("osm_session", "75def91464a5a4fc45ac277cf2a433a5");
    request.setRawHeader("sec-ch-ua",
                         " Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Microsoft Edge\";v=\"99\"");
    request.setRawHeader("user-agent",
                         "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36 Edg/99.0.1150.36");
    m_reply = m_manager->get(request);

    if (m_file.fileName() != saveFilePath)
    {

        m_file.setFileName(saveFilePath);
        m_file.open(QIODevice::WriteOnly);
    }
    ConnectReply();
}

void ImageDownloader::ConnectReply()
{
    connect(m_reply, &QNetworkReply::readyRead, this, &ImageDownloader::OnImageRead);
    connect(m_reply, &QNetworkReply::finished, this, &ImageDownloader::OnFinishedImageLoad);
    connect(m_reply, &QNetworkReply::error, m_reply, &QObject::deleteLater);
    connect(m_reply, &QNetworkReply::finished, m_reply, &QObject::deleteLater);
}

void ImageDownloader::Next()
{
    const QPair<QUrl, QString> item = m_tilesList.front();
    MakeRequest(item.first, item.second);
}
