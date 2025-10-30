#ifndef STAGEADAPTER_H
#define STAGEADAPTER_H

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include "../dbelementstage.h"
#include "../dbelementsubstage.h"

class MainWindow;

/**
 * @brief Adapter class to expose stage and substage information to JavaScript
 * 
 * The StageAdapter provides a JavaScript API for accessing the structure
 * of stages and substages in the risk analysis model. Stages represent
 * major phases in the process being analyzed, and substages represent
 * subdivisions within each stage.
 * 
 * @example
 * // Get all stages
 * var stages = Stages.getAllStages();
 * console.log("Total stages: " + stages.length);
 * 
 * // Get stages with their substages
 * var stagesWithSubs = Stages.getStagesWithSubstages();
 * for (var stageName in stagesWithSubs) {
 *     var stage = stagesWithSubs[stageName];
 *     console.log(stage.nombre + " has " + stage.substages.length + " substages");
 * }
 * 
 * // Count substages per stage
 * var counts = Stages.countSubstagesByStage();
 * console.log(Charts.generateTextBarChart("Substages per Stage", counts));
 */
class StageAdapter : public QObject
{
    Q_OBJECT

public:
    explicit StageAdapter(MainWindow *mainWindow, QObject *parent = nullptr);

public slots:
    /**
     * @brief Get all stages with their details
     * 
     * Retrieves all stages from the risk analysis model.
     * 
     * @return Array of stage objects with properties:
     *   - id (Number): Unique stage identifier
     *   - nombre (String): Stage name
     *   - codigo (String): Stage code
     *   - descripcion (String): Stage description
     *   - orden (Number): Display order
     * 
     * @example
     * var stages = Stages.getAllStages();
     * for (var i = 0; i < stages.length; i++) {
     *     console.log(stages[i].codigo + ": " + stages[i].nombre);
     * }
     */
    QVariantList getAllStages();

    /**
     * @brief Get all substages for all stages
     * 
     * Retrieves all substages from the model regardless of their parent stage.
     * 
     * @return Array of substage objects with properties:
     *   - id (Number): Unique substage identifier
     *   - nombre (String): Substage name
     *   - descripcion (String): Substage description
     *   - etapaId (Number): Parent stage ID
     *   - activo (Boolean): Whether substage is active
     *   - orden (Number): Display order within parent stage
     * 
     * @example
     * var substages = Stages.getAllSubstages();
     * console.log("Total substages: " + substages.length);
     */
    QVariantList getAllSubstages();

    /**
     * @brief Get substages for a specific stage
     * 
     * Retrieves only the substages belonging to the specified stage.
     * 
     * @param stageId The stage ID to get substages for
     * @return Array of substage objects for the specified stage
     * 
     * @example
     * // Get substages for stage 1
     * var substages = Stages.getSubstagesByStage(1);
     * console.log("Stage 1 has " + substages.length + " substages");
     * for (var i = 0; i < substages.length; i++) {
     *     console.log("  - " + substages[i].nombre);
     * }
     */
    QVariantList getSubstagesByStage(int stageId);

    /**
     * @brief Get stages grouped with their substages
     * 
     * Returns an object where keys are stage names and values are objects
     * containing stage information plus an array of substages.
     * 
     * @return Object with stage names as keys and objects containing:
     *   - Stage properties (id, nombre, codigo, descripcion, orden)
     *   - substages (Array): Array of substage objects
     * 
     * @example
     * var stagesWithSubs = Stages.getStagesWithSubstages();
     * for (var stageName in stagesWithSubs) {
     *     var stage = stagesWithSubs[stageName];
     *     console.log("=== " + stage.codigo + " - " + stageName + " ===");
     *     console.log("Substages: " + stage.substages.length);
     *     for (var i = 0; i < stage.substages.length; i++) {
     *         var sub = stage.substages[i];
     *         console.log("  " + (i+1) + ". " + sub.nombre);
     *     }
     * }
     */
    QVariantMap getStagesWithSubstages();

    /**
     * @brief Get stage by ID
     * 
     * Retrieves a single stage by its unique identifier.
     * 
     * @param stageId The stage ID to retrieve
     * @return Stage object, or empty object if not found
     * 
     * @example
     * var stage = Stages.getStageById(1);
     * if (stage.id) {
     *     console.log("Found: " + stage.nombre);
     * } else {
     *     console.log("Stage not found");
     * }
     */
    QVariantMap getStageById(int stageId);

    /**
     * @brief Get substage by ID
     * 
     * Retrieves a single substage by its unique identifier.
     * 
     * @param substageId The substage ID to retrieve
     * @return Substage object, or empty object if not found
     * 
     * @example
     * var substage = Stages.getSubstageById(5);
     * if (substage.id) {
     *     console.log("Found: " + substage.nombre);
     *     console.log("Parent stage ID: " + substage.etapaId);
     * }
     */
    QVariantMap getSubstageById(int substageId);

    /**
     * @brief Count substages per stage
     * 
     * Returns an object with stage names as keys and substage counts as values.
     * Useful for quick overview of model structure.
     * 
     * @return Object with stage names as keys and counts (Number) as values
     * 
     * @example
     * var counts = Stages.countSubstagesByStage();
     * console.log("Substages per stage:");
     * for (var stage in counts) {
     *     console.log("  " + stage + ": " + counts[stage]);
     * }
     * 
     * // Can be used with Charts
     * console.log(Charts.generateTextBarChart("Substages", counts));
     */
    QVariantMap countSubstagesByStage();
    
    /**
     * @brief Update a stage's properties
     * 
     * Updates a single stage in the database. Only non-ID properties can be changed.
     * 
     * @param stageId Stage ID to update
     * @param properties Object with properties to update (any combination of):
     *   - nombre (String): Stage name
     *   - descripcion (String): Stage description
     *   - codigo (String): Stage code
     *   - orden (Number): Display order
     * @return True if successful, false otherwise
     * 
     * @example
     * // Update stage name
     * Stages.updateStage(1, {nombre: "Nueva etapa"});
     * 
     * // Update order and code
     * Stages.updateStage(2, {orden: 5, codigo: "E05"});
     */
    bool updateStage(int stageId, const QVariantMap &properties);

private:
    MainWindow *mainWindow;

    QVariantMap stageToVariant(const EtapaDefinition &stage);
    QVariantMap substageToVariant(const SubetapaDefinition &substage);
};

#endif // STAGEADAPTER_H
