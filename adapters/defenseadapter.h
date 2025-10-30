#ifndef DEFENSEADAPTER_H
#define DEFENSEADAPTER_H

#include <QObject>
#include <QJSValue>
#include <QVariantList>
#include <QVariantMap>
#include "../dbdefense.h"
#include "../defensedefinition.h"

class MainWindow;

/**
 * @brief Adapter class to expose defense-related functionality to JavaScript
 * 
 * The DefenseAdapter provides a JavaScript API for accessing and managing
 * defenses (barriers, frequency reducers, and consequence reducers) in the
 * risk analysis model. All methods are exposed as slots and can be called
 * from JavaScript code in the scripting editor.
 * 
 * Defense types:
 * - 0: Frequency Reducer - Reduces the frequency of an initiating event
 * - 1: Barrier - Prevents the event from progressing
 * - 2: Consequence Reducer - Reduces the consequences of an event
 * 
 * Hardiness levels (0-5):
 * - 0-2: Non-human defenses (technical, automated)
 * - 3-5: Human defenses (requiring human intervention)
 * 
 * @example
 * // Get all barriers
 * var barriers = Defenses.getBarriers();
 * console.log("Total barriers: " + barriers.length);
 * 
 * // Set human barriers as not applicable
 * var changed = Defenses.setHumanBarriersNotApplicable();
 * console.log("Modified " + changed + " barriers");
 * 
 * // Get defense statistics
 * var stats = Defenses.getDefenseStatistics();
 * console.log("Active defenses: " + stats.activeDefenses);
 */
class DefenseAdapter : public QObject
{
    Q_OBJECT

public:
    explicit DefenseAdapter(MainWindow *mainWindow, QObject *parent = nullptr);

public slots:
    /**
     * @brief Get all defenses with their details
     * 
     * Retrieves all defenses from the model, optionally filtered by event.
     * 
     * @param eventId Optional event ID to filter by (0 or omitted for all events)
     * @return Array of defense objects with properties:
     *   - id (Number): Unique defense identifier
     *   - code (String): Defense code
     *   - nombre (String): Defense name
     *   - descripcion (String): Defense description
     *   - type (Number): Defense type (0=Frequency, 1=Barrier, 2=Consequence)
     *   - typeString (String): Defense type description
     *   - active (Boolean): Whether defense is active/applicable
     *   - hardiness (Number): Hardiness level (0-5)
     *   - hardinessString (String): Hardiness level description
     *   - eventId (Number): Associated event ID
     *   - orderStageId (Number): Stage ID for ordering
     *   - orderStage (String): Stage name for ordering
     *   - isHuman (Boolean): Whether defense is human-based (hardiness >= 3)
     * 
     * @example
     * // Get all defenses
     * var allDefenses = Defenses.getAllDefenses();
     * 
     * // Get defenses for specific event
     * var eventDefenses = Defenses.getAllDefenses(5);
     */
    QVariantList getAllDefenses(int eventId = 0);

    /**
     * @brief Get defenses by type
     * 
     * @param type Defense type: 0=FrequencyReducer, 1=Barrier, 2=ConsequenceReducer
     * @param eventId Optional event ID to filter by (0 for all events)
     * @return Array of defense objects matching the type criteria
     * 
     * @example
     * // Get all barriers
     * var barriers = Defenses.getDefensesByType(1);
     * 
     * // Get frequency reducers for event 3
     * var freqReducers = Defenses.getDefensesByType(0, 3);
     */
    QVariantList getDefensesByType(int type, int eventId = 0);

    /**
     * @brief Get barriers (type 1) only
     * 
     * Convenience method to get all barriers without specifying the type.
     * 
     * @param eventId Optional event ID to filter by (0 for all events)
     * @return Array of barrier objects
     * 
     * @example
     * var barriers = Defenses.getBarriers();
     * var humanBarriers = Charts.filterByProperty(barriers, 'isHuman', true);
     * console.log("Human barriers: " + humanBarriers.length);
     */
    QVariantList getBarriers(int eventId = 0);

    /**
     * @brief Get frequency reducers (type 0) only
     * 
     * @param eventId Optional event ID to filter by (0 for all events)
     * @return Array of frequency reducer objects
     * 
     * @example
     * var freqReducers = Defenses.getFrequencyReducers();
     */
    QVariantList getFrequencyReducers(int eventId = 0);

    /**
     * @brief Get consequence reducers (type 2) only
     * 
     * @param eventId Optional event ID to filter by (0 for all events)
     * @return Array of consequence reducer objects
     * 
     * @example
     * var consReducers = Defenses.getConsequenceReducers();
     */
    QVariantList getConsequenceReducers(int eventId = 0);

