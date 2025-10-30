#include "consoleadapter.h"
#include <QDebug>

ConsoleAdapter::ConsoleAdapter(QPlainTextEdit *d): QObject(0)
{
    destiny = d;
}

//ConsoleAdapter::~ConsoleAdapter() {}

void ConsoleAdapter::log(QString msg)
{
    destiny->appendPlainText(msg);
}
