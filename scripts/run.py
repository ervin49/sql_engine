import subprocess
import os
import sys
import shutil

# --- FUNCȚIE PENTRU GĂSIREA CMAKE ---
def find_cmake():
    """Găsește cmake.exe pe Windows"""
    print("[Info] Caut CMake...")

    # 1. Verifică dacă este în PATH
    cmake_path = shutil.which("cmake")
    if cmake_path:
        print(f"[OK] CMake găsit în PATH: {cmake_path}")
        return cmake_path

    # 2. Căutare manuală în locații comune
    common_paths = [
        r"C:\Program Files\CMake\bin\cmake.exe",
        r"C:\Program Files (x86)\CMake\bin\cmake.exe",
        os.path.expanduser(r"~\AppData\Local\Programs\CMake\bin\cmake.exe"),
    ]

    print("[Info] Caut în locații standard...")
    for path in common_paths:
        print(f"  Verific: {path}")
        if os.path.exists(path):
            print(f"[OK] CMake găsit: {path}")
            return path

    # 3. Scanare completă în Program Files
    print("[Info] Scanez Program Files...")
    program_files = [
        os.environ.get("ProgramFiles", r"C:\Program Files"),
        os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)")
    ]

    for pf in program_files:
        if os.path.exists(pf):
            cmake_dir = os.path.join(pf, "CMake")
            if os.path.exists(cmake_dir):
                print(f"  Scanez: {cmake_dir}")
                for root, dirs, files in os.walk(cmake_dir):
                    if "cmake.exe" in files:
                        found_path = os.path.join(root, "cmake.exe")
                        print(f"[OK] CMake găsit: {found_path}")
                        return found_path

    return None

# --- 1. CONFIGURARE CĂI ---
current_script_path = os.path.abspath(__file__)
script_dir = os.path.dirname(current_script_path)
project_dir = os.path.dirname(script_dir)

build_dir = os.path.join(project_dir, "build")
executable_name = "sql_engine"

# Comanda SQL de test
input_text = (
    "CREATE    TABLE produse IF NOT EXISTS ((id_produs, INT, 10, 0), (denumire, VARCHAR, 50, ''), (stoc, INT, 5, 0), (pret, FLOAT, 8, 0.0), (categorie, VARCHAR, 30, 'General'), (cod_bar, VARCHAR, 20, ''))\n"
    "INSERT INTO produse VALUES (101, \"Laptop Gaming X\", 15, 4500.99, \"Laptopuri\", \"LPT-GAM-2024\")\n"
    "INSERT INTO produse VALUES (102, \"Mouse Wireless\", 120, 89.50, \"Periferice\", \"PER-MSE-001\")\n"
    "INSERT INTO produse VALUES (103, \"Monitor UltraWide\", 7, 1250.00, \"Monitoare\", 998877665544332211)\n"
    "SELECT ALL FROM produse\n"
    "SELECT (denumire, pret, categorie, stoc) FROM produse where denumire = 'Mouse Wireless'\n"
)

print("=" * 70)
print(f"    SQL ENGINE - BUILD & TEST")
print("=" * 70)
print(f"Proiect: {project_dir}")
print(f"Build:   {build_dir}")
print("=" * 70 + "\n")

# --- GĂSIRE CMAKE ---
cmake_exe = find_cmake()

if not cmake_exe:
    print("\n" + "!" * 70)
    print("[EROARE] CMake nu a fost gasit!")
    print("!" * 70)
    print("\nVerifică manual unde este instalat CMake:")
    print("1. Deschide File Explorer")
    print("2. Caută 'cmake.exe' în C:\\")
    print("3. Când îl găsești, copiază calea completă (ex: C:\\Program Files\\CMake\\bin\\cmake.exe)")
    print("\nDacă nu este instalat:")
    print("- Descarcă de la: https://cmake.org/download/")
    print("- Instalează și bifează 'Add CMake to system PATH'")
    print("\nDupă instalare/găsire, modifică acest script:")
    print("  Adaugă linia: cmake_exe = r'CALEA_COMPLETA_CATRE_CMAKE.EXE'")
    print("  imediat după linia 'cmake_exe = find_cmake()'")
    print("!" * 70)

    # Permite utilizatorului să introducă manual calea
    print("\n[Opțional] Poți introduce manual calea către cmake.exe acum:")
    manual_path = input("Calea către cmake.exe (sau Enter pentru a sări): ").strip()

    if manual_path and os.path.exists(manual_path):
        cmake_exe = manual_path
        print(f"[OK] Folosim: {cmake_exe}")
    else:
        input("\nApasă Enter pentru a ieși...")
        sys.exit(1)

print(f"\n[Info] Folosim CMake: {cmake_exe}\n")

# --- 2. BUILD PROIECT ---
os.makedirs(build_dir, exist_ok=True)

print("=" * 70)
print("PASUL 1: Configurare CMake")
print("=" * 70)

