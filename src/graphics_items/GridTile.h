//
// Created by User on 2023/08/15.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_GRIDTILE_H
#define WAVE_FUNCTION_COLLAPSE_UI_GRIDTILE_H

#include <QGraphicsItem>
#include <QLabel>
#include <QWidget>
#include <QRectF>
#include <QPainter>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QJsonObject>
#include <QGraphicsLayoutItem>

class GridTile : public QGraphicsLayoutItem, public QGraphicsItem
{
    struct GridTileStyle
    {
        QString tile_hovered_color;
        QString tile_hovered_font_color;
        QString tile_unhovered_font_color;
    };

public:
    GridTile(const QString& value, int x, int y, bool is_permanently_collapsed);
    [[nodiscard]] QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem * option, QWidget* widget) override;
    void setGeometry(const QRectF& geometry) override;
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const override;
public:
    static int constexpr TILE_SIZE = 75;
    static int constexpr POSITION_OFFSET = 10;
    static int constexpr PEN_WIDTH = 3;
private:
    QLabel* m_value;
    GridTileStyle m_style;
    std::pair<int, int> m_position;
    bool m_is_permanently_collapsed;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_GRIDTILE_H
