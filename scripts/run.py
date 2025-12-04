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
    "CREATE    TABLE studenti IF NOT EXISTS ((id, INT, 11, 0), (nume, VARCHAR, 50, ''), (varsta, INT, 3, 18), (medie, FLOAT, 5, 0.0), (email, VARCHAR, 100, ''), (telefon, VARCHAR, 15, ''))\n"
    "INSERT INTO studenti VALUES (1, \"Ion Popescu\", 20, 8.75, \"ion.popescu@example.com\", \"0712345678\")\n"
    "INSERT INTO studenti VALUES (2, \"Maria Ionescu Maria\", 19, 9.30, \"maria.ionescu@example.com\", \"0722334455\")\n"
    "INSERT INTO studenti VALUES (3, \"Alexandru Constantin Regele\", 21, 9.85, \"alexandru.constantin@exemplu.com\", 072299999998877)\n"
    "SELECT ALL FROM studenti\n"
    "SELECT (nume, varsta, email, id, telefon) FROM studenti\n"
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
