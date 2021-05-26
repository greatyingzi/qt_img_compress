#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "cfiledialog.h"
#include <QDebug>
#include <QStandardPaths>
#include "networkrequestcallback.h"
#include "postfile.h"
#include "downloadfile.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(600,400);
//    setWindowIcon(QIcon(":res/img/compress.png"));

    fileSrcDialog = new CFileDialog;
    fileSaveDirDialog = new CFileDialog;
    qInfo() << "MainWindow初始化。";

    QObject::connect(ui->browserFileBtn, SIGNAL(clicked()), this, SLOT(openSrcFile()));
    QObject::connect(ui->browserFileSaveBtn, SIGNAL(clicked()), this, SLOT(openSaveDir()));
    QObject::connect(ui->compressBtn,SIGNAL(clicked()),this,SLOT(startCompress()));

    forzenWidgets(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSrcFile()
{
    if (fileSrcDialog)
        delete fileSrcDialog;
    fileSrcDialog = new CFileDialog;
    connect(fileSrcDialog,SIGNAL(accepted()),this,SLOT(onSrcFileChiose()));
    fileSrcDialog->setDirectory(getUserPath());
    fileSrcDialog->setFileMode(QFileDialog::FileMode::AnyFile);
    fileSrcDialog->setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
    fileSrcDialog->setNameFilter(tr("All Images (*.jpg *.jpeg *.png);;"
                                 /*"All Texts (*.txt *.text *.html);;"*/));
    fileSrcDialog->exec();
}

void MainWindow::onSrcFileChiose()
{
    QString files = "";

    QStringList fileList = fileSrcDialog->selectedFiles();
    qInfo() << "选中的文件:" << fileList;

    for (int i = 0;i<fileList.size();i++)
    {
        files.append(fileList.at(i)).append(";");
    }

    //移除最后的“;”
    files.remove(files.size()-1,1);

    ui->filePathEdit->setText(files);
}

void MainWindow::openSaveDir()
{
    if (fileSrcDialog)
        delete fileSrcDialog;
    fileSrcDialog = new CFileDialog;
    connect(fileSrcDialog,SIGNAL(accepted()),this,SLOT(onSaveDirChiose()));
    fileSrcDialog->setDirectory(getUserPath());
    fileSrcDialog->setFileMode(QFileDialog::FileMode::Directory);
    fileSrcDialog->setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);

    fileSrcDialog->exec();
}


void MainWindow::onSaveDirChiose()
{
    QString files = "";

    QStringList fileList = fileSrcDialog->selectedFiles();
    qInfo() << "选中的文件:" << fileList;

    for (int i = 0;i<fileList.size();i++)
    {
        files.append(fileList.at(i)).append(";");
    }

    //移除最后的“;”
    files.remove(files.size()-1,1);

    ui->filePathSaveEdit->setText(files);
}

void MainWindow::startCompress()
{
    QString src = ui->filePathEdit->text();
    saveDir = ui->filePathSaveEdit->text();

    if (src.isEmpty()||saveDir.isEmpty()){
        QMessageBox::information(this, "提示", "路径不能为空");
        return;
    }

    forzenWidgets(false);

    needOverride = ui->overrideSrc->isChecked();
    srcFiles = ui->filePathEdit->text();

    PostFile *postFile = new PostFile(srcFiles,"90");
    QObject::connect(postFile, SIGNAL(compressedSuccess(QString,QString)), this, SLOT(compressedSuccess(QString,QString)));
    postFile->startPost();
}

void MainWindow::compressedSuccess(const QString &url,const QString &srcFile)
{
    qInfo() << "文件压缩成功";
    ui->label->setText(ui->label->text().append(url));
    DownloadFile* downloadFile = new DownloadFile(url,saveDir,"filemane",srcFile);
    connect(downloadFile,SIGNAL(onDownloadSuccess(QString,QString)),this,SLOT(onDownloadSuccess(QString,QString)));
    downloadFile->startDownload();
}

void MainWindow::onDownloadSuccess(const QString &srcFilePath, const QString &newPath)
{
    QMessageBox::information(this, "提示", "图片压缩成功");
    forzenWidgets(true);
}

QString MainWindow::getUserPath()
{
    QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return userPath;
}

void MainWindow::forzenWidgets(bool forzen){
    ui->overrideSrc->setEnabled(forzen);
    ui->filePathEdit->setEnabled(forzen);
    ui->browserFileBtn->setEnabled(forzen);
    ui->compressBtn->setEnabled(forzen);
    ui->filePathSaveEdit->setEnabled(forzen);
    ui->browserFileSaveBtn->setEnabled(forzen);

    //禁用不支持的功能控件
    ui->testBtn->setEnabled(false);
    ui->overrideSrc->setEnabled(false);

}
