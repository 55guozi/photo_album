#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include <QPropertyAnimation>

namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();
    void ReloadPic();
protected:
    bool event(QEvent *event);
private:
    Ui::PicShow *ui;
    QPropertyAnimation* _animation_show_pre;
    QPropertyAnimation* _animation_show_next;
    bool _b_btnvisible;
    QString _selected_path;
    QPixmap _pix_map;

    void ShowPreNextBtns(bool b_show);
public slots:
    void SlotDoubleClickItem(const QString& path);
    void SlotUpdatePic(const QString& path);
    void SlotDeleteItem();
private slots:
    void on_previousBtn_clicked();
    void on_nextBtn_clicked();
signals:
    void SigNextClicked();
    void SigPreClicked();
};

#endif // PICSHOW_H
