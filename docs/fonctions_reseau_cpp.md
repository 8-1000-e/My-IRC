# Documentation des Fonctions Réseau et Système en C/C++

## Fonctions de Socket

### `socket()`
```cpp
int socket(int domain, int type, int protocol);
```
**Description**: Crée un point de communication (socket) et retourne un descripteur de fichier.

**Paramètres**:
- `domain`: Famille d'adresses (AF_INET pour IPv4, AF_INET6 pour IPv6)
- `type`: Type de socket (SOCK_STREAM pour TCP, SOCK_DGRAM pour UDP)
- `protocol`: Protocole spécifique (généralement 0 pour choisir automatiquement)

**Retour**: Descripteur de fichier en cas de succès, -1 en cas d'erreur

---

### `close()`
```cpp
int close(int fd);
```
**Description**: Ferme un descripteur de fichier (socket inclus).

**Paramètres**:
- `fd`: Descripteur de fichier à fermer

**Retour**: 0 en cas de succès, -1 en cas d'erreur

---

### `setsockopt()`
```cpp
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```
**Description**: Définit les options d'une socket.

**Paramètres**:
- `sockfd`: Descripteur de socket
- `level`: Niveau du protocole (SOL_SOCKET, IPPROTO_TCP, etc.)
- `optname`: Nom de l'option (SO_REUSEADDR, SO_KEEPALIVE, etc.)
- `optval`: Pointeur vers la valeur de l'option
- `optlen`: Taille de la valeur

**Retour**: 0 en cas de succès, -1 en cas d'erreur

**Options courantes**:
- `SO_REUSEADDR`: Permet de réutiliser une adresse locale
- `SO_KEEPALIVE`: Active les messages keep-alive
- `SO_RCVBUF`: Taille du buffer de réception
- `SO_SNDBUF`: Taille du buffer d'envoi

---

### `getsockname()`
```cpp
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
**Description**: Récupère l'adresse locale associée à une socket.

**Paramètres**:
- `sockfd`: Descripteur de socket
- `addr`: Pointeur vers une structure sockaddr pour stocker l'adresse
- `addrlen`: Pointeur vers la taille de la structure (entrée/sortie)

**Retour**: 0 en cas de succès, -1 en cas d'erreur

---

### `bind()`
```cpp
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
**Description**: Associe une adresse locale à une socket.

**Paramètres**:
- `sockfd`: Descripteur de socket
- `addr`: Pointeur vers la structure d'adresse
- `addrlen`: Taille de la structure d'adresse

**Retour**: 0 en cas de succès, -1 en cas d'erreur

---

### `connect()`
```cpp
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
**Description**: Établit une connexion à une adresse distante.

**Paramètres**:
- `sockfd`: Descripteur de socket
- `addr`: Pointeur vers la structure d'adresse du serveur
- `addrlen`: Taille de la structure d'adresse

**Retour**: 0 en cas de succès, -1 en cas d'erreur

---

### `listen()`
```cpp
int listen(int sockfd, int backlog);
```
**Description**: Met une socket en mode écoute pour accepter des connexions entrantes.

**Paramètres**:
- `sockfd`: Descripteur de socket
- `backlog`: Nombre maximum de connexions en attente

**Retour**: 0 en cas de succès, -1 en cas d'erreur

---

### `accept()`
```cpp
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
**Description**: Accepte une connexion entrante sur une socket en écoute.

**Paramètres**:
- `sockfd`: Descripteur de socket en écoute
- `addr`: Pointeur vers une structure pour stocker l'adresse du client (peut être NULL)
- `addrlen`: Pointeur vers la taille de la structure (entrée/sortie)

**Retour**: Nouveau descripteur de socket pour la connexion, -1 en cas d'erreur

---

### `send()`
```cpp
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```
**Description**: Envoie des données sur une socket connectée.

**Paramètres**:
- `sockfd`: Descripteur de socket
- `buf`: Pointeur vers les données à envoyer
- `len`: Nombre d'octets à envoyer
- `flags`: Flags de contrôle (généralement 0, MSG_NOSIGNAL, MSG_DONTWAIT)

**Retour**: Nombre d'octets envoyés, -1 en cas d'erreur

---

### `recv()`
```cpp
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```
**Description**: Reçoit des données depuis une socket connectée.

**Paramètres**:
- `sockfd`: Descripteur de socket
- `buf`: Buffer pour stocker les données reçues
- `len`: Taille maximale du buffer
- `flags`: Flags de contrôle (0, MSG_PEEK, MSG_WAITALL, etc.)

**Retour**: Nombre d'octets reçus, 0 si connexion fermée, -1 en cas d'erreur

---

## Fonctions de Conversion d'Adresses

### `htons()` / `htonl()`
```cpp
uint16_t htons(uint16_t hostshort);
uint32_t htonl(uint32_t hostlong);
```
**Description**: Convertit des valeurs de l'ordre des octets de l'hôte vers l'ordre réseau (big-endian).

- `htons`: Host TO Network Short (16 bits)
- `htonl`: Host TO Network Long (32 bits)

