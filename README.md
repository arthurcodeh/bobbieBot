# BOBBIE BOT - ISA
Reconstruction et reprogrammation d’un robot Meccanoid avec une architecture logicielle moderne, modulaire et maintenable.

---

## Vue d'ensemble

```
Navigateur  ──HTTP──►  ESP32-S3       ──UART──►  Arduino Uno
index.html             Passerelle WiFi            Firmware C++
                       LittleFS                   Servos + LEDs
```

Chaque couche a une responsabilité unique et ne connaît que son voisin immédiat.

---

## Structure du dépôt

```
bobbieBot/
├── src/
│   ├── esp32/                  # Firmware ESP32-S3 (passerelle WiFi)
│   │   ├── main.cpp
│   │   └── config/
│   │       ├── wifi.h.example  # Modèle de configuration WiFi (ne pas committer wifi.h)
│   │       └── wifi.h          # Ignoré par .gitignore — à créer localement
│   └── uno/                    # Firmware Arduino Uno (contrôle du robot)
│       ├── main.cpp
│       ├── config/             # Pins.h, RobotConfig.h
│       ├── members/            # Membre, Tete, Yeux, Bras + ServoSpec
│       │   ├── arm/
│       │   └── head/
│       │       └── eyes/
│       └── communication/      # SerialProtocol, MeccanoidController
│           ├── meccanoid/
│           └── serial/
├── data/                       # Interface web (uploadée sur LittleFS)
│   ├── index.html
│   ├── style.css
│   ├── commandBuilder.js
│   └── statusPoller.js
├── lib/
│   └── Meccanoid/src/          # Bibliothèque protocole PWM Meccano
└── platformio.ini              # Environnements : uno / unoHome / esp32
```

---

## Prérequis

- [PlatformIO](https://platformio.org/) (extension VS Code recommandée)
- Arduino Uno R3 + ESP32-S3 DevKitC-1
- Servos et LEDs Meccanoid (chaîne PWM)
- Réseau WiFi 2,4 GHz

---

## Mise en route

### 1. Configurer le WiFi

```bash
cp src/esp32/config/wifi.h.example src/esp32/config/wifi.h
```

Renseigner les valeurs dans `wifi.h` :

```cpp
#define WIFI_SSID     "VOTRE_SSID"
#define WIFI_PASSWORD "VOTRE_MOT_DE_PASSE"
```

> `wifi.h` est listé dans `.gitignore` — ne jamais le committer.

### 2. Flasher l'Arduino Uno

```bash
pio run --target upload -e uno
```

Utiliser `unoHome` si la carte connectée est un Mega (Serial1 matériel disponible).

### 3. Uploader l'interface web sur l'ESP32

```bash
# Upload du système de fichiers (LittleFS) — à refaire après chaque modif de data/
pio run --target uploadfs -e esp32

# Flash du firmware
pio run --target upload -e esp32
```

### 4. Ouvrir l'interface

L'IP de l'ESP32 s'affiche dans le moniteur série après connexion WiFi. Ouvrir cette adresse dans un navigateur.

---

## Protocole de commande

Les commandes sont des chaînes texte transmises via `GET /cmd?value=<commande>`, encodées en URL.

| Type   | Format                      | Exemple     | Effet                    |
|--------|-----------------------------|-------------|--------------------------|
| Servo  | `<membre> <index> <angle>`  | `head 0 90` | Tête, servo 0, angle 90° |
| Action | `<membre> <action>`         | `eyes red`  | LEDs yeux en rouge       |

Membres disponibles : `head`, `eyes`, `left`, `right`.

---

## Environnements PlatformIO

| Environnement | Cible              | Usage                          |
|---------------|--------------------|--------------------------------|
| `uno`         | Arduino Uno R3     | Port COM configuré (`COM10`)   |
| `unoHome`     | Arduino Mega 2560  | Mega avec Serial1 matériel     |
| `esp32`       | ESP32-S3 DevKitC-1 | Passerelle WiFi + LittleFS     |

---

## Documentation

Chaque sous-dossier contient un `README.md` décrivant sa structure interne.
Le code source est documenté au format Doxygen (`/** @brief ... */`).
