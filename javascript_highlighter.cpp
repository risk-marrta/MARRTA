#include "javascript_highlighter.h"

JavaScriptHighlighter::JavaScriptHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    HighlightingRule rule;

    // Keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns = {"\\bvar\\b", "\\bfunction\\b", "\\breturn\\b", "\\bif\\b", "\\belse\\b", "\\bfor\\b", "\\bwhile\\b", "\\bdo\\b", "\\bswitch\\b", "\\bcase\\b", "\\bdefault\\b", "\\bcontinue\\b", "\\bbreak\\b", "\\bnew\\b", "\\bdelete\\b", "\\btry\\b", "\\bcatch\\b", "\\bfinally\\b", "\\bthrow\\b", "\\btypeof\\b", "\\binstanceof\\b", "\\bvoid\\b", "\\bwith\\b", "\\bthis\\b", "\\bclass\\b", "\\bextends\\b", "\\bconstructor\\b", "\\bsuper\\b", "\\bimport\\b", "\\bexport\\b", "\\bfrom\\b", "\\bas\\b", "\\blet\\b", "\\bconst\\b"};
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Single-line comments
    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("//[^\\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Multi-line comments
    QTextCharFormat multiLineCommentFormat;
    multiLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

    // Strings
    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegularExpression("\".*\"|'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Numbers
    QTextCharFormat numberFormat;
    numberFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\b[0-9]+(\\.[0-9]+)?\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Function names
    QTextCharFormat functionFormat;
    functionFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Operators
    QTextCharFormat operatorFormat;
    operatorFormat.setForeground(Qt::darkYellow);
    QStringList operatorPatterns = {"\\+", "-", "\\*", "/", "=", "==", "===", "!=", "!==", "<", "<=", ">", ">=", "&&", "\\|\\|", "!", "\\?", ":", "\\+=", "-=", "\\*=", "/=", "%="};
    foreach (const QString &pattern, operatorPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = operatorFormat;
        highlightingRules.append(rule);
    }
    
    // Braces, brackets, and parentheses highlighting
    QTextCharFormat bracesFormat;
    bracesFormat.setForeground(Qt::darkGray);
    bracesFormat.setFontWeight(QFont::Bold);
    QStringList bracesPatterns = {"\\{", "\\}", "\\[", "\\]", "\\(", "\\)"};
    foreach (const QString &pattern, bracesPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = bracesFormat;
        highlightingRules.append(rule);
    }
    
}

void JavaScriptHighlighter::highlightBlock(const QString &text) {
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
} 