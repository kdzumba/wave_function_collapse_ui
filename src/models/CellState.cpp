//
// Created by User on 2023/08/22.
//

#include "CellState.h"

CellState::CellState(int index, QPixmap* pixmap) : m_index(index), m_pixmap(pixmap)
{}

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
