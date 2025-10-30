#ifndef DBEXCEPTION_H
#define DBEXCEPTION_H

#include <QException>
#include <QString>

namespace Database {


class DbException : public QException
{
private:
    QString _message;

public:
    void raise() const override { throw *this; }
    DbException *clone() const override { return new DbException(*this); }

    DbException(QString message);

    QString message();
};

}

#endif // DBEXCEPTION_H
