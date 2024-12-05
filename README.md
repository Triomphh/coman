# coman - Project Management Software

 [coman.triomph.dev](https://coman.triomph.dev)

## Infos importantes pour le flogobert
- On utilise le framework Crow <sub><sup>(pour ne pas traiter la partie communication HTTP)</sup></sub>
- Full anglais stp <sub><sup>(si jamais je veux reprendre le projet plus tard)</sup></sub>
- Fais gaffe à pas commit des fichiers inutiles (compilations, etc...) et mets les dans le `.gitignore` si t'en as
- Essaye d'avoir des messages de commit normaux


## Architecture
Le projet suit une architecture client-serveur :
- **Backend** : C++ avec APIs REST
- **Frontend** : HTML/CSS avec Tailwind CSS

## Dépendances <sub><sup>(en local / sans Docker)</sup></sub>
- CMake
- C++ 17 
- Asio development headers (1.10.9+) (peut-être devoir link son path dans le `CMakeLists.txt`)

> Note : uncomment le `CMakeLists.txt` si il y a beaucoup d'erreurs dans la première compilation


## EN REMOTE
Compilation et déploiment automatique via Jenkins + WebHook

## EN LOCAL
### Docker
Format des versions : `MAJOR.MINOR.PATCH (ex, 1.18.2)` :
- `MAJOR (1.0.0)`: Release majeures <br>
- `MINOR (0.1.0)`: Nouvelle fonctionnalité <br>
- `PATCH (0.0.1)`: Bug fix

#### Volume de Base de Données
```bash
# Création du volume persistant pour la base de données
docker volume create coman-data
```

#### Build & Run
```bash
# Build
docker build -t triomph/coman:latest .

# Run
docker run -p 18080:18080 -v coman-data:/app/data triomph/coman:latest
```

> Note : Éventuellement faire `sudo` avant chaque commande


### Compilation
```bash
mkdir build
cd build
cmake ..
make
```

## Execution
```bash
./coman
```

> Note : Obligatoirement dans la racine du projet (pas de `../coman` ou `../../coman`)
