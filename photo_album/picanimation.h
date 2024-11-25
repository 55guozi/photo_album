#ifndef PICANIMATION_H
#define PICANIMATION_H
#include "qtreewidget.h"
#include <QWidget>

class PicAnimation : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimation(QWidget *parent = nullptr);
    ~PicAnimation();
    void setPixmap(QTreeWidgetItem* item);
    void start();
    void stop();
    void SlideNext();
    void SlidePre();
public slots:
    void SlotUpSelectShow(const QString& path);
    void SlotPlayStopClicked();
protected:
    virtual void paintEvent(QPaintEvent *event)override;
private slots:
    void TimeOut();
private:
    float _factor;
    QTimer* _timer;
    QPixmap _pixmap1;
    QPixmap _pixmap2;
    QTreeWidgetItem* _cur_item;
    QMap<QString, QTreeWidgetItem*> _map_items;
    bool _b_start;
signals:
    void SigUpPreList(QTreeWidgetItem* );
    void SigUpSelect(QTreeWidgetItem* );
    void SigStart();
    void SigStop();
    void SigStartMusic();
    void SigStopMusic();
};

#endif // PICANIMATION_H
