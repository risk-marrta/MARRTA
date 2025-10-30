#ifndef FLUJOGRAMAGENERATOR_H
#define FLUJOGRAMAGENERATOR_H

#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QMap>
#include <QSet>
#include <QList>

/**
 * @brief Estructura para representar una defensa agregada
 * Las defensas se agregan por (EtapaId, Código) según el script Python
 */
struct DefensaAgregada {
    int etapaId;
    QString codigo;
    QString nombre;
    int robustez; // 0-3 (se toma el máximo al agregar)
    QSet<QString> targets; // Códigos de sucesos iniciadores (unión al agregar)
};

/**
 * @brief The FlujogramaGenerator class generates a vertical flowchart in HTML
 * matching exactly the output from generar_flujograma_v25_fix.py
 */
class FlujogramaGenerator {
public:
    /**
     * @brief Generate an HTML flowchart from the database
     * @param rutaSalida Path where the HTML file will be saved
     * @return true if successful, false otherwise
     */
    static bool generarHTML(const QString& rutaSalida);
    
private:
    // Layout constants
    static const int STAGE_HEADER_PAD = 100;
    static const int SUB_HEADER_PAD = 80;
    static const int BOTTOM_PAD = 12;
    static const int NODE_W = 420;
    static const int NODE_H_MIN = 120;
    static const int V_GAP = 50;
    static const int SUB_W = 380;
    static const int SUB_H_MIN = 100;
    static const int SUB_GAP = 14;
    static const int INIT_W_INSIDE = 340;
    static const int INIT_H_MIN = 64;
    static const int INIT_GAP = 10;
    static const int DEF_W = 300;
    static const int DEF_H_MIN = 60;
    static const int DEF_GAP = 10;
    
    // Column X positions (left to right):
    // fictional stages - barriers fict - barriers real - stages - RF real - RC real - RF fict - RC fict - fictional stages
    static const int X_FICT = 100;
    static const int X_BAR_FICT = X_FICT + NODE_W + 80;
    static const int X_BAR_REAL = X_BAR_FICT + DEF_W + 200;
    static const int X_REAL = X_BAR_REAL + DEF_W + 420;
    static const int X_RF_REAL = X_REAL + NODE_W + 260;
    static const int X_RC_REAL = X_RF_REAL + DEF_W + 160;
    static const int X_RF_FICT = X_RC_REAL + DEF_W + 260;
    static const int X_RC_FICT = X_RF_FICT + DEF_W + 160;
    static const int X_FICT_R = X_RC_FICT + DEF_W + 80;
    static const int INIT_COL_CENTER = X_REAL + NODE_W / 2;
    
    /**
     * @brief Load all data from database
     */
    static bool cargarDatosModelo(
        QVariantList& etapasReales,
        QVariantList& etapasFicticias,
        QMap<int, QVariantList>& subetapasPorEtapa,
        QMap<int, QVariantList>& iniciadorsPorSubetapa,
        QMap<int, QVariantList>& iniciadorDirectosPorEtapa,
        QList<DefensaAgregada>& barrerasReales,
        QList<DefensaAgregada>& barrerasFicticias,
        QList<DefensaAgregada>& rfReales,
        QList<DefensaAgregada>& rfFicticias,
        QList<DefensaAgregada>& rcReales,
        QList<DefensaAgregada>& rcFicticias
    );
    
    /**
     * @brief Aggregate defensas by (EtapaId, Código)
     */
    static QList<DefensaAgregada> agregarDefensas(const QVariantList& defensas);
    
    /**
     * @brief Calculate layout positions and heights
     */
    static void calcularLayout(
        const QVariantList& etapasReales,
        const QVariantList& etapasFicticias,
        const QMap<int, QVariantList>& subetapasPorEtapa,
        const QMap<int, QVariantList>& iniciadorsPorSubetapa,
        const QMap<int, QVariantList>& iniciadorDirectosPorEtapa,
        const QList<DefensaAgregada>& barrerasReales,
        const QList<DefensaAgregada>& rfReales,
        const QList<DefensaAgregada>& rcReales,
        const QList<DefensaAgregada>& barrerasFicticias,
        const QList<DefensaAgregada>& rfFicticias,
        const QList<DefensaAgregada>& rcFicticias,
        QMap<int, int>& alturasPorEtapa,
        QMap<int, int>& posicionYPorEtapa,
        QMap<int, int>& alturasPorEtapaFicticia,
        QMap<int, int>& posicionYPorEtapaFicticia
    );
    
    /**
     * @brief Generate complete HTML content
     */
    static QString generarHTMLCompleto(
        const QVariantList& etapasReales,
        const QVariantList& etapasFicticias,
        const QMap<int, QVariantList>& subetapasPorEtapa,
        const QMap<int, QVariantList>& iniciadorsPorSubetapa,
        const QMap<int, QVariantList>& iniciadorDirectosPorEtapa,
        const QList<DefensaAgregada>& barrerasReales,
        const QList<DefensaAgregada>& barrerasFicticias,
        const QList<DefensaAgregada>& rfReales,
        const QList<DefensaAgregada>& rfFicticias,
        const QList<DefensaAgregada>& rcReales,
        const QList<DefensaAgregada>& rcFicticias,
        const QMap<int, int>& alturasPorEtapa,
        const QMap<int, int>& posicionYPorEtapa,
        const QMap<int, int>& alturasPorEtapaFicticia,
        const QMap<int, int>& posicionYPorEtapaFicticia,
        const QString& modelName
    );
    
    // Helper functions
    static QString escapeHTML(const QString& text);
    static QStringList wrapText(const QString& text, int maxChars = 34);
    static int calcularAlturaTexto(const QString& title, const QString& subtitle = "");
    static int normalizarRobustez(const QString& robustezStr);
    
    // SVG generation
    static QString generarNodo(double x, double y, double w, double h,
                              const QString& clase, const QString& titulo,
                              const QString& subtitulo, const QString& tipo,
                              int etapaId, const QString& code = "",
                              const QString& extraAttrs = "");
    static QString generarFlecha(double x1, double y1, double x2, double y2,
                                const QString& clase, const QString& dataFrom,
                                const QString& dataTo);
    
    // Templates
    static QString obtenerPlantillaCSS();
    static QString obtenerPlantillaJavaScript();
};

#endif // FLUJOGRAMAGENERATOR_H
