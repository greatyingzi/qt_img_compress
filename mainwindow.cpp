#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "cfiledialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setBackgroundRole(QPalette::ColorRole::Dark);
    setFixedSize(600,400);


    CFileDialog *fileDialog = new CFileDialog;

    QObject::connect(ui->browserFileBtn,SIGNAL(clicked()),fileDialog,SLOT(exec()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

