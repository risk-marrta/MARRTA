#ifndef XLSX_H
#define XLSX_H

#include <QString>
#include <QProgressDialog>
#include "completeeventdefinition.h"
#include "modelo.h"
#include "matrizriesgo.h"

#include "dbevent.h"
#include "dbelementstage.h"
#include "dbelementsubstage.h"
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

namespace Modelos {

typedef struct {
    QString tab;        // Sheet/tab name
    QString line;       // Cell or row reference
    QString errorType;  // Type of error
    QString cellValue;  // Value in the problematic cell
    QString errorDetails; // Detailed error message
    QVector<QString> lineDetails; // Additional details for backward compatibility
} ErrorLocation;

class xlsx : public Modelo
{
public:
    static const int VERSION;


public:
    xlsx();
    bool createTables(QString path, QString title, std::unordered_map<std::string, std::vector<CompleteEventDefinition> > &events);
    /**
     * @brief importTables importa el excel indicado a la nueva base de datos.
     * @param origin Archivo XLSX
     * @param destiny Archivo PDB
     * @param title Nombre que llevará asociado el modelo
     * @return
     */
    bool importTables(QString origin, QString destiny, QString _title);

    QVector<ErrorLocation> errors() {
        return errorList;
    }

private:
    QXlsx::Document *xlsxDoc;
    QString title;
    QProgressDialog *dlg;

    /**
     * @brief etapasIdOrden
     *
     * Al importar las etapas, se guarda aquí la relación de ID con nombre de código de orden.
     * Se usará después cuando se importen las barreras.
     */
    QHash<QString, int> etapasIdOrden;
    
    /**
     * @brief validStageIds
     *
     * Set of valid stage IDs for validation
     */
    QSet<int> validStageIds;
    
    /**
     * @brief validSubstageIds
     *
     * Set of valid substage IDs (as pairs of stage_id-substage_id) for validation
     */
    QSet<QString> validSubstageIds;

private:
    std::map<QString, int> InversoDurezas{
        {"MUY BLANDA", -1},
        {"BLANDA", 0},
        {"NORMAL", 1},
        {"ROBUSTA", 2},
        {"MUY ROBUSTA", 3}
    };

    QVector<ErrorLocation> errorList;

    void insertInConfiguration();
    void insertInStages();
    void insertInSubstages();
    void insertInEvents();
    void insertInDefenses();
    /**
     * @brief insertInDefensesByType
     * @param lastId
     * @param sheetName
     * @param defensesTable
     * @param tipoDefensa
     * @return
     *
     * Inserta un tipo de defensa usando la consulta defensesTable.
     */
    int insertInDefensesByType(int lastId, QString sheetName, QSqlQuery *defensesTable, int tipoDefensa);

    /**
     * @brief normalizeString Normaliza un texto eliminando acentos y convirtiéndolo a mayúsculas
     * @param str Texto a normalizar
     * @return Texto normalizado
     */
    QString normalizeString(const QString &str);

    bool createTables(QString path) { Q_UNUSED(path); return false; }
    bool checkDetails() { return true; }
};

}
#endif // XLSX_H