# Detectează compilatorul disponibil
print("[Info] Detectez compilatorul disponibil...")

# Verifică MinGW
mingw_make = shutil.which("mingw32-make")
if not mingw_make:
    mingw_make = shutil.which("mingw64-make")
if not mingw_make:
    mingw_make = shutil.which("make")

# Verifică Ninja
ninja = shutil.which("ninja")

# Verifică Visual Studio (prin vswhere)
vswhere_path = r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
has_vs = os.path.exists(vswhere_path)

# Alege generator-ul
generator = None
if mingw_make:
    generator = "MinGW Makefiles"
    print(f"[OK] Găsit MinGW: {mingw_make}")
elif ninja:
    generator = "Ninja"
    print(f"[OK] Găsit Ninja: {ninja}")
elif has_vs:
    generator = None  # Folosește default-ul Visual Studio
    print("[OK] Găsit Visual Studio")
else:
    print("\n" + "!" * 70)
    print("[EROARE] Nu am găsit niciun compiler!")
    print("!" * 70)
    print("\nAi nevoie de unul din următoarele:")
    print("1. MinGW (recomandat pentru proiecte simple)")
    print("   - Descarcă: https://sourceforge.net/projects/mingw-w64/")
    print("   - Sau instalează prin MSYS2: https://www.msys2.org/")
    print("\n2. Visual Studio (pentru proiecte mai complexe)")
    print("   - Descarcă Community Edition: https://visualstudio.microsoft.com/")
    print("   - La instalare, bifează 'Desktop development with C++'")
    print("\n3. Ninja (opțional, dacă ai deja un compilator)")
    print("   - Descarcă: https://github.com/ninja-build/ninja/releases")
    print("!" * 70)
    input("\nApasă Enter pentru a ieși...")
    sys.exit(1)

# Rulează CMake cu generator-ul potrivit
if generator:
    print(f"[Info] Folosesc generator: {generator}")
    res_config = subprocess.run(
        [cmake_exe, "-S", project_dir, "-B", build_dir, "-G", generator],
        check=False
    )
else:
    print("[Info] Folosesc generator default (Visual Studio)")
    res_config = subprocess.run(
        [cmake_exe, "-S", project_dir, "-B", build_dir],
        check=False
    )

if res_config.returncode != 0:
    print("\n[EROARE] Configurare CMake esuata!")
    print("Verifică că ai CMakeLists.txt în folder-ul proiectului.")
    input("\nApasă Enter pentru a ieși...")
    sys.exit(res_config.returncode)

print("\n" + "=" * 70)
print("PASUL 2: Compilare proiect")
print("=" * 70)

# Pe Windows, specificăm configurația (Debug/Release)
if os.name == "nt":
    res_build = subprocess.run(
        [cmake_exe, "--build", build_dir, "--config", "Debug", "--parallel"],
        check=False
    )
else:
    res_build = subprocess.run(
        [cmake_exe, "--build", build_dir, "--parallel"],
        check=False
    )

if res_build.returncode != 0:
    print("\n[EROARE] Compilare C++ esuata!")
    print("Verifică erorile de compilare de mai sus.")
    input("\nApasă Enter pentru a ieși...")
    sys.exit(res_build.returncode)

# --- 3. GĂSIRE EXECUTABIL ---
print("\n" + "=" * 70)
print("PASUL 3: Găsire executabil")
print("=" * 70)

exe_path = os.path.join(project_dir, "bin", executable_name)
if os.name == "nt":
    exe_path += ".exe"

if not os.path.exists(exe_path):
    # Fallback paths
    paths_to_check = [
        os.path.join(build_dir, "Debug", executable_name + (".exe" if os.name == "nt" else "")),
        os.path.join(build_dir, "Release", executable_name + (".exe" if os.name == "nt" else "")),
        os.path.join(build_dir, executable_name + (".exe" if os.name == "nt" else ""))
    ]

    print("[Info] Caut executabilul...")
    for p in paths_to_check:
        print(f"  Verific: {p}")
        if os.path.exists(p):
            exe_path = p
            print(f"[OK] Găsit: {exe_path}")
            break

if not os.path.exists(exe_path):
    print(f"\n[EROARE] Nu gasesc executabilul '{executable_name}'!")
    print(f"\nCai verificate:")
    for p in paths_to_check:
        print(f"  - {p}")
    input("\nApasă Enter pentru a ieși...")
    sys.exit(1)

print(f"[OK] Executabil găsit: {exe_path}")

# --- 4. RULARE INTERACTIVĂ ---
print("\n" + "=" * 70)
print("PASUL 4: Rulare executabil")
print("=" * 70 + "\n")

process = subprocess.Popen(
    exe_path,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True,
    bufsize=1,
    universal_newlines=True
)

import threading
import queue
import time

# Cozi pentru output
output_queue = queue.Queue()
error_queue = queue.Queue()

