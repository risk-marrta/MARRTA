#ifndef DBEVENT_H
#define DBEVENT_H

#include <unordered_map>
#include "dbelement.h"
#include "eventdefinition.h"
#include "completeeventdefinition.h"
#include "matrizriesgo.h"

namespace Database {
    class DbEvent: public DbElement<EventDefinition>
    {
    public:
        DbEvent(): DbElement<EventDefinition>("suceso_iniciador") {
        }

        ~DbEvent() {}

        void getAllInfo(const MatrizRiesgo &matriz, std::unordered_map< std::string, std::vector< CompleteEventDefinition > > &events);

        QVariant create(EventDefinition &event);

        void list(QList<EventDefinition>& elementList, int stage, int substage = 0);
        void listAllForStage(QList<EventDefinition>& elementList, int stage);
        QStringList listCodesFromEventsWithStage(int stage);

        static void setCompleted(int eventId, bool completed, int riesgo);

        /**
         * @brief updateRisk Update only the risk value for an event
         * @param eventId The ID of the event
         * @param riesgo The new risk value
         */
        static void updateRisk(int eventId, int riesgo);

        static bool remove(EventDefinition&);
        void update(QList<EventDefinition>&, bool);
        /**
         * @brief updateElement actualiza el evento en la base de datos
         */
        static void updateElement(EventDefinition& );

        void checkAndUpdate(const std::unordered_map<std::string, std::vector<CompleteEventDefinition> > & events);
    };
}

Q_DECLARE_METATYPE(Database::DbEvent)

#endif // DBEVENT_H
