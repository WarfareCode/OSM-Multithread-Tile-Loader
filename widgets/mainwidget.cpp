#include "mainwidget.h"

#include <QFileDialog>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    InitUI();
    InsertWidgetIntoLayouts();
    FillUI();
    ConnectObjects();
    LoadDataFromSettings();
}

MainWidget::~MainWidget()
{
    delete m_osmValidator;

    delete m_topLayout;
    delete m_pathLayout;
    delete m_mainLayout;

    delete m_treadCountComboBox;
    delete m_layerLabel;
    delete m_layerComboBox;
    delete m_osmLabel;
    delete m_osmTockenLineEdit;
    delete m_pathLabel;
    delete m_setPathButton;
    delete m_checkerWidget;
    delete m_loaderWidget;
    delete m_tabWidget;
}

void MainWidget::InitUI()
{
    m_osmValidator = new QIntValidator(this);

    m_mainLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();
    m_threadLabel = new QLabel();
    m_treadCountComboBox = new QComboBox();
    m_layerLabel = new QLabel();
    m_layerComboBox = new QComboBox();
    m_osmLabel = new QLabel();
    m_osmTockenLineEdit = new QLineEdit();
    m_pathLayout = new QHBoxLayout();
    m_pathLabel = new QLabel();
    m_setPathButton = new QPushButton();
    m_tabWidget = new QTabWidget();
    m_loaderWidget = new LoaderWidget(Q_NULLPTR);
    m_checkerWidget = new CheckerWidget(Q_NULLPTR);
}

void MainWidget::InsertWidgetIntoLayouts()
{
    m_topLayout->addWidget(m_threadLabel, 0, Qt::AlignLeft);
    m_topLayout->addWidget(m_treadCountComboBox, 1, Qt::AlignLeft);
    m_topLayout->addWidget(m_layerLabel, 0, Qt::AlignHCenter);
    m_topLayout->addWidget(m_layerComboBox, 0, Qt::AlignHCenter);
    m_topLayout->addWidget(m_osmLabel, 0, Qt::AlignRight);
    m_topLayout->addWidget(m_osmTockenLineEdit, 0, Qt::AlignRight);
    m_mainLayout->addLayout(m_topLayout);

    m_pathLayout->addWidget(m_pathLabel);
    m_pathLayout->addWidget(m_setPathButton);

    m_mainLayout->addLayout(m_pathLayout);
    m_mainLayout->addWidget(m_tabWidget);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_mainLayout);
}

void MainWidget::FillUI()
{
    QString qtVersion = QString::fromLatin1(qVersion());
    setWindowTitle( "Качальщик карт Qt " + qtVersion + " Версия: " + APP_VERSION + " от " + BUILD_DATE);
    m_osmTockenLineEdit->setValidator(m_osmValidator);
    m_threadLabel->setText(QStringLiteral("КОЛЛИЧЕСТВО ПОТОКОВ: "));
    m_layerLabel->setText("СЛОЙ: ");
    m_osmLabel->setText("ТОКЕН ОСМ");
    for (int i = 1; i < 12; ++i)
    {
        m_treadCountComboBox->addItem(QString::number(i));
    }
    for (int i = 1; i < 19; ++i)
    {
        m_layerComboBox->addItem(QString::number(i));
    }
    m_treadCountComboBox->setEditable(false);
    m_layerComboBox->setEditable(false);
    m_tabWidget->addTab(m_loaderWidget, "Загрузка тайлов");
    m_tabWidget->addTab(m_checkerWidget, "Проверка тайлов в папке");
    m_pathLabel->setText("Путь: " + QApplication::applicationDirPath());
    m_setPathButton->setText("Выбрать путь");
    m_loaderWidget->SetFolderToSave(QApplication::applicationDirPath());
    m_checkerWidget->SetPath(QApplication::applicationDirPath());
}

void MainWidget::LoadDataFromSettings()
{
    const QSettings settings(QStringLiteral("mapLoaderNrls"));
    const int threadCounts = settings.value(threadKey, 1).toInt();
    const int layer = settings.value(layerKey, "1").toInt();
    const QString osmTocken = settings.value(osmTockenKey, "000000").toString();
    const QString folderPath = settings.value(folderToSaveKey, QDir::currentPath()).toString();
    m_pathLabel->setText("Путь: " + folderPath);
    m_loaderWidget->SetFolderToSave(folderPath);
    m_checkerWidget->SetPath(folderPath);
    m_treadCountComboBox->setCurrentIndex(threadCounts);
    m_layerComboBox->setCurrentIndex(layer);
    m_osmTockenLineEdit->setText(osmTocken);
}

void MainWidget::ConnectObjects()
{
    connect(m_setPathButton, &QPushButton::clicked, this, &MainWidget::OnSetPathButton);
    connect(m_treadCountComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), m_loaderWidget, &LoaderWidget::OnChangeThreadCount);
    connect(m_layerComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), m_loaderWidget, &LoaderWidget::OnChangeLayer);
    connect(m_osmTockenLineEdit, &QLineEdit::textChanged, m_loaderWidget, &LoaderWidget::OnSetTocken);
    connect(m_layerComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), m_checkerWidget, &CheckerWidget::OnChangeLayer);
    connect(m_osmTockenLineEdit, &QLineEdit::textChanged, m_checkerWidget, &CheckerWidget::OnSetTocken);
}

void MainWidget::OnSetPathButton()
{
    const QString folderPath = QFileDialog::getExistingDirectory(0, ("Рабочая папка"), QDir::currentPath());
    m_pathLabel->setText("Путь: " + folderPath);
    m_loaderWidget->SetFolderToSave(folderPath);
    m_checkerWidget->SetPath(folderPath);
}