---

### `ntohs()` / `ntohl()`
```cpp
uint16_t ntohs(uint16_t netshort);
uint32_t ntohl(uint32_t netlong);
```
**Description**: Convertit des valeurs de l'ordre réseau vers l'ordre des octets de l'hôte.

- `ntohs`: Network TO Host Short (16 bits)
- `ntohl`: Network TO Host Long (32 bits)

---

### `inet_addr()` (Obsolète)
```cpp
in_addr_t inet_addr(const char *cp);
```
**Description**: Convertit une adresse IPv4 en notation pointée (string) vers un entier réseau.

**Note**: Obsolète, préférer `inet_pton()`

---

### `inet_ntoa()` (Obsolète)
```cpp
char *inet_ntoa(struct in_addr in);
```
**Description**: Convertit une adresse réseau IPv4 vers une chaîne en notation pointée.

**Note**: Obsolète, utiliser `inet_ntop()`. La fonction retourne un pointeur statique (non thread-safe).

---

### `inet_ntop()`
```cpp
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
```
**Description**: Convertit une adresse réseau (IPv4 ou IPv6) en chaîne de caractères.

**Paramètres**:
- `af`: Famille d'adresses (AF_INET ou AF_INET6)
- `src`: Pointeur vers l'adresse binaire
- `dst`: Buffer pour stocker la chaîne résultante
- `size`: Taille du buffer (INET_ADDRSTRLEN pour IPv4, INET6_ADDRSTRLEN pour IPv6)

**Retour**: Pointeur vers `dst` en cas de succès, NULL en cas d'erreur

---

## Fonctions de Résolution de Noms

### `getprotobyname()`
```cpp
struct protoent *getprotobyname(const char *name);
```
**Description**: Récupère les informations sur un protocole à partir de son nom.

**Paramètres**:
- `name`: Nom du protocole ("tcp", "udp", "icmp", etc.)

**Retour**: Pointeur vers une structure protoent, NULL en cas d'erreur

**Structure protoent**:
```cpp
struct protoent {
    char  *p_name;       // Nom officiel du protocole
    char **p_aliases;    // Liste d'alias
    int    p_proto;      // Numéro du protocole
};
```

---

### `gethostbyname()` (Obsolète)
```cpp
struct hostent *gethostbyname(const char *name);
```
**Description**: Récupère les informations sur un hôte à partir de son nom.

**Note**: Obsolète, utiliser `getaddrinfo()` qui supporte IPv6.

**Structure hostent**:
```cpp
struct hostent {
    char  *h_name;       // Nom officiel de l'hôte
    char **h_aliases;    // Liste d'alias
    int    h_addrtype;   // Type d'adresse (AF_INET)
    int    h_length;     // Longueur de l'adresse
    char **h_addr_list;  // Liste d'adresses
};
```

---

### `getaddrinfo()`
```cpp
int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res);
```
**Description**: Résout un nom d'hôte et un service en adresse réseau (supporte IPv4 et IPv6).

**Paramètres**:
- `node`: Nom d'hôte ou adresse IP (peut être NULL)
- `service`: Nom du service ou numéro de port (peut être NULL)
- `hints`: Structure avec les critères de recherche (peut être NULL)
- `res`: Pointeur vers une liste chaînée de résultats

**Structure addrinfo**:
```cpp
struct addrinfo {
    int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // IPPROTO_TCP, IPPROTO_UDP
    socklen_t        ai_addrlen;   // Longueur de ai_addr
    struct sockaddr *ai_addr;      // Adresse socket
    char            *ai_canonname; // Nom canonique
    struct addrinfo *ai_next;      // Élément suivant
};
```

**Retour**: 0 en cas de succès, code d'erreur sinon

---

### `freeaddrinfo()`
```cpp
void freeaddrinfo(struct addrinfo *res);
```
**Description**: Libère la mémoire allouée par `getaddrinfo()`.

**Paramètres**:
- `res`: Pointeur vers la structure retournée par `getaddrinfo()`

---

## Fonctions de Signaux

### `signal()`
```cpp
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```
**Description**: Définit un gestionnaire de signal simple (préférer `sigaction()`).

**Paramètres**:
- `signum`: Numéro du signal (SIGINT, SIGTERM, etc.)
- `handler`: Fonction de gestion, SIG_IGN, ou SIG_DFL

**Retour**: Ancien gestionnaire en cas de succès, SIG_ERR en cas d'erreur

**Signaux courants**:
- `SIGINT`: Interruption (Ctrl+C)
- `SIGTERM`: Demande de terminaison
- `SIGPIPE`: Écriture sur un pipe fermé
- `SIGCHLD`: Fin d'un processus fils
- `SIGUSR1/SIGUSR2`: Signaux utilisateur

---

### `sigaction()`
```cpp
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```
**Description**: Définit un gestionnaire de signal avec plus de contrôle que `signal()`.

