#!/usr/bin/env python3

import sys
import math
from typing import List, Tuple

class Pixel:
    def __init__(self, r: int, g: int, b: int):
        self.r = r
        self.g = g
        self.b = b

def leer_ppm(filename: str) -> Tuple[int, int, List[Pixel]]:
    try:
        with open(filename, 'r') as file:
            lines = file.readlines()
    except Exception as e:
        print(f"Error al abrir el archivo: {filename}")
        print(f"Error: {e}")
        return 0, 0, []
    
    # Eliminar líneas de comentarios y espacios en blanco
    clean_lines = []
    for line in lines:
        line = line.strip()
        if line and not line.startswith('#'):
            clean_lines.append(line)
    
    if not clean_lines or clean_lines[0] != 'P3':
        print(f"Formato no soportado (debe ser P3): {clean_lines[0] if clean_lines else 'vacío'}")
        return 0, 0, []
    
    # Leer dimensiones
    dimensions = clean_lines[1].split()
    if len(dimensions) < 2:
        print("Error: formato de dimensiones inválido")
        return 0, 0, []
    
    try:
        width = int(dimensions[0])
        height = int(dimensions[1])
    except ValueError:
        print("Error: dimensiones no son números válidos")
        return 0, 0, []
    
    # Leer valor máximo
    try:
        max_val = int(clean_lines[2])
    except (ValueError, IndexError):
        print("Error: valor máximo no válido")
        return 0, 0, []
    
    # Leer datos de píxeles
    data = []
    pixel_values = []
    
    # Recoger todos los valores numéricos
    for line in clean_lines[3:]:
        pixel_values.extend(line.split())
    
    # Convertir a píxeles
    try:
        for i in range(0, len(pixel_values), 3):
            if i + 2 >= len(pixel_values):
                break
            r = int(pixel_values[i])
            g = int(pixel_values[i + 1])
            b = int(pixel_values[i + 2])
            data.append(Pixel(r, g, b))
    except ValueError as e:
        print(f"Error al leer datos de píxeles: {e}")
        return 0, 0, []
    
    if len(data) != width * height:
        print(f"Advertencia: número de píxeles ({len(data)}) no coincide con dimensiones ({width}x{height} = {width * height})")
    
    return width, height, data

def diferencia_pixel(a: Pixel, b: Pixel) -> float:
    return (abs(a.r - b.r) + abs(a.g - b.g) + abs(a.b - b.b)) / 3.0

def main():
    if len(sys.argv) != 3:
        print(f"Uso: {sys.argv[0]} imagen1.ppm imagen2.ppm")
        sys.exit(1)
    
    file1 = sys.argv[1]
    file2 = sys.argv[2]
    
    width1, height1, img1 = leer_ppm(file1)
    width2, height2, img2 = leer_ppm(file2)
    
    if not img1 or not img2:
        sys.exit(1)
    
    if width1 != width2 or height1 != height2:
        print("Las imágenes tienen diferente tamaño")
        sys.exit(1)
    
    if len(img1) != len(img2):
        print("Las imágenes tienen diferente número de píxeles")
        sys.exit(1)
    
    sum_squared_diff = 0.0
    
    for i in range(len(img1)):
        diff = diferencia_pixel(img1[i], img2[i])
        sum_squared_diff += diff * diff

    mse = math.sqrt(sum_squared_diff / len(img1))
    
    print(f"Error cuadrático medio (MSE): {mse}")
    
    if mse < 10:
        print("Resultado: Aceptable")
    else:
        print("Resultado: No aceptable")

if __name__ == "__main__":
    main()