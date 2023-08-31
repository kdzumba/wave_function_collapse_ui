//
// Created by User on 2023/08/17.
//

#include <random>
#include "utils.h"
#include <QDir>
#include <QJsonDocument>
#include <QStringList>
#include <QFile>
#include <QByteArray>

/**
 * Generates a random integer in the range [start, end]
 * @param start The minimum integer that can be generated
 * @param end The maximum integer that can be generated
 * @return A random integer in the range [start, end]
 */
int Utils::generate_random_int(int start, int end)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(start, end);
    return distribution(generator);
}

double Utils::generate_random_double(int start, double end)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> distribution(start, end);
    return distribution(generator);
}

double Utils::get_min_abs_half(const std::vector<double> &v)
{
    double min_abs_half = std::numeric_limits<double>::infinity();
    for(double i : v)
    {
        min_abs_half = std::min(min_abs_half, std::abs(i / 2.0));
    }
    return min_abs_half;
}

std::vector<double> Utils::get_plogp(const std::vector<double> &distribution)
{
    std::vector<double> plogp;
    for(double i : distribution)
    {
        plogp.emplace_back(i * log(i));
    }
    return plogp;
}

