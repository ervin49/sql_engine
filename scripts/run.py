import subprocess
import os
import sys
import time

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

print(f"--- Proiect: {project_dir} ---")
print(f"--- Build în: {build_dir} ---")

# --- 2. BUILD PROIECT ---
os.makedirs(build_dir, exist_ok=True)

print("--- Configurare CMake ---")
# Folosim shell=True doar pe Windows dacă cmake nu e în PATH, dar de obicei nu e nevoie
res_config = subprocess.run(["cmake", "-S", project_dir, "-B", build_dir])

if res_config.returncode != 0:
    print("\n[!] Configurare CMake eșuată.")
    sys.exit(res_config.returncode)

print("--- Compilare proiect ---")
res_build = subprocess.run(["cmake", "--build", build_dir, "--parallel"])

if res_build.returncode != 0:
    print("\n[!] Compilare C++ eșuată.")
    sys.exit(res_build.returncode)

# --- 3. GĂSIRE EXECUTABIL ---
exe_path = os.path.join(project_dir, "bin", executable_name)
if os.name == "nt":
    exe_path += ".exe"

if not os.path.exists(exe_path):
    # Fallback paths
    paths_to_check = [
        os.path.join(build_dir, executable_name),
        os.path.join(build_dir, "Debug", executable_name),
        os.path.join(build_dir, "Release", executable_name)
    ]
    for p in paths_to_check:
        if os.name == "nt" and not p.endswith(".exe"): p += ".exe"
        if os.path.exists(p):
            exe_path = p
            break

if not os.path.exists(exe_path):
    print(f"Eroare: Nu găsesc executabilul '{executable_name}'.")
    sys.exit(1)

print(f"--- Rulare executabil: {exe_path} ---")

# --- 4. RULARE INTERACTIVĂ ---
process = subprocess.Popen(
    exe_path,
    stdin=subprocess.PIPE,
    stdout=sys.stdout,
    stderr=sys.stderr,
    text=True,
    bufsize=0
)

try:
    print(f"[Python] Trimitere comenzi SQL...")
    # Încercăm să scriem. Dacă C++ crapă aici, prindem eroarea.
    process.stdin.write(input_text)
    process.stdin.flush()

    print("[Python] Comenzi trimise. Aștept input utilizator...")

    while True:
        # Verificăm constant dacă C++ mai trăiește
        if process.poll() is not None:
            break

        try:
            user_input = input()
            process.stdin.write(user_input + "\n")
            process.stdin.flush()
        except EOFError:
            break

except BrokenPipeError:
    print("\n" + "=" * 50)
    print("[!] BROKEN PIPE DETECTAT")
    print("Programul C++ s-a închis brusc în timp ce Python trimitea date.")
    print("=" * 50)

except KeyboardInterrupt:
    print("\nOprire forțată.")
    process.terminate()

# Așteptăm puțin să fim siguri că procesul și-a scris ultimul mesaj de eroare
process.wait()

# Verificăm codul de ieșire
exit_code = process.returncode
print(f"\n--- Procesul C++ s-a terminat cu codul: {exit_code} ---")

if exit_code == -11:
    print(">> Codul -11 indică un SEGMENTATION FAULT (Linux/Mac).")
    print(">> Verifică pointerii și alocarea de memorie în C++.")
elif exit_code == 3221225477 or exit_code == -1073741819:  # 0xC0000005
    print(">> Codul indică un ACCESS VIOLATION (Windows).")
    print(">> Verifică pointerii și accesul la memorie.")
elif exit_code != 0:
    print(">> Codul diferit de 0 indică o eroare (assert fail, abort, exception).")
