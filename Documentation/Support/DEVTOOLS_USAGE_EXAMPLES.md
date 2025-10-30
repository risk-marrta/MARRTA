# DevTools Usage Examples

## Quick Start

### Opening DevTools

1. Generate or open any report in MARRTA
2. Look for the "DevTools" button in the toolbar (bottom of the window)
3. Click the "DevTools" button
4. A new window titled "Herramientas de desarrollo" will open

### Common Use Cases

#### 1. Inspecting Report Layout

**Scenario**: You want to understand how the report HTML is structured

**Steps**:
```
1. Open report in viewer
2. Click "DevTools" button
3. In DevTools, click the "Elements" tab
4. Click the element selector icon (top-left of DevTools)
5. Hover over elements in the report to see their HTML
6. Click an element to select it
7. View HTML structure and CSS styles in DevTools
```

**Example Use**: Finding which CSS classes are applied to table headers

#### 2. Debugging Report Styles

**Scenario**: A table is not displaying correctly and you want to fix the CSS

**Steps**:
```
1. Open DevTools
2. Select the problematic element
3. In the "Styles" pane, you'll see all CSS rules applied
4. Try disabling/enabling rules to identify the issue
5. Edit CSS values live to test fixes
6. Once you find the fix, apply it to the source code
```

**Example Use**: Adjusting table cell padding or border styles

#### 3. Viewing JavaScript Console

**Scenario**: You want to see if there are any JavaScript errors in the report

**Steps**:
```
1. Open DevTools
2. Click the "Console" tab
3. Look for red error messages
4. Click on error messages to see stack traces
5. Use console to test JavaScript commands
```

**Example Use**: Debugging custom JavaScript in reports

#### 4. Testing JavaScript in Report Context

**Scenario**: You want to test JavaScript code against the report DOM

**Steps**:
```
1. Open DevTools
2. Go to Console tab
3. Type JavaScript commands, for example:
   - `document.querySelectorAll('table').length` (count tables)
   - `document.title` (get document title)
   - `document.body.innerHTML` (see all content)
4. Press Enter to execute
```

**Example Commands**:
```javascript
// Count all tables in the report
document.querySelectorAll('table').length

// Get all headers
document.querySelectorAll('h2, h3')

// Change a style temporarily
document.querySelector('table').style.borderCollapse = 'collapse'

// Check if element exists
document.querySelector('.report-title')
```

#### 5. Monitoring Network Requests

**Scenario**: Report loads external resources and you want to see what's being loaded

**Steps**:
```
1. Open DevTools BEFORE loading/generating the report
2. Click "Network" tab
3. Generate or reload the report
4. Observe all network requests
5. Click on requests to see details (headers, response, timing)
```

**Example Use**: Checking if images or external CSS are loading correctly

#### 6. Performance Analysis

**Scenario**: Report is slow to render and you want to find bottlenecks

**Steps**:
```
1. Open DevTools
2. Click "Performance" tab
3. Click "Record" button
4. Reload or interact with the report
5. Stop recording
6. Analyze the timeline to find slow operations
```

**Example Use**: Identifying slow CSS selectors or JavaScript execution

#### 7. Modifying Report Content Temporarily

**Scenario**: You want to test how the report looks with different content

**Steps**:
```
1. Open DevTools
2. Select an element in the Elements tab
3. Double-click the text content
4. Edit the text
5. Press Enter to apply
6. Changes are temporary and only visible in the current session
```

**Example Use**: Testing how long text wraps in table cells

#### 8. Copying Report Data

**Scenario**: You need to extract data from a specific table

**Steps**:
```
1. Open DevTools
2. Select the table element in Elements tab
3. Right-click the element
4. Choose "Copy" > "Copy outerHTML"
5. Paste into a text editor
6. Extract the data as needed
```

**Example Use**: Getting raw HTML of a specific table for documentation

## Tips and Tricks

### Keyboard Shortcuts (in DevTools)

