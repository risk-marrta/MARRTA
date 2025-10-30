#ifndef USERMANUALDOCUMENTATION_H
#define USERMANUALDOCUMENTATION_H

#include <QDialog>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QPushButton>
#include <QSplitter>

/**
 * @brief User Manual Documentation Dialog
 * 
 * Provides comprehensive user manual documentation for MARRTA application,
 * including general introduction, screen descriptions, analysis process,
 * menu options, and more.
 */
class UserManualDocumentation : public QDialog
{
    Q_OBJECT

public:
    explicit UserManualDocumentation(QWidget *parent = nullptr);

private slots:
    void onSectionChanged();

private:
    void setupUI();
    void populateDocumentation();
    void addDocumentationSection(const QString &title, const QString &content);
    
    QSplitter *splitter;
    QTreeWidget *sectionTree;
    QTextBrowser *contentBrowser;
    QPushButton *closeButton;
    
    struct DocumentationItem {
        QString title;
        QString content;
    };
    
    QList<DocumentationItem> documentationItems;
};

#endif // USERMANUALDOCUMENTATION_H
