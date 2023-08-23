//
// Created by User on 2023/08/22.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_CELLSTATE_H
#define WAVE_FUNCTION_COLLAPSE_UI_CELLSTATE_H


#include <QPixmap>

class CellState
{
public:
    explicit CellState(int index, QPixmap* pixmap = nullptr);
    bool operator==(const CellState& other) const;
    bool operator!=(const CellState& other) const;
    int index() const;
    const QPixmap* pixmap() const;
    void set_index(int index);
    void set_pixmap(QPixmap* pixmap);
private:
    int m_index;
    QPixmap* m_pixmap;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_CELLSTATE_H
