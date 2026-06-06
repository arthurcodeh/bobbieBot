# src/esp32 — Firmware ESP32-S3 (passerelle WiFi)

Ce dossier contient le firmware de l'ESP32-S3. Son rôle est d'être le pont entre le réseau WiFi et l'Arduino Uno : il héberge l'interface web, reçoit les commandes HTTP du navigateur et les transmet à l'Arduino via UART.

---

## Structure

```
src/esp32/
├── main.cpp                # Firmware complet : WiFi, WebServer, LittleFS, Serial2
└── config/
    ├── wifi.h.example      # Modèle à copier (ne jamais committer wifi.h)
    └── wifi.h              # Fichier local ignoré par .gitignore
```

---

## Rôle et flux

```
Navigateur
    │  GET /cmd?value=head%200%2090
    ▼
WebServer (port 80)
    │  handleCmd() — validation + trim
    ▼
Serial2.println("head 0 90")     ← UART 9600 baud
    │
    ▼
Arduino Uno (RX pin 2)
```

L'ESP32 ne connaît pas la sémantique des commandes : il valide leur format (non vide, ≤ 32 caractères) et les transmet telles quelles. L'interprétation est entièrement du côté Arduino.

---

## Routes HTTP exposées

| Route                   | Méthode | Description                                              |
|-------------------------|---------|----------------------------------------------------------|
| `/`                     | GET     | Sert `index.html` depuis LittleFS                        |
| `/cmd?value=<commande>` | GET     | Valide et transmet la commande à l'Arduino via Serial2   |
| `/data`                 | GET     | Retourne la dernière ligne reçue depuis l'Arduino        |
| `/test`                 | GET     | Health-check (utilisé par `statusPoller.js`)             |
| `/cmd`                  | OPTIONS | Preflight CORS (pour les appels `fetch()` cross-origin)  |

Toutes les routes ajoutent les en-têtes CORS (`Access-Control-Allow-Origin: *`) pour permettre les appels depuis l'interface web.

---

## Pins UART (Serial2)

| Pin ESP32 | Direction | Connecté à         | Constante |
|-----------|-----------|--------------------|-----------|
| GPIO 16   | RX2       | TX Arduino (pin 3) | `RXD2`    |
| GPIO 15   | TX2       | RX Arduino (pin 2) | `TXD2`    |

> Baudrate : 9600 baud (`UART_BAUD`), format 8N1.

---

## Configuration initiale

### 1. Créer le fichier WiFi

```bash
cp src/esp32/config/wifi.h.example src/esp32/config/wifi.h
```

Renseigner les valeurs :

```cpp
#define WIFI_SSID     "VOTRE_SSID"
#define WIFI_PASSWORD "VOTRE_MOT_DE_PASSE"
```

> `wifi.h` est dans `.gitignore`. Ne jamais le committer : il contient des identifiants sensibles.

### 2. Uploader l'interface web (LittleFS)

Le dossier `data/` doit être uploadé séparément du firmware :

```bash
pio run --target uploadfs -e esp32
```

> Cette étape est indépendante du flash firmware. Elle doit être refaite à chaque modification des fichiers dans `data/`.

### 3. Flasher le firmware

```bash
pio run --target upload -e esp32
```

L'IP assignée par le routeur s'affiche dans le moniteur série après connexion WiFi :

```
[WiFi] Connecté — IP : 192.168.x.x
```

---

## Comportement en cas d'erreur WiFi

Si la connexion WiFi échoue après `WIFI_TIMEOUT_STEPS` tentatives (20 × 500 ms = 10 s), l'ESP32 continue de fonctionner sans réseau. Le WebServer n'est pas joignable, mais la lecture de Serial2 reste active — utile pour déboguer l'Arduino sans connexion réseau.

---

## Points importants

- LittleFS et le firmware sont deux zones mémoire distinctes. Un flash firmware ne supprime pas les fichiers LittleFS, mais un `uploadfs` formate le FS avant d'uploader.
- `Serial2.println()` ajoute automatiquement le `\n` attendu par `SerialProtocol::read()` côté Arduino.
- La route `/data` expose la dernière ligne reçue de l'Arduino (`arduinoLastLine`). Elle est utile pour déboguer sans accès au moniteur série, mais n'est pas temps-réel (la variable est mise à jour à chaque `loop()`).