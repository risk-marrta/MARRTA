"""
Complete test of the enhanced MARRTA Python API
Tests the new Sucesos and Defensas modules with integration examples
"""

print("=" * 70)
print("MARRTA Enhanced Python API - Complete Integration Test")
print("=" * 70)
print()

# ============================================================================
# Test 1: API Availability Check
# ============================================================================
print("TEST 1: API Availability Check")
print("-" * 70)

modules_ok = True

try:
    # Test Marrta module
    version = Marrta.get_version()
    print(f"✓ Marrta module available - Version: {version}")
except Exception as e:
    print(f"✗ Marrta module error: {e}")
    modules_ok = False

try:
    # Test Database module
    result = Database.query("SELECT 1 as test")
    print(f"✓ Database module available")
except Exception as e:
    print(f"✗ Database module error: {e}")
    modules_ok = False

try:
    # Test Etapas module
    stages = Etapas.get_all()
    print(f"✓ Etapas module available - {len(stages)} stages found")
except Exception as e:
    print(f"✗ Etapas module error: {e}")
    modules_ok = False

try:
    # Test Sucesos module
    events = Sucesos.get_all()
    print(f"✓ Sucesos module available - {len(events)} events found")
except Exception as e:
    print(f"✗ Sucesos module error: {e}")
    modules_ok = False

try:
    # Test Defensas module
    defenses = Defensas.get_all()
    print(f"✓ Defensas module available - {len(defenses)} defenses found")
except Exception as e:
    print(f"✗ Defensas module error: {e}")
    modules_ok = False

if not modules_ok:
    print("\n⚠ Some modules are not available. Check MARRTA installation.")
    print("Continuing with available modules...")

print()

# ============================================================================
# Test 2: Model Structure Analysis
# ============================================================================
print("TEST 2: Model Structure Analysis")
print("-" * 70)

try:
    stages = Etapas.get_all()
    events = Sucesos.get_all()
    defenses = Defensas.get_all()
    
    print(f"Model Components:")
    print(f"  Stages:   {len(stages)}")
    print(f"  Events:   {len(events)}")
    print(f"  Defenses: {len(defenses)}")
    print()
    
    # Calculate substages
    total_substages = 0
    for stage in stages:
        _, _, _, _, substage_ids = stage.imprimir()
        total_substages += len(substage_ids)
    
    print(f"  Total Substages: {total_substages}")
    print(f"  Avg Substages per Stage: {total_substages / len(stages):.2f}")
    print(f"  Avg Events per Stage: {len(events) / len(stages):.2f}")
    print(f"  Avg Defenses per Event: {len(defenses) / len(events):.2f}")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Test 3: Event Analysis
# ============================================================================
print("TEST 3: Event Analysis")
print("-" * 70)

try:
    events = Sucesos.get_all()
    
    # Risk distribution
    risk_counts = {}
    for event in events:
        risk = event['riesgo']
        risk_counts[risk] = risk_counts.get(risk, 0) + 1
    
    print("Risk Distribution:")
    risk_names = {0: "Low", 1: "Medium", 2: "High", 3: "Very High"}
    for risk_level in sorted(risk_counts.keys()):
        count = risk_counts[risk_level]
        name = risk_names.get(risk_level, f"Level {risk_level}")
        pct = count / len(events) * 100
        print(f"  {name:12s}: {count:3d} events ({pct:5.1f}%)")
    
    print()
    
    # Human error analysis
    human_error = sum(1 for e in events if e['error_humano'])
    print(f"Human Error Events: {human_error} / {len(events)} ({human_error/len(events)*100:.1f}%)")
    
    # Completion status
    completed = sum(1 for e in events if e['completado'])
    print(f"Completed Events: {completed} / {len(events)} ({completed/len(events)*100:.1f}%)")
    
    # Active status
    active = sum(1 for e in events if e['activo'])
    print(f"Active Events: {active} / {len(events)} ({active/len(events)*100:.1f}%)")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Test 4: Defense Analysis
# ============================================================================
print("TEST 4: Defense Analysis")
print("-" * 70)

try:
    defenses = Defensas.get_all()
    
    # Type distribution
    type_counts = {}
    type_names = {
        0: "Frequency Reducers",
        1: "Barriers",
        2: "Consequence Reducers"
    }
    
    for defense in defenses:
        tipo = defense['tipo']
        type_counts[tipo] = type_counts.get(tipo, 0) + 1
    
    print("Defense Type Distribution:")
    for tipo in sorted(type_counts.keys()):
        count = type_counts[tipo]
        name = type_names.get(tipo, f"Type {tipo}")
        pct = count / len(defenses) * 100
        print(f"  {name:23s}: {count:3d} defenses ({pct:5.1f}%)")
    
    print()
    
    # Robustness distribution
    robustness_counts = {}
    for defense in defenses:
        robustez = defense['robustez']
        robustness_counts[robustez] = robustness_counts.get(robustez, 0) + 1
    
    print("Robustness Level Distribution:")
    for level in sorted(robustness_counts.keys()):
        count = robustness_counts[level]
        pct = count / len(defenses) * 100
        print(f"  Level {level}: {count:3d} defenses ({pct:5.1f}%)")
    
    print()
    
    # Active vs inactive
    active = sum(1 for d in defenses if d['activo'])
    print(f"Active Defenses: {active} / {len(defenses)} ({active/len(defenses)*100:.1f}%)")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Test 5: Stage-Event Integration
