#ifndef PYTHONSCRIPTINGDOCUMENTATION_H
#define PYTHONSCRIPTINGDOCUMENTATION_H

#include <QDialog>
#include <QTreeWidget>
#include <QTextBrowser>

/**
 * @brief Documentation window for Python scripting API
 * 
 * Provides comprehensive documentation and examples for the Python scripting API,
 * including the Marrta base object, Database access, and Etapas with Etapa.Imprimir() method.
 */
class PythonScriptingDocumentation : public QDialog
{
    Q_OBJECT

public:
    explicit PythonScriptingDocumentation(QWidget *parent = nullptr);
    ~PythonScriptingDocumentation();

signals:
    void insertExample(const QString &example);

private slots:
    void onTreeItemClicked(QTreeWidgetItem *item, int column);
    void onInsertExampleClicked();

private:
    QTreeWidget *treeWidget;
    QTextBrowser *docBrowser;
    QPushButton *insertButton;
    
    void setupUI();
    void populateDocumentation();
    QString getDocumentationHTML();
};

#endif // PYTHONSCRIPTINGDOCUMENTATION_H
