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
    for(double weight_log_weight : v)
    {
        min_abs_half = std::min(min_abs_half, std::abs(weight_log_weight / 2.0));
    }
    return min_abs_half;
}

std::vector<double> Utils::get_weight_log_weight(const std::vector<double> &pattern_weights)
{
    std::vector<double> weight_log_weight;
    for(double weight : pattern_weights)
    {
        weight_log_weight.emplace_back(weight * log(weight));
    }
    return weight_log_weight;
}

const std::vector<double>& Utils::normalize(std::vector<double> &v)
{
    double sum_weights = 0.0;
    for(auto weight : v)
    {
        sum_weights += weight;
    }
    double inverse_sum_weights = 1.0/sum_weights;
    for(auto& weight : v)
    {
        weight *= inverse_sum_weights;
    }
    return v;
}

