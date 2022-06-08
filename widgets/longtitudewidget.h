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
    bool UserWantList() const noexcept;
    int GetStartLong() const noexcept;
    QString GetLongList() const noexcept;
private:
    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_firstLayout;
    QRadioButton *m_toInfLongitudeRadio;
    QSpinBox *m_startValue;

    QHBoxLayout *m_secondLayout;
    QRadioButton *m_fromListLongRadio;
    QLineEdit *m_longsFromList;
};

#endif // LONGTITUDEWIDGET_H
