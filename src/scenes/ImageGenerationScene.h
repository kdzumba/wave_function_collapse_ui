//
// Created by User on 2023/08/21.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSCENE_H
#define WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSCENE_H

#include "AbstractScene.h"
#include "../models/Cell.h"
#include "../models/TiledModel_ImageGrid.h"
#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QImage>

class ImageGenerationScene : public AbstractScene
{
    Q_OBJECT
public:
    explicit ImageGenerationScene(const std::string& img_directory, QWidget* parent = nullptr);
    ~ImageGenerationScene() override;
    void init();
public slots:
    void animate() override;
public:
    static constexpr int IMAGE_WIDTH = 13;
    static constexpr int IMAGE_HEIGHT = 13;
private:
    TiledModel_ImageGrid* m_image_grid;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSCENE_H
