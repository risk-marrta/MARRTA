# CSS Management Architecture

## System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                    MARRTA Report Generation                      │
└─────────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────────┐
│                    User Requests Report                           │
└──────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌──────────────────────────────────────────────────────────────────┐
│              DlgConfigInforme::generateFullHtml()                 │
│                      Calls loadCSS()                              │
└──────────────────────────────────────────────────────────────────┘
                              │
                              ▼
                ┌─────────────────────────┐
                │     loadCSS() Logic     │
                └─────────────────────────┘
                              │
                ┌─────────────┴─────────────┐
                │                           │
                ▼                           ▼
    ┌──────────────────────┐    ┌──────────────────────┐
    │ User CSS Exists?     │    │ User CSS Not Found   │
    │ ~/.local/share/      │    │                      │
    │ MARRTA/              │    │                      │
    │ report_style.css     │    │                      │
    └──────────────────────┘    └──────────────────────┘
                │                           │
                │ YES                       │ NO
                ▼                           ▼
    ┌──────────────────────┐    ┌──────────────────────┐
    │  Return User CSS     │    │  getDefaultCSS()     │
    │  (Custom Styles)     │    │                      │
    └──────────────────────┘    └──────────────────────┘
                │                           │
                │                           ▼
                │               ┌──────────────────────┐
                │               │ Load from Resources  │
                │               │ :/report/resources/  │
                │               │ report_style.css     │
                │               └──────────────────────┘
                │                           │
                └───────────┬───────────────┘
                            ▼
                ┌─────────────────────────┐
                │   Apply Orientation     │
                │   Update @page rule     │
                │   (portrait/landscape)  │
                └─────────────────────────┘
                            │
                            ▼
                ┌─────────────────────────┐
                │   Inject CSS into       │
                │   HTML <style> tag      │
                └─────────────────────────┘
                            │
                            ▼
                ┌─────────────────────────┐
                │  Generate Cover Page    │
                │  Calls getLogoAsBase64()│
                └─────────────────────────┘
                            │
                            ▼
                ┌─────────────────────────┐
                │  Load logo.png from     │
                │  Qt resources           │
                └─────────────────────────┘
                            │
                            ▼
                ┌─────────────────────────┐
                │  Convert to Base64      │
                │  QByteArray::toBase64() │
                └─────────────────────────┘
                            │
                            ▼
                ┌─────────────────────────┐
                │  Create data URI        │
                │  data:image/png;base64, │
                └─────────────────────────┘
                            │
                            ▼
                ┌─────────────────────────┐
                │  Embed in <img src="">  │
                │  Complete HTML generated│
                └─────────────────────────┘
                            │
                            ▼
                ┌─────────────────────────┐
                │  Render to PDF or       │
                │  Show in Preview        │
                └─────────────────────────┘
```

## User CSS Management Flow

```
┌──────────────────────────────────────────────────────────────────┐
│                    UI: CSS Management Buttons                     │
└──────────────────────────────────────────────────────────────────┘

┌─────────────────────┐  ┌─────────────────────┐  ┌──────────────────┐
│   Guardar CSS       │  │   Importar CSS      │  │  Restaurar por   │
│   (Save/Export)     │  │   (Load/Import)     │  │  defecto (Reset) │
└─────────────────────┘  └─────────────────────┘  └──────────────────┘
          │                        │                        │
          ▼                        ▼                        ▼
┌─────────────────────┐  ┌─────────────────────┐  ┌──────────────────┐
│ loadCSS() current   │  │ User selects file   │  │ Confirmation     │
│ Get active CSS      │  │ QFileDialog         │  │ dialog shown     │
└─────────────────────┘  └─────────────────────┘  └──────────────────┘
          │                        │                        │
          ▼                        ▼                        ▼
┌─────────────────────┐  ┌─────────────────────┐  ┌──────────────────┐
│ User picks location │  │ Read CSS file       │  │ User confirms    │
│ QFileDialog         │  │ QFile::readAll()    │  │                  │
└─────────────────────┘  └─────────────────────┘  └──────────────────┘
          │                        │                        │
          ▼                        ▼                        ▼
┌─────────────────────┐  ┌─────────────────────┐  ┌──────────────────┐
│ Write CSS to file   │  │ saveUserCSS(css)    │  │ QFile::remove()  │
│ QFile::write()      │  │                     │  │ Delete user CSS  │
└─────────────────────┘  └─────────────────────┘  └──────────────────┘
          │                        │                        │
          ▼                        ▼                        ▼