    /**
     * @brief Get defenses grouped by type
     * 
     * Returns an object with defense type names as keys and arrays of
     * defenses as values.
     * 
     * @param eventId Optional event ID to filter by (0 for all events)
     * @return Object with type names as keys:
     *   - "Reductores de Frecuencia": Array of frequency reducers
     *   - "Barreras": Array of barriers
     *   - "Reductores de Consecuencia": Array of consequence reducers
     * 
     * @example
     * var grouped = Defenses.getDefensesByTypeGrouped();
     * for (var type in grouped) {
     *     console.log(type + ": " + grouped[type].length + " defenses");
     * }
     */
    QVariantMap getDefensesByTypeGrouped(int eventId = 0);

    /**
     * @brief Get defenses grouped by hardiness level
     * 
     * @param eventId Optional event ID to filter by (0 for all events)
     * @return Object with hardiness level descriptions as keys and
     *         arrays of defenses as values
     * 
     * @example
     * var byHardiness = Defenses.getDefensesByHardiness();
     * for (var level in byHardiness) {
     *     console.log(level + ": " + byHardiness[level].length);
     * }
     */
    QVariantMap getDefensesByHardiness(int eventId = 0);

    /**
     * @brief Set all human barriers to not applicable (active = false)
     * 
     * Human barriers are those with hardiness >= 3. This is useful for
     * analyzing scenarios where human intervention is not reliable.
     * 
     * @return Number of barriers modified
     * 
     * @example
     * var changed = Defenses.setHumanBarriersNotApplicable();
     * console.log("Deactivated " + changed + " human barriers");
     */
    int setHumanBarriersNotApplicable();

    /**
     * @brief Set all non-human barriers to applicable (active = true)
     * 
     * Non-human barriers are those with hardiness < 3. This is useful for
     * analyzing scenarios with only technical/automated defenses.
     * 
     * @return Number of barriers modified
     * 
     * @example
     * var changed = Defenses.setNonHumanBarriersApplicable();
     * console.log("Activated " + changed + " non-human barriers");
     */
    int setNonHumanBarriersApplicable();

    /**
     * @brief Set defense active state
     * 
     * Changes the active/applicable state of a specific defense.
     * 
     * @param defenseId Defense ID to modify
     * @param active New active state (true = active/applicable)
     * @return True if successful, false otherwise
     * 
     * @example
     * // Deactivate defense with ID 10
     * var success = Defenses.setDefenseActive(10, false);
     */
    bool setDefenseActive(int defenseId, bool active);

    /**
     * @brief Get defense statistics
     * 
     * Provides comprehensive statistics about defenses in the model.
     * 
     * @return Object with various statistics:
     *   - totalDefenses (Number): Total number of defenses
     *   - defensesByType (Object): Count of defenses per type
     *   - defensesByHardiness (Object): Count of defenses per hardiness level
     *   - activeDefenses (Number): Count of active defenses
     *   - inactiveDefenses (Number): Count of inactive defenses
     *   - humanDefenses (Number): Count of human defenses
     *   - nonHumanDefenses (Number): Count of non-human defenses
     * 
     * @example
     * var stats = Defenses.getDefenseStatistics();
     * console.log("Total: " + stats.totalDefenses);
     * console.log("Active: " + stats.activeDefenses);
     * console.log("Human: " + stats.humanDefenses);
     */
    QVariantMap getDefenseStatistics();

    /**
     * @brief Check if a defense is considered "human" based on hardiness
     * 
     * @param hardiness Defense hardiness level (0-5)
     * @return True if human (hardiness >= 3), false otherwise
     * 
     * @example
     * var isHuman = Defenses.isHumanDefense(4); // true
     * var isHuman2 = Defenses.isHumanDefense(2); // false
     */
    bool isHumanDefense(int hardiness);

    /**
     * @brief Set defense hardiness/robustness level
     * 
     * Changes the hardiness (robustness) level of a specific defense.
     * 
     * @param defenseId Defense ID to modify
     * @param hardiness New hardiness level (0-3)
     * @return True if successful, false otherwise
     * 
     * @example
     * // Set defense hardiness to level 2 (Robusta)
     * var success = Defenses.setDefenseHardiness(10, 2);
     */
    bool setDefenseHardiness(int defenseId, int hardiness);

    /**
     * @brief Recalculate risk for all events
     * 
     * Forces recalculation of frequency, probability, consequence and risk
     * for all events in the model based on current defense states.
     * This should be called after modifying defense states or hardiness.
     * 
     * @return True if successful, false otherwise
     * 
     * @example
     * // After modifying defenses, recalculate risks
     * Defenses.setDefenseActive(10, false);
     * Defenses.recalculateRisks();
     * var events = Events.getAllEvents(); // Now with updated risks
     */
    bool recalculateRisks();

private:
    MainWindow *mainWindow;

    QVariantMap defenseToVariant(const DefenseDefinition &defense);
    QString getDefenseTypeName(DefenseType type);
    QString getHardinessName(int hardiness);
};

#endif // DEFENSEADAPTER_H