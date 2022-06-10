#include "loaderwidget.h"

LoaderWidget::LoaderWidget(QWidget *parent)
    : QWidget(parent)
    , m_repository(new LongtitudesRepository())
{
    InitUI();
    InsertWidgetIntoLayouts();
    FillUI();
    ConnectObjects();
    LoadDataFromSettings();
}

LoaderWidget::~LoaderWidget()
{
    delete m_repository;
    delete m_validator;

    delete m_formLayout;
    delete m_barsLayout;
    delete m_mainLayout;

    delete m_descriptionLabel;
    delete m_startButton;
    delete m_latitudeBottomLineEdit;
    delete m_longtitudeWidget;
    delete m_latitudeTopLineEdit;

}

void LoaderWidget::InitUI()
{
    m_validator = new QIntValidator();
    m_mainLayout = new QVBoxLayout();

    m_formLayout = new QFormLayout();
    m_latitudeBottomLineEdit = new QLineEdit();
    m_latitudeTopLineEdit = new QLineEdit();
    m_longtitudeWidget = new LongtitudeWidget(this);
    m_descriptionLabel = new QLabel();
    m_startButton = new QPushButton();

    m_barsLayout = new QVBoxLayout();
}

void LoaderWidget::InsertWidgetIntoLayouts()
{
    m_formLayout->addRow("Широта(снизу наверх) нижняя", m_latitudeBottomLineEdit);
    m_formLayout->addRow("Широта верхняя", m_latitudeTopLineEdit);
    m_mainLayout->addLayout(m_formLayout);
    m_mainLayout->addWidget(m_longtitudeWidget);
    m_mainLayout->addWidget(m_descriptionLabel);
    m_mainLayout->addWidget(m_startButton);
    m_mainLayout->addLayout(m_barsLayout);

    setLayout(m_mainLayout);
}

void LoaderWidget::FillUI()
{
    m_latitudeBottomLineEdit->setValidator(m_validator);
    m_latitudeTopLineEdit->setValidator(m_validator);
    m_descriptionLabel->setText("долгота - папка(Слева направо), широта- имя файла(сверху 0; снизу число) <a href=\"http://tools.geofabrik.de/map/#14/79.4984/36.7248&type=Geofabrik_Standard&grid=1/\">КАРТА ТАЙЛОВ</a>");
    m_descriptionLabel->setTextFormat(Qt::RichText);
    m_descriptionLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    m_descriptionLabel->setOpenExternalLinks(true);
    m_startButton->setText("Начать скачку");
}

void LoaderWidget::LoadDataFromSettings()
{
    const QSettings settings(QStringLiteral("mapLoaderNrls"));
    const QString latitudeBottom = settings.value(latBottomKey, "7500").toString();
    const QString latitudeTop = settings.value(latTopKey, "0").toString();
    m_latitudeBottomLineEdit->setText(latitudeBottom);
    m_latitudeTopLineEdit->setText(latitudeTop);
}

void LoaderWidget::ConnectObjects()
{
    connect(m_startButton, &QPushButton::clicked, this, &LoaderWidget::OnStart);
}

void LoaderWidget::OnStart()
{

    const QString latitudeBottomText = m_latitudeBottomLineEdit->text(); //2 я где номерок файла в папке
    const int latitudeBottom = latitudeBottomText.toInt();
    const QString latitudeTopText = m_latitudeTopLineEdit->text();
    const int latitudeTop = latitudeTopText.toInt();
    if (m_longtitudeWidget->UserWantList())
    {
        m_repository->SetLongtitudesList(m_longtitudeWidget->GetLongList());
    }
    else
    {
        const int startLongtitude = m_longtitudeWidget->GetStartLong();
        m_repository->SetStartLongitude(startLongtitude);
    }
    QSettings settings(QStringLiteral("mapLoaderNrls"));
    settings.setValue(threadKey, m_threadCount - 1);
    settings.setValue(layerKey, m_layer - 1);
    settings.setValue(osmTockenKey, m_osmTocken);
    settings.setValue(latBottomKey, latitudeBottomText);
    settings.setValue(latTopKey, latitudeTopText);
    settings.setValue(longKey, QString::number(m_longtitudeWidget->GetStartLong()));
    settings.setValue(folderToSaveKey, m_folderToSavePath);
    settings.sync();
    SetProgressBars(latitudeTop, latitudeBottom);
    StartLoader(latitudeTop, latitudeBottom);
}

void LoaderWidget::OnProgressBarUpdate()
{
    const int number = sender()->objectName().toInt();
    QProgressBar *const progressBar = qobject_cast<QProgressBar *>
                                      (m_barsLayout->itemAt(number)->widget());
    progressBar->setValue(progressBar->value() + 1);
}

void LoaderWidget::OnProgressBarReset(int longtitude, int count)
{
    const int number = sender()->objectName().toInt();
    QProgressBar *const progressBar = qobject_cast<QProgressBar *>
                                      (m_barsLayout->itemAt(number)->widget());
    progressBar->setFormat("%v из %m | " + QString::number(longtitude));
    progressBar->setRange(0, count);
    progressBar->setValue(0);
}

void LoaderWidget::OnChangeThreadCount(int threadCount)
{
    m_threadCount = threadCount + 1;
}

void LoaderWidget::OnChangeLayer(int layer)
{
    m_layer = layer + 1;
}

void LoaderWidget::OnSetTocken(const QString &osmTocken)
{
    m_osmTocken = osmTocken;
}

void LoaderWidget::SetFolderToSave(const QString &path) noexcept
{
    m_folderToSavePath = path;
}

void LoaderWidget::SetProgressBars(int latitudeTop, int latitudeBottom)
{
    for (int i = 0; i < m_barsLayout->count(); ++i)
    {
        delete m_barsLayout->itemAt(0)->widget();
    }

    for (int i = 0; i < m_threadCount; ++i)
    {
        QProgressBar *const progressBar = new QProgressBar(this);
        progressBar->setToolTip(QString::number(i) + " поток");
        progressBar->setRange(latitudeTop, latitudeBottom);
        m_barsLayout->addWidget(progressBar);
    }
}

void LoaderWidget::StartLoader(int latitudeTop, int latitudeBottom)
{
    const QString siteUrl = "https://tile.openstreetmap.org/" + QString::number(m_layer) + '/';

    QMap<QThread *, ImageDownloader * > threadList;

    for (int i = 0; i < m_threadCount; ++i)
    {
        QThread *const thread = new QThread(Q_NULLPTR);
        ImageDownloader *const loader = new ImageDownloader(m_folderToSavePath, m_osmTocken, QString::number(m_layer), latitudeBottom, latitudeTop, m_repository, Q_NULLPTR);
        loader->moveToThread(thread);
        loader->setObjectName(QString::number(i));
        connect(thread, &QThread::started, loader, &ImageDownloader::OnMakeQueue);
        connect(loader, &ImageDownloader::ToFinishThread, thread, &QThread::quit);
        connect(loader, &ImageDownloader::ToUpdateProgressBar, this,
                &LoaderWidget::OnProgressBarUpdate);
        connect(loader, &ImageDownloader::ToResetBar, this,
                &LoaderWidget::OnProgressBarReset);
        connect(loader, &ImageDownloader::ToFinishThread, loader, &ImageDownloader::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        threadList.insert(thread, loader);
    }
    const QList<QThread *> threadsList = threadList.keys();
    for (QThread *const thread : threadsList)
    {
        thread->start();
    }
}
