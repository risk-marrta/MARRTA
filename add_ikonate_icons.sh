#!/bin/bash

# Script para añadir nuevos iconos de Ikonate a MARRTA
# Script to add new Ikonate icons to MARRTA

set -e  # Exit on error

echo "================================================"
echo "MARRTA - Ikonate Icon Addition Helper Script"
echo "Script de Ayuda para Añadir Iconos Ikonate"
echo "================================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Project paths
PROJECT_DIR="/home/runner/work/MARRTA/MARRTA"
ICONS_DIR="$PROJECT_DIR/resources/icons"
QRC_FILE="$PROJECT_DIR/icons.qrc"
QRC_BACKUP="$PROJECT_DIR/icons.qrc.backup"

# Function to print colored messages
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Function to check if file is SVG
is_svg() {
    local file="$1"
    if [[ "$file" =~ \.svg$ ]] && [[ -f "$file" ]]; then
        return 0
    fi
    return 1
}

# Function to add icon to QRC
add_icon_to_qrc() {
    local icon_name="$1"
    local qrc_line="        <file>resources/icons/${icon_name}</file>"
    
    # Check if icon already exists in QRC
    if grep -q "$qrc_line" "$QRC_FILE"; then
        print_warning "Icon $icon_name already in QRC file"
        return 1
    fi
    
    # Add before closing tags
    sed -i "/<\/qresource>/i\\    $qrc_line" "$QRC_FILE"
    print_success "Added $icon_name to QRC file"
    return 0
}

# Function to show current icons
show_current_icons() {
    print_info "Current icons in the project:"
    echo ""
    ls -1 "$ICONS_DIR"/*.svg 2>/dev/null | while read -r icon; do
        basename "$icon"
    done | sort
    echo ""
}

# Function to list icons in QRC
list_qrc_icons() {
    print_info "Icons registered in icons.qrc:"
    echo ""
    grep '<file>resources/icons/' "$QRC_FILE" | sed 's/.*<file>resources\/icons\/\(.*\)<\/file>/\1/' | sort
    echo ""
}

# Main menu
show_menu() {
    echo ""
    echo "Select an option / Selecciona una opción:"
    echo "1) Show current icons / Mostrar iconos actuales"
    echo "2) Add icons from directory / Añadir iconos desde directorio"
    echo "3) Add single icon / Añadir un solo icono"
    echo "4) List icons in QRC / Listar iconos en QRC"
    echo "5) Backup QRC file / Hacer backup del QRC"
    echo "6) Restore QRC backup / Restaurar backup del QRC"
    echo "7) Rebuild project / Recompilar proyecto"
    echo "8) Exit / Salir"
    echo ""
    read -p "Option / Opción: " choice
    echo ""
    
    case $choice in
        1)
            show_current_icons
            ;;
        2)
            read -p "Enter directory path with SVG icons / Ruta del directorio con iconos SVG: " icon_dir
            if [[ ! -d "$icon_dir" ]]; then
                print_error "Directory does not exist / El directorio no existe"
                return
            fi
            
            # Backup QRC
            cp "$QRC_FILE" "$QRC_BACKUP"
            print_success "QRC backup created / Backup del QRC creado"
            
            count=0
            for icon in "$icon_dir"/*.svg; do
                if is_svg "$icon"; then
                    icon_name=$(basename "$icon")
                    cp "$icon" "$ICONS_DIR/"
                    print_success "Copied / Copiado: $icon_name"
                    
                    if add_icon_to_qrc "$icon_name"; then
                        ((count++))
                    fi
                fi
            done
            
            print_success "Added $count new icons / Añadidos $count iconos nuevos"
            print_info "Don't forget to rebuild! / ¡No olvides recompilar!"
            ;;
        3)
            read -p "Enter SVG file path / Ruta del archivo SVG: " icon_path
            if ! is_svg "$icon_path"; then
                print_error "Not a valid SVG file / No es un archivo SVG válido"
                return
            fi
            
            icon_name=$(basename "$icon_path")
            cp "$icon_path" "$ICONS_DIR/"
            print_success "Copied / Copiado: $icon_name"
            
            # Backup QRC
            cp "$QRC_FILE" "$QRC_BACKUP"
            
            if add_icon_to_qrc "$icon_name"; then
                print_success "Icon added successfully / Icono añadido exitosamente"
                print_info "Don't forget to rebuild! / ¡No olvides recompilar!"
            fi
            ;;
        4)
            list_qrc_icons
            ;;
        5)
            cp "$QRC_FILE" "$QRC_BACKUP"
            print_success "Backup created at / Backup creado en: $QRC_BACKUP"
            ;;
        6)
            if [[ -f "$QRC_BACKUP" ]]; then
                cp "$QRC_BACKUP" "$QRC_FILE"
                print_success "QRC file restored from backup / Archivo QRC restaurado desde backup"
            else
                print_error "No backup found / No se encontró backup"
            fi
            ;;
        7)
            print_info "Rebuilding project / Recompilando proyecto..."
            cd "$PROJECT_DIR"
            
            # Try to find qmake
            QMAKE=$(which qmake-qt5 2>/dev/null || which qmake 2>/dev/null || echo "")
            
            if [[ -z "$QMAKE" ]]; then
                print_error "qmake not found. Install Qt5 development tools."
                print_error "qmake no encontrado. Instala las herramientas de desarrollo de Qt5."
                print_info "Run / Ejecuta: sudo apt-get install qt5-qmake qtbase5-dev"
                return
            fi
            
            print_info "Using qmake: $QMAKE"
            
            # Clean and rebuild
            if [[ -f "Makefile" ]]; then
                make clean 2>/dev/null || true
            fi
            
            $QMAKE marrta.pro
            make -j$(nproc)
            
            if [[ $? -eq 0 ]]; then
                print_success "Build completed successfully / Compilación completada exitosamente"
            else
                print_error "Build failed / Compilación falló"
            fi
            ;;
        8)
            print_info "Goodbye! / ¡Adiós!"
            exit 0
            ;;
        *)
            print_error "Invalid option / Opción inválida"
            ;;
    esac
}

# Main loop
while true; do
    show_menu
done
