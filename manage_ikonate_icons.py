#!/usr/bin/env python3
"""
MARRTA Ikonate Icons Manager
Gestor de Iconos Ikonate para MARRTA

This script helps manage Ikonate SVG icons in the MARRTA project.
Este script ayuda a gestionar los iconos SVG de Ikonate en el proyecto MARRTA.
"""

import os
import sys
import shutil
import xml.etree.ElementTree as ET
from pathlib import Path
from typing import List, Optional

# Project paths
PROJECT_DIR = Path(os.environ.get("MARRTA_PROJECT_DIR", Path(__file__).resolve().parent))
ICONS_DIR = PROJECT_DIR / "resources" / "icons"
QRC_FILE = PROJECT_DIR / "icons.qrc"
QRC_BACKUP = PROJECT_DIR / "icons.qrc.backup"

# ANSI color codes
class Colors:
    BLUE = '\033[0;34m'
    GREEN = '\033[0;32m'
    RED = '\033[0;31m'
    YELLOW = '\033[1;33m'
    NC = '\033[0m'  # No Color

def print_info(msg: str):
    print(f"{Colors.BLUE}[INFO]{Colors.NC} {msg}")

def print_success(msg: str):
    print(f"{Colors.GREEN}[SUCCESS]{Colors.NC} {msg}")

def print_error(msg: str):
    print(f"{Colors.RED}[ERROR]{Colors.NC} {msg}")

def print_warning(msg: str):
    print(f"{Colors.YELLOW}[WARNING]{Colors.NC} {msg}")

def is_valid_svg(file_path: Path) -> bool:
    """Check if a file is a valid SVG."""
    if not file_path.suffix.lower() == '.svg':
        return False
    
    try:
        tree = ET.parse(file_path)
        root = tree.getroot()
        # Check if it's an SVG file
        return 'svg' in root.tag.lower()
    except Exception as e:
        print_error(f"Invalid SVG file {file_path.name}: {e}")
        return False

def get_current_icons() -> List[Path]:
    """Get list of current SVG icons in the project."""
    if not ICONS_DIR.exists():
        return []
    return sorted(ICONS_DIR.glob("*.svg"))

def list_qrc_icons() -> List[str]:
    """Get list of icons registered in QRC file."""
    if not QRC_FILE.exists():
        print_error(f"QRC file not found: {QRC_FILE}")
        return []
    
    try:
        tree = ET.parse(QRC_FILE)
        root = tree.getroot()
        icons = []
        for file_elem in root.findall(".//file"):
            if file_elem.text and 'resources/icons/' in file_elem.text:
                icon_name = file_elem.text.split('/')[-1]
                icons.append(icon_name)
        return sorted(icons)
    except Exception as e:
        print_error(f"Error reading QRC file: {e}")
        return []

def add_icon_to_qrc(icon_name: str) -> bool:
    """Add an icon to the QRC file."""
    if not QRC_FILE.exists():
        print_error(f"QRC file not found: {QRC_FILE}")
        return False
    
    try:
        tree = ET.parse(QRC_FILE)
        root = tree.getroot()
        
        # Check if icon already exists
        for file_elem in root.findall(".//file"):
            if file_elem.text == f"resources/icons/{icon_name}":
                print_warning(f"Icon {icon_name} already in QRC")
                return False
        
        # Find qresource element
        qresource = root.find(".//qresource[@prefix='/icons']")
        if qresource is None:
            print_error("Could not find qresource element in QRC")
            return False
        
        # Add new file element
        file_elem = ET.SubElement(qresource, "file")
        file_elem.text = f"resources/icons/{icon_name}"
        
        # Write back with proper formatting
        tree.write(QRC_FILE, encoding='utf-8', xml_declaration=False)
        
        # Fix formatting (ElementTree doesn't preserve formatting well)
        fix_qrc_formatting()
        
        print_success(f"Added {icon_name} to QRC")
        return True
        
    except Exception as e:
        print_error(f"Error adding to QRC: {e}")
        return False

