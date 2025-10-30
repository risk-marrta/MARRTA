#ifndef HTMLPREVIEWHELPER_H
#define HTMLPREVIEWHELPER_H

#include <QObject>

class QDialog;
class QWebEngineView;
class QPushButton;

class HtmlPreviewHelper : public QObject
{
    Q_OBJECT

public:
    static HtmlPreviewHelper &instance();

    void showPreview(const QString &html, QWidget *parent);

private:
    explicit HtmlPreviewHelper(QObject *parent = nullptr);
    void initializeUi();
    void ensureDevTools();
    void restoreFocus();
    void saveCurrentCSS();
    void importCSS();
    void resetCSS();

    QDialog *previewDialog;
    QWebEngineView *previewView;
    QDialog *devToolsDialog;
    QWebEngineView *devToolsView;
    QPushButton *inspectButton;
    QPushButton *saveButton;
    QPushButton *saveStylesButton;
    QPushButton *importStylesButton;
    QPushButton *resetStylesButton;
    QPushButton *closeButton;
    QString currentHtml;
    QWidget *currentParent;
};

#endif // HTMLPREVIEWHELPER_H
