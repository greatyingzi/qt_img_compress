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
    void scanFile(QString path, QStringList &fileList);
private slots:
    void btnOpenSrcFileClick();
    void onSrcFileChiose();
    void btnOpenSaveDirClick();
    void onSaveDirChiose();
    void btnCompressClick();
    void startCompress();
    void compressedSuccess(const QString &url,const QString &srcFile);
    void onDownloadSuccess(const QString &srcFilePath, const QString &newPath);

private:
    Ui::MainWindow *ui;
    CFileDialog *fileSrcDialog;
    CFileDialog *fileSaveDirDialog;
    bool needOverride;
    QString currentFile;
    QStringList *allFiles;
    QString saveDir;
};
#endif // MAINWINDOW_H
