ReadMe mit GPT-5.6 Luna generiert

# SwitchMacro

SwitchMacro ist ein kleines Firmware-Projekt für den Raspberry Pi Pico 2 W, das zeitgesteuerte Eingaben automatisiert. Die Idee dahinter: Statt Knöpfe immer wieder manuell zu drücken, kann der Pico vordefinierte Tastendrücke über GPIO auslösen und so ganze Abläufe automatisch wiederholen.

Entstanden ist das Projekt für das Shiny-Hunting und RNG-Manipulation in **Pokémon Feuerrot und Blattgrün (Gen 3)**. Zusammen mit einem zweiten Mikrocontroller, auf dem GP2040-CE läuft, lassen sich so bestimmte Eingabesequenzen möglichst präzise ausführen.

## Wie funktioniert's?

Der Pico empfängt Befehle über USB-Serial und ordnet Zeichen wie `w`, `a`, `s`, `d`, `f`, `g`, `h` und `j` den entsprechenden GPIO-Pins zu. Über diese Pins werden kurze oder längere Impulse erzeugt, die der zweite Mikrocontroller als Tastendrücke verarbeitet.

Mit dem Befehl `m` lässt sich eine festgelegte Makro-Sequenz starten. Die einzelnen Eingaben werden dabei anhand definierter Zeitpunkte ausgelöst. So können wiederkehrende Abläufe automatisch und mit möglichst gleichbleibendem Timing ausgeführt werden.

## Verwendete Tools und Technologien

- **Raspberry Pi Pico 2 W** – führt das Makro aus und steuert die GPIO-Pins.
- **C und Pico SDK** – für die Firmware und die zeitgesteuerten Eingaben.
- **GP2040-CE** – läuft auf dem zweiten Mikrocontroller und verarbeitet die simulierten Tastendrücke.
- **CMake und ARM-GCC-Toolchain** – zum Konfigurieren und Kompilieren des Projekts.
- **USB-Serial / stdio** – zur Kommunikation mit dem Pico und zum Starten der Makros.

Intern nutzt das Programm unter anderem die GPIO- und Timer-Funktionen des Pico SDK, um Impulse zu erzeugen und ihre Dauer möglichst genau zu steuern.

## Projektstruktur

- `SwitchMacro.c` – die eigentliche Firmware und Makro-Logik
- `CMakeLists.txt` – die Build-Konfiguration
- `pico_sdk_import.cmake` – bindet das Raspberry Pi Pico SDK ein

## GPIO-Belegung

Die folgenden Pins werden für die simulierten Eingaben verwendet:

| Taste | GPIO |
|---|---:|
| `f` | 7 |
| `g` | 6 |
| `h` | 11 |
| `w` | 2 |
| `s` | 3 |
| `a` | 5 |
| `d` | 4 |
| `j` | 10 |

Die genaue Zuordnung hängt davon ab, wie der Pico mit dem zweiten Mikrocontroller verbunden ist.

## Kompilieren

Das Projekt verwendet CMake und das Raspberry Pi Pico SDK. Die Firmware wird für das Board `pico2_w` gebaut und kann anschließend als UF2-Datei auf den Pico übertragen werden.

Die grundlegenden Schritte:

1. Raspberry Pi Pico SDK und die passende ARM-GCC-Toolchain installieren.
2. Das Projekt mit CMake konfigurieren.
3. Die Firmware kompilieren.
4. Die erzeugte UF2-Datei auf den Pico übertragen.

## Worum es letztlich geht

SwitchMacro soll wiederkehrende Eingaben einfacher und reproduzierbarer machen – besonders bei zeitkritischen Abläufen wie dem RNG-Hunting in Pokémon Feuerrot und Blattgrün.

Das Projekt ist ein kleines Experiment rund um Mikrocontroller, präzises Timing und automatisierte Eingaben. Es geht dabei nicht nur darum, Knöpfe automatisch zu drücken, sondern auch darum, herauszufinden, wie zuverlässig sich bestimmte Abläufe im Spiel reproduzieren lassen.

## Impressionen

<img width="600" height="800" alt="Image 2026-10-12 at 21 10 01" src="https://github.com/user-attachments/assets/36fe1fb0-88da-4873-9bdb-29cc405f0bd3" />
<img width="600" height="800" alt="Image 2026-10-12 at 21 11 01" src="https://github.com/user-attachments/assets/22b5167d-da0c-40bf-864f-8f36ebaf705d" />




