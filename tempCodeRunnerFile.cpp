#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Struktur node
*/
#define RED 0
#define BLACK 1

typedef struct tnodes {
    int roomNumber;
    char roomType[30];
    int price;
    char status[20];
    char bookedBy[50];

    int color;
    struct tnodes *left, *right, *parent;
} tnodes;

tnodes *root = NULL;

/*
call function ditaro di sini
*/
tnodes* createNode(int roomNumber, char roomType[], int price);
void leftRotate(tnodes *x);
void rightRotate(tnodes *x);
void fixInsert(tnodes *z);
void insertRoom(int roomNumber, char roomType[], int price);
void insertRoomMenu();
tnodes* searchRoom(int roomNumber);
void searchRoomMenu();
void bookRoom(int roomNumber, char customerName[]);
void bookRoomMenu();


/*
=======================
  CREATE NODE
=======================
*/
tnodes* createNode(int roomNumber, char roomType[], int price) {
    tnodes* newNode = (tnodes*) malloc(sizeof(tnodes));

    newNode->roomNumber = roomNumber;
    strcpy(newNode->roomType, roomType);
    newNode->price = price;
    strcpy(newNode->status, "avail");
    strcpy(newNode->bookedBy, "-");

    newNode->color = RED;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    return newNode;
}


/*
=======================
  ROTASI
=======================
*/
void leftRotate(tnodes *x) {
    tnodes *y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rightRotate(tnodes *x) {
    tnodes *y = x->left;
    x->left = y->right;

    if (y->right != NULL)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}


/*
=======================
  FIX INSERT (COLORING)
=======================
*/
void fixInsert(tnodes *z) {
    while (z->parent != NULL && z->parent->color == RED) {
        tnodes *grandparent = z->parent->parent;

        if (z->parent == grandparent->left) {
            tnodes *uncle = grandparent->right;

            if (uncle != NULL && uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                z = grandparent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }

                z->parent->color = BLACK;
                grandparent->color = RED;
                rightRotate(grandparent);
            }
        } else {
            tnodes *uncle = grandparent->left;

            if (uncle != NULL && uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                z = grandparent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }

                z->parent->color = BLACK;
                grandparent->color = RED;
                leftRotate(grandparent);
            }
        }
    }

    root->color = BLACK;
}


/*
=======================
  INSERT ROOM
=======================
*/
void insertRoom(int roomNumber, char roomType[], int price) {
    tnodes *newNode = createNode(roomNumber, roomType, price);

    tnodes *y = NULL;
    tnodes *x = root;

    while (x != NULL) {
        y = x;

        if (roomNumber < x->roomNumber)
            x = x->left;
        else if (roomNumber > x->roomNumber)
            x = x->right;
        else {
            printf("Kamar nomor %d sudah ada!\n", roomNumber);
            free(newNode);
            return;
        }
    }

    newNode->parent = y;

    if (y == NULL)
        root = newNode;
    else if (roomNumber < y->roomNumber)
        y->left = newNode;
    else
        y->right = newNode;

    fixInsert(newNode);

    printf("Kamar %d berhasil ditambahkan!\n", roomNumber);
}

