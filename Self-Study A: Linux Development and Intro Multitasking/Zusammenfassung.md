# Zusammenfassung Self-Study A

## PDF 1

### 1. Intro: C/C++ – Programming under Linux

- **Toolchain & Kompilierungsprozess**:   

  - Der Weg vom Quellcode zur ausführbaren Datei verläuft in mehreren Schritten:   

    1. **Preprocessor (Präprozessor)**

          

    2. **Compiler** (erzeugt Assemblercode)   

    3. **Assembler** (erzeugt Objektcode / `.o`)   

    4. **Link Editor / Linker** (bindet Bibliotheken ein und erstellt die ausführbare Datei)   

  - Standard-Compiler unter Linux sind `gcc` (für C) und `g++` (für C++).   

  - Ausführbare Dateien besitzen unter Linux im Gegensatz zu Windows keine `.EXE`-Endung (Ausführung z. B. über `./myprog`).   

- **Wichtige Compiler-Flags (gcc/g++)**:   

  - `-o filename`: Benennt die Ausgabedatei.   
  - `-c`: Kompiliert nur (erzeugt `.o`-Dateien), ohne zu linken.   
  - `-g`: Fügt Debugging-Symbole hinzu (für `gdb`/`ddd`).   
  - `-Wall`: Aktiviert alle gängigen Warnungen.   
  - `-O`: Aktiviert Code-Optimierungen.   
  - `-I` / `-L` / `-l`: Pfade für Header-Dateien, Bibliotheken und das Verlinken von Bibliotheken (z. B. `-lm` für die Math-Bibliothek).   

- **Automatisierung mit `make` & Makefiles**:   

  - `make` liest ein `Makefile`, um den Build-Prozess zu automatisieren und Abhängigkeiten zu verwalten.   

  - Es kompiliert nur Dateien neu, deren Zeitstempel sich geändert haben.   

  - Typischer Syntax-Aufbau:

    Makefile

    ```makefile
    Target: Dependencies
    	Command
    ```

  - Konventionen: Targets wie `all` (Standard-Build) und `clean` (Löschen generierter Dateien).   

- **Standard Streams & Argument Parsing**:   

  - Die drei Standard-Streams sind: `stdin` (File Descriptor 0), `stdout` (1) und `stderr` (2).   
  - **Einstiegspunkt `main`**: `int main(int argc, char* argv[])`.   
    - `argc` zählt die übergebenen Argumente (mindestens 1, da `argv[0]` der Programmname ist).   
    - Der Rückgabewert `0` signalisiert Erfolg (`EXIT_SUCCESS`), Werte `> 0` signalisieren Fehler.   
  - **Option Parsing**: Nutzung der Funktion `getopt()` (aus `getopt.h`) zum Parsen von Befehlszeilen-Optionen (z. B. `-a` oder `-f filename`).   

### 2. Process Management

- **Prozess-Grundlagen**:   
  - Ein Prozess ist eine laufende Instanz eines Programms mit eigenem Adressraum und einer eindeutigen PID (Process ID).   
  - **Prozess-Zustände**: *Running*, *Sleeping* (Interruptable/Uninterruptable/Killable), *Stopped* und *Zombie* (beendet, aber Ressourcen noch nicht vom Elternprozess freigegeben).   
  - Der Kernel verwaltet den Prozesskontext in der Prozesstabelle (u. a. Program Counter, Stack Pointer, Register, UID/GID, Verzeichnisse, File Pointer).   
- **Erzeugung von Prozessen mit `fork()`**:   
  - `fork()` erzeugt einen neuen Kindprozess als exakte Kopie des Elternprozesses (mit eigenem Daten- und Stack-Segment).   
  - Rückgabewert von `fork()`:
    - `-1`: Fehler bei der Erzeugung.   
    - `0`: Code wird im **Kindprozess** ausgeführt.   
    - `> 0`: Code wird im **Elternprozess** ausgeführt (Wert ist die PID des Kindes).   
- **Warten auf Prozesse (`wait` / `waitpid`)**:   
  - `wait()` bzw. `waitpid()` sperrt/prüft den Elternprozess, bis ein Kindprozess beendet ist, um Zombie-Prozesse zu vermeiden.   
  - Die Option `WNOHANG` verhindert das Blockieren.   
  - Makros zur Statusauswertung des Beendigungscodes: `WIFEXITED()`, `WEXITSTATUS()`, `WIFSIGNALED()`, `WTERMSIG()`, `WIFSTOPPED()`.   
- **Überdecken von Prozessen mit `exec()`**:   
  - Ersetzt das aktuelle Prozess-Image durch ein neues Programm.   
  - Bei erfolgreichem `exec()` kehrt die Funktion nicht mehr zum aufrufenden Code zurück.   
  - Varianten: `execl`, `execv`, `execvp`, `execve` (unterscheiden sich durch Argumentübergabe als Liste/Array, `$PATH`-Suche und Umgebungsvariablen).   

