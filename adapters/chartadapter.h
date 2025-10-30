#ifndef CHARTADAPTER_H
#define CHARTADAPTER_H

#include <QObject>
#include <QJSValue>
#include <QVariantList>
#include <QVariantMap>

class MainWindow;

/**
 * @brief Adapter class to provide charting and utility functions to JavaScript
 * 
 * The ChartAdapter provides utilities for data visualization and analysis
 * in JavaScript. It includes methods for creating text-based charts, tables,
 * and data manipulation utilities like sorting, filtering, and grouping.
 * 
 * @example
 * // Create a bar chart
 * var data = {"A": 10, "B": 25, "C": 15};
 * console.log(Charts.generateTextBarChart("My Chart", data));
 * 
 * // Sort events by frequency
 * var events = Events.getAllEvents();
 * var sorted = Charts.sortByProperty(events, "frecuencia", false);
 * 
 * // Group and count
 * var grouped = Charts.groupByProperty(events, "etapa");
 * var counts = Charts.countByProperty(events, "riesgo");
 */
class ChartAdapter : public QObject
{
    Q_OBJECT

public:
    explicit ChartAdapter(MainWindow *mainWindow, QObject *parent = nullptr);

public slots:
    /**
     * @brief Create a simple bar chart data structure
     * 
     * Creates a structured object suitable for programmatic chart generation.
     * 
     * @param title Chart title
     * @param data Object with labels as keys and numeric values as values
     * @return Chart data object with properties:
     *   - type (String): "bar"
     *   - title (String): Chart title
     *   - data (Object): Original data
     *   - labels (Array): Array of label strings
     *   - values (Array): Array of numeric values
     * 
     * @example
     * var data = {"Jan": 100, "Feb": 120, "Mar": 90};
     * var chart = Charts.createBarChart("Monthly Sales", data);
     * console.log(chart.labels); // ["Jan", "Feb", "Mar"]
     * console.log(chart.values); // [100, 120, 90]
     */
    QVariantMap createBarChart(const QString &title, const QVariantMap &data);

    /**
     * @brief Create a pie chart data structure
     * 
     * Creates a structured object with percentage calculations for each segment.
     * 
     * @param title Chart title
     * @param data Object with labels as keys and numeric values as values
     * @return Chart data object with properties:
     *   - type (String): "pie"
     *   - title (String): Chart title
     *   - data (Object): Original data
     *   - segments (Array): Array of segment objects with label, value, percentage
     *   - total (Number): Sum of all values
     * 
     * @example
     * var data = {"A": 30, "B": 45, "C": 25};
     * var pie = Charts.createPieChart("Distribution", data);
     * // pie.segments[0] = {label: "A", value: 30, percentage: 30}
     */
    QVariantMap createPieChart(const QString &title, const QVariantMap &data);

    /**
     * @brief Generate a text-based bar chart for console output
     * 
     * Creates a visual bar chart using text characters, perfect for
     * displaying data in the scripting console.
     * 
     * @param title Chart title
     * @param data Object with labels as keys and numeric values as values
     * @param maxWidth Maximum width for the chart bars in characters (default 50)
     * @return String representation of the chart with title, bars, and values
     * 
     * @example
     * var counts = Events.countEventsByStage();
     * var chart = Charts.generateTextBarChart("Events by Stage", counts, 40);
     * console.log(chart);
     * // Output:
     * // Events by Stage
     * // ===============
     * // Stage 1 | ████████████████ (16)
     * // Stage 2 | ████████████████████████ (24)
     */
    QString generateTextBarChart(const QString &title, const QVariantMap &data, int maxWidth = 50);

    /**
     * @brief Generate a simple table for console output
     * 
     * Creates a formatted ASCII table with automatic column width adjustment.
     * 
     * @param title Table title
     * @param headers Array of header strings
     * @param rows Array of arrays representing table rows (each row is an array of cell values)
     * @return String representation of the formatted table
     * 
     * @example
     * var headers = ["ID", "Name", "Risk"];
     * var rows = [
     *     [1, "Event A", "High"],
     *     [2, "Event B", "Medium"]
     * ];
     * console.log(Charts.generateTable("Events", headers, rows));
     */
    QString generateTable(const QString &title, const QVariantList &headers, const QVariantList &rows);

