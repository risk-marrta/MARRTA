# HTML Report - Visual Example

This document shows a visual representation of the generated PDF report.

## Example Report Output

```
┌─────────────────────────────────────────────────────────────────────┐
│                                                                       │
│  ╔══════════════════════════════════════════════════════════════╗   │
│  ║  ETAPA: Administración de Medicamentos                        ║   │
│  ╚══════════════════════════════════════════════════════════════╝   │
│                                                                       │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │  INICIADOR: Error en identificación del paciente             │   │
│  └──────────────────────────────────────────────────────────────┘   │
│                                                                       │
│  ╭────────────────────────────────────────────────────────────╮     │
│  │                                                              │     │
│  │  Se produce un error al identificar al paciente antes de     │     │
│  │  administrar la medicación, lo que puede llevar a            │     │
│  │  administrar el medicamento al paciente equivocado.          │     │
│  │                                                              │     │
│  ╰────────────────────────────────────────────────────────────╯     │
│                                                                       │
│  ╔══════════════════════════════════════════════════════════════╗   │
│  ║  FRECUENCIA: Media    CONSECUENCIAS: Alta                    ║   │
│  ╚══════════════════════════════════════════════════════════════╝   │
│                                                                       │
│  Contexto del evento: Este tipo de error puede ocurrir durante       │
│  la fase de preparación o administración de medicamentos cuando      │
│  el personal sanitario no verifica correctamente la identidad        │
│  del paciente.                                                        │
│                                                                       │
│  ═══════════════════════════════════════════════════════════════    │
│  BARRERAS                                                             │
│  ───────────────────────────────────────────────────────────────    │
│                                                                       │
│  ┌───┬──────────────────────────┬──────────────────────────────┐   │
│  │ # │ Descripción              │ Implementación               │   │
│  ├───┼──────────────────────────┼──────────────────────────────┤   │
│  │ 1 │ Pulsera de identificación│ Verificar pulsera antes de   │   │
│  │   │ del paciente             │ administrar medicación       │   │
│  ├───┼──────────────────────────┼──────────────────────────────┤   │
│  │ 2 │ Doble verificación       │ Dos enfermeras verifican     │   │
│  │   │ de identidad             │ identidad del paciente       │   │
│  ├───┼──────────────────────────┼──────────────────────────────┤   │
│  │ 3 │ Sistema electrónico      │ Escaneo de código de barras  │   │
│  │   │ de verificación          │ en pulsera                   │   │
│  └───┴──────────────────────────┴──────────────────────────────┘   │
│                                                                       │
│  ═══════════════════════════════════════════════════════════════    │
│  REDUCTORES DE FRECUENCIA                                             │
│  ───────────────────────────────────────────────────────────────    │
│                                                                       │
│  ┌───┬──────────────────────────────────────────────────────┐       │
│  │ # │ Descripción                                          │       │
│  ├───┼──────────────────────────────────────────────────────┤       │
│  │ 1 │ Formación en identificación segura de pacientes     │       │
│  ├───┼──────────────────────────────────────────────────────┤       │
│  │ 2 │ Protocolo estandarizado de verificación             │       │
│  └───┴──────────────────────────────────────────────────────┘       │
│                                                                       │
│  ═══════════════════════════════════════════════════════════════    │
│  REDUCTORES DE CONSECUENCIAS                                          │
│  ───────────────────────────────────────────────────────────────    │
│                                                                       │
│  ┌───┬──────────────────────────────────────────────────────┐       │
│  │ # │ Descripción                                          │       │
│  ├───┼──────────────────────────────────────────────────────┤       │
│  │ 1 │ Monitorización continua de pacientes                │       │
│  └───┴──────────────────────────────────────────────────────┘       │
│                                                                       │
└─────────────────────────────────────────────────────────────────────┘

                            Page 1 / 1
```

## Color Scheme

- **Blue Headers** (#4472C4): Stage title and info bars
- **Gray Section** (#D9D9D9): Initiator section  
- **White Background**: Main content areas
- **Black Borders**: Tables and description boxes
- **Light Gray** (#F2F2F2): Table ID cells

## Report Sections Explained

1. **Header (Blue)**
   - Shows the stage name in bold, centered text
   - Uses high-contrast white text on blue background

2. **Initiator Section (Gray)**
   - Displays the event/initiator name
   - Gray background for visual separation

3. **Description Box (White with Border)**
   - Contains the detailed error description
   - Bordered for emphasis and readability

4. **Info Bar (Blue)**
   - Shows frequency and consequences in a single line
   - Important information highlighted in blue

5. **Context Paragraph**
   - Additional context about the event
   - Justified text for professional appearance

6. **Barriers Table**
   - Three columns: ID, Description, Implementation
   - Shows active barriers only
   - ID column has gray background

7. **Frequency Reducers Table**
   - Two columns: ID, Description
   - Shows active frequency reducers only

8. **Consequence Reducers Table**
   - Two columns: ID, Description
   - Shows active consequence reducers only

## Technical Implementation

The report is generated using:
- **HTML5** for structure
- **CSS3** for styling (embedded in <style> tag)
- **Qt QPrinter** for PDF rendering
- **QTextDocument** for HTML to PDF conversion

The output format is **A4 paper** (210mm × 297mm) with **15mm margins**.

## Usage Workflow

1. User selects "Generar informe HTML..." from menu
2. Dialog shows list of available events
3. User selects an event and sees preview
4. User clicks "Generar PDF" button
5. File save dialog appears
6. PDF is generated and saved
7. Success message confirms creation

## Sample Generated HTML Structure

```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <style>
        /* CSS styling embedded here */
    </style>
</head>
<body>
    <div class="header-azul">ETAPA: ...</div>
    <div class="seccion-iniciador">INICIADOR: ...</div>
    <div class="recuadro-descripcion">...</div>
    <div class="barra-info">FRECUENCIA: ... CONSECUENCIAS: ...</div>
    <div class="parrafo-contexto">...</div>
    <div class="titulo-seccion">BARRERAS</div>
    <table class="tabla-barreras">...</table>
    <!-- ... more sections ... -->
</body>
</html>
```
