#ifndef CHECKERWIDGET_H
#define CHECKERWIDGET_H

#include <QComboBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QFormLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QApplication>
#include <QProgressBar>

#include "model/imagechecker.h"

class CheckerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CheckerWidget(QWidget *parent);
    ~CheckerWidget();
private:
    void CreateObjects();
    void InitUI();
    void InsertWidgetIntoLayouts();
    void FillUI();
    void ConnectObjects();
private Q_SLOTS:
    void OnStartChecking();
    void OnSetSecondChecking(bool state);
    void OnUpdateProgressBar();
public Q_SLOTS:
    void OnChangeLayer(int layer);
    void OnSetTocken(const QString &osmTocken);
public:
    void SetPath(const QString &path);
private:
    void PrepareForChecking();
    void PrepareForLoading(int imagesToLoadCount);
private:
    int m_layer;
    QString m_osmTocken;
    QString m_path;
private:
    ImageChecker *m_checker;
private:
    QVBoxLayout *m_mainLayout;

    QRadioButton *m_byFileFomatChecking;
    QRadioButton *m_byIndexFormatChecking;
    QRadioButton *m_byIndexExsistsChecking;

    QHBoxLayout *m_bottomLayout;
    QLabel *m_fromLabel;
    QSpinBox *m_fromSpinBox;
    QLabel *m_toLabel;
    QSpinBox *m_toSpinBox;

    QProgressBar *m_bar;
    QPushButton *m_startChecking;

};

#endif // CHECKERWIDGET_H
