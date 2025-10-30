#include "defenseitemdelegate.h"
#include <QApplication>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

DefenseItemDelegate::DefenseItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

// Helper method to split defense display text into left and right parts
QPair<QString, QString> DefenseItemDelegate::splitDefenseDisplayText(const QString &fullText) const
{
    int lastBracket = fullText.lastIndexOf('[');
    QString leftPart = fullText;
    QString rightPart;
    if (lastBracket > 0) {
        leftPart = fullText.left(lastBracket).trimmed();
        rightPart = fullText.mid(lastBracket);
    }
    return qMakePair(leftPart, rightPart);
}

void DefenseItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    
    // Get the full text
    QString fullText = index.data(Qt::DisplayRole).toString();
    
    // Split into left and right parts using helper
    auto parts = splitDefenseDisplayText(fullText);
    QString leftPart = parts.first;
    QString rightPart = parts.second;
    
    // Draw the background and selection only (not the text)
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    
    // Remove the text from the option to prevent default text drawing
    opt.text = QString();
    opt.features &= ~QStyleOptionViewItem::HasDisplay;
    
    // Draw background, selection, and focus with system colors
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
    
    // Calculate text area (accounting for checkbox if present)
    // Use the style's subElementRect to get the proper content area
    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, opt.widget);
    if (textRect.isNull()) {
        // Fallback if subElementRect doesn't work
        textRect = opt.rect;
        if (opt.features & QStyleOptionViewItem::HasCheckIndicator) {
            int checkBoxWidth = style->pixelMetric(QStyle::PM_IndicatorWidth, &opt, opt.widget);
            int spacing = style->pixelMetric(QStyle::PM_CheckBoxLabelSpacing, &opt, opt.widget);
            textRect.setLeft(textRect.left() + checkBoxWidth + spacing);
        }
    }
    
    // Add some padding
    textRect.adjust(2, 2, -2, -2);
    
    painter->save();
    
    // Use normal font (not bold)
    QFont font = opt.font;
    font.setBold(false);
    painter->setFont(font);
    
    // Calculate width for hardiness text
    QFontMetrics fm(font);
    int hardnessWidth = rightPart.isEmpty() ? 0 : fm.horizontalAdvance(rightPart) + 8; // 8 pixels spacing
    
    // Always use normal text color - let only background show selection
    // This ensures text is readable on both light and dark selection backgrounds
    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active)) {
        cg = QPalette::Inactive;
    }
    
    painter->setPen(opt.palette.color(cg, QPalette::Text));
    
    // Draw left part (code and name) with wrapping, leaving space for hardiness
    QRect leftRect = textRect;
    leftRect.setRight(leftRect.right() - hardnessWidth);
    painter->drawText(leftRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, leftPart);
    
    // Draw right part (hardiness) - right aligned, vertically centered to first line
    if (!rightPart.isEmpty()) {
        QRect rightRect = textRect;
        rightRect.setLeft(rightRect.right() - hardnessWidth + 4);
        painter->drawText(rightRect, Qt::AlignRight | Qt::AlignVCenter, rightPart);
    }
    
    painter->restore();
}

QSize DefenseItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    
    // Get the full text
    QString fullText = index.data(Qt::DisplayRole).toString();
    
    // Extract parts: [code] Name [Hardiness]
    int lastBracket = fullText.lastIndexOf('[');
    QString leftPart = fullText;
    QString rightPart;
    
    if (lastBracket > 0) {
        leftPart = fullText.left(lastBracket).trimmed();
        rightPart = fullText.mid(lastBracket);
    }
    
    // Use normal font
    QFont font = opt.font;
    font.setBold(false);
    QFontMetrics fm(font);
    
    // Calculate width for hardiness
    int hardnessWidth = rightPart.isEmpty() ? 0 : fm.horizontalAdvance(rightPart) + 8;
    
    // Calculate available width for left text
    int availableWidth = option.rect.width() - hardnessWidth - 8; // 8 for padding
    
    // Calculate height needed for wrapped text
    QRect boundingRect = fm.boundingRect(0, 0, availableWidth, 1000, 
                                         Qt::AlignLeft | Qt::TextWordWrap, leftPart);
    
    // Return size with some padding
    int height = qMax(boundingRect.height() + 8, fm.height() + 8); // At least one line height
    return QSize(option.rect.width(), height);
}
