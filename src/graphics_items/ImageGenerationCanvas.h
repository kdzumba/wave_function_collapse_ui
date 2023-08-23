//
// Created by User on 2023/08/23.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONCANVAS_H
#define WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONCANVAS_H

#include <QGraphicsWidget>
#include <QGraphicsGridLayout>

class ImageGenerationCanvas : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit ImageGenerationCanvas(QGraphicsItem* parent = nullptr);
    void add_item(QGraphicsLayoutItem* item, int row, int col, Qt::Alignment  = Qt::Alignment());
private:
    QGraphicsGridLayout* m_canvas_layout;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_IMAGEGENERATIONCANVAS_H
