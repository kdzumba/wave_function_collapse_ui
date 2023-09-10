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
    m_scene_layout = new QGraphicsLinearLayout(m_scene_container);
    m_canvas = new ImageGenerationCanvas;
    m_generation_menu = new ImageGenerationSideMenu;
    m_generation_menu_proxy = this ->addWidget(m_generation_menu);

    QObject::connect(m_generation_menu, SIGNAL(generateButtonClicked()), this, SLOT(animate()));
    QObject::connect(m_generation_menu, SIGNAL(resetButtonClicked()), this, SLOT(reset()));

    m_scene_layout ->addItem(m_canvas);
    m_scene_layout ->addItem(m_generation_menu_proxy);

    this ->addItem(m_scene_container);
    //Generate a random number between 1 and 180 (QGradient::Present have values from 1 to 180)
    auto rand_gradient = Utils::generate_random_int(1, 180);
    auto gradient = QGradient(QGradient::Preset(rand_gradient));
//    auto gradient = QGradient(QGradient::HealthyWater);
    auto brush = QBrush(gradient);
    this ->setBackgroundBrush(brush);
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
            auto default_pixmap = new QPixmap("C:\\Users\\User\\CLionProjects\\wave_function_collapse_ui\\cmake-build-debug\\images\\summer-background.png");

            cell_graphics_item->setPixmap(*default_pixmap);
            this ->addItem(cell_graphics_item);
            m_canvas ->add_item(cell_graphics_item, row, col);
        }
    }
}

void ImageGenerationScene::animate()
{
    auto generate = [&]() -> void {
        reset();
        m_retries_count = 0;
        while(!(m_image_grid->in_contradiction()))
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


