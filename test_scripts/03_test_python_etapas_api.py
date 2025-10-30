"""
Test script for MARRTA Python API - Etapas and Etapa objects
Tests the Etapas.get_all() and Etapa.imprimir() methods
"""

print("=== Testing Etapas Object ===\n")

# Test 1: Get all stages
print("Test 1: Etapas.get_all()")
try:
    etapas = Etapas.get_all()
    print(f"✓ Retrieved {len(etapas)} stages\n")
    
    # Display basic info for each stage
    for etapa in etapas:
        print(f"[{etapa.codigo}] {etapa.nombre}")
        print(f"  ID: {etapa.id}")
        print(f"  Orden: {etapa.orden}")
        if etapa.descripcion:
            desc = etapa.descripcion[:50] + "..." if len(etapa.descripcion) > 50 else etapa.descripcion
            print(f"  Descripción: {desc}")
        print()
        
except Exception as e:
    print(f"✗ Error: {e}")

print()

# Test 2: Access stage properties
print("Test 2: Accessing Etapa properties")
try:
    etapas = Etapas.get_all()
    if len(etapas) > 0:
        etapa = etapas[0]
        print(f"✓ First stage properties:")
        print(f"  id: {etapa.id} (type: {type(etapa.id).__name__})")
        print(f"  codigo: {etapa.codigo} (type: {type(etapa.codigo).__name__})")
        print(f"  nombre: {etapa.nombre} (type: {type(etapa.nombre).__name__})")
        print(f"  descripcion: {etapa.descripcion} (type: {type(etapa.descripcion).__name__})")
        print(f"  orden: {etapa.orden} (type: {type(etapa.orden).__name__})")
except Exception as e:
    print(f"✗ Error: {e}")

print()

# Test 3: Etapa.imprimir() method
print("Test 3: Etapa.imprimir() method")
try:
    etapas = Etapas.get_all()
    print(f"✓ Testing imprimir() for all {len(etapas)} stages:\n")
    
    for etapa in etapas:
        # Call imprimir() method
        datos = etapa.imprimir()
        
        # Unpack the tuple
        id_etapa, codigo, nombre, desc, subetapas_ids = datos
        
        print(f"Etapa {id_etapa}: {codigo} - {nombre}")
        print(f"  Descripción: {desc[:60]}..." if len(desc) > 60 else f"  Descripción: {desc}")
        print(f"  Número de subetapas: {len(subetapas_ids)}")
        
        if len(subetapas_ids) > 0:
            print(f"  IDs de subetapas: {subetapas_ids}")
        print()
        
except Exception as e:
    print(f"✗ Error: {e}")

print()

# Test 4: Detailed substage analysis using imprimir()
print("Test 4: Detailed substage analysis")
try:
    etapas = Etapas.get_all()
    
    total_substages = 0
    stages_with_substages = 0
    max_substages = 0
    max_substages_stage = None
    
    for etapa in etapas:
        datos = etapa.imprimir()
        id_etapa, codigo, nombre, desc, subetapas_ids = datos
        
        num_substages = len(subetapas_ids)
        total_substages += num_substages
        
        if num_substages > 0:
            stages_with_substages += 1
            
        if num_substages > max_substages:
            max_substages = num_substages
            max_substages_stage = nombre
    
    print(f"✓ Analysis complete:")
    print(f"  Total stages: {len(etapas)}")
    print(f"  Stages with substages: {stages_with_substages}")
    print(f"  Total substages: {total_substages}")
    print(f"  Average substages per stage: {total_substages / len(etapas):.2f}")
    print(f"  Stage with most substages: {max_substages_stage} ({max_substages} substages)")
    
except Exception as e:
    print(f"✗ Error: {e}")

print()

# Test 5: Cross-verify with Database query
print("Test 5: Cross-verify imprimir() with Database.query()")
try:
    etapas = Etapas.get_all()
    
    print("✓ Comparing results:\n")
    
    for etapa in etapas[:3]:  # Test first 3 stages
        # Get data from imprimir()
        datos = etapa.imprimir()
        id_etapa, codigo, nombre, desc, subetapas_ids = datos
        
        # Query database directly
        sql = f"SELECT COUNT(*) as count FROM subetapas WHERE etapa_id = {id_etapa}"
        db_result = Database.query(sql)
        db_count = db_result[0]['count'] if len(db_result) > 0 else 0
        
        # Compare
        imprimir_count = len(subetapas_ids)
        match = "✓" if imprimir_count == db_count else "✗"
        
        print(f"{match} {nombre}:")
        print(f"  imprimir(): {imprimir_count} substages")
        print(f"  Database query: {db_count} substages")
        print()
        
except Exception as e:
    print(f"✗ Error: {e}")

print("=== All Etapas API tests completed ===")