def read_output(pipe, q):
    """Citește output într-un thread separat"""
    try:
        for line in iter(pipe.readline, ''):
            if line:
                q.put(line)
    except:
        pass
    finally:
        pipe.close()

# Start threads pentru citire output
stdout_thread = threading.Thread(target=read_output, args=(process.stdout, output_queue))
stderr_thread = threading.Thread(target=read_output, args=(process.stderr, error_queue))
stdout_thread.daemon = True
stderr_thread.daemon = True
stdout_thread.start()
stderr_thread.start()

try:
    print("[Python] Trimitere comenzi SQL automate...\n")

    # Trimitem comenzile SQL
    for line in input_text.split('\n'):
        if line.strip():
            print(f"\033[94m[SQL]\033[0m {line}")
            process.stdin.write(line + "\n")
            process.stdin.flush()

            # Citim output pentru această comandă
            time.sleep(0.3)  # Delay pentru a permite procesului să răspundă

            while not output_queue.empty():
                output_line = output_queue.get()
                if "Success:" in output_line:
                    print(f"\033[92m{output_line}\033[0m", end='')
                elif "Error:" in output_line or "ERROR:" in output_line:
                    print(f"\033[91m{output_line}\033[0m", end='')
                else:
                    print(output_line, end='')

            while not error_queue.empty():
                print(f"\033[91mSTDERR: {error_queue.get()}\033[0m", end='')

    print("\n" + "=" * 70)
    print("Comenzi automate finalizate!")
    print("Acum poți introduce comenzi SQL manual.")
    print("Scrie 'exit' pentru a închide programul.")
    print("=" * 70 + "\n")

    # Loop pentru input utilizator
    while True:
        # Verificăm dacă procesul mai rulează
        if process.poll() is not None:
            print("\n\033[91m[!] Procesul C++ s-a închis.\033[0m")
            break

        try:
            # Golim cozile înainte de input nou
            while not output_queue.empty():
                print(output_queue.get(), end='')

            user_input = input("\033[96mSQL> \033[0m")

            if user_input.strip().lower() == 'exit':
                process.stdin.write("exit\n")
                process.stdin.flush()
                break

            if user_input.strip():
                process.stdin.write(user_input + "\n")
                process.stdin.flush()

                # Citim output
                time.sleep(0.3)

                while not output_queue.empty():
                    output_line = output_queue.get()
                    if "Success:" in output_line:
                        print(f"\033[92m{output_line}\033[0m", end='')
                    elif "Error:" in output_line or "ERROR:" in output_line:
                        print(f"\033[91m{output_line}\033[0m", end='')
                    else:
                        print(output_line, end='')

                while not error_queue.empty():
                    print(f"\033[91mSTDERR: {error_queue.get()}\033[0m", end='')

        except EOFError:
            break
        except KeyboardInterrupt:
            print("\n\033[93m[!] Ctrl+C detectat. Oprire...\033[0m")
            break

except BrokenPipeError:
    print("\n" + "=" * 70)
    print("\033[91m[!] BROKEN PIPE DETECTAT\033[0m")
    print("Programul C++ s-a închis brusc în timp ce Python trimitea date.")
    print("=" * 70)

except Exception as e:
    print(f"\n\033[91m[!] Eroare: {e}\033[0m")
    import traceback
    traceback.print_exc()

finally:
    # Închidere gracioasă
    try:
        process.stdin.close()
    except:
        pass

    try:
        process.wait(timeout=2)
    except subprocess.TimeoutExpired:
        print("[!] Procesul nu s-a închis, îl terminăm forțat...")
        process.terminate()
        try:
            process.wait(timeout=1)
        except subprocess.TimeoutExpired:
            process.kill()

# Citim orice output rămas
while not output_queue.empty():
    print(output_queue.get(), end='')

while not error_queue.empty():
    print(f"STDERR: {error_queue.get()}", end='')

# Verificăm codul de ieșire
exit_code = process.returncode
print("\n" + "=" * 70)
if exit_code == 0:
    print("\033[92m✓ Procesul C++ s-a terminat cu succes (cod: 0)\033[0m")
elif exit_code == -11:
    print(f"\033[91m✗ SEGMENTATION FAULT (cod: {exit_code})\033[0m")
    print("  → Verifică pointerii și alocarea de memorie în C++.")
elif exit_code == 3 or exit_code == -1073741819 or exit_code == 3221225477:
    print(f"\033[91m✗ ACCESS VIOLATION / ABORT (cod: {exit_code})\033[0m")
    print("  → Verifică pointerii și accesul la memorie.")
    print("  → Poate fi std::bad_alloc (memorie insuficientă).")
elif exit_code is not None:
    print(f"\033[91m✗ Eroare (cod: {exit_code})\033[0m")
else:
    print("? Cod de ieșire necunoscut")
print("=" * 70)

input("\n\033[93mApasă Enter pentru a ieși...\033[0m")