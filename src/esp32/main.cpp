/**
 * @file   main.cpp  (src/esp32/)
 * @brief  Passerelle WiFi → Arduino pour le robot Meccanoid.
 *
 * Rôle de l'ESP32 :
 *  1. Héberge l'interface web (index.html) depuis LittleFS.
 *  2. Expose la route GET /cmd?value=... qui reçoit les commandes du navigateur.
 *  3. Transmet ces commandes à l'Arduino Uno via Serial2 (UART 9600 baud).
 *  4. Expose GET /data qui renvoie la dernière ligne reçue depuis l'Arduino.
 *  5. Expose GET /test comme health-check pour l'interface web.
 *
 * Broches UART (Serial2) :
 *   GPIO 16 → RX2 (reçoit depuis Arduino TX)
 *   GPIO 17 → TX2 (émet vers Arduino RX)
 *
 * Format des commandes transmises à l'Arduino (voir SerialProtocol.cpp) :
 *   "head 0 120"   → membre "head", servo 0, angle 120°
 *   "left 2 90"    → bras gauche, servo 2, angle 90°
 *   "eyes red"     → LEDs yeux en rouge
 *
 * @note Modifier wifi.h pour renseigner le SSID et le mot de passe.
 *       Ne jamais committer wifi.h dans le dépôt (voir .gitignore).
 */

#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "config/wifi.h"

// ── Constantes ────────────────────────────────────────────────────

/** Délai de connexion WiFi avant abandon (×500 ms) */
static constexpr uint8_t  WIFI_TIMEOUT_STEPS = 20;

/** Baudrate de communication avec l'Arduino */
static constexpr uint32_t UART_BAUD = 9600;

/** Pins UART vers l'Arduino */
static constexpr uint8_t RXD2 = 16 ; // Reçoit depuis TX (Pin 3) de l'uno
static constexpr uint8_t TXD2 = 15; // Émet vers RX (pin 2) de l'Uno

/** Longueur maximale d'une commande acceptée */
static constexpr uint8_t CMD_MAX_LEN = 32;

// ── Globals ───────────────────────────────────────────────────────

WebServer server(80);

/**
 * Dernière ligne reçue depuis l'Arduino via Serial2.
 * Exposée via GET /data pour l'interface web.
 */
String arduinoLastLine = "";


// ═══════════════════════════════════════════════════════════════
// Helpers
// ═══════════════════════════════════════════════════════════════

/**
 * @brief Envoie les en-têtes CORS pour autoriser les appels fetch()
 *        depuis n'importe quelle origine (utile en développement local).
 */
