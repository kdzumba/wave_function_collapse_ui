//
// Created by User on 2023/08/23.
//

#include "ImageGenerationSideMenu.h"
#include "../widgets/Button.h"

ImageGenerationSideMenu::ImageGenerationSideMenu(QWidget *parent) : QWidget(parent)
{
    m_controls_button_group = new ButtonGroup(this);
    setStyleSheet("background: transparent");

    auto generate_button = new Button("Generate");
    QObject::connect(generate_button, SIGNAL(clicked(bool)), this, SLOT(handleGenerateButtonClicked()));
    m_controls_button_group -> add(generate_button);

    auto reset_button = new Button("Reset");
    QObject::connect(reset_button, SIGNAL(clicked(bool)), this, SLOT(handleResetButtonClicked()));
    m_controls_button_group->add(reset_button);

    auto read_new = new Button("Next Image");
    m_controls_button_group->add(read_new);
}

void ImageGenerationSideMenu::handleGenerateButtonClicked()
{
    emit generateButtonClicked();
}

void ImageGenerationSideMenu::handleResetButtonClicked()
{
    emit resetButtonClicked();
}
