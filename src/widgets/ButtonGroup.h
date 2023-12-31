//
// Created by User on 2023/08/17.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_BUTTONGROUP_H
#define WAVE_FUNCTION_COLLAPSE_UI_BUTTONGROUP_H

#include <QBoxLayout>
#include <QAbstractButton>

class ButtonGroup : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonGroup(QWidget* parent = nullptr, QBoxLayout::Direction direction = QBoxLayout::Direction::TopToBottom);
    void add(QAbstractButton* button);
    QBoxLayout* get_layout() const {return m_layout;}
private:
    QBoxLayout* m_layout;
};

#endif //WAVE_FUNCTION_COLLAPSE_UI_BUTTONGROUP_H
