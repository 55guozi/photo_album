#include "picanimation.h"
#include "prelistwid.h"
#include "protreewidget.h"
#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"

SlideShowDlg::SlideShowDlg(QWidget *parent, QTreeWidgetItem *first_item, QTreeWidgetItem *last_item)
    : QDialog(parent)
    , ui(new Ui::SlideShowDlg),_first_item(first_item),_last_item(last_item)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    ui->slidepreBtn->SetIcons(":/icon/previous.png",
                              ":/icon/previous_hover.png",
                              ":/icon/previous_press.png");
    ui->slidenextBtn->SetIcons(":/icon/next.png",
                               ":/icon/next_hover.png",
                               ":/icon/next_press.png");
    ui->closeBtn->SetIcons(":/icon/closeshow.png",
                           ":/icon/closeshow_hover.png",
                           ":/icon/closeshow_press.png");
    ui->playBtn->setIcons(":/icon/play.png",
                          ":/icon/play_hover.png",
                          ":/icon/play_press.png",
                          ":/icon/pause.png",
                          ":/icon/pause_hover.png",
                          ":/icon/pause_press.png");

    auto* prelistWid = static_cast<PreListWid*>(ui->preListWidget);
    connect(ui->picAnimation, &PicAnimation::SigUpPreList, prelistWid, &PreListWid::SlotUpPreList);

    connect(ui->picAnimation, &PicAnimation::SigUpSelect, prelistWid, &PreListWid::SlotUpSelcct);

    connect(prelistWid, &PreListWid::SigUpSelectShow, ui->picAnimation, &PicAnimation::SlotUpSelectShow);

    connect(this, &SlideShowDlg::SigPlayStopClicked, ui->picAnimation, &PicAnimation::SlotPlayStopClicked);

    connect(ui->picAnimation, &PicAnimation::SigStart, ui->playBtn, &PicStateBtn::SlotStart);
    connect(ui->picAnimation, &PicAnimation::SigStop, ui->playBtn, &PicStateBtn::SlotStop);

    auto* protree_widget = static_cast<ProTreeWidget*>(parent);
    connect(ui->picAnimation, &PicAnimation::SigStartMusic, protree_widget, &::ProTreeWidget::SlotStartMusic);
    connect(ui->picAnimation, &PicAnimation::SigStopMusic, protree_widget, &ProTreeWidget::SlotStopMusic);

    ui->picAnimation->setPixmap(first_item);

    ui->picAnimation->start();
}


SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}

void SlideShowDlg::on_closeBtn_clicked()
{
    emit ui->picAnimation->SigStopMusic();
    close();
}


void SlideShowDlg::on_slidenextBtn_clicked()
{
    ui->picAnimation->SlideNext();
}


void SlideShowDlg::on_slidepreBtn_clicked()
{
    ui->picAnimation->SlidePre();
}


void SlideShowDlg::on_playBtn_clicked()
{
    emit SigPlayStopClicked();
}
