//
// Created by User on 2023/08/17.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_BUTTON_H
#define WAVE_FUNCTION_COLLAPSE_UI_BUTTON_H

#include <QPushButton>

class Button : public QPushButton
{
    Q_OBJECT
public:
    explicit Button(const QString& text,  QWidget* parent = nullptr);
private:
    QString m_text;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_BUTTON_H
