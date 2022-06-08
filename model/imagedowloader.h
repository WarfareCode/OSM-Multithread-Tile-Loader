#ifndef IMAGEDOWLOADER_H
#define IMAGEDOWLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QPair>
#include <QStringList>
#include <QDir>
#include <QTimer>
#include <QApplication>
#include <QThread>

#include "model/logtitudesrepository.h"
#include <forward_list>

class ImageDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloader(const QString &folderToSavePath, const QString &osmTocken, const QString &layer, int latitudeBottom, int topLatitude,
                             LongtitudesRepository *rep, QObject *parent);
    ~ImageDownloader();
Q_SIGNALS:
    void ToUpdateProgressBar();
    void ToResetBar(int longtitude, int count);
    void ToFinishThread();
public Q_SLOTS:
    void OnMakeQueue();
private Q_SLOTS:
    void OnFinishedImageLoad();
    void OnImageRead();
    void MakeRequest(const QUrl &fileUrl, const QString &fileName);
    void ConnectReply();
    void Next();
private:
    const QString m_type = QStringLiteral(".png");
    const QString m_url;
private:
    QNetworkAccessManager *const m_manager;
    const QString m_osmTocken;
    const QString m_folderToSavePath;
    const int m_latitudeBottom;
    const int m_latitudeTop;
    LongtitudesRepository *const m_repository;
    int m_currentLongtitude;


    QFile m_file;
    std::forward_list<QPair<QUrl, QString>> m_tilesList;
    QNetworkReply *m_reply;

};

#endif // IMAGEDOWLOADER_H
