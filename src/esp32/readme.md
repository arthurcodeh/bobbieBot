# src/esp32

## Rôle

L'ESP32 joue le rôle de **passerelle** entre le réseau WiFi et l'Arduino Uno :

1. Il héberge l'interface web (`site/index.html`) depuis le LittleFS.
2. Il expose une route HTTP `GET /cmd?value=...` qui reçoit les commandes du navigateur.
3. Il transmet ces commandes à l'Arduino via `Serial2` (UART, 9600 baud).

## Configuration requise

Avant de flasher, modifier dans `main.cpp` :

```cpp
const char* ssid     = "TON_WIFI";
const char* password = "TON_MDP";
```

## Pins UART

| Pin ESP32 | Fonction |
|-----------|----------|
| GPIO 16 (RX2) | Réception depuis Arduino TX |
| GPIO 17 (TX2) | Émission vers Arduino RX |

## Routes HTTP

| Route | Description |
|-------|-------------|
| `GET /` | Sert `index.html` depuis LittleFS |
| `GET /cmd?value=...` | Transmet la valeur à l'Arduino via Serial2 |
| `GET /data` | Renvoie la dernière ligne reçue depuis l'Arduino |
| `GET /test` | Health check |