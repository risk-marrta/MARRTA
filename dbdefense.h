#ifndef DBDEFENSE_H
#define DBDEFENSE_H

#include "dbelement.h"
#include "defensedefinition.h"

namespace Database {
    class DbDefense: public DbElement<DefenseDefinition>
    {
    public:
        DbDefense(): DbElement<DefenseDefinition>("defensas") {}

        ~DbDefense() {}

        void create(DefenseDefinition &event);

        /**
         * @brief list Devuelve en elementList la lista de defensas según el tipo para el evento indicado
         * @param elementList
         * @param _eventId
         * @param _defenseType
         */
        void list(QList<DefenseDefinition>& elementList, int _eventId, DefenseType _defenseType);

        std::vector<std::pair<int, int>> groupByHardiness(int _eventId, DefenseType _defenseType, bool activesOnly = true);

        void update(QList<DefenseDefinition>&, bool);

        /**
         * @brief updateElement Guarda el elemento indicado en la base de datos
         */
        static QString updateElement(DefenseDefinition&);

        /**
         * @brief getLastDefenseCode Get the last code used by a defense
         * @param int etapaOrdenId Identificador de la etapa que usar para buscar
         * @return int
         */
        static int getLastDefenseCode(int etapaOrdenId);

        /**
         * @brief createForEvent creates the defenses with the specified event.
         * It's used when a new event is created with its defenses.
         * @param event
         */
        void createForEvent(QList<DefenseDefinition>&, int event);

        /**
         * @brief removes the element from the database
         * @return
         */
        static bool remove(DefenseDefinition&);

        /**
         * @brief searchByName
         * @param name
         *
         * @return DefenseDefinition
         */
        static DefenseDefinition searchByName(QString name);

        /**
         * @brief setActive Update the active state of a defense in the database
         * @param defenseId The ID of the defense to update
         * @param active The new active state (true/1 to mark, false/0 to unmark)
         */
        static void setActive(int defenseId, bool active);
    };
}

#endif // DBDEFENSE_H
