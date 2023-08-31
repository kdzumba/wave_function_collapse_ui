//
// Created by User on 2023/08/17.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_UTILS_H
#define WAVE_FUNCTION_COLLAPSE_UI_UTILS_H

#include <QJsonObject>

namespace Utils
{
    int generate_random_int(int start, int end);
    double generate_random_double(int start, double end);
    double get_min_abs_half(const std::vector<double>& v);
    std::vector<double> get_plogp(const std::vector<double>& distribution);
}
#endif //WAVE_FUNCTION_COLLAPSE_UI_UTILS_H
