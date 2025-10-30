#include "flujogramagenerator.h"
#include "dbmanager.h"
#include <QFile>
#include <QTextStream>
#include <QStringConverter>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QMap>
#include <QSet>
#include <QRegularExpression>
#include <QtMath>
#include <algorithm>

// Helper function implementations
QString FlujogramaGenerator::escapeHTML(const QString& text) {
    QString result = text;
    result.replace("&", "&amp;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    result.replace("\"", "&quot;");
    return result;
}

QStringList FlujogramaGenerator::wrapText(const QString& text, int maxChars) {
    QStringList words = text.split(' ', Qt::SkipEmptyParts);
    if (words.isEmpty()) return QStringList{""};
    
    QStringList lines;
    QString currentLine;
    
    for (const QString& word : words) {
        int potentialLength = currentLine.length() + (currentLine.isEmpty() ? 0 : 1) + word.length();
        if (potentialLength <= maxChars) {
            if (!currentLine.isEmpty()) currentLine += " ";
            currentLine += word;
        } else {
            if (!currentLine.isEmpty()) {
                lines.append(currentLine);
            }
            currentLine = word;
        }
    }
    
    if (!currentLine.isEmpty()) {
        lines.append(currentLine);
    }
    
    return lines.isEmpty() ? QStringList{""} : lines;
}

int FlujogramaGenerator::calcularAlturaTexto(const QString& title, const QString& subtitle) {
    int numLineas = wrapText(title).size();
    if (!subtitle.isEmpty()) {
        numLineas += wrapText(subtitle).size();
    }
    return qMax(64, 18 * numLineas + 24 * 2);
}

int FlujogramaGenerator::normalizarRobustez(const QString& robustezStr) {
    QString norm = robustezStr.toLower().trimmed();
    
    // Map text values to integers
    if (norm == "muy blanda" || norm == "muy blando" || norm == "very soft") return -1;
    if (norm == "blanda" || norm == "blando" || norm == "baja" || norm == "muy baja" || norm == "low") return 0;
    if (norm == "normal" || norm == "medio" || norm == "media" || norm == "medium") return 1;
    if (norm == "robusta" || norm == "robusto" || norm == "alto" || norm == "high") return 2;
    if (norm == "muy robusta" || norm == "muy robusto" || norm == "muy alta" || norm == "very high") return 3;
    
    // Try to parse as integer
    bool ok;
    int value = robustezStr.toInt(&ok);
    if (ok) {
        return qBound(-1, value, 3);
    }
    
    return 0; // default
}

QString FlujogramaGenerator::generarNodo(double x, double y, double w, double h,
                                         const QString& clase, const QString& titulo,
                                         const QString& subtitulo, const QString& tipo,
                                         int etapaId, const QString& code,
                                         const QString& extraAttrs) {
    QStringList titleLines = wrapText(titulo);
    QStringList subtitleLines = subtitulo.isEmpty() ? QStringList() : wrapText(subtitulo);
    
    QString textSVG = QString("<text class=\"title\" x=\"%1\" y=\"12\">").arg(w/2);
    
    for (int i = 0; i < titleLines.size(); i++) {
        QString dy = (i == 0) ? "" : " dy=\"16\"";
        textSVG += QString("<tspan x=\"%1\"%2>%3</tspan>")
            .arg(w/2)
            .arg(dy)
            .arg(escapeHTML(titleLines[i]));
    }
    
    if (!subtitleLines.isEmpty()) {
        textSVG += QString("<tspan x=\"%1\" dy=\"24\" class=\"subtitle\">%2</tspan>")
            .arg(w/2)
            .arg(escapeHTML(subtitleLines[0]));
        for (int i = 1; i < subtitleLines.size(); i++) {
            textSVG += QString("<tspan x=\"%1\" dy=\"14\" class=\"subtitle\">%2</tspan>")
                .arg(w/2)
                .arg(escapeHTML(subtitleLines[i]));
        }
    }
    
    textSVG += "</text>";
    
    QString codeAttr = code.isEmpty() ? "" : escapeHTML(code);
    
    return QString(
        "<g class=\"node %1\" transform=\"translate(%2,%3)\" "
        "data-name=\"%4\" data-code=\"%5\" data-tipo=\"%6\" data-etapa=\"%7\" %8 tabindex=\"0\">"
        "<rect rx=\"12\" ry=\"12\" width=\"%9\" height=\"%10\"></rect>%11</g>")
        .arg(clase)
        .arg(x).arg(y)
        .arg(escapeHTML(titulo))
        .arg(codeAttr)
        .arg(tipo)
        .arg(etapaId)
        .arg(extraAttrs)
        .arg(w).arg(h)
        .arg(textSVG);
}

QString FlujogramaGenerator::generarFlecha(double x1, double y1, double x2, double y2,
                                          const QString& clase, const QString& dataFrom,
                                          const QString& dataTo) {
    return QString(
        "<path class=\"arrow %1\" data-from=\"%2\" data-to=\"%3\" "
        "marker-end=\"url(#arrowhead)\" d=\"M %4 %5 L %6 %7\"></path>")
        .arg(clase)
        .arg(escapeHTML(dataFrom))
        .arg(escapeHTML(dataTo))
        .arg(x1).arg(y1).arg(x2).arg(y2);
}

bool FlujogramaGenerator::generarHTML(const QString& rutaSalida) {
    qDebug() << "FlujogramaGenerator::generarHTML - Starting generation";
    
    // Step 0: Load model configuration to get model name
    QString modelName = "Modelo"; // Default name
    try {
        Database::ConfiguracionModelo config = Database::DbManager::checkDatabase(false);
        modelName = config.nombre;
        if (modelName.isEmpty()) {
            modelName = "Modelo";
        }
    } catch (...) {
        qDebug() << "Warning: Could not load model configuration, using default name";
    }
    
    // Step 1: Load data from database
    QVariantList etapasReales, etapasFicticias;
    QMap<int, QVariantList> subetapasPorEtapa, iniciadorsPorSubetapa, iniciadorDirectosPorEtapa;
    QList<DefensaAgregada> barrerasReales, barrerasFicticias, rfReales, rfFicticias, rcReales, rcFicticias;
    
    if (!cargarDatosModelo(etapasReales, etapasFicticias, subetapasPorEtapa,
                           iniciadorsPorSubetapa, iniciadorDirectosPorEtapa,
                           barrerasReales, barrerasFicticias, rfReales, rfFicticias, rcReales, rcFicticias)) {
        qDebug() << "Error loading model data";
        return false;
    }
    
    // Step 2: Calculate layout
    QMap<int, int> alturasPorEtapa, posicionYPorEtapa, alturasPorEtapaFicticia, posicionYPorEtapaFicticia;
    calcularLayout(etapasReales, etapasFicticias, subetapasPorEtapa, iniciadorsPorSubetapa,
                   iniciadorDirectosPorEtapa, barrerasReales, rfReales, rcReales,
                   barrerasFicticias, rfFicticias, rcFicticias,
                   alturasPorEtapa, posicionYPorEtapa, alturasPorEtapaFicticia, posicionYPorEtapaFicticia);
    
    // Step 3: Generate HTML
    QString html = generarHTMLCompleto(etapasReales, etapasFicticias, subetapasPorEtapa,
                                      iniciadorsPorSubetapa, iniciadorDirectosPorEtapa,
                                      barrerasReales, barrerasFicticias, rfReales, rfFicticias, rcReales, rcFicticias,
                                      alturasPorEtapa, posicionYPorEtapa, alturasPorEtapaFicticia, posicionYPorEtapaFicticia,
                                      modelName);
    
    // Step 4: Write to file
    QFile file(rutaSalida);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << rutaSalida;
        return false;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << html;
    file.close();
    
    qDebug() << "FlujogramaGenerator::generarHTML - Generation complete";
    return true;
}


