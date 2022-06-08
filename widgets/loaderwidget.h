#ifndef LOADERWIDGET_H
#define LOADERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QIntValidator>
#include <QPushButton>
#include <QUrl>
#include <QThread>
#include <QSettings>
#include <QProgressBar>
#include <QLabel>
#include <QRadioButton>

#include "widgets/longtitudewidget.h"
#include "model/imagedowloader.h"

class LoaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoaderWidget(QWidget *parent);
    ~LoaderWidget();
private:
    void InitUI();
    void InsertWidgetIntoLayouts();
    void FillUI();
    void ConnectObjects();
    void LoadDataFromSettings();
private Q_SLOTS:
    void OnStart();
    void OnProgressBarUpdate();
    void OnProgressBarReset(int longtitude, int count);
public Q_SLOTS:
    void OnChangeThreadCount(int threadCount);
    void OnChangeLayer(int layer);
    void OnSetTocken(const QString &osmTocken);
public:
    void SetFolderToSave(const QString &path) noexcept;
private:
    void SetProgressBars(int latitudeTop, int latitudeBottom);
    void StartLoader(int latitudeTop, int latitudeBottom);
private:
    LongtitudesRepository *const m_repository;

private:
    int m_threadCount;
    int m_layer;
    QString m_osmTocken;
    QString m_folderToSavePath;

private:
    QIntValidator *m_validator;

private:
    QVBoxLayout *m_mainLayout;

    QFormLayout *m_formLayout;
    QLineEdit *m_latitudeBottomLineEdit;
    QLineEdit *m_latitudeTopLineEdit;

    LongtitudeWidget *m_longtitudeWidget;
    QLabel *m_descriptionLabel;
    QPushButton *m_startButton;

    QVBoxLayout *m_barsLayout;
};

#endif // LOADERWIDGET_H
