#include "wizard.h"
#include "ui_wizard.h"

Wizard::Wizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}

void Wizard::done(int result) {
    if(result == Wizard::Rejected){
        return QWizard::done(result);
    }

    QString name, path;
    ui->wizardPage1->GetProSettings(name, path);
    emit SigProSettings(name, path);
    return QWizard::done(result);
}