static void sendCORSHeaders() {
    server.sendHeader("Access-Control-Allow-Origin",  "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

/**
 * @brief Vérifie qu'une commande est non vide et ne dépasse pas CMD_MAX_LEN.
 *
 * @param value Valeur brute reçue via le paramètre ?value=
 * @return true  si la commande est exploitable
 * @return false si elle est vide ou trop longue (potentiel buffer overflow)
 */
static bool isValidCommand(const String& value) {
    return value.length() > 0 && value.length() <= CMD_MAX_LEN;
}


// ═══════════════════════════════════════════════════════════════
// Handlers HTTP
// ═══════════════════════════════════════════════════════════════

/**
 * @brief GET /
 * Sert le fichier index.html depuis LittleFS.
 * Renvoie 404 si le fichier est absent, 500 en cas d'erreur d'ouverture.
 */
static void handleRoot() {
    if (!LittleFS.exists("/index.html")) {
        server.send(404, "text/plain", "index.html introuvable — vérifiez le upload LittleFS");
        Serial.println(F("[HTTP] 404 — index.html absent de LittleFS"));
        return;
    }

    File file = LittleFS.open("/index.html", "r");
    if (!file) {
        server.send(500, "text/plain", "Erreur lors de l'ouverture du fichier");
        Serial.println(F("[HTTP] 500 — impossible d'ouvrir index.html"));
        return;
    }

    server.streamFile(file, "text/html");
    file.close();
}

/**
 * @brief GET /cmd?value=<commande>
 *
 * Reçoit une commande texte depuis l'interface web et la transmet
 * à l'Arduino via Serial2.
 *
 * Validation :
 *  - Le paramètre `value` doit être présent.
 *  - La commande ne doit pas être vide ni dépasser CMD_MAX_LEN caractères.
 *
 * Réponses :
 *  - 200 OK          si la commande a été transmise.
 *  - 400 Bad Request si le paramètre est absent ou invalide.
 */
static void handleCmd() {
    sendCORSHeaders();

    if (!server.hasArg("value")) {
        server.send(400, "text/plain", "Paramètre 'value' manquant");
        Serial.println(F("[HTTP] 400 — paramètre value absent"));
        return;
    }

    String value = server.arg("value");
    value.trim();

    if (!isValidCommand(value)) {
        server.send(400, "text/plain", "Commande invalide (vide ou trop longue)");
        Serial.print(F("[HTTP] 400 — commande rejetée : '"));
        Serial.print(value);
        Serial.println(F("'"));
        return;
    }

    // Transmission à l'Arduino (println ajoute le '\n' attendu par SerialProtocol)
    Serial2.println(value);

    Serial.print(F("[CMD] → Arduino : '"));
    Serial.print(value);
    Serial.println(F("'"));

    server.send(200, "text/plain", "OK");
}

/**
 * @brief GET /data
 *
 * Renvoie la dernière ligne reçue depuis l'Arduino.
 * Utilisé par l'interface web pour afficher le retour de debug série.
 *
 * @note Renvoie une chaîne vide si aucune donnée n'a encore été reçue.
 */
static void handleData() {
    sendCORSHeaders();
    server.send(200, "text/plain", arduinoLastLine);
}

/**
 * @brief GET /test
 * Health-check simple pour que l'interface puisse vérifier la connexion.
 */
static void handleTest() {
    sendCORSHeaders();
    server.send(200, "text/plain", "OK");
}

/**
 * @brief Handler pour les requêtes OPTIONS (preflight CORS).
 * Nécessaire si l'interface est servie depuis un domaine différent.
 */
static void handleOptions() {
    sendCORSHeaders();
    server.send(204);
}


// ═══════════════════════════════════════════════════════════════
// Setup
// ═══════════════════════════════════════════════════════════════

void setup() {

    // --- Série debug (USB → PC) ---
    Serial.begin(115200);
    delay(1000);
    Serial.println(F("\n[ESP32] Démarrage..."));

    // --- Serial2 (UART → Arduino) ---
    Serial2.begin(UART_BAUD, SERIAL_8N1, RXD2, TXD2);
    Serial.println(F("[ESP32] Serial2 initialisé (UART Arduino) ✓"));

    // --- LittleFS ---
    if (!LittleFS.begin(true)) {
        // true = formater automatiquement si le FS est corrompu
        Serial.println(F("[ESP32] ERREUR : impossible de monter LittleFS"));
    } else {
        Serial.println(F("[ESP32] LittleFS monté ✓"));
    }

    // --- WiFi ---
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print(F("[WiFi] Connexion à '"));
    Serial.print(WIFI_SSID);
    Serial.print(F("'"));

    uint8_t step = 0;
    while (WiFi.status() != WL_CONNECTED && step < WIFI_TIMEOUT_STEPS) {
        delay(500);
        Serial.print('.');
        step++;
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println(F("\n[WiFi] ÉCHEC — vérifiez SSID/mot de passe dans config/wifi.h"));
        // L'ESP32 continue sans WiFi : les routes ne seront pas joignables,
        // mais la lecture Arduino reste active pour le debug.
    } else {
        Serial.print(F("\n[WiFi] Connecté — IP : "));
        Serial.println(WiFi.localIP());
    }

    // --- Routes HTTP ---
    server.on("/",       HTTP_GET,     handleRoot);
    server.on("/cmd",    HTTP_GET,     handleCmd);
    server.on("/data",   HTTP_GET,     handleData);
    server.on("/test",   HTTP_GET,     handleTest);
    server.on("/cmd",    HTTP_OPTIONS, handleOptions);
    server.onNotFound([]() {
        sendCORSHeaders();
        server.send(404, "text/plain", "Not found");
    });


    server.begin();
    Serial.println(F("[HTTP] Serveur web démarré sur le port 80 ✓"));
    Serial.println(F("[ESP32] Prêt.\n"));
}


// ═══════════════════════════════════════════════════════════════
// Loop
// ═══════════════════════════════════════════════════════════════

void loop() {
    // Traitement des requêtes HTTP entrantes
    server.handleClient();

    // --- Lecture des données renvoyées par l'Arduino ---
    // L'Arduino peut envoyer des lignes de debug via Serial.println().
    // On les stocke pour les exposer via GET /data.
    if (Serial2.available()) {
        String line = Serial2.readStringUntil('\n');
        line.trim();

        if (line.length() > 0) {
            arduinoLastLine = line;
            Serial.print(F("[Arduino] "));
            Serial.println(line);
        }
    }

    // Micro-délai pour éviter de saturer le watchdog
    delay(1);
}