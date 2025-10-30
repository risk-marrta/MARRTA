#ifndef DBELEMENTSUBSTAGE_H
#define DBELEMENTSUBSTAGE_H

#include "dbelement.h"

#include "subetapas.h"

namespace Database {

class DbElementSubstage : public DbElement<SubetapaDefinition>
{
private:
    int _parentId;

public:
    DbElementSubstage(int parentId): DbElement<SubetapaDefinition>("subetapas"), _parentId(parentId) {
    }

    ~DbElementSubstage() {}

    void list(QList<SubetapaDefinition>& elementList);

    static void listAll(QList<SubetapaDefinition> *elementList);

    void update(QList<SubetapaDefinition>&, bool);
};
}

#endif // DBELEMENTSUBSTAGE_H
