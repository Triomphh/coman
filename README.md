# coman - Project Management Software

## Infos importantes pour le flogobert
- On utilise le framework Crow <sub>(pour ne pas traiter la partie communication HTTP)</sub>
- Full anglais stp <sub>(si jamais je veux reprendre le projet plus tard)</sub>
- Fais gaffe à pas commit des fichiers inutiles (compilations, etc...) et mets les dans le `.gitignore` si t'en as
- Essaye d'avoir des messages de commit normaux


Je pense qu'on va devoir utiliser des APIs REST pour faire les communications client/serveur...

...on aura un backend (en C++) et un frontend classique (html, css, ...) où on pourra utiliser un framework comme tailwind ou autre **si on a le temps**

## Dépendances <sub>(avant d'implémenter le DockerFile)</sub>
- CMake
- C++ 17 (uncomment le `CMakeLists.txt` si il y a beaucoup d'erreurs dans la première compilation)
- Asio development headers (1.10.9+) (peut-être devoir link son path dans le `CMakeLists.txt`)


## Compilation
```
mkdir build
cd build
cmake ..
make
```

## Execution
Obligatoirement dans la racine du projet (pas de `../coman` ou `../../coman`)
```
./coman
```
