//
// Created by User on 2023/09/09.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_SLIDERGROUP_H
#define WAVE_FUNCTION_COLLAPSE_UI_SLIDERGROUP_H

#include <QWidget>
#include <QBoxLayout>
#include <QAbstractSlider>

class SliderGroup : public QWidget
{
    Q_OBJECT
public:
    explicit SliderGroup(QWidget* parent = nullptr, QBoxLayout::Direction direction = QBoxLayout::Direction::TopToBottom);
    void add(QAbstractSlider* slider);
    void addLayout(QLayout* layout);
    QBoxLayout* get_layout() const {return m_layout;};
private:
    QBoxLayout* m_layout;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_SLIDERGROUP_H