void insertRoomMenu() {
    int roomNumber;
    char roomType[30];
    int price;
    char choice;

    do {
        printf("\n=== TAMBAH KAMAR ===\n");
        printf("Nomor kamar  : ");
        scanf("%d", &roomNumber);

        printf("Tipe kamar   : ");
        scanf(" %[^\n]", roomType);

        printf("Harga        : Rp");
        scanf("%d", &price);

        insertRoom(roomNumber, roomType, price);

        printf("Input kamar lagi? [y/n]: ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');
}


tnodes* searchRoom(int roomNumber) {
    tnodes *current = root;

    while (current != NULL) {
        if (roomNumber == current->roomNumber)
            return current;
        else if (roomNumber < current->roomNumber)
            current = current->left;
        else
            current = current->right;
    }

    return NULL;
}

void searchRoomMenu() {
    int roomNumber;

    printf("\n=== CARI KAMAR ===\n");
    printf("Nomor kamar: ");
    scanf("%d", &roomNumber);

    tnodes *found = searchRoom(roomNumber);

    if (found == NULL) {
        printf("Kamar %d tidak ditemukan.\n", roomNumber);
    } else {
        printf("\n--- Detail Kamar ---\n");
        printf("Nomor        : %d\n", found->roomNumber);
        printf("Tipe         : %s\n", found->roomType);
        printf("Harga        : Rp%d\n", found->price);
        printf("Status       : %s\n", found->status);
        printf("Dipesan oleh : %s\n", found->bookedBy);
    }
}


/*
=======================
  BOOKING ROOM
=======================
*/
void bookRoom(int roomNumber, char customerName[]) {
    tnodes *target = searchRoom(roomNumber);

    if (target == NULL) {
        printf("Kamar %d tidak ditemukan.\n", roomNumber);
        return;
    }

    if (strcmp(target->status, "booked") == 0) {
        printf("Kamar %d sudah dipesan oleh %s.\n",
               roomNumber, target->bookedBy);
        return;
    }

    strcpy(target->status, "booked");
    strcpy(target->bookedBy, customerName);

    printf("Kamar %d berhasil dipesan oleh %s!\n",
           roomNumber, customerName);
}

void bookRoomMenu() {
    int roomNumber;
    char customerName[50];

    printf("\n=== BOOKING KAMAR ===\n");
    printf("Nomor kamar  : ");
    scanf("%d", &roomNumber);

    printf("Nama pemesan : ");
    scanf(" %[^\n]", customerName);

    bookRoom(roomNumber, customerName);
}

/*
=======================
  CANCEL BOOKING
=======================
*/
void cancelBooking(int roomNumber) {
    tnodes *target = searchRoom(roomNumber);

    if (target == NULL) {
        printf("Kamar %d tidak ditemukan.\n", roomNumber);
        return;
    }

    if (strcmp(target->status, "avail") == 0) {
        printf("Kamar %d belum dipesan, tidak bisa di-cancel.\n", roomNumber);
        return;
    }

    printf("Booking kamar %d atas nama %s berhasil dibatalkan.\n",
           roomNumber, target->bookedBy);

    strcpy(target->status, "avail");
    strcpy(target->bookedBy, "-");
}

void cancelBookingMenu() {
    int roomNumber;

    printf("\n=== CANCEL BOOKING ===\n");
    printf("Nomor kamar  : ");
    scanf("%d", &roomNumber);

    cancelBooking(roomNumber);
}

/*
=======================
  TRANSPLANT (helper delete)
=======================
*/
void transplant(tnodes *u, tnodes *v) {
    if (u->parent == NULL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}


/*
=======================
  FIND MINIMUM (helper delete)
=======================
*/
tnodes* findMinimum(tnodes *node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}


/*
=======================
  FIX DELETE (RECOLORING)
=======================
*/
void fixDelete(tnodes *x, tnodes *xParent) {
    while (x != root && (x == NULL || x->color == BLACK)) {
        if (x == xParent->left) {
            tnodes *w = xParent->right; // sibling

            // Case 1: sibling merah
            if (w != NULL && w->color == RED) {
                w->color = BLACK;
                xParent->color = RED;
                leftRotate(xParent);
                w = xParent->right;
            }

            // Case 2: kedua anak sibling hitam
            if ((w == NULL) ||
                ((w->left == NULL  || w->left->color == BLACK) &&
                 (w->right == NULL || w->right->color == BLACK))) {
                if (w != NULL) w->color = RED;
                x = xParent;
                xParent = x->parent;
            } else {
                // Case 3: anak kanan sibling hitam
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left != NULL) w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = xParent->right;
                }

                // Case 4: anak kanan sibling merah
                w->color = xParent->color;
                xParent->color = BLACK;
                if (w->right != NULL) w->right->color = BLACK;
                leftRotate(xParent);
                x = root;
                xParent = NULL;
            }
        } else {
            tnodes *w = xParent->left; // sibling (mirror)

            // Case 1 mirror
            if (w != NULL && w->color == RED) {
                w->color = BLACK;
                xParent->color = RED;
                rightRotate(xParent);
                w = xParent->left;
            }

            // Case 2 mirror
            if ((w == NULL) ||
                ((w->right == NULL || w->right->color == BLACK) &&
                 (w->left == NULL  || w->left->color == BLACK))) {
                if (w != NULL) w->color = RED;
                x = xParent;
                xParent = x->parent;
            } else {
                // Case 3 mirror
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right != NULL) w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = xParent->left;
                }

                // Case 4 mirror
                w->color = xParent->color;
                xParent->color = BLACK;
                if (w->left != NULL) w->left->color = BLACK;
                rightRotate(xParent);
                x = root;
                xParent = NULL;
            }
        }
    }

    if (x != NULL) x->color = BLACK;
}


/*
=======================
  DELETE ROOM
=======================
*/
void deleteRoom(int roomNumber) {
    tnodes *z = searchRoom(roomNumber);

    if (z == NULL) {
        printf("Kamar %d tidak ditemukan.\n", roomNumber);
        return;
    }

    if (strcmp(z->status, "booked") == 0) {
        printf("Kamar %d sedang dipesan oleh %s, tidak bisa dihapus.\n",
               roomNumber, z->bookedBy);
        return;
    }

    tnodes *y = z;
    tnodes *x = NULL;
    tnodes *xParent = NULL;
    int yOriginalColor = y->color;

    if (z->left == NULL) {
        // Kasus: tidak punya anak kiri
        x = z->right;
        xParent = z->parent;
        transplant(z, z->right);

    } else if (z->right == NULL) {
        // Kasus: tidak punya anak kanan
        x = z->left;
        xParent = z->parent;
        transplant(z, z->left);

    } else {
        // Kasus: punya dua anak -> cari successor (minimum di subtree kanan)
        y = findMinimum(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            xParent = y;
        } else {
            xParent = y->parent;
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (yOriginalColor == BLACK)
        fixDelete(x, xParent);

    printf("Kamar %d berhasil dihapus.\n", roomNumber);
}

void deleteRoomMenu() {
    int roomNumber;

    printf("\n=== HAPUS KAMAR ===\n");
    printf("Nomor kamar  : ");
    scanf("%d", &roomNumber);

    deleteRoom(roomNumber);
}

/*
ini function main
*/
int main() {
    insertRoomMenu(); 
    searchRoomMenu(); 
    bookRoomMenu();
    cancelBookingMenu(); 
    deleteRoomMenu();

    return 0;
}