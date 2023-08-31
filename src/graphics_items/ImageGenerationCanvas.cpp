//
// Created by User on 2023/08/23.
//

#include "ImageGenerationCanvas.h"

ImageGenerationCanvas::ImageGenerationCanvas(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
    m_canvas_layout = new QGraphicsGridLayout(this);
    m_canvas_layout ->setSpacing(0);
}

void ImageGenerationCanvas::add_item(QGraphicsLayoutItem *item, int row, int col, Qt::Alignment alignment)
{
    m_canvas_layout ->addItem(item, row, col, alignment);
}


