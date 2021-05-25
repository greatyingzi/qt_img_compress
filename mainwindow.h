#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cfiledialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QString getUserPath();
    ~MainWindow();

    void forzenWidgets(bool forzen);
private slots:
    void openFile();
    void onChiose();
    void startCompress();
    void readText();

private:
    Ui::MainWindow *ui;
    CFileDialog *fileDialog;
    bool needOverride;
    QString files;
};
#endif // MAINWINDOW_H
