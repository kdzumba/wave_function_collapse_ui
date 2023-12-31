//
// Created by User on 2023/08/14.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_GRAPHICSWORKSPACE_H
#define WAVE_FUNCTION_COLLAPSE_UI_GRAPHICSWORKSPACE_H

#include <QFrame>
#include "../scenes/AbstractScene.h"
#include "../AbstractSideMenu.h"
#include "sudokuscenesidemenu.h"
#include <QGraphicsView>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class GraphicsWorkspace; }
QT_END_NAMESPACE

class GraphicsWorkspace : public QGraphicsView {
Q_OBJECT

public:
    explicit GraphicsWorkspace(QWidget *parent = nullptr);
    ~GraphicsWorkspace() override;
private:
    void init_default_scenes();

private:
    Ui::GraphicsWorkspace *ui;
    std::vector<AbstractScene*> m_scene_states;
    std::vector<AbstractSideMenu*> m_menus;
    QGridLayout* m_main_grid_layout;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_GRAPHICSWORKSPACE_H
