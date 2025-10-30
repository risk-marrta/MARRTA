# MARRTA - Risk Analysis and Management Tool

MARRTA is a comprehensive risk analysis and management tool designed for conducting MARR (Metodología de Análisis de Riesgos y Rendimiento) studies.

## Features

- **Stage Management**: Define and organize project stages and substages
- **Event Management**: Create and manage initiating events
- **Defense Management**: Configure frequency reducers, barriers, and consequence reducers
- **Risk Analysis**: Calculate and visualize risk matrices
- **Reporting**: Generate comprehensive reports
- **Scripting Engine**: Automate tasks using JavaScript

## Icons

MARRTA uses [Ikonate](https://ikonate.com/) SVG icons for a modern, consistent look across all platforms.

### Adding New Icons

- **[Quick Guide](Documentation/Support/QUICK_GUIDE_IKONATE_ICONS.md)** - Fast reference for adding icons
- **[Complete How-To (ES)](Documentation/Support/HOWTO_ADD_IKONATE_ICONS.md)** - Comprehensive Spanish guide
- **[Complete How-To (EN)](Documentation/Support/HOWTO_ADD_IKONATE_ICONS_EN.md)** - Comprehensive English guide
- **[Workflow Diagram](Documentation/Support/ICON_WORKFLOW_DIAGRAM.md)** - Visual process flow
- **[Practical Example](Documentation/Support/EXAMPLE_ADDING_IKONATE.md)** - Step-by-step example
- **[FAQ](Documentation/Support/FAQ_IKONATE_ICONS.md)** - Frequently asked questions

### Tools

- `./manage_ikonate_icons.py` - Python script with validation (recommended)
- `./add_ikonate_icons.sh` - Interactive bash script

## Scripting

MARRTA includes a powerful JavaScript scripting engine that allows you to automate tasks and interact with the application programmatically.

### Quick Start

1. Open a PRAXIS file in MARRTA
2. Go to **Action → Editor**
3. Write your JavaScript code
4. Click Execute

### Documentation

- **[Scripting API Reference](Documentation/Support/SCRIPTING_API.md)** - Complete API documentation
- **[Scripting Examples](examples/scripting/)** - Ready-to-use example scripts

### Example: List All Stages

```javascript
// Get all stages including virtual stages (id < 0)
var stages = Marrta.getAllStages();

console.log("Total stages: " + stages.length);

for (var i = 0; i < stages.length; i++) {
    var stage = stages[i];
    console.log("Stage: " + stage.nombre + " (Code: " + stage.codigo + ")");
}
```

See the [examples directory](examples/scripting/) for more scripts.

## Version

Current version: 2.0.0 (2025-10-24)

## Support

For issues, questions, or contributions, please visit:
https://github.com/theotocopulitos/MARRTA

## License

[Add license information here]
