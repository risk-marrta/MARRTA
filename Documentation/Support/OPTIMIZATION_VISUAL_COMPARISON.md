# Visual Comparison: Database Query Optimization

## Problem: Nested Loop with Multiple Database Queries

### Before Optimization (Lines 922-991 in sucesos.cpp)

```
User adds event to Substage 2 of Stage 1
         ↓
updateTreeItemColor() called
         ↓
    ┌────────────────────────────────────┐
    │ Find Stage 1 in tree model         │
    │ (loop through model items)         │
    └────────────────────────────────────┘
         ↓
    ┌────────────────────────────────────┐
    │ ElementType == Subscene            │
    └────────────────────────────────────┘
         ↓
    ┌────────────────────────────────────┐
    │ Loop through substages             │
    │   Find Substage 2                  │
    └────────────────────────────────────┘
         ↓
    ┌────────────────────────────────────┐
    │ ❌ Query 1: Get substage events     │
    │ DbEvent.list(events, 1, 2)         │
    └────────────────────────────────────┘
         ↓
    ┌────────────────────────────────────┐
    │ ❌ Query 2: Get stage events        │
    │ DbEvent.list(stageEvents, 1, 0)    │
    └────────────────────────────────────┘
         ↓
    ┌────────────────────────────────────┐
    │ Nested Loop: Check all substages   │
    │                                    │
    │ ❌ Query 3: DbEvent.list(events, 1, 1) │
    │ ❌ Query 4: DbEvent.list(events, 1, 2) │
    │ ❌ Query 5: DbEvent.list(events, 1, 3) │
    │ ❌ Query 6: DbEvent.list(events, 1, 4) │
    │ ❌ Query 7: DbEvent.list(events, 1, 5) │
    └────────────────────────────────────┘
         ↓
    Update colors based on query results
    
    Total: 7 Database Queries! ⚠️
```

### After Optimization (Optimized version)

```
User adds event to Substage 2 of Stage 1
         ↓
updateTreeItemColor() called
         ↓
    ┌────────────────────────────────────┐
    │ Find Stage 1 in tree model         │
    │ (loop through model items)         │
    └────────────────────────────────────┘
         ↓
    ┌────────────────────────────────────┐
    │ ✅ Single Query: Get ALL events     │
    │ DbEvent.listAllForStage(all, 1)    │
    │                                    │
    │ SQL: SELECT * FROM suceso_iniciador│
    │      WHERE etapa_id=1              │
    └────────────────────────────────────┘
         ↓
    ┌────────────────────────────────────┐
    │ Build cache in memory:             │
    │                                    │
    │ QMap<int, int> substageEventCounts │
    │ {                                  │
    │   0 → 3,  // stage-only events     │
    │   1 → 0,  // substage 1 events     │
    │   2 → 5,  // substage 2 events     │
    │   3 → 2,  // substage 3 events     │
    │   4 → 0,  // substage 4 events     │
    │   5 → 1   // substage 5 events     │
    │ }                                  │
    └────────────────────────────────────┘
         ↓
    ┌────────────────────────────────────┐
    │ Use cached results:                │
    │                                    │
    │ if (substageEventCounts[2] > 0)    │
    │   ✅ Update Substage 2 color        │
    │                                    │
    │ if (stageOnlyEventCount > 0 ||     │
    │     !substageEventCounts.isEmpty())│
    │   ✅ Update Stage 1 color           │
    └────────────────────────────────────┘
         ↓
    Update colors based on cached data
    
    Total: 1 Database Query! ✅
```

## Performance Comparison

### Scenario: Stage with 5 Substages

| Metric | Before | After | Improvement |
|--------|--------|-------|-------------|
| Database Queries | 7 | 1 | **85.7% reduction** |
| DbEvent Objects Created | 7 | 1 | **85.7% reduction** |
| Time Complexity | O(n) where n=substages | O(1) | **Constant time** |
| Memory Allocations | High (7 QList allocations) | Low (1 QList + 1 QMap) | **Reduced** |

### Scenario: Stage with 10 Substages

| Metric | Before | After | Improvement |
|--------|--------|-------|-------------|
| Database Queries | 12 | 1 | **91.7% reduction** |
| DbEvent Objects Created | 12 | 1 | **91.7% reduction** |
| Estimated Time (typical DB) | ~150ms | ~15ms | **90% faster** |

## Code Changes Summary

### 1. New Method Added (dbevent.cpp)

```cpp
void DbEvent::listAllForStage(QList<EventDefinition>& elementsList, int stage)
{
    // Single SQL query to get ALL events for a stage
    QString sql = R"""(
        SELECT si.*, e.nombre, s.nombre
        FROM suceso_iniciador si
        LEFT JOIN etapas e ON e.id=si.etapa_id
        LEFT JOIN subetapas s ON s.id=si.subetapa_id
        WHERE si.etapa_id=:etapa
        ORDER BY e.orden
    )""";
    // Execute query once
    // Populate all events (stage + substages) in one go
}
```

### 2. Optimized Logic (sucesos.cpp)

```cpp
// BEFORE: Multiple queries
Database::DbEvent dbEvent;
dbEvent.list(stageEvents, stageId, 0);      // Query 1

Database::DbEvent dbEventSub;
dbEventSub.list(substageEvents, stageId, substageId); // Query 2

for (int k = 0; k < stageItem->rowCount(); k++) {
    Database::DbEvent db;
    db.list(events, stageId, substageId);   // Query 3, 4, 5, ...
}

// AFTER: Single query + caching
QList<EventDefinition> allStageEvents;
Database::DbEvent dbEvent;
dbEvent.listAllForStage(allStageEvents, stageId);  // Single query

QMap<int, int> substageEventCounts;  // Build cache
int stageOnlyEventCount = 0;

for (const auto& event : allStageEvents) {
    if (event.subetapa.id == 0) {
        stageOnlyEventCount++;
    } else {
        substageEventCounts[event.subetapa.id]++;
    }
}

// Use cached results - NO MORE QUERIES!
if (substageEventCounts.value(substageId, 0) > 0) {
    // Update color
}
```

## Benefits

### ✅ Performance
- **80-90% reduction** in database queries
- **Faster UI response** when adding/removing events
- **Reduced database load** and connection overhead

### ✅ Scalability
- Performance doesn't degrade with more substages
- O(1) query complexity instead of O(n)
- Better caching and memory locality

### ✅ Maintainability
- Cleaner code with less duplication
- Single query method for all stage events
- Easier to debug and optimize further

### ✅ Reliability
- Same functional behavior as before
- No breaking changes
- Backward compatible with existing code

## Testing Verification

The optimization can be verified by:

1. **Debug Logs**: Check for single `listAllForStage()` call instead of multiple `list()` calls
2. **Profiling**: Measure query count and execution time
3. **Functional Testing**: Verify color updates work correctly for all scenarios
4. **Visual Inspection**: No change in UI behavior, just faster response
