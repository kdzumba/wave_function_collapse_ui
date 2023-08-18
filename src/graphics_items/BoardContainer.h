//
// Created by User on 2023/08/16.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_BOARDCONTAINER_H
#define WAVE_FUNCTION_COLLAPSE_UI_BOARDCONTAINER_H

#include <QGraphicsWidget>
#include <QGraphicsItem>
#include <QGraphicsLayout>
#include <QGraphicsGridLayout>

//This is a top level container class that adds other layouts to its main layout
class BoardContainer : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit BoardContainer(QGraphicsItem* parent = nullptr, int size = 9);
    void add(QGraphicsLayout* item, int row, int col, Qt::Alignment alignment = Qt::Alignment());
    QList<QGraphicsWidget*> getInnerGridWidgets();
    QList<QGraphicsGridLayout*> getInnerGridLayouts();
    [[nodiscard]] std::size_t getSize() const;

public slots:
    void childGridGeometryChanged();

private:
    QGraphicsGridLayout* m_main_grid_layout;
    QList<QGraphicsGridLayout*> m_inner_grid_layouts;
    QList<QGraphicsWidget*> m_inner_grid_widgets;
    std::size_t m_size;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_BOARDCONTAINER_H