def fix_qrc_formatting():
    """Fix QRC file formatting after ElementTree modification."""
    with open(QRC_FILE, 'r') as f:
        content = f.read()
    
    # Add proper indentation
    lines = content.split('\n')
    formatted_lines = []
    indent_level = 0
    
    for line in lines:
        stripped = line.strip()
        if not stripped:
            continue
            
        # Decrease indent for closing tags
        if stripped.startswith('</'):
            indent_level -= 1
        
        formatted_lines.append('    ' * indent_level + stripped)
        
        # Increase indent for opening tags (that aren't self-closing or immediately closed)
        if stripped.startswith('<') and not stripped.startswith('</') and not stripped.endswith('/>'):
            if '<file>' not in stripped:  # Don't indent after <file> tags
                indent_level += 1
    
    with open(QRC_FILE, 'w') as f:
        f.write('\n'.join(formatted_lines) + '\n')

def backup_qrc() -> bool:
    """Create backup of QRC file."""
    try:
        shutil.copy2(QRC_FILE, QRC_BACKUP)
        print_success(f"Backup created: {QRC_BACKUP}")
        return True
    except Exception as e:
        print_error(f"Error creating backup: {e}")
        return False

def restore_qrc() -> bool:
    """Restore QRC from backup."""
    if not QRC_BACKUP.exists():
        print_error("No backup found")
        return False
    
    try:
        shutil.copy2(QRC_BACKUP, QRC_FILE)
        print_success("QRC restored from backup")
        return True
    except Exception as e:
        print_error(f"Error restoring backup: {e}")
        return False

def add_icons_from_directory(directory: Path) -> int:
    """Add all SVG icons from a directory."""
    if not directory.exists():
        print_error(f"Directory not found: {directory}")
        return 0
    
    # Backup QRC first
    backup_qrc()
    
    count = 0
    svg_files = list(directory.glob("*.svg"))
    
    if not svg_files:
        print_warning(f"No SVG files found in {directory}")
        return 0
    
    for svg_file in svg_files:
        if not is_valid_svg(svg_file):
            continue
        
        # Copy to icons directory
        dest = ICONS_DIR / svg_file.name
        try:
            shutil.copy2(svg_file, dest)
            print_success(f"Copied: {svg_file.name}")
            
            # Add to QRC
            if add_icon_to_qrc(svg_file.name):
                count += 1
        except Exception as e:
            print_error(f"Error copying {svg_file.name}: {e}")
    
    print_success(f"Added {count} new icons")
    return count

def add_single_icon(icon_path: Path) -> bool:
    """Add a single icon."""
    if not icon_path.exists():
        print_error(f"File not found: {icon_path}")
        return False
    
    if not is_valid_svg(icon_path):
        print_error(f"Not a valid SVG file: {icon_path}")
        return False
    
    # Backup QRC first
    backup_qrc()
    
    # Copy to icons directory
    dest = ICONS_DIR / icon_path.name
    try:
        shutil.copy2(icon_path, dest)
        print_success(f"Copied: {icon_path.name}")
        
        # Add to QRC
        return add_icon_to_qrc(icon_path.name)
    except Exception as e:
        print_error(f"Error copying file: {e}")
        return False

def show_current_icons():
    """Display current icons."""
    icons = get_current_icons()
    print_info("Current icons in the project:")
    print()
    for icon in icons:
        print(f"  - {icon.name}")
    print(f"\nTotal: {len(icons)} icons")

def show_qrc_icons():
    """Display icons in QRC."""
    icons = list_qrc_icons()
    print_info("Icons registered in icons.qrc:")
    print()
    for icon in icons:
        print(f"  - {icon}")
    print(f"\nTotal: {len(icons)} icons in QRC")

def find_missing_icons():
    """Find icons in directory but not in QRC."""
    file_icons = {icon.name for icon in get_current_icons()}
    qrc_icons = set(list_qrc_icons())
    
    missing = file_icons - qrc_icons
    if missing:
        print_warning("Icons in directory but not in QRC:")
        for icon in sorted(missing):
            print(f"  - {icon}")
    else:
        print_success("All icons in directory are registered in QRC")
    
    extra = qrc_icons - file_icons
    if extra:
        print_warning("Icons in QRC but not in directory:")
        for icon in sorted(extra):
            print(f"  - {icon}")

