#include "chartadapter.h"
#include "../mainwindow.h"
#include <QDebug>
#include <algorithm>
#include <limits>

ChartAdapter::ChartAdapter(MainWindow *mainWindow, QObject *parent)
    : QObject(parent), mainWindow(mainWindow)
{
}

QVariantMap ChartAdapter::createBarChart(const QString &title, const QVariantMap &data)
{
    QVariantMap chart;
    chart["type"] = "bar";
    chart["title"] = title;
    chart["data"] = data;
    
    QVariantList labels;
    QVariantList values;
    
    for (auto it = data.begin(); it != data.end(); ++it) {
        labels.append(it.key());
        values.append(it.value());
    }
    
    chart["labels"] = labels;
    chart["values"] = values;
    
    return chart;
}

QVariantMap ChartAdapter::createPieChart(const QString &title, const QVariantMap &data)
{
    QVariantMap chart;
    chart["type"] = "pie";
    chart["title"] = title;
    chart["data"] = data;
    
    QVariantList segments;
    int total = 0;
    
    // Calculate total
    for (auto it = data.begin(); it != data.end(); ++it) {
        total += it.value().toInt();
    }
    
    // Create segments with percentages
    for (auto it = data.begin(); it != data.end(); ++it) {
        QVariantMap segment;
        segment["label"] = it.key();
        segment["value"] = it.value();
        segment["percentage"] = total > 0 ? (it.value().toDouble() / total * 100) : 0;
        segments.append(segment);
    }
    
    chart["segments"] = segments;
    chart["total"] = total;
    
    return chart;
}

QString ChartAdapter::generateTextBarChart(const QString &title, const QVariantMap &data, int maxWidth)
{
    QString result;
    result += "\n" + title + "\n";
    result += repeatChar('=', title.length()) + "\n\n";
    
    if (data.isEmpty()) {
        result += "No hay datos para mostrar.\n";
        return result;
    }
    
    // Find maximum value and label width
    int maxValue = 0;
    int maxLabelWidth = 0;
    
    for (auto it = data.begin(); it != data.end(); ++it) {
        int value = it.value().toInt();
        if (value > maxValue) maxValue = value;
        if (it.key().length() > maxLabelWidth) maxLabelWidth = it.key().length();
    }
    
    // Generate bars
    for (auto it = data.begin(); it != data.end(); ++it) {
        QString label = padString(it.key(), maxLabelWidth);
        int value = it.value().toInt();
        int barLength = maxValue > 0 ? (value * maxWidth / maxValue) : 0;
        
        QString bar = repeatChar('█', barLength);
        result += QString("%1 | %2 (%3)\n").arg(label).arg(bar).arg(value);
    }
    
    result += "\n";
    return result;
}

QString ChartAdapter::generateTable(const QString &title, const QVariantList &headers, const QVariantList &rows)
{
    QString result;
    result += "\n" + title + "\n";
    result += repeatChar('=', title.length()) + "\n\n";
    
    if (headers.isEmpty() || rows.isEmpty()) {
        result += "No hay datos para mostrar.\n";
        return result;
    }
    
    // Calculate column widths
    QList<int> colWidths;
    for (int i = 0; i < headers.size(); i++) {
        colWidths.append(headers[i].toString().length());
    }
    
    // Check row data for wider columns
    for (const auto &rowVar : rows) {
        QVariantList row = rowVar.toList();
        for (int i = 0; i < row.size() && i < colWidths.size(); i++) {
            int width = row[i].toString().length();
            if (width > colWidths[i]) {
                colWidths[i] = width;
            }
        }
    }
    
    // Generate header
    QString headerRow = "|";
    QString separator = "|";
    for (int i = 0; i < headers.size(); i++) {
        headerRow += " " + padString(headers[i].toString(), colWidths[i]) + " |";
        separator += repeatChar('-', colWidths[i] + 2) + "|";
    }
    
    result += headerRow + "\n";
    result += separator + "\n";
    
    // Generate rows
    for (const auto &rowVar : rows) {
        QVariantList row = rowVar.toList();
        QString rowStr = "|";
        for (int i = 0; i < colWidths.size(); i++) {
            QString cellValue = (i < row.size()) ? row[i].toString() : "";
            rowStr += " " + padString(cellValue, colWidths[i]) + " |";
        }
        result += rowStr + "\n";
    }
    
    result += "\n";
    return result;
}

QVariantList ChartAdapter::sortByProperty(const QVariantList &array, const QString &property, bool ascending)
{
    QVariantList result = array;
    
    std::sort(result.begin(), result.end(), [property, ascending](const QVariant &a, const QVariant &b) {
        QVariantMap mapA = a.toMap();
        QVariantMap mapB = b.toMap();
        
        QVariant valueA = mapA[property];
        QVariant valueB = mapB[property];
        
        if (ascending) {
            return valueA.toString() < valueB.toString();
        } else {
            return valueA.toString() > valueB.toString();
        }
    });
    
    return result;
}

QVariantMap ChartAdapter::groupByProperty(const QVariantList &array, const QString &property)
{
    QVariantMap result;
    
    for (const auto &itemVar : array) {
        QVariantMap item = itemVar.toMap();
        QString key = item[property].toString();
        
        if (!result.contains(key)) {
            result[key] = QVariantList();
        }
        
        QVariantList list = result[key].toList();
        list.append(item);
        result[key] = list;
    }
    
    return result;
}

QVariantMap ChartAdapter::countByProperty(const QVariantList &array, const QString &property)
{
    QVariantMap result;
    
    for (const auto &itemVar : array) {
        QVariantMap item = itemVar.toMap();
        QString key = item[property].toString();
        result[key] = result[key].toInt() + 1;
    }
    
    return result;
}

QVariantList ChartAdapter::filterByProperty(const QVariantList &array, const QString &property, const QVariant &value)
{
    QVariantList result;
    
    for (const auto &itemVar : array) {
        QVariantMap item = itemVar.toMap();
        if (item[property] == value) {
            result.append(item);
        }
    }
    
    return result;
}

QVariantMap ChartAdapter::getNumericStats(const QVariantList &array, const QString &property)
{
    QVariantMap result;
    
    if (array.isEmpty()) {
        result["count"] = 0;
        return result;
    }
    
    double sum = 0;
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
    int count = 0;
    
    for (const auto &itemVar : array) {
        QVariantMap item = itemVar.toMap();
        bool ok = false;
        double value = item[property].toDouble(&ok);
        
        if (ok) {
            sum += value;
            count++;
            if (value < min) min = value;
            if (value > max) max = value;
        }
    }
    
    result["count"] = count;
    result["sum"] = sum;
    result["min"] = count > 0 ? min : 0;
    result["max"] = count > 0 ? max : 0;
    result["avg"] = count > 0 ? (sum / count) : 0;
    
    return result;
}

QString ChartAdapter::repeatChar(char ch, int count)
{
    if (count <= 0) return "";
    return QString(count, ch);
}

QString ChartAdapter::padString(const QString &str, int width, bool leftAlign)
{
    if (str.length() >= width) return str;
    
    int padding = width - str.length();
    if (leftAlign) {
        return str + repeatChar(' ', padding);
    } else {
        return repeatChar(' ', padding) + str;
    }
}