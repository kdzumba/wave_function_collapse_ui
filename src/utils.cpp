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

QJsonObject Utils::load_style(const QString& path)
{
    QDir directory(path);
    QStringList files = directory.entryList(QStringList() << "*.json", QDir::Files);
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = document.object();
    return jsonObject;
}

