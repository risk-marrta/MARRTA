# Performance Optimization: Database Query Consolidation

## Problem Statement

The `updateTreeItemColor` method in `sucesos.cpp` had a performance issue where it created multiple `Database::DbEvent` instances and executed separate SQL queries for each substage in nested loops. This resulted in:

- **Up to 4-5+ database queries** for a single color update operation
- **O(n) database queries** where n is the number of substages
- Unnecessary object creation and destruction in tight loops

### Original Code Issues (Lines 922-991)

```cpp
// Issue 1: Query for stage events
Database::DbEvent dbEvent;
dbEvent.list(stageEvents, stageId, 0);

// Issue 2: Query for each substage individually
Database::DbEvent dbEventSub;
dbEventSub.list(substageEvents, stageId, substageId);

// Issue 3: Query stage events AGAIN
Database::DbEvent dbEvent;
dbEvent.list(stageEvents, stageId, 0);

// Issue 4: Nested loop querying EACH substage
for (int k = 0; k < stageItem->rowCount(); k++) {
    Database::DbEvent db;
    db.list(events, stageId, subElemId.second.second);  // Separate query per substage!
}
```

## Solution

### 1. Added New Method `listAllForStage` to `DbEvent` class

**File: `dbevent.h`**
- Added method declaration: `void listAllForStage(QList<EventDefinition>& elementList, int stage);`

**File: `dbevent.cpp`**
- Implemented method to fetch ALL events for a stage (including all substages) in a single SQL query
- SQL: `SELECT ... FROM suceso_iniciador WHERE etapa_id=:etapa`
- This query retrieves both stage-only events and all substage events

### 2. Refactored `updateTreeItemColor` Method

**File: `sucesos.cpp`**

#### Before: Multiple Queries
- 1 query for stage events (Scene case)
- OR 1 query for substage + 1 query for stage + n queries for all substages (Subscene case)
- **Total: 1-3+(n) queries** depending on the case

#### After: Single Query + In-Memory Processing
- 1 query to get ALL events for the stage (both stage and substages)
- Build a `QMap<int, int>` to cache event counts per substage
- Use cached data for all subsequent lookups
- **Total: 1 query** regardless of case

### Key Changes

```cpp
// Query once for all events
QList<EventDefinition> allStageEvents;
Database::DbEvent dbEvent;
dbEvent.listAllForStage(allStageEvents, stageId);

// Build lookup map
QMap<int, int> substageEventCounts;
int stageOnlyEventCount = 0;

for (const auto& event : allStageEvents) {
    int eventSubstageId = event.subetapa.id;
    if (eventSubstageId == 0) {
        stageOnlyEventCount++;
    } else {
        substageEventCounts[eventSubstageId]++;
    }
}

// Use cached results for lookups
if (substageEventCounts.value(substageId, 0) > 0) {
    // Update color
}
```

## Performance Impact

### Before
- **Scene update**: 1 database query
- **Subscene update with n substages**: 3 + n database queries
- Example with 5 substages: **8 queries**

### After
- **Scene update**: 1 database query
- **Subscene update with n substages**: 1 database query
- Example with 5 substages: **1 query**

### Performance Gain
- **87.5% reduction** in database queries for substage updates (with 5 substages)
- **O(1) vs O(n)** query complexity
- Reduced object creation/destruction overhead
- Better cache locality with in-memory map

## Files Modified

1. **dbevent.h** - Added `listAllForStage` method declaration
2. **dbevent.cpp** - Implemented `listAllForStage` method
3. **sucesos.cpp** - 
   - Added `#include <QMap>` header
   - Refactored `updateTreeItemColor` to use single query with caching

## Testing Recommendations

1. Test stage color updates when adding/removing events
2. Test substage color updates when adding/removing events  
3. Verify color updates work correctly for:
   - Stages with no substages
   - Stages with multiple substages
   - Mixed scenarios (some substages with events, some without)
4. Monitor database query logs to confirm single query execution

## Backward Compatibility

- No breaking changes to existing APIs
- New method `listAllForStage` is additive
- Existing `list` method remains unchanged
- Same visual behavior as before, just more efficient
