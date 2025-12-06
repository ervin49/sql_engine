import subprocess
import os
import shutil

print("=" * 70)
print("VERIFICARE COMPILATOR MINGW")
print("=" * 70 + "\n")

# Verifică g++
print("[1] Verificare g++...")
gpp = shutil.which("g++")
if gpp:
    print(f"    ✓ Găsit: {gpp}")
    try:
        result = subprocess.run(["g++", "--version"], capture_output=True, text=True)
        print(f"    Versiune: {result.stdout.split()[2] if result.stdout else 'necunoscută'}")
    except:
        pass
else:
    print("    ✗ NU a fost găsit în PATH")

# Verifică gcc
print("\n[2] Verificare gcc...")
gcc = shutil.which("gcc")
if gcc:
    print(f"    ✓ Găsit: {gcc}")
else:
    print("    ✗ NU a fost găsit în PATH")

# Verifică mingw32-make
print("\n[3] Verificare mingw32-make...")
make = shutil.which("mingw32-make")
if make:
    print(f"    ✓ Găsit: {make}")
else:
    print("    ✗ NU a fost găsit în PATH")

# Verifică cmake
print("\n[4] Verificare cmake...")
cmake = shutil.which("cmake")
if cmake:
    print(f"    ✓ Găsit: {cmake}")
else:
    print("    ✗ NU a fost găsit în PATH")

# Căutare manuală MinGW
print("\n" + "=" * 70)
print("CĂUTARE MANUALĂ MINGW")
print("=" * 70 + "\n")

possible_paths = [
    r"C:\MinGW\bin",
    r"C:\MinGW64\bin",
    r"C:\msys64\mingw64\bin",
    r"C:\msys64\mingw32\bin",
    r"C:\msys64\ucrt64\bin",
    r"C:\Program Files\mingw-w64\bin",
    r"C:\Program Files (x86)\mingw-w64\bin",
]

found_paths = []
for path in possible_paths:
    if os.path.exists(path):
        gpp_exe = os.path.join(path, "g++.exe")
        if os.path.exists(gpp_exe):
            print(f"✓ Găsit MinGW în: {path}")
            found_paths.append(path)

if not found_paths:
    print("✗ Nu am găsit MinGW în locațiile standard")
    print("\nCaută manual 'g++.exe' pe disk și vezi în ce folder este.")

# Verifică PATH actual
print("\n" + "=" * 70)
print("PATH-UL CURENT")
print("=" * 70 + "\n")

current_path = os.environ.get("PATH", "")
path_entries = current_path.split(";")

mingw_in_path = False
for entry in path_entries:
    if "mingw" in entry.lower() or "msys" in entry.lower():
        print(f"✓ {entry}")
        mingw_in_path = True

if not mingw_in_path:
    print("✗ MinGW NU este în PATH")

# SOLUȚII
print("\n" + "=" * 70)
print("SOLUȚII")
print("=" * 70 + "\n")

if found_paths and not mingw_in_path:
    print("✓ MinGW este instalat dar NU este în PATH!")
    print("\nPAȘI DE URMAT:")
    print("1. Apasă Windows + R")
    print("2. Scrie: sysdm.cpl")
    print("3. Mergi la tab-ul 'Advanced' → 'Environment Variables'")
    print("4. Sub 'System variables', selectează 'Path' → Edit")
    print("5. Click 'New' și adaugă:")
    for path in found_paths:
        print(f"   {path}")
    print("6. Click OK → OK → OK")
    print("7. ÎNCHIDE COMPLET PyCharm/IDE-ul și deschide-l din nou")
    print("8. Rulează acest script din nou pentru verificare")

elif not found_paths:
    print("✗ MinGW nu este instalat sau nu l-am găsit")
    print("\nOPȚIUNI DE INSTALARE:")
    print("\n=== OPȚIUNEA 1: MSYS2 (RECOMANDAT) ===")
    print("1. Descarcă: https://www.msys2.org/")
    print("2. Instalează în C:\\msys64")
    print("3. Deschide 'MSYS2 MSYS' din Start Menu")
    print("4. Rulează: pacman -Syu")
    print("5. Închide și redeschide MSYS2")
    print("6. Rulează: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake make")
    print("7. Adaugă în PATH: C:\\msys64\\mingw64\\bin")
    print("\n=== OPȚIUNEA 2: MinGW-w64 ===")
    print("1. Descarcă: https://sourceforge.net/projects/mingw-w64/")
    print("2. Instalează")
    print("3. Adaugă folder-ul bin în PATH")

else:
    print("✓ Totul este configurat corect!")
    print("\nDacă tot nu merge:")
    print("1. ÎNCHIDE COMPLET PyCharm/IDE-ul")
    print("2. DESCHIDE-L DIN NOU")
    print("3. Rulează script-ul de build")

input("\n\nApasă Enter pentru a ieși...")