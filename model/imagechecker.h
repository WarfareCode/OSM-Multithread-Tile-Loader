#ifndef IMAGECHECKER_H
#define IMAGECHECKER_H

#include <QObject>
#include <QThread>
#include <QApplication>

class ImageChecker : public QObject
{
    Q_OBJECT
public:
    explicit ImageChecker(QObject *parent);
Q_SIGNALS:
    void ToUpdateProgressBar();
public:
    void SetPaths(const QStringList &dirs);
    QStringList StartCheckAllFiles();
    QStringList StartCheckByIndex(int from, int to, bool isNeedToCheckFormat);
    void LoadImages(int layer, const QString &osmTocken, const QStringList &images);
    static QPair<QString, QString> GetLongAndLatFromFileName(const QString &filePath);
private:
    static bool IsImageBroken(const QString &path);
private:
    QStringList m_dirs;
};

#endif // IMAGECHECKER_H
