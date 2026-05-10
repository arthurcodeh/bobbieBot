# src/uno

## Rôle

Code embarqué sur l'Arduino Uno. Il reçoit des commandes série depuis l'ESP32
et pilote les membres du robot (tête, bras gauche, bras droit).

## Structure

```
src/uno/
├── main.cpp                        # setup() et loop() principaux
├── config/
│   ├── Pins.h                      # Définition des pins matériels
│   ├── RobotConfig.h               # Constantes globales (nb servos, baudrate)
│   └── Limits.h                    # Limites globales min/max pour les servos
├── safety/
│   └── limits.h                    # Struct Limits avec méthode clamp()
├── members/
│   ├── ServoSpec.h                 # Struct décrivant un servo (min, max, position, destination)
│   ├── Member.h / Member.cpp       # Classe de base Membre
│   ├── head/
│   │   ├── Head.h / Head.cpp       # Classe Tete (1 servo)
│   └── arm/
│       ├── Arm.h / Arm.cpp         # Classe Bras (3 servos, symétrie gauche/droite)
└── communication/
    ├── meccanoid/
    │   ├── MeccanoidController.h   # Parseur de commandes série
    │   └── MeccanoidController.cpp
    └── serial/
        ├── SerialProtocol.h        # (réservé — à implémenter si besoin d'ACK)
        └── SerialProtocol.cpp
```

## Ajouter un nouveau membre

1. Créer une classe héritant de `Membre` dans `members/`.
2. Définir un tableau statique de `ServoSpec` avec les limites mécaniques.
3. L'instancier dans `main.cpp` et l'enregistrer via `controller.add_member(...)`.
4. Ajouter les boutons correspondants dans `site/index.html`.