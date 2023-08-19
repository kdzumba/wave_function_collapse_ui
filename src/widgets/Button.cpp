//
// Created by User on 2023/08/17.
//

#include "Button.h"

Button::Button(const QString &text, QWidget *parent) : QPushButton(text, parent), m_text(text)
{

    this ->setStyleSheet("background-color: white;"
                         "border-radius: 10px; "
                         "font-family: Arial;"
                         "font-size: 18px;"
                         "padding: 20px;"
                         "color: black");
}
