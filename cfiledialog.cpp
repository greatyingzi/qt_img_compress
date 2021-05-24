#include "cfiledialog.h"
#include <QFileDialog>
#include <QListView>
#include <QTreeView>
#include <QDialogButtonBox>
#include <QDebug>

CFileDialog::CFileDialog(QWidget *parent)
    :QFileDialog(parent)
{
    this->setOption(QFileDialog::DontUseNativeDialog,true);
    QListView *pListView = this->findChild<QListView*>("listView");
    if(pListView){
        pListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    QTreeView *pTreeView = this->findChild<QTreeView*>();
    if (pTreeView)
            pTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QDialogButtonBox *pButton = this->findChild<QDialogButtonBox *>("buttonBox");

    disconnect(pButton, SIGNAL(accepted()), this, SLOT(accept()));//使链接失效
    connect(pButton, SIGNAL(accepted()), this, SLOT(onChiose()));//改成自己的槽
}

CFileDialog::~CFileDialog()
{
    qInfo() << "选中的文件:" << selectedFiles();
}


void CFileDialog::onChiose()
{
    QDialog::accept();
    qDebug() << "选中的文件:" << selectedFiles();
}
