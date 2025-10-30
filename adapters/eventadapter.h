#ifndef EVENTADAPTER_H
#define EVENTADAPTER_H

#include <QObject>
#include <QJSValue>
#include <QVariantList>
#include <QVariantMap>
#include "../dbevent.h"
#include "../dbdefense.h"
#include "../dbelementstage.h"
#include "../matrizriesgo.h"

class MainWindow;

/**
 * @brief Adapter class to expose event-related functionality to JavaScript
 * 
 * The EventAdapter provides a JavaScript API for accessing and analyzing
 * initiating events in the risk analysis model. All methods are exposed
 * as slots and can be called from JavaScript code in the scripting editor.
 * 
 * @example
 * // Get all events
 * var events = Events.getAllEvents();
 * console.log("Total events: " + events.length);
 * 
 * // Get events by stage
 * var stageEvents = Events.getEventsByStage(1, 0);
 * 
 * // Get statistics
 * var stats = Events.getEventStatistics();
 * console.log("Human error events: " + stats.humanErrorEvents);
 */
class EventAdapter : public QObject
{
    Q_OBJECT

public:
    explicit EventAdapter(MainWindow *mainWindow, QObject *parent = nullptr);

public slots:
    /**
     * @brief Get all events with their details
     * 
     * Retrieves all initiating events from the risk analysis model with
     * complete information including risk calculations.
     * 
     * @return Array of event objects with properties:
     *   - id (Number): Unique event identifier
     *   - codigo (String): Event code
     *   - nombre (String): Event name
     *   - descripcion (String): Event description
     *   - etapa (String): Stage name
     *   - etapaId (Number): Stage ID
     *   - subetapa (String): Substage name
     *   - subetapaId (Number): Substage ID
     *   - frecuenciaBase (Number): Base frequency level from database (0-3)
     *   - consecuenciaBase (Number): Base consequence level from database (0-3)
     *   - riesgoBase (Number): Base risk level calculated with max probability (0-3)
     *   - riesgoBaseTexto (String): Base risk text (RB, RM, RA, RMA)
     *   - frecuencia (Number): Calculated frequency level with defenses (0-3)
     *   - frecuenciaTexto (String): Frequency text (FMB, FB, FM, FA)
     *   - probabilidad (Number): Calculated probability level from barriers (0-3)
     *   - probabilidadTexto (String): Probability text (PMB, PB, PM, PA)
     *   - consecuencia (Number): Calculated consequence level with defenses (0-3)
     *   - consecuenciaTexto (String): Consequence text (CB, CM, CA, CMA)
     *   - errorHumano (Boolean): Whether event involves human error
     *   - base (Boolean): Whether this is a base event
     *   - riesgo (Number): Calculated risk level from risk matrix (0-3)
     *   - riesgoTexto (String): Risk level description (RB, RM, RA, RMA)
     * 
     * Note: Probability is calculated from barriers, frequency from frequency reducers,
     * consequence from consequence reducers, and risk from the risk matrix applied to P, F, C.
     * 
     * @example
     * var events = Events.getAllEvents();
     * for (var i = 0; i < events.length; i++) {
     *     console.log(events[i].codigo + ": " + events[i].nombre);
     * }
     */
    QVariantList getAllEvents();

    /**
     * @brief Get events filtered by stage
     * 
     * @param stageId Stage ID to filter by (required)
     * @param substageId Optional substage ID (0 for all substages in the stage)
     * @return Array of event objects matching the filter criteria
     * 
     * @example
     * // Get all events in stage 1
     * var stageEvents = Events.getEventsByStage(1, 0);
     * 
     * // Get events in specific substage
     * var substageEvents = Events.getEventsByStage(1, 2);
     */
    QVariantList getEventsByStage(int stageId, int substageId = 0);

    /**
     * @brief Get events grouped by stage
     * 
     * Returns an object where keys are stage names and values are arrays
     * of events belonging to that stage.
     * 
     * @return Object with stage names as keys and arrays of events as values
     * 
     * @example
     * var grouped = Events.getEventsByStageGrouped();
     * for (var stageName in grouped) {
     *     console.log(stageName + ": " + grouped[stageName].length + " events");
     * }
     */
    QVariantMap getEventsByStageGrouped();

    /**
     * @brief Count events by stage
     * 
     * Returns an object with stage names as keys and event counts as values.
     * 
     * @return Object with stage names as keys and counts (Number) as values
     * 
     * @example
     * var counts = Events.countEventsByStage();
     * console.log(Charts.generateTextBarChart("Events per Stage", counts));
     */
    QVariantMap countEventsByStage();

