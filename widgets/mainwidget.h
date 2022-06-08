#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QLineEdit>

#include "loaderwidget.h"
#include "checkerwidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent);
    ~MainWidget();
private:
    void InitUI();
    void InsertWidgetIntoLayouts();
    void FillUI();
    void ConnectObjects();
    void LoadDataFromSettings();
private Q_SLOTS:
    void OnSetPathButton();
private:
    QIntValidator *m_osmValidator;
private:
    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_topLayout;
    QLabel *m_threadLabel;
    QComboBox *m_treadCountComboBox;
    QLabel *m_layerLabel;
    QComboBox *m_layerComboBox;
    QLabel *m_osmLabel;
    QLineEdit *m_osmTockenLineEdit;

    QHBoxLayout *m_pathLayout;
    QLabel *m_pathLabel;
    QPushButton *m_setPathButton;

    QTabWidget *m_tabWidget;

    LoaderWidget *m_loaderWidget;
    CheckerWidget *m_checkerWidget;
};

#endif // MAINWIDGET_H
