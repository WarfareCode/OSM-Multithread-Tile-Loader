#ifndef LONGTITUDEWIDGET_H
#define LONGTITUDEWIDGET_H

#include <QLineEdit>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QIntValidator>
#include <QSpinBox>

#include "SettingsKeys.h"

class LongtitudeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LongtitudeWidget(QWidget *parent);
    ~LongtitudeWidget();
private:
    void InitUI();
    void InsertWidgetIntoLayouts();
    void FillUI();
    void SetDataFromSettings();
public:
    bool IsListLoading() const noexcept;
    int GetStartLong() const noexcept;
    QString GetLongList() const noexcept;
private:
    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_firstLayout;
    QRadioButton *m_beginLongRadio;
    QSpinBox *m_beginLongValue;

    QHBoxLayout *m_secondLayout;
    QRadioButton *m_longListRadio;
    QLineEdit *m_longListEdit;
};

#endif // LONGTITUDEWIDGET_H
