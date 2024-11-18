#include "picshow.h"
#include "ui_picshow.h"

#include <QGraphicsOpacityEffect>

PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow)
{
    ui->setupUi(this);

    ui->previousBtn->SetIcons(":/icon/previous.png",
                         ":/icon/previous_hover.png",
                         ":/icon/previous_press.png");
    ui->nextBtn->SetIcons(":/icon/next.png",
                          ":/icon/next_hover.png",
                          ":/icon/next_press.png");

    //设置初始透明度
    QGraphicsOpacityEffect* opacity_pre = new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0);
    ui->previousBtn->setGraphicsEffect(opacity_pre);

    QGraphicsOpacityEffect* opacity_next = new QGraphicsOpacityEffect(this);
    opacity_next->setOpacity(0);
    ui->nextBtn->setGraphicsEffect(opacity_next);

    //设置渐隐渐显效果
    _animation_show_pre = new QPropertyAnimation(opacity_pre, "opacity", this);
    _animation_show_pre->setDuration(500);
    _animation_show_pre->setEasingCurve(QEasingCurve::Linear);

    _animation_show_next = new QPropertyAnimation(opacity_next, "opacity", this);
    _animation_show_next->setDuration(500);
    _animation_show_next->setEasingCurve(QEasingCurve::Linear);

    connect(ui->nextBtn,&QPushButton::clicked,this, &PicShow::SigNextClicked);
    connect(ui->previousBtn,&QPushButton::clicked,this, &PicShow::SigPreClicked);
}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::ReloadPic()
{
    if(_selected_path!=""){
        auto width = ui->gridLayout->geometry().width() - 20;
        auto height = ui->gridLayout->geometry().height() - 20;

        _pix_map.load(_selected_path);
        _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);

        ui->label->setPixmap(_pix_map);
    }
}

bool PicShow::event(QEvent *event)
{
    switch(event->type()){
    case QEvent::Enter:
        ShowPreNextBtns(true);
        break;
    case QEvent::Leave:
        ShowPreNextBtns(false);
        break;
    default:
        break;
    }
    return QDialog::event(event);
}

//实现渐隐渐显逻辑
void PicShow::ShowPreNextBtns(bool b_show)
{
    if(!b_show && _b_btnvisible){
        _animation_show_pre->setStartValue(1);
        _animation_show_pre->setEndValue(0);
        _animation_show_pre->start();

        _animation_show_next->setStartValue(1);
        _animation_show_next->setEndValue(0);
        _animation_show_next->start();

        _b_btnvisible = false;

        return;
    }

    if(b_show && !_b_btnvisible){
        _animation_show_pre->setStartValue(0);
        _animation_show_pre->setEndValue(1);
        _animation_show_pre->start();

        _animation_show_next->setStartValue(0);
        _animation_show_next->setEndValue(1);
        _animation_show_next->start();

        _b_btnvisible = true;

        return;
    }
}

void PicShow::SlotDoubleClickItem(const QString &path)
{
    _selected_path = path;
    _pix_map.load(path);

    auto width = this->width() - 20;
    auto height = this->height() - 20;

    _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);

    ui->label->setPixmap(_pix_map);
}

void PicShow::SlotUpdatePic(const QString &path)
{
    _selected_path = path;
    if(_selected_path!=""){
        auto width = ui->gridLayout->geometry().width() - 20;
        auto height = ui->gridLayout->geometry().height() - 20;

        _pix_map.load(_selected_path);
        _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);

        ui->label->setPixmap(_pix_map);
    }
}

void PicShow::SlotDeleteItem()
{
    _selected_path = "";
    _pix_map = QPixmap();
    ui->label->clear();
}



void PicShow::on_previousBtn_clicked()
{
    emit SigPreClicked();
}


void PicShow::on_nextBtn_clicked()
{
    emit SigNextClicked();
}

