#ifndef JAVASCRIPT_HIGHLIGHTER_H
#define JAVASCRIPT_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class JavaScriptHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    JavaScriptHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    
    // Add brace highlighting format
    QTextCharFormat braceFormat;
};

#endif // JAVASCRIPT_HIGHLIGHTER_H 