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
#include <QThread>
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>

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
    int m_retries_count;
    QGraphicsGridLayout* m_scene_layout;
    QGraphicsWidget* m_scene_container;
    QThread* m_animation_thread;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSCENE_H