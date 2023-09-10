//
// Created by User on 2023/08/23.
//

#include "ImageGenerationSideMenu.h"
#include "../widgets/Button.h"

ImageGenerationSideMenu::ImageGenerationSideMenu(QWidget *parent) : AbstractSideMenu(parent)
{
    setStyleSheet("background: rgba(20, 20, 20, 0.1);"
                  "margin: 0 10 0 10;");

    m_controls_button_group = new ButtonGroup(this);

    auto generate_button = new Button("Generate New");
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