def generate_usage_examples():
    """Generate C++ code examples for using icons."""
    icons = get_current_icons()
    
    print_info("C++ Usage Examples:")
    print("\n// In a menu:")
    if icons:
        icon = icons[0].name
        print(f'menu.addAction(')
        print(f'    QIcon(":/icons/resources/icons/{icon}"),')
        print(f'    "Action text",')
        print(f'    [=]{{ /* action code */ }}')
        print(f');')
    
    print("\n// In a button:")
    if len(icons) > 1:
        icon = icons[1].name
        print(f'QPushButton *btn = new QPushButton("Click me", this);')
        print(f'btn->setIcon(QIcon(":/icons/resources/icons/{icon}"));')
    
    print("\n// Available icons:")
    for icon in icons[:5]:  # Show first 5
        print(f'// QIcon(":/icons/resources/icons/{icon.name}")')
    if len(icons) > 5:
        print(f'// ... and {len(icons) - 5} more')

def show_menu():
    """Show interactive menu."""
    print("\n" + "=" * 50)
    print("MARRTA - Ikonate Icons Manager")
    print("=" * 50)
    print("\n1) Show current icons / Mostrar iconos actuales")
    print("2) Show QRC icons / Mostrar iconos en QRC")
    print("3) Find missing icons / Encontrar iconos faltantes")
    print("4) Add icons from directory / Añadir iconos desde directorio")
    print("5) Add single icon / Añadir un solo icono")
    print("6) Backup QRC / Hacer backup del QRC")
    print("7) Restore QRC / Restaurar QRC")
    print("8) Generate usage examples / Generar ejemplos de uso")
    print("9) Exit / Salir")
    
    choice = input("\nOption / Opción: ").strip()
    print()
    
    if choice == '1':
        show_current_icons()
    elif choice == '2':
        show_qrc_icons()
    elif choice == '3':
        find_missing_icons()
    elif choice == '4':
        directory = input("Enter directory path / Ruta del directorio: ").strip()
        add_icons_from_directory(Path(directory))
    elif choice == '5':
        icon_path = input("Enter icon file path / Ruta del archivo: ").strip()
        add_single_icon(Path(icon_path))
    elif choice == '6':
        backup_qrc()
    elif choice == '7':
        restore_qrc()
    elif choice == '8':
        generate_usage_examples()
    elif choice == '9':
        print_info("Goodbye! / ¡Adiós!")
        sys.exit(0)
    else:
        print_error("Invalid option / Opción inválida")

def main():
    """Main entry point."""
    if len(sys.argv) > 1:
        # Command line mode
        command = sys.argv[1]
        
        if command == 'list':
            show_current_icons()
        elif command == 'qrc':
            show_qrc_icons()
        elif command == 'check':
            find_missing_icons()
        elif command == 'add' and len(sys.argv) > 2:
            path = Path(sys.argv[2])
            if path.is_dir():
                add_icons_from_directory(path)
            else:
                add_single_icon(path)
        elif command == 'backup':
            backup_qrc()
        elif command == 'restore':
            restore_qrc()
        elif command == 'examples':
            generate_usage_examples()
        else:
            print("Usage:")
            print("  python3 manage_ikonate_icons.py list         - Show current icons")
            print("  python3 manage_ikonate_icons.py qrc          - Show QRC icons")
            print("  python3 manage_ikonate_icons.py check        - Check for missing icons")
            print("  python3 manage_ikonate_icons.py add <path>   - Add icon(s)")
            print("  python3 manage_ikonate_icons.py backup       - Backup QRC")
            print("  python3 manage_ikonate_icons.py restore      - Restore QRC")
            print("  python3 manage_ikonate_icons.py examples     - Generate usage examples")
            print("\nOr run without arguments for interactive mode")
    else:
        # Interactive mode
        try:
            while True:
                show_menu()
        except KeyboardInterrupt:
            print("\n")
            print_info("Goodbye! / ¡Adiós!")
            sys.exit(0)

if __name__ == "__main__":
    main()