### 3. Threads

- **Konzept & Unterschied zu Prozessen**:   
  - Ein Prozess ist die Einheit des Ressourceneigentums; ein Thread ist eine Ausführungseinheit (*dispatchable unit of work* / „Lightweight Process“).   
  - Threads innerhalb desselben Prozesses teilen sich den Adressraum und Systemressourcen (z. B. File Handles), besitzen jedoch eigene Execution Stacks, Register und Ausführungszustände.   
  - **Vorteile gegenüber `fork()`**: Schnellere Erzeugung/Beendigung, schnellerer Kontextwechsel und einfachere Kommunikation über gemeinsamen Speicher.   
- **Architekturmodelle für Multi-Threading**:   
  - **Dispatcher/Worker Model**: Ein Dispatcher-Thread nimmt Aufgaben an und verteilt sie an Worker-Threads.   
  - **Team Model**: Alle Threads sind gleichberechtigt und greifen auf einen gemeinsamen Input-Puffer zu (Synchronisation erforderlich).   
  - **Pipeline Model**: Threads arbeiten sequentiell als Kette zusammen.   
- **User- vs. Kernel-Level Threads**:   
  - *User Level Threads*: Vom Betriebssystem nicht direkt erkannt; Threading wird durch eine Anwendungsbibliothek gesteuert.   
  - *Kernel Level Threads*: Das Betriebssystem verwaltet die Threads direkt im Kernel.   
- **POSIX Threads (`pthread`)**:   
  - Standardisierte C-Schnittstelle (`#include <pthread.h>`).   
  - Erzeugung mittels `pthread_create()`.   
  - Beim Kompilieren muss das Linker-Flag `-lpthread` angegeben werden.   

## PDF 2

Hier ist die Zusammenfassung der Präsentation zum Thema **Threads** (`PowerPoint-Präsentation - 06 threads.pdf`):

### 1. Einleitung & Motivation

- **Problem bei Prozessen:** Ein klassischer Prozess bündelt Ressourcen und besitzt nur einen einzigen Ausführungsstrang (*Single-Threaded*). Die Kommunikation zwischen mehreren Prozessen ist aufwendig, da sie keinen gemeinsamen Adressraum nutzen (z. B. bei `fork()`).   
- **Lösung durch Threads:** Threads erlauben die **nebenläufige Ausführung** von Instruktionen innerhalb desselben Prozesses (*Lightweight Processes* / Ausführungseinheiten).   

### 2. Eigenschaften & Attribute von Threads

- **Gemeinsam genutzt (Shared):** Alle Threads eines Prozesses teilen sich denselben Adressraum sowie gemeinsame Ressourcen (z. B. File Handles). Endet der Prozess, werden auch alle zugehörigen Threads beendet.   
- **Eigenständig (Eigener Kontext):** Jeder Thread besitzt einen eigenen Ausführungszustand (*Running*, *Ready*, *Blocked*), einen eigenen Execution Stack und eigene Register.   

### 3. Vorteile gegenüber Prozessen (`fork()`)

- Geringerer Meta-Informationsaufwand.   
- Schnellere Erzeugung und Beendigung.   
- Schnellerer Kontextwechsel (*Thread Switch* statt vollständigem *Process Context Switch*).   
- Leistungssteigerung durch echte Parallelität auf Mehrprozessorsystemen.   
- Einfache Kommunikation ohne Kernel-Aufrufe (erfordert jedoch Synchronisation bei kritischen Abschnitten).   

### 4. Anwendungsbeispiele & Thread-Modelle

- **Beispiele:** Textverarbeitung (Auto-Save, Rechtschreibprüfung, Formatierung) oder Webserver (1 Thread pro Client-Anfrage).   
- **Architekturmodelle:**
  1. **Dispatcher/Worker Model:** Ein Dispatcher-Thread nimmt Aufgaben an und verteilt sie an Worker-Threads.   
  2. **Team Model:** Alle Threads arbeiten gleichberechtigt und greifen gemeinsam auf den Input-Puffer zu (Synchronisation notwendig).   
  3. **Pipeline Model:** Threads sind sequentiell als Kette organisiert; jeder übernimmt einen Teilbereich der Aufgabe.   

### 5. Arten von Threads & Implementierung

- **User Level vs. Kernel Level:**
  - *User Level Threads:* Das Betriebssystem weiß nichts von den Threads; die Verwaltung erfolgt rein über eine Anwendungsbibliothek im User Space.   
  - *Kernel Level Threads:* Das Betriebssystem verwaltet die Threads direkt im Kernel Space.   
