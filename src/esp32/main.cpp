#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

const char *ssid = "TON_WIFI";
const char *password = "TON_MDP";

WebServer server(80);

// UART Arduino
#define RXD2 16
#define TXD2 17

String arduinoData = "";

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

    // Attendre que le serial soit prêt
    delay(1000);

    // Montage LittleFS avec vraie gestion d'erreur
    if (!LittleFS.begin(true)) {
        // true = format si échec
        Serial.println("Erreur LittleFS");
        // Ne pas retourner - continuer sans filesystem
    }

    // Configuration WiFi avec timeout
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("Connexion WiFi");
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
        delay(500);
        Serial.print(".");
        timeout++;
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nÉchec connexion WiFi!");
        return;
    }

    Serial.println("\nConnecté !");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    // Page HTML avec vérification
    server.on("/", HTTP_GET, []() {
        if (!LittleFS.exists("/index.html")) {
            server.send(404, "text/plain", "index.html non trouvé");
            return;
        }
        File file = LittleFS.open("/index.html", "r");
        if (!file) {
            server.send(500, "text/plain", "Erreur ouverture fichier");
            return;
        }
        server.streamFile(file, "text/html");
        file.close();
    });

    // Commande vers Arduino
    server.on("/cmd", HTTP_GET, []() {
        if (server.hasArg("value")) {
            String cmd = server.arg("value");
            Serial2.println(cmd);
            Serial.print("Envoyé à Arduino: ");
            Serial.println(cmd);
            server.send(200, "text/plain", "OK");
        } else {
            server.send(400, "text/plain", "Missing value");
        }
    });

    // Données Arduino
    server.on("/data", HTTP_GET, []() {
        server.send(200, "text/plain", arduinoData);
    });

    // Route de test
    server.on("/test", HTTP_GET, []() {
        server.send(200, "text/plain", "ESP32 OK");
    });

    server.begin();
    Serial.println("Serveur web démarré");
}

void loop() {
    server.handleClient();

    // Lecture données Arduino avec trim des espaces
    if (Serial2.available()) {
        arduinoData = Serial2.readStringUntil('\n');
        arduinoData.trim(); // Enlever \r\n
        if (arduinoData.length() > 0) {
            Serial.print("Reçu Arduino: ");
            Serial.println(arduinoData);
        }
    }

    // Petit délai pour éviter le watchdog
    delay(1);
}
