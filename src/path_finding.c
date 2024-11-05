#include "../include/path_finding.h"

// Fonction pour créer un nouveau tas binaire
MinHeap* create_min_heap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->nodes = (Node*)malloc(capacity * sizeof(Node));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// Fonction pour échanger deux nœuds dans le tas
void swap_nodes(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

// Fonction pour réorganiser le tas après l'insertion d'un nouvel élément
void heapify_up(MinHeap* heap, int idx) {
    while (idx > 0 && heap->nodes[idx].dist < heap->nodes[(idx - 1) / 2].dist) {
        swap_nodes(&heap->nodes[idx], &heap->nodes[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
}

// Fonction pour réorganiser le tas après l'extraction de l'élément minimum
void heapify_down(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left].dist < heap->nodes[smallest].dist) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right].dist < heap->nodes[smallest].dist) {
        smallest = right;
    }
    if (smallest != idx) {
        swap_nodes(&heap->nodes[idx], &heap->nodes[smallest]);
        heapify_down(heap, smallest);
    }
}

// Fonction pour insérer un nouveau nœud dans le tas
void insert_min_heap(MinHeap* heap, int x, int y, int dist) {
    if (heap->size == heap->capacity) {
        fprintf(stderr, "Heap overflow\n");
        return;
    }
    heap->nodes[heap->size] = (Node){x, y, dist};
    heapify_up(heap, heap->size);
    heap->size++;
}

// Fonction pour extraire le nœud avec la plus petite distance du tas
Node extract_min(MinHeap* heap) {
    if (heap->size == 0) {
        fprintf(stderr, "Heap underflow\n");
        return (Node){-1, -1, INT_MAX};
    }
    Node min_node = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return min_node;
}

// Fonction pour vérifier si le déplacement est valide
bool is_valid_move(Maze* maze, int x, int y, bool** visited) {
    return (x >= 0 && x < maze->width && y >= 0 && y < maze->height && !visited[y][x]);
}

int dijkstra(Maze* maze) {
    int** dist = malloc(maze->height * sizeof(int*));  // Allocation de mémoire pour la table des distances
    bool** visited = malloc(maze->height * sizeof(bool*));  // Allocation de mémoire pour la table des cellules visitées

    int start_x = -1, start_y = -1;

    // Initialisation des tables des distances et des cellules visitées
    for (int i = 0; i < maze->height; i++) {
        dist[i] = malloc(maze->width * sizeof(int));
        visited[i] = malloc(maze->width * sizeof(bool));
        for (int j = 0; j < maze->width; j++) {
            dist[i][j] = INT_MAX;  // Initialisation des distances à l'infini
            visited[i][j] = false;  // Aucune cellule visitée initialement

            if (maze->grid[i][j].start) {
                start_x = j;
                start_y = i;
            }
        }
    }

    // Si aucune cellule de départ n'est trouvée, retourner une erreur
    if (start_x == -1 || start_y == -1) {
        fprintf(stderr, "Error: start cell not found\n");
        return -1;
    }

    // Initialisation de la distance de la cellule de départ à 0
    dist[start_y][start_x] = 0;

    // Création du tas binaire
    MinHeap* heap = create_min_heap(maze->width * maze->height);
    insert_min_heap(heap, start_x, start_y, 0);

    // Directions : gauche, droite, haut, bas
    int dir_x[] = {-1, 1, 0, 0};
    int dir_y[] = {0, 0, -1, 1};

    // Boucle principale pour explorer les cellules
    while (heap->size > 0) {
        // Extraire la cellule avec la plus petite distance
        Node min_node = extract_min(heap);
        int curr_x = min_node.x;
        int curr_y = min_node.y;

        // Marquer la cellule actuelle comme visitée
        visited[curr_y][curr_x] = true;

        // Si la cellule actuelle est la sortie, retourner la distance
        if (maze->grid[curr_y][curr_x].end) {
            int result = dist[curr_y][curr_x];
            // Libérer la mémoire avant de retourner
            for (int i = 0; i < maze->height; i++) {
                free(dist[i]);
                free(visited[i]);
            }
            free(dist);
            free(visited);
            free(heap->nodes);
            free(heap);
            return result;
        }

        // Explorer les voisins (haut, bas, gauche, droite)
        for (int d = 0; d < 4; d++) {
            int nx = curr_x + dir_x[d];
            int ny = curr_y + dir_y[d];

            // Vérifier si le voisin est dans les limites et non visité
            if (is_valid_move(maze, nx, ny, visited)) {
                // Vérifier les murs
                if ((d == 0 && !maze->grid[curr_y][curr_x].left) ||   // Gauche
                    (d == 1 && !maze->grid[curr_y][curr_x].right) ||  // Droite
                    (d == 2 && !maze->grid[curr_y][curr_x].up) ||     // Haut
                    (d == 3 && !maze->grid[curr_y][curr_x].down)) {   // Bas

                    // Si la distance peut être réduite, la mettre à jour
                    if (dist[curr_y][curr_x] + 1 < dist[ny][nx]) {
                        dist[ny][nx] = dist[curr_y][curr_x] + 1;
                        insert_min_heap(heap, nx, ny, dist[ny][nx]);
                    }
                }
            }
        }
    }

    // Si aucune solution n'est trouvée, libérer la mémoire et retourner -1
    for (int i = 0; i < maze->height; i++) {
        free(dist[i]);
        free(visited[i]);
    }
    free(dist);
    free(visited);
    free(heap->nodes);
    free(heap);

    return -1;
}