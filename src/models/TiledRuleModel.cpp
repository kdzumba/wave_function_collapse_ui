//
// Created by User on 2023/08/24.
//

#include "TiledRuleModel.h"

TiledRuleModel::TiledRuleModel(const std::string &left, const std::string &right, const std::string& up, const std::string& down)
{
    m_left = left;
    m_right = right;
    m_up = up;
    m_down = down;
}

bool TiledRuleModel::operator==(const TiledRuleModel &other) const {
    return m_left == other.m_left && m_right == other.m_right && m_up==other.m_up && m_down== other.m_down;
}

bool TiledRuleModel::operator!=(const TiledRuleModel &other) const {
    return !(*this == other);
}
