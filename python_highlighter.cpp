#include "python_highlighter.h"

PythonHighlighter::PythonHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Keywords
    keywordFormat.setForeground(QColor(86, 156, 214)); // Blue
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns = {
        "\\bFalse\\b", "\\bNone\\b", "\\bTrue\\b", "\\band\\b", "\\bas\\b",
        "\\bassert\\b", "\\basync\\b", "\\bawait\\b", "\\bbreak\\b", "\\bclass\\b",
        "\\bcontinue\\b", "\\bdef\\b", "\\bdel\\b", "\\belif\\b", "\\belse\\b",
        "\\bexcept\\b", "\\bfinally\\b", "\\bfor\\b", "\\bfrom\\b", "\\bglobal\\b",
        "\\bif\\b", "\\bimport\\b", "\\bin\\b", "\\bis\\b", "\\blambda\\b",
        "\\bnonlocal\\b", "\\bnot\\b", "\\bor\\b", "\\bpass\\b", "\\braise\\b",
        "\\breturn\\b", "\\btry\\b", "\\bwhile\\b", "\\bwith\\b", "\\byield\\b"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Built-in functions
    builtinFormat.setForeground(QColor(220, 220, 170)); // Yellow
    QStringList builtinPatterns = {
        "\\babs\\b", "\\ball\\b", "\\bany\\b", "\\bbin\\b", "\\bbool\\b",
        "\\bbytearray\\b", "\\bbytes\\b", "\\bchr\\b", "\\bdict\\b", "\\bdir\\b",
        "\\bdivmod\\b", "\\benumerate\\b", "\\beval\\b", "\\bfilter\\b", "\\bfloat\\b",
        "\\bformat\\b", "\\bfrozenset\\b", "\\bgetattr\\b", "\\bhasattr\\b", "\\bhex\\b",
        "\\bid\\b", "\\binput\\b", "\\bint\\b", "\\bisinstance\\b", "\\bissubclass\\b",
        "\\biter\\b", "\\blen\\b", "\\blist\\b", "\\bmap\\b", "\\bmax\\b",
        "\\bmin\\b", "\\bnext\\b", "\\bobject\\b", "\\boct\\b", "\\bopen\\b",
        "\\bord\\b", "\\bpow\\b", "\\bprint\\b", "\\bproperty\\b", "\\brange\\b",
        "\\brepr\\b", "\\breversed\\b", "\\bround\\b", "\\bset\\b", "\\bsetattr\\b",
        "\\bslice\\b", "\\bsorted\\b", "\\bstr\\b", "\\bsum\\b", "\\bsuper\\b",
        "\\btuple\\b", "\\btype\\b", "\\bzip\\b"
    };
    for (const QString &pattern : builtinPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = builtinFormat;
        highlightingRules.append(rule);
    }

    // Numbers
    numberFormat.setForeground(QColor(181, 206, 168)); // Light green
    rule.pattern = QRegularExpression("\\b[0-9]+\\.?[0-9]*\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Operators
    operatorFormat.setForeground(QColor(212, 212, 212)); // Light grey
    rule.pattern = QRegularExpression("[+\\-*/=%<>!&|^~]");
    rule.format = operatorFormat;
    highlightingRules.append(rule);

    // Decorators
    decoratorFormat.setForeground(QColor(220, 220, 170)); // Yellow
    decoratorFormat.setFontItalic(true);
    rule.pattern = QRegularExpression("@\\w+");
    rule.format = decoratorFormat;
    highlightingRules.append(rule);

    // Strings (double quotes)
    stringFormat.setForeground(QColor(206, 145, 120)); // Orange-brown
    rule.pattern = QRegularExpression("\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Strings (single quotes)
    rule.pattern = QRegularExpression("'[^'\\\\]*(\\\\.[^'\\\\]*)*'");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Triple-quoted strings
    rule.pattern = QRegularExpression("\"\"\"[^\"]*\"\"\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("'''[^']*'''");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Comments
    commentFormat.setForeground(QColor(106, 153, 85)); // Green
    commentFormat.setFontItalic(true);
    rule.pattern = QRegularExpression("#[^\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);
}

void PythonHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