┌─────────────────────┐  ┌─────────────────────┐  ┌──────────────────┐
│ Success message     │  │ Write to AppData    │  │ Success message  │
│                     │  │ ~/.local/share/...  │  │                  │
└─────────────────────┘  └─────────────────────┘  └──────────────────┘
                                   │                        │
                                   ▼                        ▼
                         ┌─────────────────────┐  ┌──────────────────┐
                         │ Success message     │  │ Next report uses │
                         │ CSS persists        │  │ default CSS      │
                         └─────────────────────┘  └──────────────────┘
```

## File Locations

```
┌─────────────────────────────────────────────────────────────────┐
│                        Qt Resources                              │
├─────────────────────────────────────────────────────────────────┤
│  :/report/resources/report_style.css                            │
│  - Default CSS embedded in application                           │
│  - ~500 lines, professional blue theme                           │
│  - Read-only at runtime                                          │
└─────────────────────────────────────────────────────────────────┘
                              │
                              │ Compiled into app
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                     User Data Directory                          │
├─────────────────────────────────────────────────────────────────┤
│  Linux:   ~/.local/share/MARRTA/report_style.css               │
│  Windows: %APPDATA%\MARRTA\report_style.css                     │
│  macOS:   ~/Library/Application Support/MARRTA/report_style.css│
│                                                                  │
│  - Created when user imports custom CSS                         │
│  - Takes precedence over default CSS                            │
│  - Can be deleted to restore default                            │
└─────────────────────────────────────────────────────────────────┘
```

## Data Flow: Logo Embedding

```
┌──────────────────────────────────────────────────────────────────┐
│                    Logo Processing Pipeline                       │
└──────────────────────────────────────────────────────────────────┘

Step 1: Load from Resources
┌─────────────────────────┐
│  :/logos/resources/     │
│  logos/logo.png         │
│  315x313 PNG, 48KB      │
└─────────────────────────┘
            │
            ▼
Step 2: Read Binary Data
┌─────────────────────────┐
│  QFile::readAll()       │
│  Returns QByteArray     │
│  Raw PNG bytes          │
└─────────────────────────┘
            │
            ▼
Step 3: Base64 Encode
┌─────────────────────────┐
│  QByteArray::toBase64() │
│  Converts to ASCII      │
│  ~64KB encoded string   │
└─────────────────────────┘
            │
            ▼
Step 4: Create Data URI
┌─────────────────────────┐
│  QString format         │
│  "data:image/png;       │
│   base64,iVBORw0..."    │
└─────────────────────────┘
            │
            ▼
Step 5: Embed in HTML
┌─────────────────────────┐
│  <img src="data:image/  │
│  png;base64,..." />     │
│  Self-contained HTML    │
└─────────────────────────┘
            │
            ▼
Step 6: Render
┌─────────────────────────┐
│  Browser decodes base64 │
│  Displays PNG image     │
│  No external files      │
└─────────────────────────┘
```

## CSS Priority and Inheritance

```
                        Report Generation
                               │
                               ▼
                    ┌──────────────────────┐
                    │  Check User CSS Path │
                    └──────────────────────┘
                               │
                 ┌─────────────┴─────────────┐
                 │                           │
           File Exists                 File Not Found
                 │                           │
                 ▼                           ▼
        ┌─────────────────┐         ┌─────────────────┐
        │  User CSS       │         │  Default CSS    │
        │  (Priority 1)   │         │  (Priority 2)   │
        │  Custom styles  │         │  Built-in       │
        └─────────────────┘         └─────────────────┘
                 │                           │
                 └───────────┬───────────────┘
                             │
                             ▼
                  ┌────────────────────┐
                  │  Apply Orientation │
                  │  Modification      │
                  └────────────────────┘
                             │
                             ▼
                  ┌────────────────────┐
                  │  Final CSS Used    │
                  │  in HTML           │
                  └────────────────────┘
```

## Benefits of This Architecture

### For Users
- ✅ **Easy Customization**: Edit CSS without touching code
- ✅ **Persistence**: Settings saved across sessions
- ✅ **Portability**: HTML files work anywhere
- ✅ **Safety**: Can always reset to default
- ✅ **Sharing**: Export and share custom styles

### For Developers
- ✅ **Separation of Concerns**: CSS separate from logic
- ✅ **Maintainability**: Easy to update default styles
- ✅ **Testability**: CSS can be tested independently
- ✅ **Extensibility**: Easy to add new CSS features
- ✅ **Clean Code**: Removed 470 lines from .cpp file

### Technical Advantages
- ✅ **No External Dependencies**: Everything self-contained
- ✅ **Cross-Platform**: Works on Windows, Linux, macOS
- ✅ **Browser Compatible**: Standard HTML/CSS/base64
- ✅ **Resource Efficient**: CSS loaded once per report
- ✅ **Backward Compatible**: Existing reports still work
