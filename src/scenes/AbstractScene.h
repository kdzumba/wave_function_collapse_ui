//
// Created by User on 2023/08/14.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_ABSTRACTSCENE_H
#define WAVE_FUNCTION_COLLAPSE_UI_ABSTRACTSCENE_H

#include <QGraphicsScene>
#include <QWidget>

class AbstractScene : public QGraphicsScene
{
public:
    explicit AbstractScene(QWidget* parent) : QGraphicsScene(parent){}
    virtual void animate() = 0;
};

#endif //WAVE_FUNCTION_COLLAPSE_UI_ABSTRACTSCENE_H
