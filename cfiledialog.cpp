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

    //    QDialogButtonBox *pButton = this->findChild<QDialogButtonBox *>("buttonBox");
}

CFileDialog::~CFileDialog()
{

}
