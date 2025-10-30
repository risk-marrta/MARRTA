#ifndef PYTHON_HIGHLIGHTER_H
#define PYTHON_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

/**
 * @brief Syntax highlighter for Python code
 * 
 * Provides syntax highlighting for the Python scripting editor,
 * including keywords, strings, comments, numbers, and built-in functions.
 */
class PythonHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit PythonHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat builtinFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat decoratorFormat;
};

#endif // PYTHON_HIGHLIGHTER_H
