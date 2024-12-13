# COMAN - Documentation

La documentation moderne et avancée est accessible via [coman.triomph.dev/doc](https://coman.triomph.dev/doc)

## À propos
COMAN est un logiciel de gestion de projet moderne et efficace, accessible via [coman.triomph.dev](https://coman.triomph.dev). Cette application suit une architecture client-serveur robuste, conçue pour offrir une expérience utilisateur fluide et performante.

## Architecture Technique

### Backend
- Développé en C++ (version 17)
- API REST utilisant le framework Crow
- Base de données persistante
- Architecture en couches (MVC)
  - Models : Représentation des données
  - Controllers : Gestion des requêtes HTTP
  - Services : Logique métier
  - Repositories : Accès aux données

### Frontend
- Interface utilisateur en HTML/CSS
- Framework CSS : Tailwind CSS
- Design responsive et moderne

## Déploiement en Production

Le projet est automatiquement déployé sur un VPS (Virtual Private Server) grâce à :
- **Jenkins** pour l'intégration continue
- **Webhooks GitHub** qui déclenchent automatiquement le déploiement à chaque push sur la branche principale
- Pipeline de déploiement automatisé incluant :
  - Compilation du code
  - Exécution des tests
  - Déploiement sur le serveur de production

Cette configuration permet un déploiement continu et sécurisé sans intervention manuelle.

## Prérequis d'Installation

### Installation Locale
#### Dépendances Requises
- CMake
- C++ 17 ou supérieur
- Headers de développement Asio (version 1.10.9+)

### Installation via Docker
Docker simplifie considérablement le déploiement de l'application.

#### Prérequis Docker
- Docker installé sur votre système
- Droits administrateur pour certaines commandes

## Guide d'Installation

### Méthode 1 : Installation avec Docker (Recommandée)

1. **Création du volume pour la base de données**
   ```bash
   docker volume create coman-data
   ```

2. **Construction de l'image**
   ```bash
   docker build -t triomph/coman:latest .
   ```

3. **Lancement de l'application**
   ```bash
   docker run -p 18080:18080 -v coman-data:/app/data triomph/coman:latest
   ```

Note : Selon votre configuration, vous pourriez avoir besoin d'utiliser `sudo` pour les commandes Docker.

#### Tests
Pour exécuter les tests avec Docker :
```bash
docker run --rm triomph/coman:latest /bin/bash -c './coman_test'
```

### Méthode 2 : Installation Locale

1. **Compilation**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

2. **Exécution**
   ```bash
   ./coman
   ```

Important : L'exécution doit se faire depuis la racine du projet.

## Gestion des Versions

Le projet suit la convention de versionnement sémantique (SemVer) :
- **MAJOR.MINOR.PATCH** (exemple : 1.18.2)
  - MAJOR (1.0.0) : Changements incompatibles avec les versions précédentes
  - MINOR (0.1.0) : Ajout de fonctionnalités rétrocompatibles
  - PATCH (0.0.1) : Corrections de bugs

## Déploiement

### Déploiement Automatisé
- Intégration continue via Jenkins
- Déploiement automatique via WebHooks
- Tests automatisés avant déploiement

## Maintenance et Support

### Bonnes Pratiques
- Utilisation de l'anglais dans le code source
- Messages de commit descriptifs
- Gestion des fichiers via `.gitignore`
- Documentation à jour

### Résolution des Problèmes Courants
1. **Erreurs de compilation initiales** : Vérifier les commentaires dans `CMakeLists.txt`
2. **Problèmes de droits Docker** : Utiliser `sudo` si nécessaire
3. **Erreurs de chemin d'accès** : S'assurer d'être dans le bon répertoire pour l'exécution
4. **Site inaccessible** : Essayer un autre réseau (e.g. 5G, 4G)

## Sécurité
- Gestion sécurisée des données utilisateur
- Base de données persistante et sécurisée
- Communication HTTP sécurisée
