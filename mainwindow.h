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
    void openSrcFile();
    void onSrcFileChiose();
    void openSaveDir();
    void onSaveDirChiose();
    void startCompress();
    void compressedSuccess(const QString &url,const QString &srcFile);
    void onDownloadSuccess(const QString &srcFilePath, const QString &newPath);

private:
    Ui::MainWindow *ui;
    CFileDialog *fileSrcDialog;
    CFileDialog *fileSaveDirDialog;
    bool needOverride;
    QString srcFiles;
    QString saveDir;
};
#endif // MAINWINDOW_H