- **POSIX Threads (`pthread`):**
  - Standardisierte C-Schnittstelle (`pthread.h`) mit rund 100 Funktionen (Präfix `pthread_`).   
  - Erzeugung mittels `pthread_create()` über eine Callback-Funktion.   
  - Zum Kompilieren/Linken ist das Flag `-lpthread` erforderlich.   

## PDF 3

Hier ist die Zusammenfassung der Präsentation zum Thema **Process Management / Prozesse** (`Process management - Processes.pdf`):

### 1. Prozess-Grundlagen & Zustände

- **Definition:** Ein Prozess ist eine laufende Instanz eines Programms und stellt eine zentrale Abstraktion des Betriebssystems dar.   
- **Prozess-ID (PID):** Jede Prozessinstanz besitzt einen eigenen Adressraum und eine eindeutige PID zur Ressourcenverwaltung durch das Betriebssystem.   
- **Prozesszustände:**
  - **Running:** Wird aktuell ausgeführt oder ist dafür eingeplant.   
  - **Sleeping:** Bereit, aber inaktiv; wartet auf Ereignisse (z. B. I/O-Operationen) – unterteilt in *Interruptable*, *Uninterruptable* und *Killable*.   
  - **Stopped:** Nicht bereit; benötigt ein `SIGCONT`-Signal zur Fortsetzung.   
  - **Zombie:** Prozess ist beendet, aber seine Ressourcen wurden vom Elternprozess noch nicht freigegeben.   

### 2. Prozesskontext & Attribute

- **Prozesstabelle:** Das Betriebssystem speichert den Prozesskontext in einer Prozesstabelle (Einträge für Prozess-, Speicher- und Dateiverwaltung).   
- **Attribute eines Prozesses:**
  - **Prozesssteuerung:** PID, Parent-PID (PPID), Priorität, Prozesszustand, Erstellungszeitstempel, genutzte CPU-Zeit (auch von Kindprozessen).   
  - **Register & Speicher:** Program Counter (PC), Stack Pointer (SP), Program Status Word (PSW), Pointer auf Code- und Datensegmente.   
  - **Rechte & Dateisystem:** Real/Effective User ID (UID), Real/Effective Group ID (GID), Arbeitsverzeichnis, Root-Verzeichnis, Umask-Bits, File-Pointer.   
- **C-Funktionen zur Identifikation:** `getpid()` liefert die eigene PID, `getppid()` die PID des Elternprozesses.   

### 3. Erzeugung von Kindprozessen (`fork()`)

- **Funktionsweise:** `fork()` erstellt einen neuen Kindprozess als exakte Kopie des Elternprozesses (inklusive Deskriptoren sowie eigenem Daten- und Stacksegment).   
- **Rückgabewerte von `fork()`:**
  - `-1`: Fehler bei der Erzeugung.   
  - `0`: Ausführung im **Kindprozess**.   
  - `> 0`: Ausführung im **Elternprozess** (Rückgabewert ist die PID des Kindes).   

### 4. Warten auf Kindprozesse (`wait` / `waitpid`)

- **Prozess-Synchronisation:**
  - `wait(int *stat_loc)` blockiert, bis ein Kindprozess endet, und gibt dessen PID zurück.   
  - `waitpid(pid_t pid, int *stat_loc, int options)` ermöglicht das gezielte Warten auf eine bestimmte PID (oder alle mit `pid == -1`).   
- **Zombie-Vermeidung:** Die Verwendung von `waitpid(-1, NULL, WNOHANG)` in einer Schleife gilt als Best Practice, um beendete Kindprozesse aufzuräumen, ohne den Elternprozess zu blockieren.   
- **Statusauswertung:** Makros aus `<sys/wait.h>` zur Prüfung des Beendigungsgrunds:
  - `WIFEXITED` / `WEXITSTATUS`: Normale Beendigung / Rückgabecode.   
  - `WIFSIGNALED` / `WTERMSIG`: Beendigung durch ein Signal.   
  - `WIFSTOPPED` / `WSTOPSIG`: Anhalten durch ein Signal.   

### 5. Ersetzen des Prozess-Images (`exec()`)

- **Funktionsweise:** Die `exec`-Funktionsfamilie überdeckt den aktuellen Prozessspeicher mit einem neuen Programm-Image, damit der Kindprozess anderen Code ausführen kann als der Elternprozess.   
- **Eigenschaft:** Bei erfolgreicher Ausführung kehrt eine `exec`-Funktion nicht mehr zurück.   
- **Varianten:**
  - `l` (List) / `v` (Vector): Argumente als Argumentenliste oder als `argv`-Array.   
  - `p` (Path): Nutzt die `$PATH`-Umgebungsvariable zur Programmsuche.   
  - `e` (Environment): Erlaubt die Angabe eigener Umgebungsvariablen.   
- **Vererbung:** Viele Attribute wie PID, UID, Arbeitsverzeichnis und offene Dateideskriptoren bleiben bei einem `exec()`-Aufruf erhalten.   