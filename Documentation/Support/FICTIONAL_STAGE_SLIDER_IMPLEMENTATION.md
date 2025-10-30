# Fictional Stage Slider Implementation

## Overview

This document describes the implementation of a vertical slider control that allows users to adjust the Y position of fictional stages (etapas ficticias) in the flowchart visualization.

## Problem Statement

In large models, fictional stages positioned at y=0 caused positioning and visualization problems. Users needed a way to dynamically adjust the vertical position of fictional stages to better accommodate different model sizes and improve readability.

## Solution

A vertical slider control was added to the flowchart HTML panel that allows users to move fictional stages and their associated defenses up and down from y=0 to the maximum Y position of real stages.

## Technical Implementation

### 1. SVG Structure Changes

Fictional stage elements are now grouped in a dedicated SVG group for easy transformation:

```xml
<g id="fictional-edges"><!-- Edges from fictional defenses --></g>
<g id="fictional-group" transform="translate(0, 0)">
  <g id="fictional-nodes"><!-- Fictional stages and defenses --></g>
</g>
```

This grouping allows all fictional elements to be moved together with a single transform operation.

### 2. Data Attributes

The maximum Y position of real stages is stored as a data attribute on the SVG canvas:

```xml
<svg id="canvas" data-initcol="..." data-maxrealy="1000" ...>
```

This value is used to set the maximum range of the slider.

### 3. HTML Slider Control

A new card was added to the control panel with a range slider:

```html
<div class="card">
  <h4>Posición vertical etapas ficticias</h4>
  <div class="slider-container">
    <label class="small">Desplazamiento vertical: <span id="fict-offset-value">0</span> px</label>
    <input type="range" id="fict-offset-slider" min="0" max="[maxRealStageY]" value="0" step="10">
  </div>
</div>
```

### 4. CSS Styling

Custom styling was added for the range input to provide a modern, consistent appearance:

```css
.slider-container{display:flex;flex-direction:column;gap:4px}
input[type="range"]{-webkit-appearance:none;appearance:none;height:6px;border-radius:3px;background:#e5e7eb;outline:none}
input[type="range"]::-webkit-slider-thumb{-webkit-appearance:none;appearance:none;width:16px;height:16px;border-radius:50%;background:#2563eb;cursor:pointer}
input[type="range"]::-moz-range-thumb{width:16px;height:16px;border-radius:50%;background:#2563eb;cursor:pointer;border:none}
```

### 5. JavaScript Functionality

JavaScript handles the slider input events and applies the transform:

```javascript
const fictionalGroup = document.getElementById('fictional-group');
const fictOffsetSlider = document.getElementById('fict-offset-slider');
const fictOffsetValue = document.getElementById('fict-offset-value');

function updateFictionalOffset() {
  const offset = parseInt(fictOffsetSlider.value) || 0;
  fictOffsetValue.textContent = offset;
  if (fictionalGroup) {
    fictionalGroup.setAttribute('transform', 'translate(0, ' + offset + ')');
  }
}

fictOffsetSlider.addEventListener('input', updateFictionalOffset);
```

The "Reiniciar" (Reset) button also resets the slider to position 0.

## Code Changes

### Modified Files

- **flujogramagenerator.cpp**: Main implementation file with all changes

### Key Changes

1. **Separated fictional elements into dedicated variables** (lines 617-618):
   - `fictionalNodes` - Stores fictional stage and defense nodes
   - `fictionalEdges` - Stores edges from fictional defenses

2. **Updated fictional stage generation** (lines 809-819):
   - Changed to append to `fictionalNodes` instead of `nodes`

3. **Updated fictional defense generation** (lines 822-894):
   - Changed to append to `fictionalNodes` and `fictionalEdges`

4. **Calculated maximum real stage Y position** (lines 956-958):
   - Used to set slider maximum value

5. **Updated HTML template** (lines 962-1050):
   - Added `data-maxrealy` attribute to SVG
   - Added `fictional-group` with separate edges and nodes groups
   - Added slider control card at the top of the panel

6. **Added slider CSS** (lines 1132-1135):
   - Styling for slider container and range input

7. **Added slider JavaScript** (lines 1144-1171):
   - Event handlers and transform logic

## Usage

1. Open a flowchart in the browser
2. Locate the "Posición vertical etapas ficticias" card at the top of the control panel
3. Move the slider up or down to adjust the vertical position of fictional stages
4. The current offset is displayed in pixels above the slider
5. Click "Reiniciar" to reset the position to 0

## Benefits

- **Improved flexibility**: Users can position fictional stages anywhere in the vertical range
- **Better readability**: Fictional stages can be moved to reduce overlap with real stages
- **Real-time updates**: Changes are applied immediately as the slider is moved
- **Smooth control**: 10px step increments provide precise positioning
- **Reset capability**: Easy to return to the default position

## Visual Examples

### Initial Position (offset = 0)
Fictional stages are positioned at the top of the diagram.

![Slider at 0px](https://github.com/user-attachments/assets/5b438b8b-d0a3-432a-bf15-11cb68512a66)

### Adjusted Position (offset = 500)
Fictional stages have been moved down 500px to align better with real stages.

![Slider at 500px](https://github.com/user-attachments/assets/a273ccd3-8a3a-4122-a05d-bab65e2fdad6)

## Future Enhancements

Potential improvements for future versions:

1. **Remember position**: Store the slider position in browser local storage
2. **Auto-position**: Calculate an optimal position based on model size
3. **Keyboard shortcuts**: Allow keyboard control of the slider
4. **Position presets**: Quick buttons for common positions (top, middle, bottom)
