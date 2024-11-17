#include "removeprodialog.h"
#include "ui_removeprodialog.h"

RemoveProDIalog::RemoveProDIalog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RemoveProDIalog)
{
    ui->setupUi(this);
}

RemoveProDIalog::~RemoveProDIalog()
{
    delete ui;
}

bool RemoveProDIalog::isRemove(){
    bool bChecked = ui->radioButton->isChecked();
    return bChecked;
}
