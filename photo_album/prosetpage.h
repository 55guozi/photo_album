#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = nullptr);
    ~ProSetPage();
    void GetProSettings(QString& name, QString& path);
    virtual bool isComplete() const override;

private:
    Ui::ProSetPage *ui;
private slots:
    void on_pushButton_clicked();
};

#endif // PROSETPAGE_H
