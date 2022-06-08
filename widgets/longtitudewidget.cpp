#include "longtitudewidget.h"

#include <QSettings>

LongtitudeWidget::LongtitudeWidget(QWidget *parent)
    : QWidget{parent}
{
    InitUI();
    InsertWidgetIntoLayouts();
    FillUI();
    SetDataFromSettings();
}

LongtitudeWidget::~LongtitudeWidget()
{
    delete m_secondLayout;
    delete m_firstLayout;
    delete m_mainLayout;

    delete m_toInfLongitudeRadio;
    delete m_startValue;
    delete m_fromListLongRadio;
    delete m_longsFromList;
}

void LongtitudeWidget::InitUI()
{
    m_mainLayout = new QVBoxLayout();

    m_firstLayout = new QHBoxLayout();
    m_toInfLongitudeRadio = new QRadioButton();
    m_startValue = new QSpinBox();

    m_secondLayout = new QHBoxLayout();
    m_fromListLongRadio = new QRadioButton();
    m_longsFromList = new QLineEdit();
}

void LongtitudeWidget::InsertWidgetIntoLayouts()
{
    m_firstLayout->addWidget(m_toInfLongitudeRadio);
    m_firstLayout->addWidget(m_startValue);

    m_secondLayout->addWidget(m_fromListLongRadio);
    m_secondLayout->addWidget(m_longsFromList);

    m_mainLayout->addLayout(m_firstLayout);
    m_mainLayout->addLayout(m_secondLayout);
    setLayout(m_mainLayout);
}

void LongtitudeWidget::FillUI()
{
    m_toInfLongitudeRadio->setText(QStringLiteral("Начиная с долготы: "));
    m_fromListLongRadio->setText("Список долгот через зяпятую: ");
    m_startValue->setMaximum(99999999);
}

void LongtitudeWidget::SetDataFromSettings()
{
    const QSettings settings(QStringLiteral("mapLoaderNrls"));
    const int longtitude = settings.value(longKey, "10").toInt();
    m_startValue->setValue(longtitude);
}

bool LongtitudeWidget::UserWantList() const noexcept
{
    return m_fromListLongRadio->isChecked();
}

int LongtitudeWidget::GetStartLong() const noexcept
{
    return m_startValue->value();
}

QString LongtitudeWidget::GetLongList() const noexcept
{
    return m_longsFromList->text();
}
