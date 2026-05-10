#include <vector>
#include <MecanoidForArduino.Meccanoid.h>
#include <string>

/*
  ServoSpec = "fiche" d’un servo individuel.
  On sépare ici :
  - ce que le servo *est* (pointeur vers l'objet Servo de la lib)
  - ses contraintes mécaniques (min/max)
  - son état logique côté logiciel (position actuelle / destination)
*/
struct ServoSpec {
    // Pointeur vers le servo dans la chaîne (fourni par Chain::getServo(index)).
    // On ne crée pas le servo nous-mêmes : la "Chain" les gère.
    Servo* servo;

    // Limites autorisées (sécurité mécanique).
    // Exemple : éviter de forcer un bras à aller au-delà de ce qu’il peut physiquement.
    int min;
    int max;

    // Position "connue" par le programme (état interne).
    // Note : ce n’est pas forcément une lecture du servo, c’est ce que *nous* pensons avoir envoyé.
    int position = 90;

    // Destination = angle cible demandé (consigne).
    int destination = 90;
};

/*
  Membre = base commune pour une partie du robot (tête, bras, etc.)
  Un "membre" contient :
  - un nom (pour l’identifier)
  - une Chain (bus/chaîne Meccanoid sur un pin)
  - plusieurs ServoSpec (un par servo du membre)
  - une vitesse (pas de mouvement par appel à move())
*/
class Membre {
protected:
    int pin;                 // pin utilisé pour piloter la chaîne de servos
    std::string name;        // nom logique ("head", "left", "right", etc.)

    // Liste des servos de ce membre, chacun avec ses limites + état.
    std::vector<ServoSpec> servos;

    // Objet de la lib qui représente la chaîne de servos reliés à "pin".
    Chain chain;

    // Ces champs ne sont pas utilisés dans move() ici (redondants avec ServoSpec).
    // Ils pourraient servir si on voulait "un seul angle" pour tout le membre.
    int position = 90;
    int destination = 90;

    // vitesse = incrément/decrément max par tick (par appel à move()).
    // Ex: vitesse=5 => bouge par pas de 5 degrés vers la cible.
    int vitesse = 5;

public:
    /*
      specs = liste des (min,max) pour chaque servo du membre.
      Exemple pour une tête à 2 servos : { {10,170}, {45,135} }
    */
    Membre(int pin, std::string name, const std::vector<std::pair<int,int>>& specs)
        : pin(pin), name(name), chain(pin) // IMPORTANT: chain construit avec le pin
    {
        // Optimisation: on réserve la place, évite des reallocations pendant push_back.
        servos.reserve(specs.size());

        // On associe chaque paire (min,max) à un servo récupéré dans la chaîne.
        int index = 0;
        for (const auto& s : specs) {
            // s.first = min, s.second = max
            // chain.getServo(index) récupère "le servo numéro index" de cette chaîne.
            servos.push_back(ServoSpec{ chain.getServo(index), s.first, s.second });
            index++;
        }
    }

    int getPin() const { return pin; }

    // Ici on renvoie par valeur (copie). Pour éviter la copie, on pourrait renvoyer const std::string&.
    std::string getName() const { return name; }

    /*
      Fixe la destination (angle cible) d’un servo donné.
      - index = quel servo dans ce membre (0, 1, 2, ...)
      - dest  = angle demandé (sera "clampé" dans [min,max])
    */
    void setDestination(size_t index, int dest) {
        // Sécurité: si index invalide, on ignore la commande.
        if (index >= servos.size()) return;

        // Clamp: on force la destination à rester dans les limites.
        if (dest < servos[index].min) dest = servos[index].min;
        if (dest > servos[index].max) dest = servos[index].max;

        // On ne bouge pas tout de suite : on met juste à jour la consigne.
        servos[index].destination = dest;
    }

    /*
      move() = à appeler régulièrement (ex: dans loop()).
      Il fait un "petit pas" vers la destination pour chaque servo.

      Intérêt :
      - mouvements plus fluides
      - évite de sauter brutalement à l’angle cible
      - permet à plusieurs servos de progresser en parallèle
    */
    void move() {
        bool anyMove = false; // sert à éviter chain.update() si rien n’a changé

        // On parcourt tous les servos du membre
        for (auto& s : servos) {
            int current = s.position;      // position logicielle actuelle
            int target  = s.destination;   // consigne

            // Si déjà à la cible : rien à faire
            if (current == target) continue;

            int next = current;

            // Approche de la cible par pas de "vitesse"
            if (target > current) {
                next = current + vitesse;

                // Anti-dépassement : si on a dépassé, on se cale sur target
                if (next > target) next = target;
            } else {
                next = current - vitesse;

                // Anti-dépassement dans l'autre sens
                if (next < target) next = target;
            }

            // Applique le nouveau pas :
            // 1) mise à jour de notre état interne
            s.position = next;

            // 2) envoi de l'angle au servo via la lib
            s.servo->setPosition(next);

            anyMove = true;
        }

        // On n’actualise la chaîne qu’une seule fois pour tous les servos
        // (souvent plus efficace que de le faire après chaque servo).
        if (anyMove) {
            chain.update();
        }
    }
};

/*
  Tete = Membre spécialisé (héritage).
  On fixe :
  - name = "head"
  - 2 servos, avec leurs limites.
*/
class Tete : public Membre {
public:
    explicit Tete(int pin)
        : Membre(pin, "head", { {10,170}, {45,135} }) {
        serialPrint("Tete initialized on pin " + std::to_string(pin));
    }
};

/*
  Bras = Membre spécialisé pour un bras.
  Il a 3 servos.
  Le 3e servo n’a pas les mêmes limites selon que c’est le bras gauche ou droit
  (typiquement à cause de la symétrie mécanique).
*/
class Bras : public Membre {
public:
    Bras(int pin, const std::string &sens)
        : Membre(
            pin,
            sens, // le nom du membre devient "left" ou "right"
            (sens == "left")
                ? std::vector<std::pair<int,int>>{ {10,170}, {10,180}, {0,90} }
                : std::vector<std::pair<int,int>>{ {10,170}, {10,180}, {90,180} }
        )
    {}
};
