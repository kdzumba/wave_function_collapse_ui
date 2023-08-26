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
    explicit CellState(int index, const std::string& name, const std::string& symmetry, int orientation = 0, QPixmap* pixmap = nullptr);
    bool operator==(const CellState& other) const;
    bool operator!=(const CellState& other) const;
    int index() const;
    const std::string& get_name();
    const std::string& get_symmetry();
    int get_orientation() const;
    void set_orientation(int orientation);
    [[nodiscard]] const QPixmap* pixmap() const;
    void set_index(int index);
    void set_pixmap(QPixmap* pixmap);
public:
    static const std::map<std::string, int> SYMMETRY_CARDINALITY_MAP;
private:
    int m_index;
    QPixmap* m_pixmap;
    std::string m_name;
    std::string m_symmetry;
    int m_orientation;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_CELLSTATE_H
