//
// Created by User on 2023/09/09.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_SLIDER_H
#define WAVE_FUNCTION_COLLAPSE_UI_SLIDER_H

#include <QSlider>
#include <QLabel>
#include <QStyleOptionSlider>
#include <QGridLayout>
#include <QLineEdit>

class Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(const QString& label, int minimum, int maximum, Qt::Orientation orientation = Qt::Horizontal, QWidget* parent = nullptr);
    QLayout* get_layout() const {return m_main_layout;}
public slots:
    void setLabelValue(int value);
    void setValueFromInput(const QString& input);
signals:
    void valueSetFromInput(int value);
private:
    QLabel* m_label;
    QLineEdit* m_value;
    QStyleOptionSlider* m_style_options;
    QGridLayout* m_layout;
    QBoxLayout* m_main_layout;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_SLIDER_H
