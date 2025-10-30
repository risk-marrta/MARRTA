#!/usr/bin/env python3
"""
Test script to verify MARRTA Python API and environment
Run this from MARRTA's Python console to verify the setup
"""

import sys
print(f"Python version: {sys.version}")
print(f"Python executable: {sys.executable}")
print("")

# Test pandas
try:
    import pandas as pd
    print(f"✓ pandas version: {pd.__version__}")
except ImportError as e:
    print(f"❌ pandas not available: {e}")
    sys.exit(1)

# Test numpy
try:
    import numpy as np
    print(f"✓ numpy version: {np.__version__}")
except ImportError as e:
    print(f"❌ numpy not available: {e}")
    sys.exit(1)

print("")
print("Testing MARRTA Python API...")

# Test Etapas API
try:
    etapas_data = Etapas.get_all()
    print(f"✓ Etapas.get_all() returned {len(etapas_data)} items")
    if len(etapas_data) > 0:
        first = etapas_data[0]
        print(f"  First item type: {type(first).__name__}")
        if isinstance(first, dict):
            print(f"  Is dict with keys: {list(first.keys())}")
        else:
            attrs = [a for a in dir(first) if not a.startswith('_')]
            print(f"  Is object with {len(attrs)} attributes")
            print(f"  Sample attributes: {attrs[:5]}")
except NameError:
    print("❌ Etapas API not available (run from MARRTA Python console)")
    sys.exit(1)
except Exception as e:
    print(f"❌ Error testing Etapas API: {e}")
    import traceback
    traceback.print_exc()

# Test Sucesos API
try:
    sucesos_data = Sucesos.get_all()
    print(f"✓ Sucesos.get_all() returned {len(sucesos_data)} items")
    if len(sucesos_data) > 0:
        first = sucesos_data[0]
        print(f"  First item type: {type(first).__name__}")
        if isinstance(first, dict):
            print(f"  Is dict with keys: {list(first.keys())}")
        else:
            attrs = [a for a in dir(first) if not a.startswith('_')]
            print(f"  Is object with {len(attrs)} attributes")
            print(f"  Sample attributes: {attrs[:5]}")
except NameError:
    print("❌ Sucesos API not available")
except Exception as e:
    print(f"❌ Error testing Sucesos API: {e}")
    import traceback
    traceback.print_exc()

# Test Defensas API
try:
    defensas_data = Defensas.get_all()
    print(f"✓ Defensas.get_all() returned {len(defensas_data)} items")
    if len(defensas_data) > 0:
        first = defensas_data[0]
        print(f"  First item type: {type(first).__name__}")
        if isinstance(first, dict):
            print(f"  Is dict with keys: {list(first.keys())}")
        else:
            attrs = [a for a in dir(first) if not a.startswith('_')]
            print(f"  Is object with {len(attrs)} attributes")
            print(f"  Sample attributes: {attrs[:5]}")
except NameError:
    print("❌ Defensas API not available")
except Exception as e:
    print(f"❌ Error testing Defensas API: {e}")
    import traceback
    traceback.print_exc()

print("")
print("=== Test DataFrame Creation ===")

# Test creating DataFrames
try:
    # Test with Etapas (known to return objects)
    etapas_list = []
    for item in etapas_data[:3]:  # Just first 3 items
        if isinstance(item, dict):
            etapas_list.append(item)
        else:
            item_dict = {}
            for attr in dir(item):
                if not attr.startswith('_'):
                    try:
                        item_dict[attr] = getattr(item, attr)
                    except Exception:
                        pass
            etapas_list.append(item_dict)
    
    df = pd.DataFrame(etapas_list)
    print(f"✓ Created DataFrame with {len(df)} rows and {len(df.columns)} columns")
    print(f"  Columns: {list(df.columns)}")
    
except Exception as e:
    print(f"❌ Error creating DataFrame: {e}")
    import traceback
    traceback.print_exc()

print("")
print("✓ All tests completed!")
print("")
print("If you see this message, the Python environment is working correctly.")
print("You can now run the flujograma script.")
