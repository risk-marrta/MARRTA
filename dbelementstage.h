#ifndef DBELEMENTETAPAS_H
#define DBELEMENTETAPAS_H

#include <unordered_map>
#include "dbelement.h"
#include "completeeventdefinition.h"
#include "dbexception.h"

#include "etapas.h"

struct Summary {
    QString name;
    int orden;
    int rma;
    int ra;
    int rm;
    int rb;

    bool operator<(const Summary& b){ return this->orden < b.orden; }
};

namespace Database {
    struct Orders{
        std::string name;
        std::string code;
        int order;
        int max;
    };

    class DbElementStage: public DbElement<EtapaDefinition>
    {
    public:
        DbElementStage();

        ~DbElementStage() {}

        /**
         * @brief getNextAvailableCode devuelve el próximo código disponible para la etapa indicada
         * @param stageId
         * @return
         */
        static QString getNextAvailableCode(int stageId);

        /**
         * @brief getNextAvailableDefenseCode
         * @return
         */
        static int getNextAvailableDefenseCode(QString);

        /**
         * @brief list Lista las definiciones de las etapas
         * @param elementList
         * @param todasLasEtapas Si es verdadero listará también las etapas virtuales (id < 0)
         */
        void list(QList<EtapaDefinition>& elementList, bool todasLasEtapas = false);

        static std::map<int, Database::Orders> listNamesCodes();

        /**
         * @brief ordenCodigoEtapa devuelve el próximo orden y código de una etapa
         * @return
         */
        QPair<int, QString> static ordenCodigoEtapa(int);

        void update(QList<EtapaDefinition>&, bool);

        std::vector<Summary> summary(std::unordered_map<std::string, std::vector<CompleteEventDefinition> > *eventsInStages);

    public:
        static const std::vector<QString> HeaderTitles;
    };
}

#endif // DBELEMENTETAPAS_H