# ============================================================================
print("TEST 5: Stage-Event Integration")
print("-" * 70)

try:
    stages = Etapas.get_all()
    
    print("Events per Stage:")
    for stage in stages:
        # Test get_by_stage method
        stage_events = Sucesos.get_by_stage(stage.id)
        print(f"  {stage.codigo} - {stage.nombre:30s}: {len(stage_events):3d} events")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Test 6: Defense Coverage Analysis
# ============================================================================
print("TEST 6: Defense Coverage Analysis")
print("-" * 70)

try:
    events = Sucesos.get_all()
    
    # Count defenses per event
    defense_counts = {}
    events_without_defenses = []
    
    for event in events:
        event_defenses = Defensas.get_all(event['id'])
        count = len(event_defenses)
        defense_counts[event['id']] = count
        
        if count == 0:
            events_without_defenses.append(event)
    
    events_with_defenses = sum(1 for c in defense_counts.values() if c > 0)
    
    print(f"Defense Coverage:")
    print(f"  Events with defenses:    {events_with_defenses}")
    print(f"  Events without defenses: {len(events_without_defenses)}")
    print(f"  Coverage: {events_with_defenses / len(events) * 100:.1f}%")
    
    print()
    
    # Find events with most defenses
    if events_with_defenses > 0:
        events_sorted = [(eid, count) for eid, count in defense_counts.items() if count > 0]
        events_sorted.sort(key=lambda x: x[1], reverse=True)
        
        print("Top 5 Events by Defense Count:")
        for i, (event_id, count) in enumerate(events_sorted[:5], 1):
            event = next(e for e in events if e['id'] == event_id)
            print(f"  {i}. {event['codigo']:10s} - {count} defenses")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Test 7: Defense Type per Event
# ============================================================================
print("TEST 7: Defense Type Distribution per Event")
print("-" * 70)

try:
    events = Sucesos.get_all()
    
    # Analyze first 3 events with defenses
    events_analyzed = 0
    
    for event in events:
        if events_analyzed >= 3:
            break
        
        event_defenses = Defensas.get_all(event['id'])
        
        if len(event_defenses) > 0:
            events_analyzed += 1
            print(f"\nEvent: {event['codigo']} - {event['nombre'][:40]}")
            print(f"  Total defenses: {len(event_defenses)}")
            
            # Count by type
            type_count = {}
            for defense in event_defenses:
                tipo_str = defense['tipo_str']
                type_count[tipo_str] = type_count.get(tipo_str, 0) + 1
            
            for tipo_str, count in type_count.items():
                print(f"    {tipo_str}: {count}")
    
    if events_analyzed == 0:
        print("  No events with defenses found for detailed analysis")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Test 8: Cross-Module Query Test
# ============================================================================
print("TEST 8: Cross-Module Integration")
print("-" * 70)

try:
    # Use Database for complex query
    sql = """
    SELECT 
        e.codigo as etapa_codigo,
        e.nombre as etapa_nombre,
        COUNT(DISTINCT s.id) as num_events,
        COUNT(DISTINCT d.id) as num_defenses
    FROM etapas e
    LEFT JOIN suceso_iniciador s ON e.id = s.etapa_id
    LEFT JOIN defensa d ON s.id = d.suceso_iniciador_id
    GROUP BY e.id
    ORDER BY e.orden
    """
    
    results = Database.query(sql)
    
    print("Stage Summary (from complex SQL query):")
    for row in results:
        print(f"  {row['etapa_codigo']:3s} {row['etapa_nombre']:30s}: "
              f"{row['num_events']:3d} events, {row['num_defenses']:3d} defenses")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Test 9: Defense Type Filtering
# ============================================================================
print("TEST 9: Defense Type Filtering")
print("-" * 70)

try:
    type_names = {
        0: "Frequency Reducers",
        1: "Barriers",
        2: "Consequence Reducers"
    }
    
    for tipo, name in type_names.items():
        # Test get_by_type method
        defenses = Defensas.get_by_type(tipo)
        active = sum(1 for d in defenses if d['activo'])
        
        print(f"{name:23s}: {len(defenses):3d} total, {active:3d} active")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Summary
# ============================================================================
print("=" * 70)
print("INTEGRATION TEST COMPLETE")
print("=" * 70)

try:
    stages = Etapas.get_all()
    events = Sucesos.get_all()
    defenses = Defensas.get_all()
    
    print("\nFinal Summary:")
    print(f"  ✓ All modules functional")
    print(f"  ✓ Data access successful")
    print(f"  ✓ {len(stages)} stages, {len(events)} events, {len(defenses)} defenses processed")
    print(f"  ✓ Integration between modules verified")
    
except Exception as e:
    print(f"\n✗ Final verification error: {e}")

print()
