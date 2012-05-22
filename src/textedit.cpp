#include "textedit.h"
#include <QTextEdit>

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent){ }

void TextEdit::focusInEvent(QFocusEvent *event){
     sendFocusIn();
     QTextEdit::focusInEvent(event);
}