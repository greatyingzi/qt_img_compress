#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "cfiledialog.h"
#include <QDebug>
#include <QStandardPaths>
#include "networkrequestcallback.h"
#include "postfile.h"
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setBackgroundRole(QPalette::ColorRole::Dark);
    setFixedSize(600,400);

    fileDialog = new CFileDialog;
    qInfo() << "MainWindow初始化。";

    QObject::connect(ui->browserFileBtn, SIGNAL(clicked()), this, SLOT(openFile()));
    QObject::connect(ui->compressBtn,SIGNAL(clicked()),this,SLOT(startCompress()));
    QObject::connect(ui->testBtn,SIGNAL(clicked()),this,SLOT(readText()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    if (fileDialog)
        delete fileDialog;
    fileDialog = new CFileDialog;
    connect(fileDialog,SIGNAL(accepted()),this,SLOT(onChiose()));
    fileDialog->setDirectory(getUserPath());
    fileDialog->setFileMode(QFileDialog::FileMode::AnyFile);
    fileDialog->setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
    fileDialog->setNameFilter(tr("All Images (*.jpg *.jpeg *.png);;"
                                 "All Texts (*.txt *.text *.html);;"));
    fileDialog->exec();
}

void MainWindow::onChiose()
{
    QString files = "";

    QStringList fileList = fileDialog->selectedFiles();
    qInfo() << "选中的文件:" << fileList;

    for (int i = 0;i<fileList.size();i++)
    {
        files.append(fileList.at(i)).append(";");
    }

    //移除最后的“;”
    files.remove(files.size()-1,1);

    ui->filePathEdit->setText(files);
}

void MainWindow::startCompress()
{

    forzenWidgets(true);

    needOverride = ui->overrideSrc->isChecked();
    files = ui->filePathEdit->text();

    PostFile *postFile = new PostFile(&files,new QString("90"));
    postFile->startPost();
}

void MainWindow::readText()
{
    files = ui->filePathEdit->text();
    QFile *textFile = new QFile(files);
//    QTextStream *textStream = new QTextStream(&files,QIODevice::ReadWrite|QIODevice::Text);
    textFile->open(QIODevice::ReadWrite|QIODevice::Text);
    QString content = textFile->readAll();
    textFile->close();
    ui->textBrowser->setText(content);

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
}
