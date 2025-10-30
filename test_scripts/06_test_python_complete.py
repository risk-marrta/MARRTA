"""
Complete integration test for MARRTA Python API
Tests all available objects: Marrta, Database, and Etapas
"""

print("=" * 60)
print("MARRTA Python API - Complete Integration Test")
print("=" * 60)
print()

# ============================================================================
# Section 1: Marrta Object
# ============================================================================
print("SECTION 1: Marrta Object")
print("-" * 60)

try:
    version = Marrta.get_version()
    print(f"✓ MARRTA Version: {version}")
except Exception as e:
    print(f"✗ Error getting version: {e}")

print()

# ============================================================================
# Section 2: Database Object
# ============================================================================
print("SECTION 2: Database Object")
print("-" * 60)

# 2.1: Count records in main tables
print("\n2.1 Database Statistics:")
tables = ['etapas', 'subetapas', 'suceso_iniciador', 'defensas']

for table in tables:
    try:
        result = Database.query(f"SELECT COUNT(*) as count FROM {table}")
        count = result[0]['count'] if len(result) > 0 else 0
        print(f"  {table}: {count} records")
    except Exception as e:
        print(f"  {table}: Error - {e}")

print()

# 2.2: Query high-risk events
print("2.2 High-Risk Events (Frequency ≥ 4, Consequence ≥ 4):")
try:
    sql = """
    SELECT nombre, frecuencia, consecuencia
    FROM suceso_iniciador
    WHERE frecuencia >= 4 AND consecuencia >= 4
    ORDER BY frecuencia DESC, consecuencia DESC
    LIMIT 5
    """
    results = Database.query(sql)
    
    if len(results) > 0:
        for i, event in enumerate(results, 1):
            print(f"  {i}. {event['nombre']}")
            print(f"     Risk: F={event['frecuencia']} x C={event['consecuencia']}")
    else:
        print("  No high-risk events found")
except Exception as e:
    print(f"  Error: {e}")

print()

# ============================================================================
# Section 3: Etapas Object
# ============================================================================
print("SECTION 3: Etapas Object")
print("-" * 60)

# 3.1: Get all stages
print("\n3.1 All Stages:")
try:
    etapas = Etapas.get_all()
    print(f"✓ Total stages: {len(etapas)}\n")
    
    for etapa in etapas:
        print(f"  [{etapa.codigo}] {etapa.nombre} (ID: {etapa.id}, Orden: {etapa.orden})")
        
except Exception as e:
    print(f"✗ Error: {e}")

print()

# 3.2: Use Etapa.imprimir() method
print("3.2 Detailed Stage Information (using imprimir()):")
try:
    etapas = Etapas.get_all()
    
    for etapa in etapas:
        datos = etapa.imprimir()
        id_etapa, codigo, nombre, desc, subetapas_ids = datos
        
        print(f"\n  Etapa {id_etapa}: {codigo} - {nombre}")
        
        # Show description (truncated)
        if desc:
            desc_short = desc[:60] + "..." if len(desc) > 60 else desc
            print(f"    Descripción: {desc_short}")
        
        # Show substage information
        num_substages = len(subetapas_ids)
        print(f"    Subetapas: {num_substages}")
        
        if num_substages > 0:
            # Get substage names from database
            ids_str = ",".join(str(id) for id in subetapas_ids)
            sql = f"SELECT id, nombre FROM subetapas WHERE id IN ({ids_str}) ORDER BY orden"
            substages = Database.query(sql)
            
            for substage in substages:
                print(f"      - {substage['nombre']} (ID: {substage['id']})")
        
except Exception as e:
    print(f"✗ Error: {e}")

print()

# ============================================================================
# Section 4: Cross-Object Analysis
# ============================================================================
print("SECTION 4: Cross-Object Analysis")
print("-" * 60)

# 4.1: Events per stage
print("\n4.1 Events per Stage:")
try:
    etapas = Etapas.get_all()
    
    for etapa in etapas:
        # Count events in this stage
        sql = f"""
        SELECT COUNT(DISTINCT s.id) as count
        FROM suceso_iniciador s
        LEFT JOIN subetapas sub ON s.subetapa_id = sub.id
        WHERE s.etapa_id = {etapa.id} OR sub.etapa_id = {etapa.id}
        """
        result = Database.query(sql)
        count = result[0]['count'] if len(result) > 0 else 0
        
        print(f"  {etapa.codigo} - {etapa.nombre}: {count} events")
        
except Exception as e:
    print(f"✗ Error: {e}")

print()

# 4.2: Summary
print("4.2 Summary:")
try:
    etapas = Etapas.get_all()
    
    # Count total substages using imprimir()
    total_substages = sum(len(e.imprimir()[4]) for e in etapas)
    
    # Count total events
    event_result = Database.query("SELECT COUNT(*) as count FROM suceso_iniciador")
    total_events = event_result[0]['count'] if len(event_result) > 0 else 0
    
    # Count total defenses
    defense_result = Database.query("SELECT COUNT(*) as count FROM defensas")
    total_defenses = defense_result[0]['count'] if len(defense_result) > 0 else 0
    
    print(f"  Total Stages: {len(etapas)}")
    print(f"  Total Substages: {total_substages}")
    print(f"  Total Events: {total_events}")
    print(f"  Total Defenses: {total_defenses}")
    print(f"  Average Substages per Stage: {total_substages / len(etapas):.2f}")
    print(f"  Average Events per Stage: {total_events / len(etapas):.2f}")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()
print("=" * 60)
print("Integration Test Complete")
print("=" * 60)
