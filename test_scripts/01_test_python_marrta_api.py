"""
Test script for MARRTA Python API - Marrta object
Tests the basic Marrta object functionality
"""

print("=== Testing Marrta Object ===\n")

# Test 1: Get version
print("Test 1: Marrta.get_version()")
try:
    version = Marrta.get_version()
    print(f"✓ Version: {version}")
except Exception as e:
    print(f"✗ Error: {e}")

print("\n=== All Marrta API tests completed ===")
