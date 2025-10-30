"""
Test script for MARRTA Python Defensas (Defenses) API
Tests the new Defensas module for accessing defenses (barriers, frequency and consequence reducers)
"""

print("=" * 60)
print("MARRTA Python API - Defensas (Defenses) Test")
print("=" * 60)
print()

# ============================================================================
# Section 1: Get All Defenses
# ============================================================================
print("SECTION 1: Get All Defenses")
print("-" * 60)

try:
    defenses = Defensas.get_all()
    print(f"✓ Total defenses: {len(defenses)}")
    print()
    
    if len(defenses) > 0:
        print("First 5 defenses:")
        for i, defense in enumerate(defenses[:5]):
            print(f"  {i+1}. [{defense['codigo']}] {defense['nombre']}")
            print(f"     Type: {defense['tipo_str']} (tipo={defense['tipo']})")
            print(f"     Robustness: {defense['robustez']}")
            print(f"     Active: {defense['activo']}")
            print(f"     Event ID: {defense['suceso_id']}")
            print()
    else:
        print("  No defenses found in database")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Section 2: Get Defenses by Type
# ============================================================================
print("SECTION 2: Get Defenses by Type")
print("-" * 60)

defense_types = [
    (0, "Reductores de Frecuencia"),
    (1, "Barreras"),
    (2, "Reductores de Consecuencia")
]

try:
    for tipo, nombre in defense_types:
        defenses = Defensas.get_by_type(tipo)
        print(f"{nombre} (tipo {tipo}): {len(defenses)} defenses")
        
        if len(defenses) > 0:
            # Show first 2 examples
            print("  Examples:")
            for defense in defenses[:2]:
                print(f"    - {defense['nombre']} (robustez: {defense['robustez']}, activo: {defense['activo']})")
        print()
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Section 3: Defense Statistics
# ============================================================================
print("SECTION 3: Defense Statistics")
print("-" * 60)

try:
    defenses = Defensas.get_all()
    
    # Count by type
    type_counts = {}
    for defense in defenses:
        tipo = defense['tipo_str']
        type_counts[tipo] = type_counts.get(tipo, 0) + 1
    
    print("Defenses by type:")
    for tipo, count in type_counts.items():
        print(f"  {tipo}: {count} defenses")
    
    print()
    
    # Count by robustness
    robustness_counts = {}
    for defense in defenses:
        robustez = defense['robustez']
        robustness_counts[robustez] = robustness_counts.get(robustez, 0) + 1
    
    print("Defenses by robustness level:")
    for level in sorted(robustness_counts.keys()):
        count = robustness_counts[level]
        print(f"  Level {level}: {count} defenses")
    
    print()
    
    # Count active vs inactive
    active_count = sum(1 for d in defenses if d['activo'])
    inactive_count = len(defenses) - active_count
    print(f"Active defenses: {active_count}")
    print(f"Inactive defenses: {inactive_count}")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Section 4: Defenses per Event
# ============================================================================
print("SECTION 4: Defenses per Event Analysis")
print("-" * 60)

try:
    events = Sucesos.get_all()
    
    if len(events) > 0:
        # Analyze first few events
        for event in events[:3]:
            event_defenses = Defensas.get_all(event['id'])
            print(f"Event: {event['codigo']} - {event['nombre']}")
            print(f"  Total defenses: {len(event_defenses)}")
            
            if len(event_defenses) > 0:
                # Count by type
                type_count = {}
                for defense in event_defenses:
                    tipo = defense['tipo_str']
                    type_count[tipo] = type_count.get(tipo, 0) + 1
                
                for tipo, count in type_count.items():
                    print(f"    {tipo}: {count}")
            print()
    else:
        print("  No events found to analyze")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Section 5: Integration Test - Events and Defenses
# ============================================================================
print("SECTION 5: Integration - Events with Defenses")
print("-" * 60)

try:
    events = Sucesos.get_all()
    
    # Count total defenses per event
    events_with_defenses = []
    
    for event in events:
        defenses = Defensas.get_all(event['id'])
        if len(defenses) > 0:
            events_with_defenses.append({
                'event': event,
                'defense_count': len(defenses)
            })
    
    print(f"Events with defenses: {len(events_with_defenses)} / {len(events)}")
    
    if len(events_with_defenses) > 0:
        # Sort by defense count
        events_with_defenses.sort(key=lambda x: x['defense_count'], reverse=True)
        
        print(f"\nTop 5 events by defense count:")
        for i, item in enumerate(events_with_defenses[:5]):
            event = item['event']
            count = item['defense_count']
            print(f"  {i+1}. {event['codigo']}: {count} defenses")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()
print("=" * 60)
print("Defensas API Test Complete")
print("=" * 60)