    /**
     * @brief Get events grouped by consequence level
     * 
     * Groups events by their consequence level for analysis purposes.
     * 
     * @return Object with consequence levels as keys (e.g., "Consecuencia 3")
     *         and arrays of events as values
     * 
     * @example
     * var byConsequence = Events.getEventsByConsequence();
     * for (var level in byConsequence) {
     *     var events = byConsequence[level];
     *     console.log(level + ": " + events.length + " events");
     * }
     */
    QVariantMap getEventsByConsequence();

    /**
     * @brief Get events grouped by risk level
     * 
     * Groups events by their calculated risk level.
     * 
     * @return Object with risk levels as keys (e.g., "Riesgo Tolerable")
     *         and arrays of events as values
     * 
     * @example
     * var byRisk = Events.getEventsByRisk();
     * var highRisk = byRisk["Riesgo Inaceptable"] || [];
     * console.log("High risk events: " + highRisk.length);
     */
    QVariantMap getEventsByRisk();

    /**
     * @brief Get event statistics
     * 
     * Provides comprehensive statistics about events in the model.
     * 
     * @return Object with various statistics:
     *   - totalEvents (Number): Total number of events
     *   - eventsByStage (Object): Count of events per stage
     *   - eventsByRisk (Object): Count of events per risk level
     *   - eventsByConsequence (Object): Count of events per consequence level
     *   - eventsByFrequency (Object): Count of events per frequency level
     *   - humanErrorEvents (Number): Count of events involving human error
     *   - nonHumanErrorEvents (Number): Count of events not involving human error
     * 
     * @example
     * var stats = Events.getEventStatistics();
     * console.log("Total: " + stats.totalEvents);
     * console.log("Human error: " + stats.humanErrorEvents);
     * console.log("By risk: ", stats.eventsByRisk);
     */
    QVariantMap getEventStatistics();
    
    /**
     * @brief Update an event's properties
     * 
     * Updates a single event in the database. All writable properties can be changed.
     * 
     * @param eventId Event ID to update
     * @param properties Object with properties to update (any combination of):
     *   - nombre (String): Event name
     *   - descripcion (String): Event description
     *   - codigo (String): Event code
     *   - frecuencia (Number): Frequency level (0-5)
     *   - consecuencia (Number): Consequence level (0-5)
     *   - etapaId (Number): Stage ID
     *   - subetapaId (Number): Substage ID
     *   - errorHumano (Boolean): Whether involves human error
     *   - completado (Boolean): Whether event is applicable/completed
     * @return True if successful, false otherwise
     * 
     * @example
     * // Mark event as completed
     * Events.updateEvent(1, {completado: true});
     * 
     * // Change event frequency and consequence
     * Events.updateEvent(5, {frecuencia: 3, consecuencia: 4});
     * 
     * // Update event name and description
     * Events.updateEvent(10, {
     *     nombre: "New name",
     *     descripcion: "Updated description"
     * });
     */
    bool updateEvent(int eventId, const QVariantMap &properties);
    
    /**
     * @brief Mark all events as applicable (completado=true)
     * 
     * Sets the "completado" flag to true for all events in the model.
     * 
     * @return Number of events updated
     * 
     * @example
     * var count = Events.setAllEventsApplicable();
     * console.log("Marked " + count + " events as applicable");
     */
    int setAllEventsApplicable();
    
    /**
     * @brief Mark all events as not applicable (completado=false)
     * 
     * Sets the "completado" flag to false for all events in the model.
     * 
     * @return Number of events updated
     * 
     * @example
     * var count = Events.setAllEventsNotApplicable();
     * console.log("Marked " + count + " events as not applicable");
     */
    int setAllEventsNotApplicable();
    
    /**
     * @brief Search and replace text in event descriptions
     * 
     * Finds all occurrences of a search term in event descriptions and
     * replaces them with a replacement term.
     * 
     * @param searchTerm Text to search for
     * @param replaceTerm Text to replace with
     * @param caseSensitive Whether search is case-sensitive (default: false)
     * @return Number of events modified
     * 
     * @example
     * // Replace "paciente" with "usuario" in all descriptions
     * var count = Events.searchReplaceInDescriptions("paciente", "usuario");
     * console.log("Updated " + count + " event descriptions");
     * 
     * // Case-sensitive replacement
     * var count = Events.searchReplaceInDescriptions("Error", "Fallo", true);
     */
    int searchReplaceInDescriptions(const QString &searchTerm, const QString &replaceTerm, bool caseSensitive = false);

private:
    MainWindow *mainWindow;
    MatrizRiesgo riskMatrix;

    QVariantMap eventToVariant(const EventDefinition &event);
    QString getStageName(int stageId);
    QString getSubstageName(int substageId);
};

#endif // EVENTADAPTER_H