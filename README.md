# coman - Project Management Software

## Infos importantes pour le flogobert
- On utilise le framework Crow <sub><sup>(pour ne pas traiter la partie communication HTTP)</sup></sub>
- Full anglais stp <sub><sup>(si jamais je veux reprendre le projet plus tard)</sup></sub>
- Fais gaffe à pas commit des fichiers inutiles (compilations, etc...) et mets les dans le `.gitignore` si t'en as
- Essaye d'avoir des messages de commit normaux


Je pense qu'on va devoir utiliser des APIs REST pour faire les communications client/serveur...

...on aura un backend (en C++) et un frontend classique (html, css, ...) où on pourra utiliser un framework comme tailwind ou autre **si on a le temps**

## Dépendances <sub><sup>(avant d'implémenter le DockerFile)</sup></sub>
- CMake
- C++ 17 (uncomment le `CMakeLists.txt` si il y a beaucoup d'erreurs dans la première compilation)
- Asio development headers (1.10.9+) (peut-être devoir link son path dans le `CMakeLists.txt`)

## Docker
Format des versions : `MAJOR.MINOR.PATCH (ex, 1.18.2)` <br>
`MAJOR (1.0.0)`: Release majeures <br>
`MINOR (0.1.0)`: Nouvelle fonctionnalité <br>
`PATCH (0.0.1)`: Bug fix

<br>

Éventuellement faire `sudo` avant chaque commande `docker`

<sub> **Volume** pour la base de données (fichier persistant à travers les versions)</sub>
```bash
docker volume create coman-data
```

<sub> **Build & Run** </sub>
```bash
docker build -t triomph/coman:0.0.1 -t triomph/coman:latest .
docker run -p 18080:18080 -v coman-data:/app triomph/coman:latest
```



## Compilation
```bash
mkdir build
cd build
cmake ..
make
```

## Execution
Obligatoirement dans la racine du projet (pas de `../coman` ou `../../coman`)
```bash
./coman
```
