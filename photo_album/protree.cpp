#include "protree.h"
#include "ui_protree.h"

ProTree::ProTree(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProTree)
{
    ui->setupUi(this);
}

ProTree::~ProTree()
{
    delete ui;
}

void ProTree::AddProtoTree(const QString name, const QString path)
{
    ui->treeWidget->AddProToTree(name, path);
}
