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
    m_canvas = new ImageGenerationCanvas;

    m_scene_layout ->addItem(m_canvas, 0, 0);

    this ->addItem(m_scene_container);

//    //Generate a random number between 1 and 180 (QGradient::Present have values from 1 to 180)
//    auto rand_gradient = Utils::generate_random_int(1, 180);
//    auto gradient = QGradient(QGradient::Preset(rand_gradient));
////    auto gradient = QGradient(QGradient::GentleCare);
//    auto brush = QBrush(gradient);
//    this ->setBackgroundBrush(brush);

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
        std::vector<CellGraphicsItem*> ui_row;
        for(auto col = 0; col < height; col++)
        {
            auto state = m_image_grid->grid().at(row).at(col);
            auto cell_graphics_item = new CellGraphicsItem(state, row, col);
            this ->addItem(cell_graphics_item);
            ui_row.emplace_back(cell_graphics_item);
            m_canvas ->add_item(cell_graphics_item, row, col);
        }
    }
    animate();
}

void ImageGenerationScene::animate()
{
    auto generate = [&]() -> void {
        reset();
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

void ImageGenerationScene::reset()
{
    auto reset = [&]() -> void {
        m_image_grid -> reset();
    };
    m_reset_thread = QThread::create(reset);
    m_reset_thread ->start();
    m_reset_thread ->wait();
}


