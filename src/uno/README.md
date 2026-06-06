# src/uno — Firmware Arduino Uno

Ce dossier contient l'intégralité du code embarqué sur l'Arduino Uno. Il reçoit les commandes série depuis l'ESP32 et pilote les membres physiques du robot (tête, yeux, bras).

---

## Structure

```
src/uno/
├── main.cpp                            # setup() et loop() — point d'entrée
├── config/
│   ├── Pins.h                          # Assignation des pins matériels
│   └── RobotConfig.h                   # Constantes globales (baudrate, vitesse servos, etc.)
├── members/
│   ├── ServoSpec.h                     # Struct décrivant un servo (min, max, position, destination)
│   ├── Member.h / Member.cpp           # Classe de base Membre (interpolation, LED feedback)
│   ├── head/
│   │   ├── Head.h / Head.cpp           # Tete — 2 servos (rotation + inclinaison)
│   │   └── eyes/
│   │       └── eyes.h                  # Yeux — 2 LEDs Meccanoid (override move())
│   └── arm/
│       ├── Arm.h / Arm.cpp             # Bras — 3 servos, symétrie gauche/droite
└── communication/
    ├── meccanoid/
    │   ├── MeccanoidController.h       # Dispatch des commandes vers les membres
    │   └── MeccanoidController.cpp
    └── serial/
        ├── SerialProtocol.h            # Parser de commandes série (SoftwareSerial)
        └── SerialProtocol.cpp
```

---

## Flux d'exécution

```
loop()
 ├── controller.update()
 │    ├── SerialProtocol::read()         → lit SoftwareSerial / Serial, parse en Command
 │    └── MeccanoidController::update()  → dispatch vers le membre par nom (strcmp)
 ├── tete->move()                        → interpolation servo pas-à-pas
 └── yeux->move()                        → chain.update() × 2 (maintien protocole LED)
```

---

## Hiérarchie des classes

```
Membre  (Member.h)
 ├── Tete   (Head.h)    — 2 servos, pin 6
 ├── Yeux   (eyes.h)    — 0 servo / 2 LEDs, pin 10  — override move()
 └── Bras   (Arm.h)     — 3 servos, pin 8 ou 9, symétrie G/D
```

`Membre` gère l'interpolation position → destination (par pas de `SERVO_DEFAULT_SPEED` degrés par tick), le mode limiteur des servos (`setLim`) et le feedback LED (rouge = en mouvement, vert = au repos).

`Yeux` ne gère pas de servos : son `move()` se contente d'appeler `chain.update()` deux fois pour maintenir la synchronisation du protocole Meccanoid.

---

## Protocole de commande (SerialProtocol)

Deux formats acceptés, terminés par `\n` :

```
head 0 90       → membre "head", servo index 0, angle 90°
eyes red        → membre "eyes", action "red"
```

Le parser détecte automatiquement le type selon le nombre de tokens. En cas de commande mal formée, `cmd.valid = false` et la commande est ignorée.

---

## Pins matériels (Pins.h)

| Composant     | Pin | Constante       |
|---------------|-----|-----------------|
| Tête (servos) | 6   | `PIN_HEAD`      |
| Yeux (LEDs)   | 10  | `HEAD_LED_PIN`  |
| Bras gauche   | 9   | `PIN_ARM_LEFT`  |
| Bras droit    | 8   | `PIN_ARM_RIGHT` |
| RX ESP32      | 2   | `RX_ESP32`      |
| TX ESP32      | 3   | `TX_ESP32`      |

---

## Communication série

| Port        | Usage                                 | Baudrate |
|-------------|---------------------------------------|----------|
| `Serial`    | Moniteur série USB (debug PC)         | 9600     |
| `espSerial` | SoftwareSerial → ESP32 (RX:2 / TX:3) | 9600     |

> Sur Arduino Mega (`env:unoHome`), `Serial1` hardware remplace `SoftwareSerial`.

---

## Ajouter un nouveau membre

1. Créer une classe héritant de `Membre` dans `members/`.
2. Déclarer un tableau **statique** de `ServoSpec` avec les limites mécaniques.
3. L'instancier dans `main.cpp` et l'enregistrer via `controller.add_member(...)`.
4. Appeler `son_membre->move()` dans `loop()`.
5. Ajouter les contrôles correspondants dans `data/index.html`.

---

## Points importants

- `chain.update()` doit être appelé en continu dans `loop()` pour maintenir la synchronisation du protocole Meccanoid. Ne l'appeler qu'une seule fois au démarrage provoque une désynchronisation des LEDs.
- `Serial.begin()` doit précéder tout appel à `Serial.print()`. Ne pas logger dans les constructeurs.
- Les specs de servos sont des tableaux **statiques** par classe (`static ServoSpec headSpecs[]`) — une seule copie en mémoire, partagée entre toutes les instances.
- Le cast `static_cast<Yeux*>` dans `MeccanoidController` est sécurisé grâce au `MemberType::EYES` retourné par `getType()` (RTTI non disponible sur Uno).