- `Ctrl+Shift+C` (or `Cmd+Shift+C` on Mac): Element selector tool
- `Ctrl+F` (or `Cmd+F`): Search in current panel
- `Ctrl+Shift+F` (or `Cmd+Shift+F`): Search in all files
- `F5`: Refresh the inspected page

### Useful Console Commands for Reports

```javascript
// Get all table rows
document.querySelectorAll('tr').length

// Get all cells in first table
document.querySelector('table').querySelectorAll('td, th').length

// Find all elements with specific text
[...document.querySelectorAll('*')].filter(el => el.textContent.includes('Risk'))

// Get computed styles
getComputedStyle(document.querySelector('table'))

// Measure element dimensions
document.querySelector('table').getBoundingClientRect()
```

### Preserving DevTools State

The DevTools window is created once and reused:
- Your selected tab (Elements, Console, etc.) persists
- Console history is maintained
- Window position and size are remembered
- Close and reopen without losing state

### Working with Multiple Reports

If you open multiple report viewers:
- Each has its own DevTools button
- Each gets its own DevTools window
- DevTools are scoped to their respective reports

## Troubleshooting

### DevTools Button Not Visible
- Ensure you're in the Report Viewer window
- Check the bottom toolbar
- Button should be between spacer and XML button

### DevTools Window Doesn't Open
- Check Qt version (requires Qt 6.0+)
- Verify QtWebEngine is installed
- Look for error messages in application console

### DevTools Shows Blank Page
- The DevTools page is created on first click
- Try closing and reopening the DevTools window
- Check if the report has loaded properly

### Changes in DevTools Don't Affect Report
- DevTools modifications are temporary
- Permanent changes must be made in source code
- Use DevTools to prototype, then implement in code

## Best Practices

1. **Open DevTools BEFORE the issue occurs**: Especially for network and performance monitoring

2. **Use Element Selector**: Instead of manually navigating the DOM tree, use the selector tool

3. **Preserve Log**: In Console tab, enable "Preserve log" to keep messages across page reloads

4. **Disable Cache**: In Network tab, check "Disable cache" for testing

5. **Take Screenshots**: Use DevTools screenshot feature to document issues

6. **Copy Relevant Info**: When reporting bugs, copy relevant console errors or network failures

## Integration with MARRTA Workflow

### During Report Development

1. Generate report with test data
2. Open DevTools
3. Inspect HTML structure
4. Test CSS changes live
5. Copy working CSS to source code
6. Regenerate report to verify

### During Report Debugging

1. Open DevTools before generating report
2. Check Console for errors
3. Monitor Network for failed resources
4. Use Elements inspector to verify structure
5. Document findings for fixes

### When Customizing Reports

1. Generate sample report
2. Open DevTools
3. Prototype styling changes in Elements panel
4. Test with different data sets
5. Implement validated changes in code
6. Verify with fresh report generation

## Advanced Features

### Remote Debugging (Future Enhancement)

Currently, DevTools works within the application. For remote debugging:
- Would require additional configuration
- Could enable debugging from external browser
- Feature request could be filed for this

### Saving DevTools Workspace (Future Enhancement)

Currently, DevTools state is per-session. For persistent workspaces:
- Could save breakpoints and watch expressions
- Would require additional implementation
- Feature request could be filed for this

## Related Documentation

- [DEVTOOLS_IMPLEMENTATION.md](DEVTOOLS_IMPLEMENTATION.md) - Technical implementation details
- [DEVTOOLS_TESTING_GUIDE.md](DEVTOOLS_TESTING_GUIDE.md) - Manual testing procedures
- Qt WebEngine DevTools: https://doc.qt.io/qt-6/qwebenginepage.html#setDevToolsPage

## Feedback and Improvements

If you have suggestions for improving the DevTools integration:
1. Document the use case
2. Describe the desired behavior
3. Note any technical constraints
4. Submit as feature request or enhancement issue