**Structure sigaction**:
```cpp
struct sigaction {
    void (*sa_handler)(int);              // Gestionnaire simple
    void (*sa_sigaction)(int, siginfo_t *, void *); // Gestionnaire avancé
    sigset_t sa_mask;                     // Signaux à bloquer pendant le traitement
    int sa_flags;                         // Flags (SA_RESTART, SA_SIGINFO, etc.)
    void (*sa_restorer)(void);            // Obsolète
};
```

**Paramètres**:
- `signum`: Numéro du signal
- `act`: Nouvelle action à installer
- `oldact`: Ancienne action (peut être NULL)

**Retour**: 0 en cas de succès, -1 en cas d'erreur

---

### `sigemptyset()`
```cpp
int sigemptyset(sigset_t *set);
```
**Description**: Initialise un ensemble de signaux vide.

---

### `sigfillset()`
```cpp
int sigfillset(sigset_t *set);
```
**Description**: Initialise un ensemble contenant tous les signaux.

---

### `sigaddset()`
```cpp
int sigaddset(sigset_t *set, int signum);
```
**Description**: Ajoute un signal à un ensemble.

---

### `sigdelset()`
```cpp
int sigdelset(sigset_t *set, int signum);
```
**Description**: Retire un signal d'un ensemble.

---

### `sigismember()`
```cpp
int sigismember(const sigset_t *set, int signum);
```
**Description**: Vérifie si un signal fait partie d'un ensemble.

**Retour**: 1 si présent, 0 si absent, -1 en cas d'erreur

---

## Fonctions de Fichiers

### `lseek()`
```cpp
off_t lseek(int fd, off_t offset, int whence);
```
**Description**: Déplace le pointeur de lecture/écriture d'un fichier.

**Paramètres**:
- `fd`: Descripteur de fichier
- `offset`: Déplacement en octets
- `whence`: Point de référence
  - `SEEK_SET`: Début du fichier
  - `SEEK_CUR`: Position courante
  - `SEEK_END`: Fin du fichier

**Retour**: Nouvelle position, -1 en cas d'erreur

---

### `fstat()`
```cpp
int fstat(int fd, struct stat *statbuf);
```
**Description**: Récupère les informations sur un fichier à partir de son descripteur.

**Structure stat**:
```cpp
struct stat {
    dev_t     st_dev;     // ID du périphérique
    ino_t     st_ino;     // Numéro d'inode
    mode_t    st_mode;    // Mode (permissions)
    nlink_t   st_nlink;   // Nombre de liens
    uid_t     st_uid;     // UID du propriétaire
    gid_t     st_gid;     // GID du groupe
    dev_t     st_rdev;    // ID du périphérique (si fichier spécial)
    off_t     st_size;    // Taille totale en octets
    blksize_t st_blksize; // Taille de bloc pour I/O
    blkcnt_t  st_blocks;  // Nombre de blocs alloués
    time_t    st_atime;   // Dernier accès
    time_t    st_mtime;   // Dernière modification
    time_t    st_ctime;   // Dernier changement de statut
};
```

---

### `fcntl()`
```cpp
int fcntl(int fd, int cmd, ... /* arg */);
```
**Description**: Manipule les descripteurs de fichiers (contrôle de fichier).

**Commandes courantes**:
- `F_GETFL`: Obtenir les flags de statut
- `F_SETFL`: Définir les flags de statut
- `F_GETFD`: Obtenir les flags du descripteur
- `F_SETFD`: Définir les flags du descripteur
- `F_DUPFD`: Dupliquer un descripteur

**Flags pour F_SETFL**:
- `O_NONBLOCK`: Mode non-bloquant
- `O_APPEND`: Mode ajout
- `O_ASYNC`: I/O asynchrone

**Retour**: Dépend de la commande, -1 en cas d'erreur

---

## Fonction de Multiplexage I/O

### `poll()`
```cpp
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```
**Description**: Attend des événements sur plusieurs descripteurs de fichiers.

**Structure pollfd**:
```cpp
struct pollfd {
    int   fd;         // Descripteur de fichier
    short events;     // Événements à surveiller
    short revents;    // Événements survenus
};
```

**Événements (events/revents)**:
- `POLLIN`: Données disponibles en lecture
- `POLLOUT`: Prêt pour l'écriture
- `POLLERR`: Condition d'erreur
- `POLLHUP`: Raccrochage (connexion fermée)
- `POLLNVAL`: Descripteur invalide

**Paramètres**:
- `fds`: Tableau de structures pollfd
- `nfds`: Nombre d'éléments dans le tableau
- `timeout`: Timeout en millisecondes (-1 pour infini, 0 pour non-bloquant)

**Retour**: Nombre de descripteurs prêts, 0 si timeout, -1 en cas d'erreur

---

## Notes Importantes

### Gestion des Erreurs
Toujours vérifier les valeurs de retour et utiliser `perror()` ou `strerror(errno)` pour les messages d'erreur détaillés.

### Thread-Safety
- `inet_ntoa()` n'est pas thread-safe, utiliser `inet_ntop()`
- `gethostbyname()` n'est pas thread-safe, utiliser `getaddrinfo()`

