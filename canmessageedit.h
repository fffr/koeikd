#ifndef CANMESSAGEEDIT_H
#define CANMESSAGEEDIT_H

#include <QLineEdit>

class CanMessageEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CanMessageEdit(QWidget *parent = 0);

protected:
    void focusOutEvent(QFocusEvent *e);
    void focusInEvent(QFocusEvent *e);

signals:
    void focussed(bool hasFocus);
};

#endif // CANMESSAGEEDIT_H
