#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>

class PicButton : public QPushButton
{
public:
    PicButton(QWidget *parent = nullptr);
    ~PicButton();
    void SetIcons(const QString &normal, const QString &hover, const QString &pressed);
protected:
    bool event(QEvent *event) override;
private:
    QString _normal;
    QString _hover;
    QString _pressed;

    void setNormalIcon();
    void setHoverIcon();
    void setPressedIcon();


};

#endif // PICBUTTON_H
