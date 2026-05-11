#include <iostream>
#include <sstream>
#include <vector>

// Ce fichier est écrit "style Arduino" (setup/loop),
// mais il utilise aussi des éléments C++ standard (std::string, std::vector, std::istringstream).
// Objectif global :
// - Maintenir une liste de "Membre" (tête, bras gauche, bras droit)
// - Recevoir des commandes sur le port série
// - Parser ces commandes et appeler setDestination(servoIndex, angle) sur le bon membre

// Liste globale des membres du robot (pointeurs car on fait new Tete / new Bras).
// Attention : en embarqué, l'allocation dynamique peut fragmenter la mémoire si on en abuse.
// Ici, c'est fait une fois dans setup(), donc c'est généralement acceptable.
std::vector<Membre*> membres;

void setup() {
    // Initialisation série (USB/UART) pour diagnostic et réception de commandes.
    Serial.begin(9600);

    // Messages de debug côté console série.
    Serial.println("==== MECCANOID - MODE DIAGNOSTIC ====");
    Serial.println("Connexion en cours...\n");

    // Timeout pour les lectures de type readString() (en millisecondes).
    // Si aucun caractère n'arrive, readString() attendra au max ce délai.
    Serial.setTimeout(100);

    // Création des objets représentant les membres.
    // Chaque membre est piloté par une "Chain" sur un pin donné.
    //
    // NOTE IMPORTANTE :
    // - La signature montrée ici (new Tete(6, "head")) ne correspond pas à la classe Tete
    //   que tu as montrée plus haut (Tete(int pin) seulement).
    // - Si tu compiles et que ça casse, c’est probablement dû à cette différence de signature.
    membres.push_back(new Tete(6, "head"));
    membres.push_back(new Bras(8, "right"));
    membres.push_back(new Bras(9, "left"));
}

void loop() {
    // Buffer local qui contiendra le texte reçu sur le port série.
    // On utilise std::string, mais sur Arduino on voit souvent String (Arduino).
    std::string texte;

    // On ne tente de lire que si quelque chose est disponible.
    if (Serial.available()) {
        // readString() renvoie un "String" Arduino, pas un std::string.
        // Selon ton environnement/cores, une conversion implicite peut ne pas exister.
        // Ici le code suppose que "texte = Serial.readString();" est valide.
        texte = Serial.readString();

        // On utilise '&' comme séparateur de commandes dans le flux.
        // Exemple attendu (idée) : "head 0 120&left 2 60&"
        size_t pos = texte.find("&");

        // Debug: affichage du texte brut reçu.
        // ATTENTION : std::cout sur Arduino n’est pas toujours disponible.
        // En Arduino pur, on utilise plutôt Serial.print/println.
        std::cout << "texte recu: " << texte << std::endl;

        // Tant qu'on trouve un '&', on traite une "commande" puis on continue sur le reste.
        while (pos != std::string::npos) {
            // cmd = nom du membre ("head"/"left"/"right" selon ton protocole)
            // a   = index du servo dans ce membre (0,1,2...)
            // b   = destination (angle)
            std::string cmd;
            int a, b;

            // istringstream permet de parser "cmd a b" depuis une chaîne.
            // Exemple: "head 1 90" => cmd="head", a=1, b=90
            std::istringstream iss(texte);
            iss >> cmd >> a >> b;

            // On cherche dans la liste le membre dont le nom correspond à cmd.
            // NOTE : membres contient des pointeurs, donc en C++ on appelle les méthodes avec "->".
            // Ici le code utilise "m.getName()" et "m.setDestination()", ce qui ne compile pas :
            // il faudrait "m->getName()" et "m->setDestination(...)"
            for (Membre* m : membres) {
                if (m.getName() == cmd) {
                    m.setDestination(a, b);
                }
            }

            // Debug: affiche la commande parsée
            std::cout << cmd << " " << a << " " << b << std::endl;

            // On "consomme" la commande courante en supprimant tout jusqu'au '&' inclus.
            // Puis on cherche le prochain '&'.
            texte = texte.substr(pos + 1);
            pos = texte.find("&");
        }

        // Ici, on semble vouloir accumuler du texte restant (commande incomplète) avec un buffer précédent.
        // MAIS : "texte2" n'est pas défini dans ce fichier -> compilation impossible en l'état.
        // Idée probable :
        // - texte2 stocke le morceau incomplet reçu la dernière fois
        // - on le concatène pour reconstruire une commande complète
        texte += texte2;
    }
}

// -----------------------------------------------------------------------------
// Le bloc ci-dessous est du code plus ancien / expérimental, laissé en commentaire.
// Il montre une approche "directe" de test des servos : récupérer un servo,
// lui donner une position, puis appeler chain.update() + delay.
// -----------------------------------------------------------------------------

// #include <Meccanoid.h>
//
// typedef MeccanoServo Servo;
// typedef MeccanoLed Led;
//
// int pin_head   = 7;
// int pin_led    = 7;
// int pin_right  = 8;
// int pin_left   = 9;
//
// Chain chainHead(pin_head);
// Chain chainLed(pin_led);
// Chain chainRight(pin_right);
// Chain chainLeft(pin_left);
//
// int step = 0;
//
// void setup() {
//   Serial.begin(9600);
//   Serial.println("==== MECCANOID - MODE DIAGNOSTIC ====");
//   Serial.println("Connexion en cours...\n");
//
//   randomSeed(analogRead(0));
// }
// void printConnect(String message) {
//   Serial.print("[CONNECT] ");
//   Serial.println(message);
// }
//
// void moveServo(Servo servo, String name) {
//   if (step % 20 == 0) {
//     int position = random(0, 180);
//     servo.setPosition(position);
//     Serial.print(name + " -> Position: ");
//     Serial.println(position);
//   }
// }
// void moveServo(Servo servo, String name, int position) {
//   if (step % 20 == 0) {
//     servo.setPosition(position);
//     Serial.print(name + " -> Position: ");
//     Serial.println(position);
//   }
// }
//
//
//
// void loop() {
//
//   Serial.println("in Loop");
//   Servo s1 = chainHead.getServo(0);
// //  Servo s2 = chainHead.getServo(1);
// //  Servo s3 = chainHead.getServo(2);
//   if (s1.justConnected()) printConnect("Tete Servo 1");
//   Serial.print(s1.isConnected());
//   if (s1.isConnected()){
//     moveServo(s1, "left Servo 1", 90);
//     for (int i = 0; i <= 6; i++) {
//       moveServo(s1, "left Servo 1", 90 + i * 5);
//       Serial.println("move");
//
//       chainLeft.update();
//       delay(200);
//     }
//   }else{
//     Serial.println("not connected");
//     delay(1000);
//   }
// //  HeadYes();
// //  LeftArmBase();
//   //RightArmBase();
// //  testHead();
// //  testLed();
// //  testRightArm();
//
// }
