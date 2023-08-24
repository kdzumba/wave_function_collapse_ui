//
// Created by User on 2023/08/23.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSIDEMENU_H
#define WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSIDEMENU_H

#include <QWidget>
#include "../widgets/ButtonGroup.h"

class ImageGenerationSideMenu : public QWidget
{
    Q_OBJECT
public:
    explicit ImageGenerationSideMenu(QWidget* parent = nullptr);
private slots:
    void handleGenerateButtonClicked();
signals:
    void generateButtonClicked();
private:
    ButtonGroup* m_controls_button_group;

};


#endif //WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSIDEMENU_H
