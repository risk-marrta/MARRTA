"""
Test script for MARRTA Python API - Database object
Tests database query functionality
"""

print("=== Testing Database Object ===\n")

# Test 1: Query all stages
print("Test 1: Database.query() - Get all stages")
try:
    results = Database.query("SELECT * FROM etapas ORDER BY orden")
    print(f"✓ Retrieved {len(results)} stages")
    
    if len(results) > 0:
        print("\nFirst stage:")
        first = results[0]
        for key, value in first.items():
            print(f"  {key}: {value}")
except Exception as e:
    print(f"✗ Error: {e}")

print()

# Test 2: Query events with filtering
print("Test 2: Database.query() - Filter events by frequency")
try:
    sql = """
    SELECT nombre, frecuencia, consecuencia 
    FROM suceso_iniciador 
    WHERE frecuencia >= 3 
    ORDER BY frecuencia DESC 
    LIMIT 5
    """
    results = Database.query(sql)
    print(f"✓ Found {len(results)} high-frequency events")
    
    for i, event in enumerate(results, 1):
        print(f"  {i}. {event['nombre']}")
        print(f"     Frecuencia: {event['frecuencia']}, Consecuencia: {event['consecuencia']}")
except Exception as e:
    print(f"✗ Error: {e}")

print()

# Test 3: Query with COUNT
print("Test 3: Database.query() - Count total events")
try:
    results = Database.query("SELECT COUNT(*) as total FROM suceso_iniciador")
    if len(results) > 0:
        total = results[0]['total']
        print(f"✓ Total events in database: {total}")
except Exception as e:
    print(f"✗ Error: {e}")

print()

# Test 4: Query substages
print("Test 4: Database.query() - Get substages")
try:
    sql = """
    SELECT s.nombre as substage, e.nombre as stage
    FROM subetapas s
    JOIN etapas e ON s.etapa_id = e.id
    ORDER BY e.orden, s.orden
    LIMIT 10
    """
    results = Database.query(sql)
    print(f"✓ Retrieved {len(results)} substages")
    
    for item in results:
        print(f"  {item['stage']} > {item['substage']}")
except Exception as e:
    print(f"✗ Error: {e}")

print("\n=== All Database API tests completed ===")
