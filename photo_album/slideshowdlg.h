#ifndef SLIDESHOWDLG_H
#define SLIDESHOWDLG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class SlideShowDlg;
}

class SlideShowDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SlideShowDlg(QWidget *parent = nullptr, QTreeWidgetItem* first_item = nullptr, QTreeWidgetItem* last_item = nullptr);
    ~SlideShowDlg();

private:
    Ui::SlideShowDlg *ui;
    QTreeWidgetItem* _first_item;
    QTreeWidgetItem* _last_item;
signals:
    void SigPlayStopClicked();
private slots:
    void on_playBtn_clicked();
    void on_closeBtn_clicked();
    void on_slidepreBtn_clicked();
    void on_slidenextBtn_clicked();
};

#endif // SLIDESHOWDLG_H