bool FlujogramaGenerator::cargarDatosModelo(
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
    QList<DefensaAgregada>& rcFicticias) {
    
    qDebug() << "Loading model data from database...";
    
    // Load etapas - real (id > 0) and fictitious (id <= 0)
    QSqlQuery queryEtapas;
    queryEtapas.prepare("SELECT id, nombre, codigo, orden FROM etapas WHERE activo = 1 ORDER BY orden");
    
    if (!queryEtapas.exec()) {
        qDebug() << "Error querying etapas:" << queryEtapas.lastError().text();
        return false;
    }
    
    QMap<QString, int> codigoToId; // For mapping defensa.etapa_orden_id via codigo
    
    while (queryEtapas.next()) {
        QVariantMap etapa;
        int id = queryEtapas.value(0).toInt();
        etapa["id"] = id;
        etapa["nombre"] = queryEtapas.value(1).toString();
        etapa["codigo"] = queryEtapas.value(2).toString();
        etapa["orden"] = queryEtapas.value(3).toInt();
        
        QString codigo = queryEtapas.value(2).toString();
        if (!codigo.isEmpty()) {
            codigoToId[codigo] = id;
        }
        
        if (id > 0) {
            etapasReales.append(etapa);
        } else {
            etapasFicticias.append(etapa);
        }
    }
    
    qDebug() << "Loaded" << etapasReales.size() << "real stages and" << etapasFicticias.size() << "fictional stages";
    
    // Load subetapas
    QSqlQuery querySubetapas;
    querySubetapas.prepare("SELECT id, nombre, etapa_id, orden FROM subetapas WHERE activo = 1 ORDER BY etapa_id, orden");
    
    if (!querySubetapas.exec()) {
        qDebug() << "Error querying subetapas:" << querySubetapas.lastError().text();
        return false;
    }
    
    while (querySubetapas.next()) {
        QVariantMap subetapa;
        subetapa["id"] = querySubetapas.value(0).toInt();
        subetapa["nombre"] = querySubetapas.value(1).toString();
        int etapaId = querySubetapas.value(2).toInt();
        subetapa["etapa_id"] = etapaId;
        subetapa["orden"] = querySubetapas.value(3).toInt();
        
        subetapasPorEtapa[etapaId].append(subetapa);
    }
    
    qDebug() << "Loaded subetapas for" << subetapasPorEtapa.size() << "stages";
    
    // Load iniciadores (suceso_iniciador)
    QSqlQuery queryIniciadores;
    queryIniciadores.prepare(
        "SELECT id, nombre, codigo, etapa_id, subetapa_id, riesgo "
        "FROM suceso_iniciador WHERE activo = 1 ORDER BY codigo");
    
    if (!queryIniciadores.exec()) {
        qDebug() << "Error querying iniciadores:" << queryIniciadores.lastError().text();
        return false;
    }
    
    while (queryIniciadores.next()) {
        QVariantMap iniciador;
        iniciador["id"] = queryIniciadores.value(0).toInt();
        iniciador["nombre"] = queryIniciadores.value(1).toString();
        iniciador["codigo"] = queryIniciadores.value(2).toString();
        int etapaId = queryIniciadores.value(3).toInt();
        QVariant subetapaIdVar = queryIniciadores.value(4);
        iniciador["etapa_id"] = etapaId;
        iniciador["subetapa_id"] = subetapaIdVar;
        
        // Convert risk integer to text code
        QVariant riesgoVar = queryIniciadores.value(5);
        QString riesgoStr;
        if (!riesgoVar.isNull()) {
            int riesgoInt = riesgoVar.toInt();
            if (riesgoInt == 0) riesgoStr = "RB";
            else if (riesgoInt == 1) riesgoStr = "RM";
            else if (riesgoInt == 2) riesgoStr = "RA";
            else if (riesgoInt == 3) riesgoStr = "RMA";
        }
        iniciador["riesgo"] = riesgoStr;
        
        if (subetapaIdVar.isNull()) {
            // Direct initiator (no substage)
            iniciadorDirectosPorEtapa[etapaId].append(iniciador);
        } else {
            int subetapaId = subetapaIdVar.toInt();
            iniciadorsPorSubetapa[subetapaId].append(iniciador);
        }
    }
    
    qDebug() << "Loaded iniciadores";
    
    // Load defensas and aggregate
    QSqlQuery queryDefensas;
    queryDefensas.prepare(
        "SELECT d.id, d.codigo, d.nombre, d.robustez, d.tipo, d.suceso_iniciador_id, d.etapa_orden_id, e.codigo as etapa_codigo, si.codigo as target_codigo "
        "FROM defensa d "
        "LEFT JOIN etapas e ON d.etapa_orden_id = e.id "
        "LEFT JOIN suceso_iniciador si ON d.suceso_iniciador_id = si.id "
        "WHERE d.activo = 1");
    
    if (!queryDefensas.exec()) {
        qDebug() << "Error querying defensas:" << queryDefensas.lastError().text();
        return false;
    }
    
    QVariantList defensasBarrera, defensasRF, defensasRC;
    
    while (queryDefensas.next()) {
        QVariantMap defensa;
        defensa["id"] = queryDefensas.value(0).toInt();
        defensa["codigo"] = queryDefensas.value(1).toString();
        defensa["nombre"] = queryDefensas.value(2).toString();
        defensa["robustez"] = queryDefensas.value(3).toInt();
        int tipo = queryDefensas.value(4).toInt();
        defensa["tipo"] = tipo;
        
        // Get suceso_iniciador_id - skip if NULL or 0
        QVariant sucesoIniciadorIdVar = queryDefensas.value(5);
        if (sucesoIniciadorIdVar.isNull() || sucesoIniciadorIdVar.toInt() == 0) {
            continue; // Skip defensas without a valid target iniciador
        }
        defensa["suceso_iniciador_id"] = sucesoIniciadorIdVar.toInt();
        
        // Get target_codigo - skip if NULL (means no iniciador found)
        QVariant targetCodigoVar = queryDefensas.value(8);
        if (targetCodigoVar.isNull() || targetCodigoVar.toString().isEmpty()) {
            continue; // Skip defensas without a valid target iniciador código
        }
        defensa["target_codigo"] = targetCodigoVar.toString();
        
        // Get etapa_id from etapa_orden_id
        QVariant etapaOrdenIdVar = queryDefensas.value(6);
        if (!etapaOrdenIdVar.isNull()) {
            defensa["etapa_id"] = etapaOrdenIdVar.toInt();
        }
        
        // Categorize by type (tipo field mapping: 0=RF, 1=Barrera, 2=RC)
        if (tipo == 0) {
            defensasRF.append(defensa);
        } else if (tipo == 1) {
            defensasBarrera.append(defensa);
        } else if (tipo == 2) {
            defensasRC.append(defensa);
        }
    }
    
    qDebug() << "Loaded" << defensasBarrera.size() << "barriers," 
             << defensasRF.size() << "RF," << defensasRC.size() << "RC";
    
    // Aggregate defensas
    barrerasReales = agregarDefensas(defensasBarrera.mid(0)); // Filter real later
    rfReales = agregarDefensas(defensasRF);
    rcReales = agregarDefensas(defensasRC);
    
    // Separate real from fictional (etapaId > 0 vs <= 0)
    QList<DefensaAgregada> barrTemp = barrerasReales;
    barrerasReales.clear();
    barrerasFicticias.clear();
    for (const auto& def : barrTemp) {
        if (def.etapaId > 0) {
            barrerasReales.append(def);
        } else {
            barrerasFicticias.append(def);
        }
    }
    
    QList<DefensaAgregada> rfTemp = rfReales;
    rfReales.clear();
    rfFicticias.clear();
    for (const auto& def : rfTemp) {
        if (def.etapaId > 0) {
            rfReales.append(def);
        } else {
            rfFicticias.append(def);
        }
    }
    
    QList<DefensaAgregada> rcTemp = rcReales;
    rcReales.clear();
    rcFicticias.clear();
    for (const auto& def : rcTemp) {
        if (def.etapaId > 0) {
            rcReales.append(def);
        } else {
            rcFicticias.append(def);
        }
    }
    
    qDebug() << "Data loading complete";
    return true;
}

QList<DefensaAgregada> FlujogramaGenerator::agregarDefensas(const QVariantList& defensas) {
    // Aggregate by (EtapaId, Código)
    QMap<QPair<int, QString>, DefensaAgregada> agrupadas;
    
    for (const QVariant& var : defensas) {
        QVariantMap defensa = var.toMap();
        
        if (!defensa.contains("etapa_id")) continue;
        
        int etapaId = defensa["etapa_id"].toInt();
        QString codigo = defensa["codigo"].toString();
        
        if (codigo.isEmpty()) continue;
        
        QPair<int, QString> key(etapaId, codigo);
        
        if (!agrupadas.contains(key)) {
            DefensaAgregada nueva;
            nueva.etapaId = etapaId;
            nueva.codigo = codigo;
            nueva.nombre = defensa["nombre"].toString();
            nueva.robustez = defensa["robustez"].toInt();
            nueva.targets.insert(defensa["target_codigo"].toString());
            agrupadas[key] = nueva;
        } else {
            // Aggregate: max robustez, union targets
            DefensaAgregada& existente = agrupadas[key];
            existente.robustez = qMax(existente.robustez, defensa["robustez"].toInt());
            existente.targets.insert(defensa["target_codigo"].toString());
        }
    }
    
    return agrupadas.values();
}

