//
// Created by User on 2023/08/24.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_TILEDMODELRULE_H
#define WAVE_FUNCTION_COLLAPSE_UI_TILEDMODELRULE_H


#include <string>

class TiledModelRule
{
public:
    TiledModelRule(const std::string& left, const std::string& right);
    std::string m_right;
    std::string m_left;
};


#endif //WAVE_FUNCTION_COLLAPSE_UI_TILEDMODELRULE_H
