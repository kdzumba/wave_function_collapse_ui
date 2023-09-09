//
// Created by User on 2023/09/09.
//

#include "Slider.h"

Slider::Slider(const QString& label, int minimum, int maximum, Qt::Orientation orientation, QWidget *parent) : QSlider(orientation, parent)
{
    m_label = new QLabel(label);
    m_label ->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_label->setStyleSheet("font-size: 14px;"
                           "background: transparent;"
                           "font-family: Arial");

    m_value = new QLineEdit("0");
    m_value->setStyleSheet("font-size: 12px;"
                           "background: white;"
                           "color: black;"
                           "font-family: Arial");

    m_style_options = new QStyleOptionSlider;
    m_style_options->orientation = orientation;
    m_style_options->minimum = minimum;
    m_style_options->maximum = maximum;

    this->setRange(minimum, maximum);
    this->initStyleOption(m_style_options);
    this->setStyleSheet("background: transparent");

    m_layout = new QGridLayout;
    m_layout->addWidget(this, 0,0);
    m_layout->addWidget(m_value, 0, 1);
    m_main_layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, parent);
    m_main_layout->addWidget(m_label);
    m_main_layout->addLayout(m_layout);

    QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(setLabelValue(int)));
    QObject::connect(m_value, SIGNAL(textChanged(const QString&)), this, SLOT(setValueFromInput(const QString&)));
    QObject::connect(this, SIGNAL(valueSetFromInput(int)), this, SLOT(setValue(int)));
}

void Slider::setLabelValue(int value)
{
    m_value->setText(QString::number(value));
}

void Slider::setValueFromInput(const QString& input)
{
    auto value = input.toInt();
    emit valueSetFromInput(value);
}