void FlujogramaGenerator::calcularLayout(
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
    QMap<int, int>& posicionYPorEtapaFicticia) {
    
    qDebug() << "Calculating layout...";
    
    // Calculate heights and Y positions for real stages
    int yPos = 0;
    for (const QVariant& var : etapasReales) {
        QVariantMap etapa = var.toMap();
        int etapaId = etapa["id"].toInt();
        
        // Calculate height based on substages and initiators
        int altura = NODE_H_MIN;
        
        // Calculate total height needed for substages
        int totalSubsH = 0;
        QVariantList subs = subetapasPorEtapa.value(etapaId);
        if (!subs.isEmpty()) {
            for (const QVariant& subVar : subs) {
                QVariantMap subetapa = subVar.toMap();
                int subetapaId = subetapa["id"].toInt();
                
                // Calculate substage height based on its initiators
                QVariantList initsInSub = iniciadorsPorSubetapa.value(subetapaId);
                int subH = SUB_H_MIN;
                if (!initsInSub.isEmpty()) {
                    int totalInitH = 0;
                    for (const QVariant& initVar : initsInSub) {
                        QVariantMap init = initVar.toMap();
                        int initH = calcularAlturaTexto(init["nombre"].toString(), init["codigo"].toString());
                        totalInitH += initH;
                    }
                    totalInitH += INIT_GAP * (initsInSub.size() - 1);
                    subH = qMax(SUB_H_MIN, SUB_HEADER_PAD + totalInitH + BOTTOM_PAD);
                }
                
                totalSubsH += subH;
            }
            totalSubsH += SUB_GAP * (subs.size() - 1);
        }
        
        // Calculate total height needed for direct initiators (no substage)
        int directInitH = 0;
        QVariantList directInits = iniciadorDirectosPorEtapa.value(etapaId);
        if (!directInits.isEmpty()) {
            for (const QVariant& initVar : directInits) {
                QVariantMap init = initVar.toMap();
                int initH = calcularAlturaTexto(init["nombre"].toString(), init["codigo"].toString());
                directInitH += initH;
            }
            directInitH += INIT_GAP * (directInits.size() - 1);
        }
        
        // Add gap between substages and direct initiators if both exist
        int gapBetween = (totalSubsH > 0 && directInitH > 0) ? SUB_GAP : 0;
        
        // Calculate stage height based on content
        int contentHeight = totalSubsH + gapBetween + directInitH;
        altura = qMax(NODE_H_MIN, STAGE_HEADER_PAD + contentHeight + BOTTOM_PAD);
        
        // Also check heights needed for defensas in this stage
        auto calcDefensasHeight = [&](const QList<DefensaAgregada>& defensas) -> int {
            int totalH = 0;
            QList<DefensaAgregada> defsForStage;
            for (const DefensaAgregada& def : defensas) {
                if (def.etapaId == etapaId) {
                    defsForStage.append(def);
                }
            }
            if (!defsForStage.isEmpty()) {
                for (const DefensaAgregada& def : defsForStage) {
                    totalH += qMax(DEF_H_MIN, calcularAlturaTexto(def.nombre, def.codigo));
                }
                totalH += DEF_GAP * (defsForStage.size() - 1);
            }
            return totalH;
        };
        
        int barH = calcDefensasHeight(barrerasReales);
        int rfH = calcDefensasHeight(rfReales);
        int rcH = calcDefensasHeight(rcReales);
        
        // Stage height is the maximum of content height and defensa heights
        altura = qMax(altura, qMax(barH, qMax(rfH, rcH)));
        
        alturasPorEtapa[etapaId] = altura;
        posicionYPorEtapa[etapaId] = yPos;
        yPos += altura + V_GAP;
    }
    
    // Calculate heights and Y positions for fictional stages
    yPos = 0;
    for (const QVariant& var : etapasFicticias) {
        QVariantMap etapa = var.toMap();
        int etapaId = etapa["id"].toInt();
        
        // Calculate height based on defensas
        auto calcDefensasHeight = [&](const QList<DefensaAgregada>& defensas) -> int {
            int totalH = 0;
            QList<DefensaAgregada> defsForStage;
            for (const DefensaAgregada& def : defensas) {
                if (def.etapaId == etapaId) {
                    defsForStage.append(def);
                }
            }
            if (!defsForStage.isEmpty()) {
                for (const DefensaAgregada& def : defsForStage) {
                    totalH += qMax(DEF_H_MIN, calcularAlturaTexto(def.nombre, def.codigo));
                }
                totalH += DEF_GAP * (defsForStage.size() - 1);
            }
            return totalH;
        };
        
        int barH = calcDefensasHeight(barrerasFicticias);
        int rfH = calcDefensasHeight(rfFicticias);
        int rcH = calcDefensasHeight(rcFicticias);
        
        int altura = qMax(NODE_H_MIN, qMax(barH, qMax(rfH, rcH)));
        
        alturasPorEtapaFicticia[etapaId] = altura;
        posicionYPorEtapaFicticia[etapaId] = yPos;
        yPos += altura + V_GAP;
    }
    
    qDebug() << "Layout calculation complete";
}

