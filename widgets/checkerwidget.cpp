#include "checkerwidget.h"

#include <QDir>
#include <QThread>

CheckerWidget::CheckerWidget(QWidget *parent)
    : QWidget{parent}
{
    CreateObjects();
    InitUI();
    InsertWidgetIntoLayouts();
    FillUI();
    ConnectObjects();
}

CheckerWidget::~CheckerWidget()
{
    delete m_checker;
    delete m_bottomLayout;
    delete m_mainLayout;


    delete m_byFileFomatChecking;
    delete m_byIndexFormatChecking;
    delete m_byIndexExsistsChecking;

    delete m_fromLabel;
    delete m_fromSpinBox;
    delete m_toLabel;
    delete m_toSpinBox;

    delete m_bar;
    delete m_startChecking;

}

void CheckerWidget::CreateObjects()
{
    m_checker = new ImageChecker(this);
}

void CheckerWidget::InitUI()
{
    m_mainLayout = new QVBoxLayout();
    m_byFileFomatChecking = new QRadioButton();
    m_byIndexFormatChecking = new QRadioButton();
    m_byIndexExsistsChecking = new QRadioButton();

    m_bar = new QProgressBar();
    m_startChecking = new QPushButton();

    m_bottomLayout = new QHBoxLayout();
    m_fromLabel = new QLabel();
    m_fromSpinBox = new QSpinBox();
    m_toLabel = new QLabel();
    m_toSpinBox = new QSpinBox();
}

void CheckerWidget::InsertWidgetIntoLayouts()
{
    m_mainLayout->addWidget(m_byFileFomatChecking);
    m_mainLayout->addWidget(m_byIndexFormatChecking);
    m_mainLayout->addWidget(m_byIndexExsistsChecking);

    m_bottomLayout->addWidget(m_fromLabel);
    m_bottomLayout->addWidget(m_fromSpinBox);
    m_bottomLayout->addWidget(m_toLabel);
    m_bottomLayout->addWidget(m_toSpinBox);

    m_mainLayout->addLayout(m_bottomLayout);
    m_mainLayout->addWidget(m_startChecking);
    m_mainLayout->addWidget(m_bar);
    setLayout(m_mainLayout);
}

void CheckerWidget::FillUI()
{

    m_byFileFomatChecking->setText("Проверка во всех папках всех картинок на валидность (медленно)");
    m_byIndexFormatChecking->setText("Проверка во всех папках картинок по индексу на валидность (оч медленно)");
    m_byIndexExsistsChecking->setText("Проверка во всех папках картинок по индeксу на наличие файла (быстро)");
    m_byIndexExsistsChecking->setChecked(true);

    m_fromLabel->setText("Индексы (в порядке возрастания) с ");
    m_toLabel->setText(" до ");

    m_startChecking->setText("Начать проверку");
    m_fromSpinBox->setMaximum(99999999);
    m_toSpinBox->setMaximum(99999999);
}

void CheckerWidget::ConnectObjects()
{
    connect(m_startChecking, &QPushButton::clicked, this, &CheckerWidget::OnStartChecking);
    connect(m_byFileFomatChecking, &QRadioButton::toggled, this, &CheckerWidget::OnSetSecondChecking);
    connect(m_checker, &ImageChecker::ToUpdateProgressBar, this, &CheckerWidget::OnUpdateProgressBar);
}

void CheckerWidget::OnStartChecking()
{
    if (m_byFileFomatChecking->isChecked())
    {
        PrepareForChecking();
        const QStringList brokenList = m_checker->StartCheckAllFiles();
        PrepareForLoading(brokenList.count());
        m_checker->LoadImages(m_layer, m_osmTocken, brokenList);
        QMessageBox::information(this, "Готово", "Готово");
    }
    else
    {
        const int fromIndex = m_fromSpinBox->value();
        const int toIndex = m_toSpinBox->value();
        if (fromIndex < toIndex)
        {
            PrepareForChecking();
            const bool isNeedToCheckFormat=m_byIndexFormatChecking->isChecked();
            const QStringList brokenList = m_checker->StartCheckByIndex(fromIndex, toIndex, isNeedToCheckFormat);
            PrepareForLoading(brokenList.count());
            m_checker->LoadImages(m_layer, m_osmTocken, brokenList);
            QMessageBox::information(this, "Готово", "Готово");
        }
        else
        {
            QMessageBox::warning(this, "Недопустимые значения", "Значение правого поля должно быть меньше левого");
            return;
        }
    }
}

void CheckerWidget::OnSetSecondChecking(bool state)
{
    m_fromSpinBox->setDisabled(state);
    m_toSpinBox->setDisabled(state);
}

void CheckerWidget::OnUpdateProgressBar()
{
    m_bar->setValue(m_bar->value() + 1);
}

void CheckerWidget::OnChangeLayer(int layer)
{
    m_layer = layer + 1;
}

void CheckerWidget::OnSetTocken(const QString &osmTocken)
{
    m_osmTocken = osmTocken;
}

void CheckerWidget::SetPath(const QString &path)
{
    m_path = path + '/';
}

void CheckerWidget::PrepareForChecking()
{
    const QDir foldersDir(m_path);
    QStringList allDirs = foldersDir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
    for (QString &dir : allDirs)
    {
        dir = m_path + dir;
    }
    m_bar->reset();
    m_bar->setRange(0, allDirs.count());
    m_bar->setFormat("%v из %m | Папок проверено");
    m_checker->SetPaths(allDirs);
}

void CheckerWidget::PrepareForLoading(int imagesToLoadCount)
{
    m_bar->reset();
    m_bar->setRange(0, imagesToLoadCount);
    m_bar->setFormat("%v из %m | Тайлов скачано");
}
