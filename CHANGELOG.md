# Changelog

Alle relevanten Änderungen an diesem Projekt werden in dieser Datei festgehalten.

Das Format orientiert sich an [Keep a Changelog](https://keepachangelog.com/de/1.1.0/),
und dieses Projekt folgt [Semantic Versioning](https://semver.org/lang/de/).

---

## [1.1.0] – 2026-10-18

### Changed
- Harmonisiertes Versionsschema: Controller und Remote werden auf eine gemeinsame Version `1.1.0` gesetzt.
- „-beta"-Suffix der Remote-Version entfernt.
- Versionsnummern werden in beiden Projekten als `SW_VERSION_MAJOR` / `SW_VERSION_MINOR` / `SW_VERSION_PATCH` plus abgeleitete `SW_VERSION` definiert.

### Fixed
- **EEPROM / Modell-Daten**: Modell wurde nur beim allerersten Start nach dem Flashen gefunden, beim nächsten Neustart nicht mehr (Daten schienen verloren).
  - **CRC-Berechnung** (`helper.cpp`/`helper.h`): `calculateCRC` brach bei `\0`-Bytes im Modellnamen ab, da intern `strlen` verwendet wurde. Eingeführt wurde eine überladene Variante `calculateCRC(const char*, size_t)`, die über die volle Pufferlänge iteriert. Die alte Signatur bleibt als Wrapper für die UART-Kommunikation erhalten.
  - **Datenlänge** (`ModelParameters.h`): `MODEL_DATA_SIZE` von `55` (15 + 20×2) auf `49` (15 + 17×2) korrigiert.
  - **Aufruf** (`ModelParameters.cpp::calcCkecksum`): nutzt jetzt die `length`-Variante der CRC-Funktion mit `MODEL_DATA_SIZE`.
  - **Adress-Überlappung** (`variables.h`): `EEPROM_ADR_MODEL` von `13` auf `100` verschoben, weil `SystemConfig` (durch Padding) tatsächlich deutlich mehr als 13 Bytes belegt und dadurch beim ersten `resetToDefaults()` den Model-Bereich überschrieb.

### Notes
- Nach dem Update **EEPROM einmalig komplett löschen** und Modell neu anlegen – alte EEPROM-Inhalte sind mit dem neuen Layout nicht mehr lesbar.

---

## [1.0.x] – vor 1.1.0

Initiale veröffentlichte Versionen:
- Controller-Firmware `1.0.1` (sTankTeil_Controller).
- Remote-Firmware `1.1.0-beta` (sTankTeil_Remote).

(Vor diesem Eintrag wurde kein gemeinsames Changelog geführt.)
