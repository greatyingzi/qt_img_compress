#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "cfiledialog.h"
#include <QDebug>
#include <QStandardPaths>
#include "postfile.h"
#include "downloadfile.h"
#include <QMessageBox>
#include <QDirIterator>
#include <QStringList>
#include <QList>
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(600,400);
    this->allFiles = new QStringList();
    this->forzenWidgets = new QList<QWidget*>();
    //    setWindowIcon(QIcon(":res/img/compress.png"));

    qInfo() << "MainWindow初始化。";

    QObject::connect(ui->browserFileBtn, SIGNAL(clicked()), this, SLOT(btnOpenSrcFileClick()));
    QObject::connect(ui->browserFileSaveBtn, SIGNAL(clicked()), this, SLOT(btnOpenSaveDirClick()));
    QObject::connect(ui->compressBtn,SIGNAL(clicked()),this,SLOT(btnCompressClick()));
//    QObject::connect(ui->overrideSrc,SIGNAL(clicked(bool)),ui->filePathSaveEdit,SLOT(setDisabled(bool)));
//    QObject::connect(ui->overrideSrc,SIGNAL(clicked(bool)),ui->browserFileSaveBtn,SLOT(setDisabled(bool)));
    QObject::connect(ui->overrideSrc,SIGNAL(stateChanged(int)),this,SLOT(overrideSrcStateChanged(int)));
    toggleWidgetsStatus(true);
}

MainWindow::~MainWindow()
{
    allFiles->clear();
    delete allFiles;
    allFiles = nullptr;

    forzenWidgets->clear();
    delete forzenWidgets;
    forzenWidgets = nullptr;

    delete ui;
}

void MainWindow::btnOpenSrcFileClick()
{
    fileSrcDialog = new CFileDialog(this);
    connect(fileSrcDialog,SIGNAL(accepted()),this,SLOT(onSrcFileChiose()));
    fileSrcDialog->setDirectory(getUserPath());
    if (ui->selectDirCheckBox->isChecked()) {
        fileSrcDialog->setFileMode(QFileDialog::FileMode::Directory);
    }else {
        fileSrcDialog->setFileMode(QFileDialog::FileMode::ExistingFiles);
        fileSrcDialog->setNameFilter(tr("All Images (*.jpg *.jpeg *.png);;"
                                        /*"All Texts (*.txt *.text *.html);;"*/));
    }
    fileSrcDialog->setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);

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

void MainWindow::btnOpenSaveDirClick()
{
    fileSrcDialog = new CFileDialog(this);
    connect(fileSrcDialog,SIGNAL(accepted()),this,SLOT(onSaveDirChiose()));
    fileSrcDialog->setDirectory(getUserPath());
    fileSrcDialog->setFileMode(QFileDialog::FileMode::DirectoryOnly);
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

void MainWindow::btnCompressClick()
{
    QString src = ui->filePathEdit->text();
    saveDir = ui->filePathSaveEdit->text();
    needOverride = ui->overrideSrc->isChecked();
    if (src.isEmpty()||(!needOverride && saveDir.isEmpty())){
        QMessageBox::information(this, "提示", "路径不能为空");
        return;
    }

    toggleWidgetsStatus(false);

    QString tmpFilePath = ui->filePathEdit->text();
    auto filelist = tmpFilePath.split(";");

    foreach (QString s, filelist) {
        QFile tmpFile(s);
        QFileInfo tmpFileInfo(s);

        if (tmpFileInfo.isFile()){
            allFiles->append(s);
        } else {
            //todo 遍历文件夹，获取文件列表
            scanFile(tmpFilePath, *allFiles);
        }
    }
    ui->progressBar->setMaximum(allFiles->size());
    ui->progressBar->setValue(0);
    startCompress();
}

void MainWindow::scanFile(QString path, QStringList &fileList){
    QStringList filters;
    filters << QString("*.jpg") << QString("*.png" )<< QString("*.jpeg");

    //定义迭代器并设置过滤器
    QDirIterator dir_iterator(path,
                              filters,
                              QDir::Files | QDir::NoSymLinks,
                              QDirIterator::Subdirectories);

    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString absolute_file_path = file_info.absoluteFilePath();
        fileList.append(absolute_file_path);
        qInfo() << absolute_file_path;
    }

}

void MainWindow::startCompress()
{
    if (allFiles && !allFiles->isEmpty()) {
        currentFile = allFiles->first();
        allFiles->pop_front();
    }
    PostFile postFile(currentFile,"90");
    QObject::connect(&postFile, SIGNAL(compressedSuccess(QString,QString)), this, SLOT(compressedSuccess(QString,QString)));
    postFile.startPost();
}

void MainWindow::forzenWidget(QWidget &widget, bool forzen)
{
    widget.setEnabled(!forzen);

    if (!forzen) {
        forzenWidgets->removeOne(&widget);
//        forzenWidgets->takeAt(forzenWidgets->indexOf(&widget,0));
    }else {
//        forzenWidgets->append(widget);
        forzenWidgets->insert(0, &widget);

    }

}

void MainWindow::compressedSuccess(const QString &url,const QString &srcFile)
{
    qInfo() << "文件压缩成功";
    ui->label->setText(ui->label->text().append("\r\n").append(url));
    DownloadFile* downloadFile;
    if (needOverride) {
        downloadFile = new DownloadFile(this, url, srcFile, true);
    }else {
        downloadFile = new DownloadFile(this, url, saveDir, srcFile);
    }

    connect(downloadFile,SIGNAL(onDownloadSuccess(QString,QString)),this,SLOT(onDownloadSuccess(QString,QString)));
    downloadFile->startDownload();
}

void MainWindow::onDownloadSuccess(const QString &srcFilePath, const QString &newPath)
{
    ui->progressBar->setValue(ui->progressBar->value()+1);
    if (allFiles->isEmpty()) {
        QMessageBox::information(this, "提示", "图片压缩成功");
        toggleWidgetsStatus(true);
    }
    else {
        startCompress();
    }
}

void MainWindow::overrideSrcStateChanged(int state)
{
    bool checked = state==Qt::CheckState::Checked;
    forzenWidget(*ui->filePathSaveEdit, checked);
    forzenWidget(*ui->browserFileSaveBtn, checked);
}

QString MainWindow::getUserPath()
{
    QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return userPath;
}

void MainWindow::toggleWidgetsStatus(bool forzen){
    ui->progressBar->setValue(0);

    ui->overrideSrc->setEnabled(forzen);
    ui->filePathEdit->setEnabled(forzen);
    ui->browserFileBtn->setEnabled(forzen);
    ui->compressBtn->setEnabled(forzen);
    ui->filePathSaveEdit->setEnabled(forzen);
    ui->browserFileSaveBtn->setEnabled(forzen);

    //todo 完善迭代修改需要冻结的控件
    if (!forzen) {
        for (int i = 0;i<forzenWidgets->length();i++) {
            forzenWidgets->value(i)->setEnabled(forzen);
        }
    }
}
