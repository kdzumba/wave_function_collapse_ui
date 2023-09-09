//
// Created by User on 2023/08/17.
//

#ifndef WAVE_FUNCTION_COLLAPSE_RADIO_BUTTON
#define WAVE_FUNCTION_COLLAPSE_RADIO_BUTTON

#include <QCheckBox>

class CheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit CheckBox(const QString& text, QWidget* parent = nullptr);
private:
    QString m_text;
};


#endif //WAVE_FUNCTION_COLLAPSE_RADIO_BUTTON