    /**
     * @brief Sort an array of objects by a specific property
     * 
     * @param array Array to sort (must be array of objects)
     * @param property Property name to sort by
     * @param ascending Sort order (default true for ascending)
     * @return New sorted array (original array is not modified)
     * 
     * @example
     * var events = Events.getAllEvents();
     * // Sort by frequency, descending
     * var sorted = Charts.sortByProperty(events, "frecuencia", false);
     * for (var i = 0; i < sorted.length; i++) {
     *     console.log(sorted[i].nombre + ": " + sorted[i].frecuencia);
     * }
     */
    QVariantList sortByProperty(const QVariantList &array, const QString &property, bool ascending = true);

    /**
     * @brief Group an array of objects by a specific property
     * 
     * Creates an object where keys are the unique values of the specified
     * property and values are arrays of objects having that property value.
     * 
     * @param array Array to group (must be array of objects)
     * @param property Property name to group by
     * @return Object with property values as keys and arrays of objects as values
     * 
     * @example
     * var events = Events.getAllEvents();
     * var grouped = Charts.groupByProperty(events, "etapa");
     * for (var stage in grouped) {
     *     console.log(stage + ": " + grouped[stage].length + " events");
     * }
     */
    QVariantMap groupByProperty(const QVariantList &array, const QString &property);

    /**
     * @brief Count occurrences of values for a specific property
     * 
     * Similar to groupByProperty but returns counts instead of arrays.
     * 
     * @param array Array to analyze (must be array of objects)
     * @param property Property name to count
     * @return Object with property values as keys and counts (Number) as values
     * 
     * @example
     * var events = Events.getAllEvents();
     * var counts = Charts.countByProperty(events, "riesgo");
     * console.log(Charts.generateTextBarChart("Events by Risk", counts));
     */
    QVariantMap countByProperty(const QVariantList &array, const QString &property);

    /**
     * @brief Filter an array of objects by property value
     * 
     * Returns a new array containing only objects where the specified
     * property equals the given value.
     * 
     * @param array Array to filter (must be array of objects)
     * @param property Property name to filter by
     * @param value Value to match (can be String, Number, Boolean, etc.)
     * @return New filtered array
     * 
     * @example
     * var events = Events.getAllEvents();
     * // Get only high-risk events
     * var highRisk = Charts.filterByProperty(events, "riesgo", 3);
     * console.log("High risk events: " + highRisk.length);
     * 
     * // Get events with human error
     * var humanError = Charts.filterByProperty(events, "errorHumano", true);
     */
    QVariantList filterByProperty(const QVariantList &array, const QString &property, const QVariant &value);

    /**
     * @brief Generate summary statistics for numeric property
     * 
     * Calculates min, max, average, sum, and count for a numeric property
     * across all objects in the array.
     * 
     * @param array Array to analyze (must be array of objects)
     * @param property Property name (must contain numeric values)
     * @return Object with statistics:
     *   - count (Number): Number of valid numeric values
     *   - sum (Number): Sum of all values
     *   - min (Number): Minimum value
     *   - max (Number): Maximum value
     *   - avg (Number): Average value
     * 
     * @example
     * var events = Events.getAllEvents();
     * var stats = Charts.getNumericStats(events, "frecuencia");
     * console.log("Frequency statistics:");
     * console.log("  Min: " + stats.min);
     * console.log("  Max: " + stats.max);
     * console.log("  Average: " + stats.avg.toFixed(2));
     * console.log("  Total events: " + stats.count);
     */
    QVariantMap getNumericStats(const QVariantList &array, const QString &property);

private:
    MainWindow *mainWindow;
    
    QString repeatChar(char ch, int count);
    QString padString(const QString &str, int width, bool leftAlign = true);
};

#endif // CHARTADAPTER_H