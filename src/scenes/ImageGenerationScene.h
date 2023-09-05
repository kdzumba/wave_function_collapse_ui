//
// Created by User on 2023/08/21.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSCENE_H
#define WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSCENE_H

#include "AbstractScene.h"
#include "../models/Cell.h"
#include "../models/TiledModel_ImageGrid.h"
#include "../graphics_items/ImageGenerationCanvas.h"
#include "../graphics_items/CellGraphicsItem.h"
#include "../ui_forms/ImageGenerationSideMenu.h"
#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QThread>
#include <QGraphicsGridLayout>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>

class ImageGenerationScene : public AbstractScene
{
    Q_OBJECT
public:
    explicit ImageGenerationScene(const std::string& img_directory, QWidget* parent = nullptr);
    ~ImageGenerationScene() override;
    void init();
public slots:
    void animate() override;
    void reset();
public:
    static constexpr int NUMBER_OF_ROWS = 25;
    static constexpr int NUMBER_OF_COLS = 35;
private:
    TiledModel_ImageGrid* m_image_grid;
    int m_retries_count;
    QGraphicsGridLayout* m_scene_layout;
    ImageGenerationCanvas* m_canvas;
    QGraphicsWidget* m_scene_container;
    ImageGenerationSideMenu* m_generation_menu;
    QGraphicsProxyWidget* m_generation_menu_proxy;
    QThread* m_animation_thread;
    QThread* m_reset_thread;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONSCENE_H
