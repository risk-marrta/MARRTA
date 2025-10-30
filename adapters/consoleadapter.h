#ifndef CONSOLEADAPTER_H
#define CONSOLEADAPTER_H

#include <QObject>
#include <QPlainTextEdit>

class ConsoleAdapter: public QObject
{
    Q_OBJECT
public:
    ConsoleAdapter(QPlainTextEdit *);
    //virtual ~ConsoleAdapter() = 0;

signals:

public slots:
    void log(QString msg);

private:
    QPlainTextEdit *destiny;
};

#endif // CONSOLEADAPTER_H
