#ifndef IMAGECHECKER_H
#define IMAGECHECKER_H

#include <QObject>
#include <QApplication>

class ImageChecker : public QObject
{
    Q_OBJECT
public:
    explicit ImageChecker(QObject *parent);
    ~ImageChecker();
Q_SIGNALS:
    void ToUpdateProgressBar();
public:
    void SetPaths(const QStringList &dirs);
    QStringList StartCheckAllFiles();
    QStringList StartCheckByIndex(int from, int to);
    void LoadImages(int layer, const QString &osmTocken, const QStringList &images);
    QPair<QString, QString> GetLongAndLatFromFileName(const QString &filePath) const;
private:
    QStringList m_dirs;
};

#endif // IMAGECHECKER_H
