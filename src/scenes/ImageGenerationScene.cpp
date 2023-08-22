//
// Created by User on 2023/08/21.
//

#include "ImageGenerationScene.h"
#include "../utils.h"
#include <QGraphicsPixmapItem>

ImageGenerationScene::ImageGenerationScene(const std::string &img_directory, QWidget *parent)  : AbstractScene(parent)
{
    m_image_grid = new TiledModel_ImageGrid(IMAGE_WIDTH, IMAGE_HEIGHT, img_directory);
    init();
}

ImageGenerationScene::~ImageGenerationScene()
{
    delete m_image_grid;
    m_image_grid = nullptr;
}

void ImageGenerationScene::init()
{
    auto width = std::get<0>(m_image_grid -> dimensions());
    auto height = std::get<1>(m_image_grid -> dimensions());

    for(auto row = 0; row < width; row++)
    {
        for(auto col = 0; col < height; col++)
        {
            auto rando_tile_index = Utils::generate_random_int(0, 12);
            auto tile = m_image_grid -> states().at(rando_tile_index);
            auto x = row * tile->width();
            auto y = col * tile -> height();
            auto item = new QGraphicsPixmapItem;
            item ->setPixmap(*tile);
            item ->setPos(x, y);
            this ->addItem(item);
        }
    }
}

void ImageGenerationScene::animate()
{
    qDebug() << "Animating image generation";
}


