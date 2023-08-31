//

#include "CellState.h"

//
// Created by User on 2023/08/22.

/**
 *
 */
const std::map<std::string, int> CellState::symmetry_cardinality_map = {
        {"L", 4},
        {"T", 4},
        {"I", 2},
        {"\\",2},
        {"F", 8},
        {"X", 1}
};

const std::map<std::string, std::vector<int>> CellState::symmetry_rotation_map = {
        {"L", {1, 2, 3, 0}},
        {"I", {1, 0}},
        {"X", {0}},
        {"T", {1, 2, 3, 0}},
        {"\\", {1, 0}}
};

const std::map<std::string, std::vector<int>> CellState::symmetry_reflection_map = {
        {"L", {1, 0, 3, 2}},
        {"I", {0, 1}},
        {"X", {0}},
        {"T", {0, 3, 2, 1}},
        {"\\", {1, 0}}
};

CellState::CellState(int index, QPixmap* pixmap) : m_index(index), m_pixmap(pixmap)
{}

CellState::CellState(int index, const std::string &name,const  std::string &symmetry, int orientation, double weight, QPixmap *pixmap):
        CellState{index, pixmap}
{
    m_name = name;
    m_symmetry = symmetry;
    m_orientation = orientation;
    m_weight = weight;
}

bool CellState::operator==(const CellState &other) const
{
    return this -> m_index == other.m_index;
}

bool CellState::operator!=(const CellState &other) const
{
    return this -> m_index != other.m_index;
}

int CellState::index() const {return m_index; }

const QPixmap *CellState::pixmap() const
{
    return m_pixmap;
}

void CellState::set_pixmap(QPixmap *pixmap)
{
    m_pixmap = pixmap;
}

void CellState::set_index(int index)
{
    m_index = index;
}

const std::string &CellState::get_name()
{
    return m_name;
}

const std::string &CellState::get_symmetry()
{
    return m_symmetry;
}

int CellState::get_orientation() const
{
    return m_orientation;
}

void CellState::set_orientation(int orientation)
{
    m_orientation = orientation;
}

void CellState::set_weight(double weight)
{
    m_weight = weight;
}

double CellState::get_weight() const
{
    return m_weight;
}
