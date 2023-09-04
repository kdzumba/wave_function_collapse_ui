//
// Created by User on 2023/08/24.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_TILEDRULEMODEL_H
#define WAVE_FUNCTION_COLLAPSE_UI_TILEDRULEMODEL_H


#include <string>

class TiledRuleModel
{
public:
    TiledRuleModel(const std::string& left, const std::string& right, const std::string& up, const std::string& down);
    std::string m_right;
    std::string m_left;
    std::string m_up;
    std::string m_down;
public:
    bool operator==(const TiledRuleModel& other) const;
    bool operator!=(const TiledRuleModel& other) const;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_TILEDRULEMODEL_H