QString FlujogramaGenerator::generarHTMLCompleto(
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
    const QString& modelName) {
    
    qDebug() << "Generating HTML...";
    
    QString nodes, edges, headers;
    QString fictionalNodes, fictionalEdges; // Separate storage for fictional stage elements
    QMap<QString, QPair<double, double>> iniciadorPositions; // iniciadorCode -> (centerX, centerY)
    
    // Generate real stage nodes with substages and initiators
    for (const QVariant& var : etapasReales) {
        QVariantMap etapa = var.toMap();
        int etapaId = etapa["id"].toInt();
        int y = posicionYPorEtapa.value(etapaId, 0);
        int h = alturasPorEtapa.value(etapaId, NODE_H_MIN);
        
        nodes += generarNodo(X_REAL, y, NODE_W, h, "real",
                            etapa["nombre"].toString(),
                            etapa["codigo"].toString(),
                            "Etapa", etapaId);
        
        // Generate substages for this stage
        double subY = y + STAGE_HEADER_PAD;
        QVariantList subs = subetapasPorEtapa.value(etapaId);
        for (const QVariant& subVar : subs) {
            QVariantMap subetapa = subVar.toMap();
            int subetapaId = subetapa["id"].toInt();
            
            // Calculate substage height based on its initiators
            QVariantList initsInSub = iniciadorsPorSubetapa.value(subetapaId);
            int subH = SUB_H_MIN;
            if (!initsInSub.isEmpty()) {
                int totalInitH = 0;
                for (const QVariant& initVar : initsInSub) {
                    QVariantMap init = initVar.toMap();
                    int initH = calcularAlturaTexto(init["nombre"].toString(), init["codigo"].toString());
                    totalInitH += initH;
                }
                totalInitH += INIT_GAP * (initsInSub.size() - 1);
                subH = qMax(SUB_H_MIN, SUB_HEADER_PAD + totalInitH + BOTTOM_PAD);
            }
            
            double subX = X_REAL + (NODE_W - SUB_W) / 2.0;
            nodes += generarNodo(subX, subY, SUB_W, subH, "subetapa",
                                subetapa["nombre"].toString(),
                                "", "Subetapa", etapaId,
                                "", QString("data-subetapa=\"%1\"").arg(subetapaId));
            
            // Generate initiators within this substage
            if (!initsInSub.isEmpty()) {
                double initX = subX + (SUB_W - INIT_W_INSIDE) / 2.0;
                double initY = subY + SUB_HEADER_PAD;
                
                for (const QVariant& initVar : initsInSub) {
                    QVariantMap init = initVar.toMap();
                    int iniciadorId = init["id"].toInt();
                    QString iniciadorCode = init["codigo"].toString();
                    int initH = calcularAlturaTexto(init["nombre"].toString(), init["codigo"].toString());
                    
                    QString riesgoStr = init["riesgo"].toString();
                    QString extraAttrs = QString("data-subetapa=\"%1\" data-riesgo=\"%2\"")
                                            .arg(subetapaId)
                                            .arg(escapeHTML(riesgoStr));
                    nodes += generarNodo(initX, initY, INIT_W_INSIDE, initH, "iniciador",
                                        init["nombre"].toString(),
                                        init["codigo"].toString(),
                                        "Iniciador", etapaId, init["codigo"].toString(),
                                        extraAttrs);
                    
                    // Store position for edge generation using code
                    iniciadorPositions[iniciadorCode] = qMakePair(initX + INIT_W_INSIDE / 2.0, initY + initH / 2.0);
                    qDebug() << "  Stored iniciador position:" << iniciadorCode << "at" << (initX + INIT_W_INSIDE / 2.0) << "," << (initY + initH / 2.0);
                    
                    initY += initH + INIT_GAP;
                }
            }
            
            subY += subH + SUB_GAP;
        }
        
        // Generate direct initiators (no substage)
        QVariantList directInits = iniciadorDirectosPorEtapa.value(etapaId);
        if (!directInits.isEmpty()) {
            double initX = X_REAL + (NODE_W - INIT_W_INSIDE) / 2.0;
            double initY = subY; // After substages
            
            for (const QVariant& initVar : directInits) {
                QVariantMap init = initVar.toMap();
                int iniciadorId = init["id"].toInt();
                QString iniciadorCode = init["codigo"].toString();
                int initH = calcularAlturaTexto(init["nombre"].toString(), init["codigo"].toString());
                
                QString riesgoStr = init["riesgo"].toString();
                QString extraAttrs = QString("data-riesgo=\"%1\"").arg(escapeHTML(riesgoStr));
                nodes += generarNodo(initX, initY, INIT_W_INSIDE, initH, "iniciador",
                                    init["nombre"].toString(),
                                    init["codigo"].toString(),
                                    "Iniciador", etapaId, init["codigo"].toString(),
                                    extraAttrs);
                
                // Store position for edge generation using code
                iniciadorPositions[iniciadorCode] = qMakePair(initX + INIT_W_INSIDE / 2.0, initY + initH / 2.0);
                qDebug() << "  Stored direct iniciador position:" << iniciadorCode << "at" << (initX + INIT_W_INSIDE / 2.0) << "," << (initY + initH / 2.0);
                
                initY += initH + INIT_GAP;
            }
        }
    }
    
    // Generate defensas nodes (barriers, RF, RC) for real stages
    auto generarDefensasColumna = [&](const QList<DefensaAgregada>& defensas, double colX, const QString& clase, const QString& tipo, const QString& prefix) {
        QMap<int, QList<DefensaAgregada>> defensasPorEtapa;
        for (const DefensaAgregada& def : defensas) {
            defensasPorEtapa[def.etapaId].append(def);
        }
        
        for (auto it = defensasPorEtapa.begin(); it != defensasPorEtapa.end(); ++it) {
            int etapaId = it.key();
            if (!posicionYPorEtapa.contains(etapaId)) continue;
            
            int y = posicionYPorEtapa.value(etapaId);
            int h = alturasPorEtapa.value(etapaId);
            
            // Calculate total height needed for defensas
            QList<DefensaAgregada> defs = it.value();
            int totalDefH = 0;
            QList<int> defHeights;
            for (const DefensaAgregada& def : defs) {
                int defH = qMax(DEF_H_MIN, calcularAlturaTexto(def.nombre, def.codigo));
                defHeights.append(defH);
                totalDefH += defH;
            }
            if (!defs.isEmpty()) {
                totalDefH += DEF_GAP * (defs.size() - 1);
            }
            
            // Center defensas vertically within stage height
            double defY = y + (h - totalDefH) / 2.0;
            
            for (int i = 0; i < defs.size(); i++) {
                const DefensaAgregada& def = defs[i];
                int defH = defHeights[i];
                QString key = QString("%1|%2|%3").arg(prefix).arg(etapaId).arg(def.codigo);
                
                nodes += generarNodo(colX, defY, DEF_W, defH, clase,
                                    def.nombre, def.codigo, tipo, etapaId, def.codigo,
                                    QString("data-rob=\"%1\" data-key=\"%2\"").arg(def.robustez).arg(key));
                
                // Generate edges from this defensa to its target iniciadores
                double defCenterY = defY + defH / 2.0;
                qDebug() << "Generating edges for" << prefix << def.codigo << "with" << def.targets.size() << "targets";
                for (const QString& targetCode : def.targets) {
                    if (iniciadorPositions.contains(targetCode)) {
                        auto pos = iniciadorPositions[targetCode];
                        double targetX = pos.first;
                        double targetY = pos.second;
                        
                        QString arrowClass = (prefix == "bar") ? "arrow-bar" : ((prefix == "rf") ? "arrow-rf" : "arrow-rc");
                        QString dataFrom = QString("%1:%2|%3").arg(prefix).arg(etapaId).arg(def.codigo);
                        QString dataTo = QString("init:%1").arg(targetCode);
                        
                        // Arrows from left (barriers) go to left edge of iniciador
                        // Arrows from right (RF/RC) go to right edge of iniciador
                        double edgeX1, edgeX2;
                        if (prefix == "bar") {
                            edgeX1 = colX + DEF_W; // Right edge of defensa
                            edgeX2 = targetX - INIT_W_INSIDE / 2.0; // Left edge of iniciador
                        } else {
                            edgeX1 = colX; // Left edge of defensa
                            edgeX2 = targetX + INIT_W_INSIDE / 2.0; // Right edge of iniciador
                        }
                        
                        edges += generarFlecha(edgeX1, defCenterY, edgeX2, targetY, arrowClass, dataFrom, dataTo);
                        qDebug() << "  Edge generated:" << dataFrom << "->" << dataTo;
                    } else {
                        qDebug() << "  Target" << targetCode << "not found in iniciadorPositions";
                    }
                }
                
                defY += defH + DEF_GAP;
            }
        }
    };
    
    generarDefensasColumna(barrerasReales, X_BAR_REAL, "barrera", "Barrera", "bar");
    generarDefensasColumna(rfReales, X_RF_REAL, "rf", "Reductor de Frecuencia", "rf");
    generarDefensasColumna(rcReales, X_RC_REAL, "rc", "Reductor de Consecuencias", "rc");
    
    // Generate fictional stages (left and right)
    for (const QVariant& var : etapasFicticias) {
        QVariantMap etapa = var.toMap();
        int etapaId = etapa["id"].toInt();
        if (!posicionYPorEtapaFicticia.contains(etapaId)) continue;
        
        int y = posicionYPorEtapaFicticia.value(etapaId);
        int h = alturasPorEtapaFicticia.value(etapaId);
        
        // Left fictional stage
        fictionalNodes += generarNodo(X_FICT, y, NODE_W, h, "ficticia",
                            etapa["nombre"].toString(),
                            etapa["codigo"].toString(),
                            "Etapa Ficticia", etapaId);
        
        // Right fictional stage (mirror)
        fictionalNodes += generarNodo(X_FICT_R, y, NODE_W, h, "ficticia",
                            etapa["nombre"].toString(),
                            etapa["codigo"].toString(),
                            "Etapa Ficticia", etapaId);
    }
    
    // Generate defensas for fictional stages
    auto generarDefensasFicticias = [&](const QList<DefensaAgregada>& defensas, double colX, const QString& clase, const QString& tipo, const QString& prefix) {
        QMap<int, QList<DefensaAgregada>> defensasPorEtapa;
        for (const DefensaAgregada& def : defensas) {
            defensasPorEtapa[def.etapaId].append(def);
        }
        
        for (auto it = defensasPorEtapa.begin(); it != defensasPorEtapa.end(); ++it) {
            int etapaId = it.key();
            if (!posicionYPorEtapaFicticia.contains(etapaId)) continue;
            
            int y = posicionYPorEtapaFicticia.value(etapaId);
            int h = alturasPorEtapaFicticia.value(etapaId);
            
            // Calculate total height needed for defensas
            QList<DefensaAgregada> defs = it.value();
            int totalDefH = 0;
            QList<int> defHeights;
            for (const DefensaAgregada& def : defs) {
                int defH = qMax(DEF_H_MIN, calcularAlturaTexto(def.nombre, def.codigo));
                defHeights.append(defH);
                totalDefH += defH;
            }
            if (!defs.isEmpty()) {
                totalDefH += DEF_GAP * (defs.size() - 1);
            }
            
            // Center defensas vertically within stage height
            double defY = y + (h - totalDefH) / 2.0;
            
            for (int i = 0; i < defs.size(); i++) {
                const DefensaAgregada& def = defs[i];
                int defH = defHeights[i];
                QString key = QString("%1|%2|%3").arg(prefix).arg(etapaId).arg(def.codigo);
                
                fictionalNodes += generarNodo(colX, defY, DEF_W, defH, clase,
                                    def.nombre, def.codigo, tipo, etapaId, def.codigo,
                                    QString("data-rob=\"%1\" data-key=\"%2\"").arg(def.robustez).arg(key));
                
            // Generate edges from this defensa to its target iniciadores
                double defCenterY = defY + defH / 2.0;
                qDebug() << "Generating edges for" << prefix << def.codigo << "with" << def.targets.size() << "targets";
                for (const QString& targetCode : def.targets) {
                    if (iniciadorPositions.contains(targetCode)) {
                        auto pos = iniciadorPositions[targetCode];
                        double targetX = pos.first;
                        double targetY = pos.second;
                        
                        QString arrowClass = (prefix == "bar") ? "arrow-bar" : ((prefix == "rf") ? "arrow-rf" : "arrow-rc");
                        QString dataFrom = QString("%1:%2|%3").arg(prefix).arg(etapaId).arg(def.codigo);
                        QString dataTo = QString("init:%1").arg(targetCode);
                        
                        // Arrows from left (barriers) go to left edge of iniciador
                        // Arrows from right (RF/RC) go to right edge of iniciador
                        double edgeX1, edgeX2;
                        if (prefix == "bar") {
                            edgeX1 = colX + DEF_W; // Right edge of defensa
                            edgeX2 = targetX - INIT_W_INSIDE / 2.0; // Left edge of iniciador
                        } else {
                            edgeX1 = colX; // Left edge of defensa
                            edgeX2 = targetX + INIT_W_INSIDE / 2.0; // Right edge of iniciador
                        }
                        
                        fictionalEdges += generarFlecha(edgeX1, defCenterY, edgeX2, targetY, arrowClass, dataFrom, dataTo);
                        qDebug() << "  Edge generated:" << dataFrom << "->" << dataTo;
                    } else {
                        qDebug() << "  Target" << targetCode << "not found in iniciadorPositions";
                    }
                }
                
                defY += defH + DEF_GAP;
            }
        }
    };
    
    generarDefensasFicticias(barrerasFicticias, X_BAR_FICT, "barrera", "Barrera", "bar");
    generarDefensasFicticias(rfFicticias, X_RF_FICT, "rf", "Reductor de Frecuencia", "rf");
    generarDefensasFicticias(rcFicticias, X_RC_FICT, "rc", "Reductor de Consecuencias", "rc");
    
    // Generate column headers
    auto addHeader = [&](double x, const QString& text) {
        headers += QString("<text class=\"colhdr\" x=\"%1\" y=\"-80\">%2</text>")
            .arg(x)
            .arg(escapeHTML(text));
    };
    
    // Check if there are fictional stages
    bool hasFictionalStages = !etapasFicticias.isEmpty();
    
    // Headers for fictional stage columns (only if fictional stages exist)
    if (hasFictionalStages) {
        addHeader(X_FICT + NODE_W / 2.0, "ETAPAS DE APOYO");
        addHeader(X_BAR_FICT + DEF_W / 2.0, "BARRERAS");
    }
    
    // Headers for real stage columns (always shown)
    addHeader(X_BAR_REAL + DEF_W / 2.0, "BARRERAS");
    addHeader(X_REAL + NODE_W / 2.0, "ETAPAS / SUBETAPAS / INICIADORES");
    addHeader(X_RF_REAL + DEF_W / 2.0, "REDUCTORES FRECUENCIA");
    addHeader(X_RC_REAL + DEF_W / 2.0, "REDUCTORES CONSECUENCIAS");
    
    // Headers for fictional stage defenses (only if fictional stages exist)
    if (hasFictionalStages) {
        addHeader(X_RF_FICT + DEF_W / 2.0, "REDUCTORES FRECUENCIA");
        addHeader(X_RC_FICT + DEF_W / 2.0, "REDUCTORES CONSECUENCIAS");
        addHeader(X_FICT_R + NODE_W / 2.0, "ETAPAS DE APOYO");
    }
    
    // Generate option lists for filters (ordered by id)
    QString optInits, optDefs;
    
    // Sort etapas by id for dropdowns
    QVariantList etapasRealesOrdenadas = etapasReales;
    std::sort(etapasRealesOrdenadas.begin(), etapasRealesOrdenadas.end(), 
              [](const QVariant& a, const QVariant& b) {
                  return a.toMap()["id"].toInt() < b.toMap()["id"].toInt();
              });
    
    for (const QVariant& var : etapasRealesOrdenadas) {
        QVariantMap etapa = var.toMap();
        optInits += QString("<option value=\"%1\">%2</option>")
            .arg(etapa["id"].toInt())
            .arg(escapeHTML(etapa["nombre"].toString()));
    }
    
    for (const QVariant& var : etapasRealesOrdenadas) {
        QVariantMap etapa = var.toMap();
        optDefs += QString("<option value=\"%1\">%2</option>")
            .arg(etapa["id"].toInt())
            .arg(escapeHTML(etapa["nombre"].toString()));
    }
    
    // Sort fictional etapas by id for dropdowns
    QVariantList etapasFicticiasOrdenadas = etapasFicticias;
    std::sort(etapasFicticiasOrdenadas.begin(), etapasFicticiasOrdenadas.end(), 
              [](const QVariant& a, const QVariant& b) {
                  return a.toMap()["id"].toInt() < b.toMap()["id"].toInt();
              });
    
    for (const QVariant& var : etapasFicticiasOrdenadas) {
        QVariantMap etapa = var.toMap();
        optDefs += QString("<option value=\"%1\">%2</option>")
            .arg(etapa["id"].toInt())
            .arg(escapeHTML(etapa["nombre"].toString()));
    }
    
    // Calculate canvas dimensions
    int width = X_FICT_R + NODE_W + 400;
    int maxRealStageY = posicionYPorEtapa.isEmpty() ? 0 : posicionYPorEtapa.last() + alturasPorEtapa.last();
    int maxFictionalStageY = posicionYPorEtapaFicticia.isEmpty() ? 0 : posicionYPorEtapaFicticia.last() + alturasPorEtapaFicticia.last();
    int height = qMax(maxRealStageY, maxFictionalStageY) + 720;
    
    // Build complete HTML
    QString html = QString(R"HTML(<!DOCTYPE html>
<html lang="es"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>Flujograma vertical — v25 fix</title>
%1
</head>
<body>
<div id="wrap">
  <svg id="canvas" data-initcol="%2" data-maxrealy="%12" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 %3 %4" role="img" aria-label="flujograma">
    <defs><marker id="arrowhead" markerWidth="10" markerHeight="7" refX="8" refY="3.5" orient="auto"><polygon points="0 0, 10 3.5, 0 7" fill="#0f172a"/></marker></defs>
    <g id="viewport" transform="translate(240,160) scale(1)">
      <g id="edges">%5</g>
      <g id="nodes">%6</g>
      <g id="fictional-group" transform="translate(0, 0)">
        <g id="fictional-nodes">%14</g>
      </g>
      <g id="fictional-edges">%13</g>
      <g id="headers">%7</g>
    </g>
  </svg>

  <div id="panel" class="panel">
    <div class="handle">
      <span>%11</span>
      <button id="toggle-panel" class="btn">▾</button>
    </div>
    <div class="content">
      <div class="card">
        <h4>Filtrar por etapas</h4>
        <div class="grid2">
          <div><label class="small">Etapa de <b>sucesos</b></label><select id="f-etapa-inits">
            <option value="">(todas)</option>%8
          </select></div>
          <div><label class="small">Etapa de <b>defensas</b></label><select id="f-etapa-defs">
            <option value="">(todas)</option>%9
          </select></div>
        </div>
      </div>

      <div class="card">
        <h4>Mostrar</h4>
        <div class="row">
          <label class="small"><input type="checkbox" id="show-bar" checked> Barreras</label>
          <label class="small"><input type="checkbox" id="show-rf" checked> Reductores Frecuencia</label>
          <label class="small"><input type="checkbox" id="show-rc" checked> Reductores Consecuencias</label>
        </div>
        <div class="row">
          <label class="small"><input type="checkbox" id="only-defs-on-visible"> Solo defensas que actúan sobre sucesos visibles</label>
        </div>
      </div>

      <div class="card">
        <div class="row"><h4>Búsqueda</h4></div>
        <input type="text" style="width:70%" id="f-texto" placeholder="Nombre o código a buscar…">
      </div>

      <div class="card">
        <h4>Robustez (todas las defensas)</h4>
        <div class="row" id="robbox">
          <label class="small"><input type="checkbox" class="rob" value="-1" checked> -1 muy blanda</label>
          <label class="small"><input type="checkbox" class="rob" value="0" checked> 0 blanda</label>
          <label class="small"><input type="checkbox" class="rob" value="1" checked> 1 normal</label>
          <label class="small"><input type="checkbox" class="rob" value="2" checked> 2 robusta</label>
          <label class="small"><input type="checkbox" class="rob" value="3" checked> 3 muy robusta</label>
        </div>
      </div>

      <div class="card">
        <h4>Riesgo (sucesos iniciadores)</h4>
        <div class="row" id="riesgobox">
          <label class="small"><input type="checkbox" class="riesgo" value="RB" checked> RB bajo</label>
          <label class="small"><input type="checkbox" class="riesgo" value="RM" checked> RM medio</label>
          <label class="small"><input type="checkbox" class="riesgo" value="RA" checked> RA alto</label>
          <label class="small"><input type="checkbox" class="riesgo" value="RMA" checked> RMA muy alto</label>
        </div>
      </div>

      <div class="footer">
        <div class="row">
          <button id="zoom-in" class="btn">+</button>
          <button id="zoom-out" class="btn">−</button>
          <button id="reset" class="btn">Reiniciar</button>
        </div>
        <div class="instructions">
          <small><b>Zoom:</b> Rueda del ratón | <b>Pan:</b> Botón izquierdo + Arrastrar | <b>Mover etapas ficticias:</b> Botón derecho + Arrastrar | <b>Pan rápido:</b> Shift + Arrastrar</small>
        </div>
        <div class="row"><span id="js-status" class="badge">JS listo</span></div>
      </div>

      <div class="counts">Iniciadores visibles: <b id="count-inits">–</b> · Barreras visibles: <b id="count-bar">–</b> · RF visibles: <b id="count-rf">–</b> · RC visibles: <b id="count-rc">–</b></div>
      <div class="counts"><b>Riesgos</b>:   RB:   <b id="count-rb">–</b>   ·   RM:   <b id="count-rm">–</b>   ·   RA:   <b id="count-ra">–</b>   ·   RMA:   <b id="count-rma">–</b></div>
    </div>
  </div>
  %10
</body></html>
)HTML")
        .arg(obtenerPlantillaCSS())
        .arg(INIT_COL_CENTER)
        .arg(width)
        .arg(height)
        .arg(edges)
        .arg(nodes)
        .arg(headers)
        .arg(optInits)
        .arg(optDefs)
        .arg(obtenerPlantillaJavaScript())
        .arg(escapeHTML(modelName))
        .arg(maxRealStageY)
        .arg(fictionalEdges)
        .arg(fictionalNodes);
    
    return html;
}


