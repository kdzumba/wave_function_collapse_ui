//
// Created by User on 2023/08/17.
//

#ifndef WAVE_FUNCTION_COLLAPSE_UI_UTILS_H
#define WAVE_FUNCTION_COLLAPSE_UI_UTILS_H

#include <QJsonObject>

namespace Utils
{
    int generate_random_int(int start, int end);
    QJsonObject load_style(const QString& filename);
}
#endif //WAVE_FUNCTION_COLLAPSE_UI_UTILS_H
