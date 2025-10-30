# InformeGenerator Examples

This directory contains example code demonstrating how to use the HTML Report Generator (`InformeGenerator`) class in MARRTA.

## Available Examples

### informe_generator_example.cpp

A comprehensive example showing different use cases of the `InformeGenerator` class:

1. **Simple Report** - Basic event information without barriers or reducers
2. **Complete Report** - Full report with barriers and all types of reducers
3. **Barriers Only** - Report with barriers but no reducers
4. **Special Characters** - Testing HTML escaping and Unicode support

## How to Use the Examples

### Option 1: Standalone Compilation

You can compile the example as a standalone application:

```bash
# Navigate to the examples directory
cd examples

# Compile with qmake/Qt
qmake
make

# Run the example
./informe_generator_example
```

The example will generate several PDF files in the current directory:
- `ejemplo_informe_simple.pdf`
- `ejemplo_informe_completo.pdf`
- `ejemplo_informe_barreras.pdf`
- `ejemplo_caracteres_especiales.pdf`

### Option 2: Integrate into Your Code

Copy the relevant parts of the example into your own MARRTA code:

```cpp
#include "informegenerator.h"

// Create data structure
DatosInforme datos;
datos.etapa = "Your Stage";
datos.iniciador = "Your Event";
// ... fill in other fields

// Add barriers
Barrera barrera;
barrera.id = 1;
barrera.descripcion = "Barrier description";
barrera.implementacion = "Implementation details";
datos.barreras.append(barrera);

// Generate PDF
QString outputPath = "output.pdf";
InformeGenerator::generarPDF(datos, outputPath);
```

## Data Structure Reference

### DatosInforme

Main structure containing all report data:

```cpp
struct DatosInforme {
    QString etapa;                              // Stage name
    QString iniciador;                          // Event/initiator name
    QString descripcionError;                   // Event description
    QString frecuencia;                         // Frequency level (text)
    QString consecuencias;                      // Consequences level (text)
    QString contexto;                           // Additional context
    QVector<Barrera> barreras;                 // List of barriers
    QVector<Reductor> reductoresFrecuencia;    // Frequency reducers
    QVector<Reductor> reductoresConsecuencias; // Consequence reducers
};
```

### Barrera

Barrier/defense information:

```cpp
struct Barrera {
    int id;                    // Barrier ID number
    QString descripcion;       // Barrier description
    QString implementacion;    // Implementation status/details
};
```

### Reductor

Reducer (frequency or consequence) information:

```cpp
struct Reductor {
    int id;                 // Reducer ID number
    QString descripcion;    // Reducer description
};
```

## Integration with MARRTA Database

In a real MARRTA application, you would populate the data from the database:

```cpp
// Get event from database
CompleteEventDefinition evento = getEventFromDatabase(eventId);

// Get stage name
Database::DbElementStage dbStage;
QList<ElementStage> etapas;
dbStage.list(etapas);
QString nombreEtapa = findStageName(evento.etapa, etapas);

// Populate report data
DatosInforme datos;
datos.etapa = nombreEtapa;
datos.iniciador = evento.nombre;
datos.descripcionError = evento.descripcion;
datos.frecuencia = getFrecuenciaText(evento.frecuencia);
datos.consecuencias = getConsecuenciasText(evento.consecuencia);
datos.contexto = "Context: " + evento.descripcion;

// Add active barriers
for (const auto& barrera : evento.barreras) {
    if (barrera.activo) {
        Barrera b;
        b.id = barrera.id;
        b.descripcion = barrera.nombre;
        b.implementacion = barrera.descripcion;
        datos.barreras.append(b);
    }
}

// Add active frequency reducers
for (const auto& reductor : evento.reductoresFrecuencias) {
    if (reductor.activo) {
        Reductor r;
        r.id = reductor.id;
        r.descripcion = reductor.nombre;
        datos.reductoresFrecuencia.append(r);
    }
}

// Add active consequence reducers
for (const auto& reductor : evento.reductoresConsecuencias) {
    if (reductor.activo) {
        Reductor r;
        r.id = reductor.id;
        r.descripcion = reductor.nombre;
        datos.reductoresConsecuencias.append(r);
    }
}

// Generate the report
InformeGenerator::generarPDF(datos, "output.pdf");
```

## Output Format

The generated PDF will have:
- **A4 page size** (210mm x 297mm)
- **15mm margins** on all sides
- **Professional styling** with blue headers and gray sections
- **Tables** for barriers and reducers (when present)
- **HTML-escaped content** to prevent formatting issues

## Tips

1. **Always escape user input**: The generator automatically escapes HTML, but ensure your source data is clean
2. **Check file paths**: Make sure the output path is valid and writable
3. **Handle empty sections**: The generator automatically hides sections with no data (barriers, reducers)
4. **Test with special characters**: Use the special characters example to verify encoding works correctly
5. **Verify PDF output**: Always open and verify the generated PDF before distributing

## Troubleshooting

### PDF not generated

- Check that the output path is valid
- Verify you have write permissions
- Ensure Qt Print Support is properly configured

### Missing data in PDF

- Verify all fields in `DatosInforme` are populated
- Check that barriers/reducers are added to the respective vectors
- Ensure `activo` flag is true for defenses from database

### Formatting issues

- Check that special characters are properly encoded (UTF-8)
- Verify HTML content is being escaped
- Test with the special characters example

## See Also

- **Technical Documentation**: `Documentation/Support/HTML_REPORT_GENERATOR.md`
- **User Guide** (Spanish): `Documentation/Support/HTML_REPORT_USER_GUIDE.md`
- **Source Code**: `informegenerator.h` and `informegenerator.cpp`
- **Dialog Implementation**: `dlggenerarinforme.cpp`

## Contact

For questions or issues:
- GitHub: https://github.com/theotocopulitos/MARRTA
- Documentation: See `Documentation/Support/` directory
