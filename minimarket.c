#include <stdio.h>
#include <string.h>

#define PRODUCT_COUNT 5
#define MEMBER_COUNT  3

typedef struct {
    char name[50];
    int price;
    int stock;
} Product;

typedef struct {
    char name[50];
    int points;
} Member;

/* ---------- Display functions ---------- */
void checkProduct(Product db[], int size) {
    printf("\n--- Product Catalog ---\n");
    for (int i = 0; i < size; i++) {
        printf("%d. %s - Rp%d (Stock: %d)\n", i + 1, db[i].name, db[i].price, db[i].stock);
    }
}

void checkMember(Member db[], int size) {
    printf("\n--- Member Data ---\n");
    for (int i = 0; i < size; i++) {
        printf("%d. %s - Points: %d\n", i + 1, db[i].name, db[i].points);
    }
}

/* ---------- Challenge 1: update product price ---------- */
void updatePrice(Product *p, int newPrice) {
    int oldPrice = p->price;
    p->price = newPrice;
    printf("\n[ADMIN] Price of %s changed: Rp%d -> Rp%d\n", p->name, oldPrice, p->price);
}

/* ---------- Challenge 2: update product name ---------- */
void updateName(Product *p, char newName[]) {
    char oldName[50];
    strcpy(oldName, p->name);
    strncpy(p->name, newName, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';
    printf("\n[ADMIN] Product name changed: %s -> %s\n", oldName, p->name);
}

/* ---------- Transaction + Challenge 3: redeem points ---------- */
void buyProduct(Product *p, Member *m, int qty) {
    if (qty <= 0) {
        printf("\n[FAILED] Quantity must be at least 1!\n");
        return;
    }
    if (p->stock < qty) {
        printf("\n[FAILED] Stock for %s (Remaining: %d) is not enough to buy %d!\n",
               p->name, p->stock, qty);
        return;
    }

    p->stock -= qty;
    int total = p->price * qty;

    /* Redeem: members with >= 1000 points get 10% off, costing 1000 points */
    if (m != NULL && m->points >= 1000) {
        int discount = total / 10;
        total -= discount;
        m->points -= 1000;
        printf("\n[REDEEM] %s used 1000 points -> 10%% discount (-Rp%d)\n", m->name, discount);
    }

    printf("\n[TRANSACTION] %d %s sold. Total: Rp%d\n", qty, p->name, total);

    /* Earn points on the final amount paid */
    if (total >= 20000 && m != NULL) {
        m->points += 100;
        printf("  -> Member %s earned +100 Points!\n", m->name);
    }
}

/* ---------- Input helpers ---------- */
/* Reads an int; returns 0 and flushes the line on invalid input. */
int readInt(const char *prompt, int *out) {
    printf("%s", prompt);
    if (scanf("%d", out) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        return 0;
    }
    return 1;
}

int main() {
    /* Database setup */
    Product dbProduct[PRODUCT_COUNT] = {
        {"Soap", 5000, 10}, {"Shampoo", 15000, 5}, {"Bread", 12000, 8},
        {"Milk", 25000, 15}, {"Coffee", 22000, 20}
    };
    Member dbMember[MEMBER_COUNT] = {
        {"Anisa", 500}, {"Budi", 100}, {"Citra", 1200}
    };

    int choice;
    printf("=== WELCOME TO THE MINIMARKET ===\n");

    while (1) {
        printf("\n==============================================\n");
        printf("[1] View Products | [2] Buy Product | [3] Check Members | [4] Exit\n");
        printf("[5] Admin: Update Price | [6] Admin: Update Name\n");
        printf("==============================================\n");

        if (!readInt("Choose menu: ", &choice)) {
            printf("\n[ERROR] Please enter a number!\n");
            continue;
        }

        if (choice == 4) {
            printf("\nThank you for visiting. Goodbye!\n");
            break;
        }

        switch (choice) {
            case 1:
                checkProduct(dbProduct, PRODUCT_COUNT);
                break;

            case 2: {
                int pIdx, mIdx, qty;
                checkProduct(dbProduct, PRODUCT_COUNT);
                if (!readInt("\nProduct number: ", &pIdx) || pIdx < 1 || pIdx > PRODUCT_COUNT) {
                    printf("\n[ERROR] Invalid product number!\n");
                    break;
                }
                checkMember(dbMember, MEMBER_COUNT);
                if (!readInt("\nMember number (0 = non-member): ", &mIdx) || mIdx < 0 || mIdx > MEMBER_COUNT) {
                    printf("\n[ERROR] Invalid member number!\n");
                    break;
                }
                if (!readInt("Quantity: ", &qty)) {
                    printf("\n[ERROR] Invalid quantity!\n");
                    break;
                }
                buyProduct(&dbProduct[pIdx - 1], mIdx == 0 ? NULL : &dbMember[mIdx - 1], qty);
                break;
            }

            case 3:
                checkMember(dbMember, MEMBER_COUNT);
                break;

            case 5: {
                int pIdx, newPrice;
                checkProduct(dbProduct, PRODUCT_COUNT);
                if (!readInt("\nProduct number: ", &pIdx) || pIdx < 1 || pIdx > PRODUCT_COUNT) {
                    printf("\n[ERROR] Invalid product number!\n");
                    break;
                }
                if (!readInt("New price (Rp): ", &newPrice) || newPrice < 0) {
                    printf("\n[ERROR] Invalid price!\n");
                    break;
                }
                updatePrice(&dbProduct[pIdx - 1], newPrice);
                break;
            }

            case 6: {
                int pIdx;
                char newName[50];
                checkProduct(dbProduct, PRODUCT_COUNT);
                if (!readInt("\nProduct number: ", &pIdx) || pIdx < 1 || pIdx > PRODUCT_COUNT) {
                    printf("\n[ERROR] Invalid product number!\n");
                    break;
                }
                printf("New name: ");
                if (scanf(" %49[^\n]", newName) != 1) {
                    printf("\n[ERROR] Invalid name!\n");
                    break;
                }
                updateName(&dbProduct[pIdx - 1], newName);
                break;
            }

            default:
                printf("\n[ERROR] Unknown menu option!\n");
        }
    }

    return 0;
}
