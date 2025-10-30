#include "dbexception.h"

namespace Database {

DbException::DbException(QString message)
{
    _message = message;
}

QString DbException::message()
{
    return _message;
}

}
