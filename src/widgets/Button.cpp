//
// Created by User on 2023/08/17.
//

#include "Button.h"

Button::Button(const QString &text, QWidget *parent) : QPushButton(text, parent), m_text(text)
{
    this ->setStyleSheet("background-color: white;"
                         "padding: 20px; "
                         "border-radius: 10px; "
                         "width: 200px;"
                         "color: red");
}
