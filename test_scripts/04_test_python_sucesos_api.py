"""
Test script for MARRTA Python Sucesos (Events) API
Tests the new Sucesos module for accessing initiating events
"""

print("=" * 60)
print("MARRTA Python API - Sucesos (Events) Test")
print("=" * 60)
print()

# ============================================================================
# Section 1: Get All Events
# ============================================================================
print("SECTION 1: Get All Events")
print("-" * 60)

try:
    events = Sucesos.get_all()
    print(f"✓ Total events: {len(events)}")
    print()
    
    if len(events) > 0:
        print("First 5 events:")
        for i, event in enumerate(events[:5]):
            print(f"  {i+1}. [{event['codigo']}] {event['nombre']}")
            print(f"     Stage: {event['etapa_codigo']} - {event['etapa_nombre']}")
            print(f"     Substage: {event['subetapa_nombre']}")
            print(f"     Risk: F={event['frecuencia']}, C={event['consecuencia']}, R={event['riesgo']}")
            print(f"     Active: {event['activo']}, Completed: {event['completado']}")
            print()
    else:
        print("  No events found in database")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Section 2: Get Events by Stage
# ============================================================================
print("SECTION 2: Get Events by Stage")
print("-" * 60)

try:
    # Get all stages first
    stages = Etapas.get_all()
    
    if len(stages) > 0:
        # Get events for first stage
        stage = stages[0]
        stage_events = Sucesos.get_by_stage(stage.id)
        
        print(f"Events in stage {stage.id} ({stage.codigo} - {stage.nombre}):")
        print(f"  Total: {len(stage_events)} events")
        
        if len(stage_events) > 0:
            print("\n  Details:")
            for event in stage_events[:3]:  # Show first 3
                print(f"    - {event['codigo']}: {event['nombre']}")
                print(f"      Substage: {event['subetapa_nombre']}")
        print()
    else:
        print("  No stages found to test with")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Section 3: Event Statistics
# ============================================================================
print("SECTION 3: Event Statistics")
print("-" * 60)

try:
    events = Sucesos.get_all()
    
    # Count by risk level
    risk_counts = {}
    for event in events:
        risk = event['riesgo']
        risk_counts[risk] = risk_counts.get(risk, 0) + 1
    
    print("Events by risk level:")
    risk_names = {0: "Bajo", 1: "Medio", 2: "Alto", 3: "Muy Alto"}
    for risk_level in sorted(risk_counts.keys()):
        count = risk_counts[risk_level]
        name = risk_names.get(risk_level, f"Level {risk_level}")
        print(f"  {name}: {count} events")
    
    print()
    
    # Count human error events
    human_error_count = sum(1 for e in events if e['error_humano'])
    print(f"Events with human error: {human_error_count} / {len(events)}")
    
    # Count completed events
    completed_count = sum(1 for e in events if e['completado'])
    print(f"Completed events: {completed_count} / {len(events)}")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Section 4: Cross-Reference with Stages
# ============================================================================
print("SECTION 4: Events per Stage Summary")
print("-" * 60)

try:
    events = Sucesos.get_all()
    stages = Etapas.get_all()
    
    # Count events per stage
    events_by_stage = {}
    for event in events:
        stage_id = event['etapa_id']
        events_by_stage[stage_id] = events_by_stage.get(stage_id, 0) + 1
    
    print("Distribution of events across stages:")
    for stage in stages:
        count = events_by_stage.get(stage.id, 0)
        print(f"  {stage.codigo} - {stage.nombre}: {count} events")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()
print("=" * 60)
print("Sucesos API Test Complete")
print("=" * 60)
