//
// Created by User on 2023/08/21.
//

#include <iostream>
#include "ImageGenerationScene.h"
#include "../utils.h"

ImageGenerationScene::ImageGenerationScene(const std::string &img_directory, QWidget *parent)  : AbstractScene(parent)
{
    m_retries_count = 0;
    m_image_grid = new TiledModel_ImageGrid(NUMBER_OF_ROWS, NUMBER_OF_COLS, img_directory);
    m_scene_container = new QGraphicsWidget;
    m_scene_layout = new QGraphicsGridLayout(m_scene_container);
    m_canvas = new ImageGenerationCanvas;
    m_generation_menu = new ImageGenerationSideMenu;
    m_generation_menu_proxy = this ->addWidget(m_generation_menu);

    QObject::connect(m_generation_menu, SIGNAL(generateButtonClicked()), this, SLOT(animate()));

    m_scene_layout ->addItem(m_canvas, 0, 0);
    m_scene_layout ->addItem(m_generation_menu_proxy, 1, 0);

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
    auto number_of_rows = std::get<0>(m_image_grid -> dimensions());
    auto number_of_cols = std::get<1>(m_image_grid -> dimensions());

    for(auto row = 0; row < number_of_rows; row++)
    {
        for(auto col = 0; col < number_of_cols; col++)
        {
            auto cell_model = m_image_grid->grid().at(row).at(col);
            auto cell_graphics_item = new CellGraphicsItem(cell_model, row, col);

            auto map = m_image_grid->get_name_image_mapping();
            auto default_pixmap = new QPixmap();
            default_pixmap -> fill(Qt::white);
//            cell_graphics_item ->setPixmap(*map.at("component"));
            this ->addItem(cell_graphics_item);
            m_canvas ->add_item(cell_graphics_item, row, col);
        }
    }
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
        qDebug() << "is_fully_generated: " << m_image_grid -> is_fully_generated();
        qDebug() << "The End of animation: Retries = " << m_retries_count;
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


