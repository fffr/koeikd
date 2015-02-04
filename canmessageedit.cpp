#include "canmessageedit.h"

CanMessageEdit::CanMessageEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void CanMessageEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    emit focussed(false);
}

void CanMessageEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    emit focussed(true);
}
