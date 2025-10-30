#ifndef SCRIPTINGDOCUMENTATION_H
#define SCRIPTINGDOCUMENTATION_H

#include <QDialog>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QPushButton>
#include <QSplitter>

class ScriptingDocumentation : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptingDocumentation(QWidget *parent = nullptr);

private slots:
    void onSectionChanged();
    void onExampleInserted();

signals:
    void insertExample(const QString &example);

private:
    void setupUI();
    void populateDocumentation();
    void addDocumentationSection(const QString &title, const QString &content);
    void addExampleSection(const QString &title, const QString &code, const QString &description);
    
    QSplitter *splitter;
    QTreeWidget *sectionTree;
    QTextBrowser *contentBrowser;
    QPushButton *insertButton;
    QPushButton *closeButton;
    
    struct DocumentationItem {
        QString title;
        QString content;
        QString code;
        bool isExample;
    };
    
    QList<DocumentationItem> documentationItems;
};

#endif // SCRIPTINGDOCUMENTATION_H