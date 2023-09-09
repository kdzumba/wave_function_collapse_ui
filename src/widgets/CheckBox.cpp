//
// Created by User on 2023/08/17.
//

#include "CheckBox.h"

CheckBox::CheckBox(const QString &text, QWidget *parent): QCheckBox(text, parent), m_text(text)
{

    this ->setStyleSheet("background-color: transparent;"
                         "border-radius: 10px; "
                         "color: white; "
                         "font-family: Arial;"
                         "font-size: 18px;"
                         "padding: 10;");
}
