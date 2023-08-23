//
// Created by User on 2023/08/21.
//

#include "ImageGenerationScene.h"
#include "../utils.h"
#include "../graphics_items/CellGraphicsItem.h"

ImageGenerationScene::ImageGenerationScene(const std::string &img_directory, QWidget *parent)  : AbstractScene(parent)
{
    m_retries_count = 0;
    m_image_grid = new TiledModel_ImageGrid(IMAGE_WIDTH, IMAGE_HEIGHT, img_directory);
    m_scene_container = new QGraphicsWidget;
    m_scene_layout = new QGraphicsGridLayout(m_scene_container);

    this ->addItem(m_scene_container);
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
            auto state = m_image_grid->grid().at(row).at(col);
            auto cell_graphics_item = new CellGraphicsItem(state, row, col);
            this ->addItem(cell_graphics_item);
        }
    }
}

void ImageGenerationScene::animate()
{
    auto generate = [&]() -> void {
        m_image_grid -> init_generation();
        m_retries_count = 0;
        while(!(m_image_grid -> is_fully_generated()) && m_retries_count < 10000)
        {
            m_image_grid->generate();
            m_retries_count++;
        }
    };
    m_animation_thread = QThread::create(generate);
    m_animation_thread -> start();
}


