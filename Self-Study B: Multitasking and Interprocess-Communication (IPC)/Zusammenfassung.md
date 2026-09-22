# Zusammenfassung Self-Study B

## PDF 1

### **1. Grundlagen der Thread-Synchronisation**

- **Notwendigkeit:** Komplexe Programme erfordern oft die gemeinsame Nutzung von Daten (Shared Data) durch mehrere Threads. Um Konsistenz zu wahren, muss der Zugriff in einer definierten Reihenfolge erfolgen.   
- **Kritischer Abschnitt („Critical Section“):** Codebereiche, die gemeinsame Daten parallel manipulieren. Ohne Synchronisation können Daten inkonsistent werden. Es muss sichergestellt werden, dass während einer Datenmanipulation keine andere Instanz zugreifen kann.   
- **Kooperativer Prozess:** Der Lese- und Schreibzugriff muss kritische Abschnitte schützen, indem Zugriffe geprüft und autorisiert werden.   

### **2. Synchronisationsmechanismen**

- **Mutex (MUTual Exclusive):**

  - Eine spezielle Semaphore-Implementierung zur gegenseitigen Ausschlusssteuerung.   

  - **Funktionen & Makros:**

    - `pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;`

         

    - `pthread_mutex_init(...)` & `pthread_mutex_destroy(...)`

         

    - `pthread_mutex_lock(...)`, `pthread_mutex_trylock(...)`, `pthread_mutex_unlock(...)`

         

- **Condition Variables:** (Werdende Bedingungen für Threads).   

### **3. Threads und Mutexes ab C++11**

- In C++11 wurden Concurrency-Features direkt in die Standardbibliothek integriert (Kompilierung mit `-std=c++11 -pthread`).   
- **Threads:** Erstellung über `std::thread t(funktion)` und Warten auf Beendigung mittels `t.join()`.   
- **Mutex:** Verwendung über `std::mutex` mit den Methoden `.lock()` und `.unlock()`.   

### **4. Coroutines und Fibers (C++20 & Systemebene)**

- **Coroutines (C++20):**
  - Funktionen, deren Ausführung unterbrochen und später fortgesetzt werden kann.   
  - Ermöglichen asynchronen Code im sequentiellen Stil (z. B. für nicht-blockierendes I/O).   
  - Hauptschlüsselwörter: `co_await`, `co_yield`, `co_return`.   
- **Fibers:**
  - Ähnliches Konzept wie Coroutines, jedoch auf Systemebene (leichtgewichtige Threads, die nicht parallel laufen).   
  - Unterscheidung: Coroutines sind Konstrukte auf Sprachebene (Kontrollfluss), während Fibers auf Systemebene arbeiten.   

## PDF 2

### **1. Grundlagen von InterProcess Communication (IPC)**

- **Ziel:** Datenaustausch, Koordination und Synchronisation zwischen Prozessen, da Daten im virtuellen Adressraum eines Prozesses grundsätzlich geschützt sind.   
- **Notwendigkeit:** Prozesse müssen oft Daten teilen, hängen voneinander ab oder müssen den Zugriff auf Systemressourcen koordinieren.   
- **Synchronisationsarten:**
  - **Explizit:** Verwendung spezifischer Synchronisationsmechanismen.   
  - **Implizit:** Basiert auf blockierendem Datenfluss (z. B. beim Senden/Empfangen von Nachrichten oder beim Lesen/Schreiben in Pipes).   

### **2. Signale (Signals)**

- **Asynchrone IPC** zwischen Prozessen (z. B. `<Ctrl + C>` / `SIGINT` oder das Beenden eines Kindprozesses via `SIGCHLD`).   
- **Signalhandler:** Routinen zur Behandlung von Signalen, die mit `signal()` registriert werden und asynchron zum normalen Programmablauf aufgerufen werden.   

### **3. Message Queues vs. Named Pipes**

- **Message Queues:**
  - **Paket- / strukturorientierte** Kommunikation über Warteschlangen.   
  - Eindeutige systemweite Kennung (Key) als „Briefkasten“.   
  - Nachrichten können priorisiert werden (Abruf nach FIFO oder nach Priorität).   
  - C-Systemaufrufe: `msgget()` (Erstellen/Zugreifen), `msgsnd()` (Senden), `msgrcv()` (Empfangen), `msgctl()` (Löschen/Verwalten).   
  - Verwaltung über die Shell mittels `ipcs -q` und `ipcrm`.   
- **Named Pipes (FIFOs):**
  - **Datenstrom- / unstrukturierte** Kommunikation (Verwendung ähnlich wie Dateizugriff).   
  - Identifikation über Dateinamen im Dateisystem; rein sequentielles FIFO-Lesen.   
  - Lesende Empfänger konsumieren Daten (Lesen löscht die Nachricht aus der Pipe).   
  - Erstellung über `mkfifo()`, Zugriff über Standard-I/O-Funktionen (`fopen`, `fgets`, `fputs`, etc.), Löschen mit `remove()`.   
  - Blockierverhalten: Öffnen/Lesen/Schreiben blockiert, wenn die Gegenseite nicht geöffnet oder die Pipe voll/leer ist.   

### **4. Unnamed Pipes**

- Kommunikation zwischen **verwandten Prozessen** (z. B. durch `fork()` vererbt, wie bei Shell-Befehlen `ps x | less`).   
- **Halbduplex-Verbindung** (unidirektional); für bidirektionale Kommunikation werden zwei Pipes benötigt.   
- Erstellung über ein Array aus zwei Dateideskriptoren: `fd[0]` (Lesen) und `fd[1]` (Schreiben). Unbenutzte Deskriptoren in den Eltern-/Kindprozessen müssen geschlossen werden.   
- **Redirection & `popen()`:**
  - Deskriptoren können mit `dup2()` auf Standardkanäle (`stdin`/`stdout`) umgeleitet werden.   
  - `popen()` vereint `pipe()`, `fork()` und `exec()` in einer einfachen Funktion zum Ausführen von Befehlen und Umleiten deren I/O-Streams.   