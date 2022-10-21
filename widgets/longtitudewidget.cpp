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

    delete m_beginLongRadio;
    delete m_beginLongValue;
    delete m_longListRadio;
    delete m_longListEdit;
}

void LongtitudeWidget::InitUI()
{
    m_mainLayout = new QVBoxLayout();

    m_firstLayout = new QHBoxLayout();
    m_beginLongRadio = new QRadioButton();
    m_beginLongValue = new QSpinBox();

    m_secondLayout = new QHBoxLayout();
    m_longListRadio = new QRadioButton();
    m_longListEdit = new QLineEdit();
}

void LongtitudeWidget::InsertWidgetIntoLayouts()
{
    m_firstLayout->addWidget(m_beginLongRadio);
    m_firstLayout->addWidget(m_beginLongValue);

    m_secondLayout->addWidget(m_longListRadio);
    m_secondLayout->addWidget(m_longListEdit);

    m_mainLayout->addLayout(m_firstLayout);
    m_mainLayout->addLayout(m_secondLayout);
    setLayout(m_mainLayout);
}

void LongtitudeWidget::FillUI()
{
    m_beginLongRadio->setChecked(true);
    m_beginLongRadio->setText(QStringLiteral("Начиная с долготы: "));
    m_longListRadio->setText("Список долгот: ");
    m_longListRadio->setToolTip("Через запятую или тире например: 2,3,5-9 , 11, 44-77");
    m_beginLongValue->setMaximum(99999999);
}

void LongtitudeWidget::SetDataFromSettings()
{
    const QSettings settings(QStringLiteral("mapLoaderNrls"));
    const int longtitude = settings.value(longKey, "10").toInt();
    m_beginLongValue->setValue(longtitude);
}

bool LongtitudeWidget::IsListLoading() const noexcept
{
    return m_longListRadio->isChecked();
}

int LongtitudeWidget::GetStartLong() const noexcept
{
    return m_beginLongValue->value();
}

QString LongtitudeWidget::GetLongList() const noexcept
{
    return m_longListEdit->text();
}