QString FlujogramaGenerator::obtenerPlantillaCSS() {
    return R"CSS(<style>
:root{--card:#fff;--bd:#e5e7eb;--txt:#0f172a;--muted:#475569;--bg:#f4f6fb}
body{margin:0;font-family:system-ui,-apple-system,Segoe UI,Roboto,Arial,sans-serif;background:var(--bg)}
#wrap{height:100vh;width:100vw;overflow:hidden}
svg{width:100%;height:100%;display:block;cursor:grab;user-select:none;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none}svg:active{cursor:grabbing}
.node rect{fill:#fff;stroke:var(--txt);stroke-width:1.2}
.node.real rect{fill:#dbeafe}.node.subetapa rect{fill:#eff6ff;stroke-dasharray:3 2}.node.iniciador rect{fill:#fff7ed}
.node.ficticia rect{fill:#e5e7eb}
.node.barrera rect,.node.rf rect,.node.rc rect{fill:#ffffff}
.node.iniciador[data-riesgo="RB"] rect{fill:#e0e0e0}
.node.iniciador[data-riesgo="RM"] rect{fill:#d4f1d4}
.node.iniciador[data-riesgo="RA"] rect{fill:#ffe4b3}
.node.iniciador[data-riesgo="RMA"] rect{fill:#ffd6d6}
.node.barrera[data-rob="-1"] rect,.node.rf[data-rob="-1"] rect,.node.rc[data-rob="-1"] rect{fill:#f3f4f6}
.node.barrera[data-rob="0"] rect,.node.rf[data-rob="0"] rect,.node.rc[data-rob="0"] rect{fill:#ffffff}
.node.barrera[data-rob="1"] rect,.node.rf[data-rob="1"] rect,.node.rc[data-rob="1"] rect{fill:#eefdd3}
.node.barrera[data-rob="2"] rect,.node.rf[data-rob="2"] rect,.node.rc[data-rob="2"] rect{fill:#ffedd5}
.node.barrera[data-rob="3"] rect,.node.rf[data-rob="3"] rect,.node.rc[data-rob="3"] rect{fill:#fee2e2}
.node .title{font-size:16px;font-weight:600;text-anchor:middle;dominant-baseline:hanging;fill:var(--txt)}
.node .subtitle{font-size:16px;fill:#334155;text-anchor:middle;dominant-baseline:hanging}
.node:focus rect,.node:hover rect{stroke-width:2}
.arrow{stroke:var(--txt);stroke-width:1;opacity:.5;fill:none}
.arrow-bar{stroke:#000000}
.arrow-rf{stroke:#16a34a}
.arrow-rc{stroke:#2563eb}
.arrow.focus{opacity:1; stroke-width:2.6}
.arrow.dim{opacity:.20}
.arrow.dimx{opacity:.08}
.dim{opacity:.25} .dimx{opacity:.12} .hi{outline:2px solid #2563eb}
.colhdr{font-weight:400;fill:#111827;text-anchor:middle;font-size:22px}
.panel{position:fixed;right:12px;top:12px;background:var(--card);border:1px solid var(--bd);border-radius:12px;box-shadow:0 2px 12px rgba(0,0,0,.08);z-index:5;max-width:520px;min-width:360px}
.panel .handle{cursor:move;background:#f8fafc;border-bottom:1px solid var(--bd);padding:7.5px 12px;border-top-left-radius:12px;border-top-right-radius:12px;font-weight:525;display:flex;justify-content:space-between;align-items:center}
.panel .content{padding:10px 12px;display:grid;gap:10px}
.panel.collapsed .content{display:none}
.card{border:1px solid var(--bd);border-radius:10px;padding:8px 10px}
.card h4{margin:0 0 6px 0;font-size:13px;color:var(--muted)}
.grid2{display:grid;grid-template-columns:1fr 1fr;gap:8px}
.grid3{display:grid;grid-template-columns:1fr 1fr 1fr;gap:8px}
.row{display:flex;gap:8px;align-items:center;flex-wrap:wrap}
label.small{font-size:12px;color:#374151}
select,input[type=\"text\"]{width:100%;padding:6px 8px;border:1px solid var(--bd);border-radius:8px;font-size:13px}
button.btn{padding:6px 10px;border-radius:8px;border:1px solid var(--bd);background:#f9fafb;cursor:pointer}
.badge{display:inline-block;padding:2px 6px;border-radius:6px;border:1px solid var(--bd);font-size:11px;color:var(--muted)}
.footer{display:flex;justify-content:space-between;align-items:center;gap:8px;flex-wrap:wrap}
.instructions{font-size:13px;color:#64748b;line-height:1.4}
.counts{font-size:12px;color:#374151}
.panel.bottom{left:50%;transform:translateX(-50%);right:auto;top:auto;bottom:12px}
</style>)CSS";
}

QString FlujogramaGenerator::obtenerPlantillaJavaScript() {
    // Split JavaScript into multiple parts to avoid MSVC string literal size limit (C2026)
    QString js1 = R"JS(<script>
(function(){
  const svg = document.getElementById('canvas');
  const vp  = document.getElementById('viewport');
  const fictionalGroup = document.getElementById('fictional-group');
  const fictionalEdgesGroup = document.getElementById('fictional-edges');
  const statusEl = document.getElementById('js-status');
  function setStatus(t){ if(statusEl) statusEl.textContent = t; }

  let INIT_COL_X = parseFloat(svg.getAttribute('data-initcol')) || 0;
  const MAX_REAL_Y = parseFloat(svg.getAttribute('data-maxrealy')) || 0;
  let scale = 1, minScale = 0.12, maxScale = 18, tx = 240, ty = 160;
  let dragging=false, rightDragging=false, lx=0, ly=0;
  let fictOffset = 0;  // Offset for fictional stages
  const PAN_K = 8.0, TURBO = 2.0;
  function state(){ return `scale=${scale.toFixed(2)} tx=${tx.toFixed(0)} ty=${ty.toFixed(0)} initx=${INIT_COL_X.toFixed(0)}`; }
  function update(){ vp.setAttribute('transform', 'translate(' + tx + ',' + ty + ') scale(' + scale + ')'); setStatus(state()); }
  
  function updateFictionalOffset() {
    if (fictionalGroup) {
      fictionalGroup.setAttribute('transform', 'translate(0, ' + fictOffset + ')');
    }
    
    // Update fictional edges to connect moving defenses to static initiators
    if (fictionalEdgesGroup) {
      const edges = fictionalEdgesGroup.querySelectorAll('.arrow');
      edges.forEach(function(edge) {
        const dataFrom = edge.getAttribute('data-from') || '';
        // Only update edges from fictional defenses (bar, rf, rc with fictional stages)
        if (dataFrom.startsWith('bar:') || dataFrom.startsWith('rf:') || dataFrom.startsWith('rc:')) {
          const dAttr = edge.getAttribute('d');
          if (dAttr) {
            // Parse the path: "M x1 y1 L x2 y2"
            const match = dAttr.match(/M\s+([\d.]+)\s+([\d.]+)\s+L\s+([\d.]+)\s+([\d.]+)/);
            if (match) {
              const x1 = parseFloat(match[1]);
              const y1Original = parseFloat(match[2]);
              const x2 = parseFloat(match[3]);
              const y2 = parseFloat(match[4]); // Target Y stays the same (iniciador)
              
              // Get original Y1 from data attribute (stored on first load)
              let originalY1 = parseFloat(edge.getAttribute('data-original-y1'));
              if (isNaN(originalY1)) {
                // First time - store the original Y
                originalY1 = y1Original;
                edge.setAttribute('data-original-y1', originalY1);
              }
              
              // Apply offset to Y1 (defense position)
              const newY1 = originalY1 + fictOffset;
              const newPath = 'M ' + x1 + ' ' + newY1 + ' L ' + x2 + ' ' + y2;
              edge.setAttribute('d', newPath);
            }
          }
        }
      });
    }
  }
)JS";

    QString js2 = R"JS(

  function zoomAroundInitCol(delta, anchorY){
    anchorY = anchorY || null;
    const ns = Math.min(maxScale, Math.max(minScale, scale + delta));
    if(ns===scale) return;
    const screenX_init = tx + scale * INIT_COL_X;
    tx = screenX_init - ns * INIT_COL_X;
    const rect = svg.getBoundingClientRect();
    const screenY = (anchorY==null) ? (rect.top + rect.height/2) : anchorY;
    const worldY = (screenY - ty) / scale;
    ty = screenY - ns * worldY;
    scale = ns; update();
  }

  svg.addEventListener('wheel', function(e){ e.preventDefault(); const d = -Math.sign(e.deltaY)*0.22; zoomAroundInitCol(d, e.clientY); }, {passive:false});
  document.getElementById('zoom-in').addEventListener('click', function(){ zoomAroundInitCol(+0.35); });
  document.getElementById('zoom-out').addEventListener('click', function(){ zoomAroundInitCol(-0.35); });
  document.getElementById('reset').addEventListener('click', function(){ scale=1; tx=240; ty=160; fictOffset=0; update(); updateFictionalOffset(); });

  svg.addEventListener('mousedown', function(e){ 
    if(e.button === 2) { 
      // Right mouse button - move fictional stages vertically
      e.preventDefault(); 
      rightDragging=true; 
      lx=e.clientX; 
      ly=e.clientY; 
    } else if(e.button === 0) { 
      // Left mouse button - pan viewport in all directions
      dragging=true; 
      lx=e.clientX; 
      ly=e.clientY; 
    } 
  });
  
  svg.addEventListener('contextmenu', function(e){ e.preventDefault(); }); // Prevent context menu on right click
  
  window.addEventListener('mousemove', function(e){ 
    if(rightDragging) { 
      // Right mouse button - move fictional stages only
      const dy=(e.clientY-ly)*PAN_K; 
      lx=e.clientX;  // Update lx to keep it in sync
      ly=e.clientY; 
      fictOffset+=dy; 
      // Limit movement: cannot go above Y=0 initial position (fictOffset < 0)
      if(fictOffset < 0) fictOffset = 0;
      updateFictionalOffset(); 
    } else if(dragging) { 
      // Left mouse button - pan in all directions
      const turbo = e.shiftKey ? TURBO : 1.0; 
      const dx=(e.clientX-lx)*PAN_K*turbo, dy=(e.clientY-ly)*PAN_K*turbo; 
      lx=e.clientX; 
      ly=e.clientY; 
      tx+=dx; 
      ty+=dy; 
      update(); 
    } 
  });
  
  window.addEventListener('mouseup', function(){ dragging=false; rightDragging=false; });

  const fEtapaInits=document.getElementById('f-etapa-inits'),
        fEtapaDefs =document.getElementById('f-etapa-defs'),
        fTexto     =document.getElementById('f-texto'),
        onlyDefsOnVisible =document.getElementById('only-defs-on-visible'),
        robChecks  = Array.from(document.querySelectorAll('.rob')),
        riesgoChecks = Array.from(document.querySelectorAll('.riesgo')),
        showBar    = document.getElementById('show-bar'),
        showRF     = document.getElementById('show-rf'),
        showRC     = document.getElementById('show-rc');

  function isVisible(el){ return el && el.style.display !== 'none'; }
  function nodeByKey(key){
    const parts = (key||'').split(':');
    const type = parts[0];
    const rest = parts[1];
    if(type==='init'){ return document.querySelector('.node.iniciador[data-code="'+rest+'"]'); }
    if(type==='bar' || type==='rf' || type==='rc'){
      return document.querySelector('.node[data-key="'+type+'|'+rest+'"]');
    }
    return null;
  }
)JS";

    QString js3 = R"JS(
  function getVisibleInitiators(){
    const ids = new Set();
    document.querySelectorAll('.node.iniciador').forEach(function(el){ if(isVisible(el)){ ids.add(el.getAttribute('data-code')); } });
    return ids;
  }
  function updateCounters(){
    const nInits = Array.from(document.querySelectorAll('.node.iniciador')).filter(isVisible).length;
    const nBar  = Array.from(document.querySelectorAll('.node.barrera')).filter(isVisible).length;
    const nRF  = Array.from(document.querySelectorAll('.node.rf')).filter(isVisible).length;
    const nRC  = Array.from(document.querySelectorAll('.node.rc')).filter(isVisible).length;
    
    // Count iniciadores by risk level
    const nRB = Array.from(document.querySelectorAll('.node.iniciador[data-riesgo="RB"]')).filter(isVisible).length;
    const nRM = Array.from(document.querySelectorAll('.node.iniciador[data-riesgo="RM"]')).filter(isVisible).length;
    const nRA = Array.from(document.querySelectorAll('.node.iniciador[data-riesgo="RA"]')).filter(isVisible).length;
    const nRMA = Array.from(document.querySelectorAll('.node.iniciador[data-riesgo="RMA"]')).filter(isVisible).length;
    
    const ci=document.getElementById('count-inits'), cbar=document.getElementById('count-bar'), crf=document.getElementById('count-rf'), crc=document.getElementById('count-rc');
    if(ci) ci.textContent = nInits; 
    if(cbar) cbar.textContent = nBar;
    if(crf) crf.textContent = nRF;
    if(crc) crc.textContent = nRC;
    
    // Update risk level counters
    const crb=document.getElementById('count-rb'), crm=document.getElementById('count-rm'), cra=document.getElementById('count-ra'), crma=document.getElementById('count-rma');
    if(crb) crb.textContent = nRB;
    if(crm) crm.textContent = nRM;
    if(cra) cra.textContent = nRA;
    if(crma) crma.textContent = nRMA;
  }

  function showEdgeIfNodesVisible(l){
    const kf=l.getAttribute('data-from')||''; const kt=l.getAttribute('data-to')||'';
    const nf=nodeByKey(kf); const nt=nodeByKey(kt);
    const visNodes = (!kf||(nf&&isVisible(nf))) && (!kt||(nt&&isVisible(nt)));
    let visType = true;
    if(l.classList.contains('arrow-bar') && !showBar.checked) visType=false;
    if(l.classList.contains('arrow-rf')  && !showRF.checked)  visType=false;
    if(l.classList.contains('arrow-rc')  && !showRC.checked)  visType=false;
    l.style.display = (visNodes && visType) ? '' : 'none';
  }

  function apply(){
    const etInits=fEtapaInits.value; 
    const etDefs =fEtapaDefs.value; 
    const q=(fTexto.value||'').toLowerCase();
    const robSelected = new Set(robChecks.filter(function(c){return c.checked;}).map(function(c){return c.value;}));
    const riesgoSelected = new Set(riesgoChecks.filter(function(c){return c.checked;}).map(function(c){return c.value;}));
    const onlyOnVisible = onlyDefsOnVisible.checked;

    document.querySelectorAll('.node').forEach(function(el){
      const tipo=el.getAttribute('data-tipo')||'';
      const name=(el.getAttribute('data-name')||'').toLowerCase();
      const code=(el.getAttribute('data-code')||'').toLowerCase();
      const eid =el.getAttribute('data-etapa')||'';
      let vis=true;

      if(tipo==='Barrera' || tipo==='Reductor de Frecuencia' || tipo==='Reductor de Consecuencias'){
        const rob = el.getAttribute('data-rob')||'';
        if(!robSelected.has(rob)) vis=false;
        if(etDefs && eid && eid!=etDefs) vis=false;
        if(tipo==='Barrera' && !showBar.checked) vis=false;
        if(tipo==='Reductor de Frecuencia' && !showRF.checked) vis=false;
        if(tipo==='Reductor de Consecuencias' && !showRC.checked) vis=false;
      }

      if(tipo==='Iniciador'){
        const riesgo = el.getAttribute('data-riesgo')||'';
        if(riesgo && !riesgoSelected.has(riesgo)) vis=false;
        if(etInits && eid && eid!=etInits) vis=false;
      }

      if((tipo==='Subetapa' || tipo==='Etapa')){
        if(etInits && eid && eid!=etInits) vis=false;
      }
      
      // Fictional stages (Etapa Ficticia) should NOT be hidden when filtering by stage
      // They remain visible regardless of etInits filter

      if(q && !(name.includes(q)||code.includes(q))) vis=false;
      el.style.display = vis ? '' : 'none';
    });

    if(onlyOnVisible){
      const visibleInits = getVisibleInitiators();
      const involvedDefs = new Set();
      document.querySelectorAll('.arrow').forEach(function(edge){
        const to=edge.getAttribute('data-to')||'';
        const from=edge.getAttribute('data-from')||'';
        const partsTo=to.split(':'), partsFrom=from.split(':');
        if(partsTo[0]==='init' && visibleInits.has(partsTo[1]) && (partsFrom[0]==='bar' || partsFrom[0]==='rf' || partsFrom[0]==='rc')){
          involvedDefs.add(partsFrom[0]+':'+partsFrom[1]);
        }
      });
      document.querySelectorAll('.node.barrera, .node.rf, .node.rc').forEach(function(b){
        const type = b.classList.contains('barrera')?'bar':(b.classList.contains('rf')?'rf':'rc');
        const key = b.getAttribute('data-key')||'';
        const keyShort = key.split('|').slice(1).join('|'); // EtapaId|Codigo
        if(b.style.display !== 'none' && !involvedDefs.has(type+':'+keyShort)){
          b.style.display='none';
        }
      });
    }

    document.querySelectorAll('.arrow').forEach(showEdgeIfNodesVisible);
    updateCounters();
  }
)JS";

    QString js4 = R"JS(

  function clearHighlights(){
    document.querySelectorAll('.node.barrera, .node.rf, .node.rc, .node.iniciador').forEach(function(el){ el.classList.remove('dim','dimx','hi'); });
    document.querySelectorAll('.arrow').forEach(function(el){ el.classList.remove('dim','dimx','focus'); });
  }

  // Selección extendida: Barreras, RF y RC participan
  function bindInteractions(){
    // Clic en iniciador
    document.querySelectorAll('.node.iniciador').forEach(function(init){
      init.addEventListener('click', function(e){
        e.stopPropagation();
        const iid = init.getAttribute('data-code');
        const involved = new Set();
        document.querySelectorAll('.arrow[data-to="init:'+iid+'"]').forEach(function(edge){
          const kf=edge.getAttribute('data-from')||'';
          const parts=kf.split(':');
          if(parts[0]==='bar' || parts[0]==='rf' || parts[0]==='rc') involved.add(kf);
        });
        clearHighlights();
        document.querySelectorAll('.node.barrera, .node.rf, .node.rc').forEach(function(dn){
          const type = dn.classList.contains('barrera')?'bar':(dn.classList.contains('rf')?'rf':'rc');
          const key  = dn.getAttribute('data-key');
          const keyShort = key.split('|').slice(1).join('|');
          const combo = type+':'+keyShort;
          if(!involved.has(combo)){ dn.classList.add('dim'); } else { dn.classList.add('hi'); }
        });
        document.querySelectorAll('.arrow').forEach(function(edge){
          const from=edge.getAttribute('data-from')||'';
          const to=edge.getAttribute('data-to')||'';
          if(to==='init:'+iid && involved.has(from)) edge.classList.add('focus');
          else edge.classList.add('dim');
        });
      });
    });
    // Clic en defensa
    document.querySelectorAll('.node.barrera, .node.rf, .node.rc').forEach(function(defNode){
      defNode.addEventListener('click', function(e){
        e.stopPropagation();
        const type = defNode.classList.contains('barrera')?'bar':(defNode.classList.contains('rf')?'rf':'rc');
        const key  = defNode.getAttribute('data-key');
        const keyShort = key.split('|').slice(1).join('|');
        const combo = type+':'+keyShort;
        const connectedInits = new Set();
        document.querySelectorAll('.arrow').forEach(function(edge){
          const from=edge.getAttribute('data-from')||'';
          const to=edge.getAttribute('data-to')||'';
          if(from===combo){
            const parts=to.split(':'); if(parts[0]==='init' && parts[1]) connectedInits.add(parts[1]);
          }
        });
        clearHighlights();
        document.querySelectorAll('.node.barrera, .node.rf, .node.rc').forEach(function(dn){
          const t = dn.classList.contains('barrera')?'bar':(dn.classList.contains('rf')?'rf':'rc');
          const k = dn.getAttribute('data-key');
          const kShort = k.split('|').slice(1).join('|');
          if(t+':'+kShort!==combo){ dn.classList.add('dimx'); } else { dn.classList.add('hi'); }
        });
        document.querySelectorAll('.arrow').forEach(function(edge){
          const from=edge.getAttribute('data-from')||'';
          if(from===combo) edge.classList.add('focus');
          else edge.classList.add('dimx');
        });
        document.querySelectorAll('.node.iniciador').forEach(function(init){
          const iid = init.getAttribute('data-code');
          if(!connectedInits.has(iid)) init.classList.add('dimx');
        });
      });
    });
    document.getElementById('wrap').addEventListener('click', function(){ clearHighlights(); });
  }
)JS";

    QString js5 = R"JS(

  // Tooltip
  const tooltip = (function(){
    const el = document.createElement('div'); el.id='tooltip'; el.style.position='fixed'; el.style.pointerEvents='none';
    el.style.background='rgba(15,23,42,.92)'; el.style.color='#fff'; el.style.padding='8px 10px'; el.style.borderRadius='8px'; el.style.fontSize='12px';
    el.style.lineHeight='1.35'; el.style.transform='translate(-50%,-120%)'; el.style.opacity='0'; el.style.transition='opacity .15s'; document.body.appendChild(el); return el;
  })();
  function showTip(evt, el){
    const name=el.getAttribute('data-name')||'', code=el.getAttribute('data-code')||'', tipo=el.getAttribute('data-tipo')||'', etapa=el.getAttribute('data-etapa')||'';
    const rob=el.getAttribute('data-rob'); 
    const riesgo=el.getAttribute('data-riesgo');
    const robtxt = (rob==='-1')?'muy blanda':(rob==='0')?'blanda':(rob==='1')?'normal':(rob==='2')?'robusta':(rob==='3')?'muy robusta':'';
    const riesgotxt = (riesgo==='RB')?'Bajo':(riesgo==='RM')?'Medio':(riesgo==='RA')?'Alto':(riesgo==='RMA')?'Muy Alto':'';
    tooltip.innerHTML = '<strong>'+name+'</strong><br/>Tipo: '+tipo+' · Etapa: '+etapa + (code?'<br/>Código: '+code:'') + (robtxt?('<br/>Robustez: '+robtxt):'') + (riesgotxt?('<br/>Riesgo: '+riesgotxt):'');
    tooltip.style.left = evt.clientX+'px'; tooltip.style.top = evt.clientY+'px'; tooltip.style.opacity='1';
  }
  function hideTip(){ tooltip.style.opacity='0'; }
  document.querySelectorAll('.node').forEach(function(el){
    el.addEventListener('mousemove', function(e){ showTip(e, el); });
    el.addEventListener('mouseleave', hideTip);
  });

  const controls = [fEtapaInits, fEtapaDefs, fTexto, onlyDefsOnVisible, showBar, showRF, showRC].concat(robChecks).concat(riesgoChecks);
  controls.forEach(function(c){ 
    c.addEventListener('input', function(){ apply(); document.querySelectorAll('.arrow').forEach(showEdgeIfNodesVisible); }); 
    c.addEventListener('change', function(){ apply(); document.querySelectorAll('.arrow').forEach(showEdgeIfNodesVisible); }); 
  });

  // Panel: arrastrable y colapsable
  const panelEl = document.getElementById('panel');
  const handleEl = panelEl.querySelector('.handle');
  let draggingPanel=false, px0=0, py0=0, pLeft=0, pTop=0;
  handleEl.addEventListener('mousedown', function(e){
    if((e.target && e.target.id)==='toggle-panel') return;
    draggingPanel=true; const rect=panelEl.getBoundingClientRect();
    pLeft = rect.left; pTop = rect.top; px0=e.clientX; py0=e.clientY;
    panelEl.style.right='auto'; panelEl.style.bottom='auto';
    document.body.style.userSelect='none';
  });
  window.addEventListener('mousemove', function(e){
    if(!draggingPanel) return;
    const nx = pLeft + (e.clientX - px0);
    const ny = pTop  + (e.clientY - py0);
    panelEl.style.left = nx+'px';
    panelEl.style.top  = ny+'px';
  });
  window.addEventListener('mouseup', function(){ draggingPanel=false; document.body.style.userSelect=''; });

  const toggleBtn = document.getElementById('toggle-panel');
  if(toggleBtn){ toggleBtn.addEventListener('click', function(e){ e.stopPropagation(); panelEl.classList.toggle('collapsed'); }); }


  // Inicialización
  update();
  bindInteractions();
  apply();
  // Note: apply() already calls showEdgeIfNodesVisible for all arrows
})();
</script>)JS";

    return js1 + js2 + js3 + js4 + js5;
}
