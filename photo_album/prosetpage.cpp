#include "prosetpage.h"
#include "ui_prosetpage.h"

#include <QDir>
#include <QFileDialog>

ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    //将lineEdit注册为Wizard的Field,内容为空时无法进行下一步
    registerField("proPath", ui->lineEdit);
    registerField("proName", ui->lineEdit_2);

    //连接槽信号,判断路径是否正确
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &ProSetPage::completeChanged);
    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, &ProSetPage::completeChanged);

    //将当前目录地址设置到lineEdit_2上
    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText(curPath);
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());

    //设置清空按钮
    ui->lineEdit->setClearButtonEnabled(true);
    ui->lineEdit_2->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit->text();
    path = ui->lineEdit_2->text();
}

bool ProSetPage::isComplete() const
{
    if(ui->lineEdit->text() == "" || ui->lineEdit_2->text() == ""){
        return false;
    }
    QDir dir(ui->lineEdit_2->text());
    if(!dir.exists()){
        ui->tips->setText("project path is exists");
        return false;
    }

    QString absFilePath = dir.absoluteFilePath(ui->lineEdit->text());
    QDir dist_dir = QDir(absFilePath);
    if(dist_dir.exists()){
        ui->tips->setText("project has exists, change path or name!");
        return false;
    }

    ui->tips->setText("");
    return QWizardPage::isComplete();

}

void ProSetPage::on_pushButton_clicked()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("选择要导入的文件");
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }

    if(fileNames.length() <= 0){
        return;
    }

    QString import_path = fileNames.at(0);
    ui->lineEdit_2->setText(import_path);
}

