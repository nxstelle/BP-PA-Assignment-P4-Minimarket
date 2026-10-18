# Minimarket & Membership CLI App (Struct & Pointer)

A command-line minimarket simulation written in C for the **Struct & Pointer** practicum (P4). It manages a small product catalog and a membership system, and demonstrates how pointers let functions modify data directly in memory (*pass by reference*).

## Project Overview

- **`Product`** struct: `name`, `price`, `stock`
- **`Member`** struct: `name`, `points`
- Both are stored as arrays of structs (5 products, 3 members).
- Functions receive `Product *` / `Member *` and use the arrow operator (`->`) to change the original data, so every change persists across transactions.

Base rules from the tutorial: buying reduces stock, and a purchase total of **Rp20,000 or more** earns a member **+100 points**. A non-member can buy by passing `NULL` as the member pointer.

## How to Run

```bash
gcc -Wall -Wextra -o minimarket minimarket.c
./minimarket
```

## Solution

### Mandatory Task 1: Update Product Price
`void updatePrice(Product *p, int newPrice)` reassigns `p->price` through the pointer, so the new price is permanent. Available from the menu as **[5] Admin: Update Price**.

```c
void updatePrice(Product *p, int newPrice) {
    int oldPrice = p->price;
    p->price = newPrice;
    printf("[ADMIN] Price of %s changed: Rp%d -> Rp%d\n", p->name, oldPrice, p->price);
}
```

insert screenshot

### Mandatory Task 2: Update Product Name
`void updateName(Product *p, char newName[])` uses `strncpy` (a bounded variant of `strcpy`) because a `char` array cannot be assigned with `=`. The string is always null-terminated to avoid overflow. Available as **[6] Admin: Update Name**.

insert screenshot

### Mandatory Task 3: Redeem Points for Discount
Inside `buyProduct`, after the stock check, if the member pointer is not `NULL` and `m->points >= 1000`, the total gets a 10% discount and `m->points -= 1000`. Points are earned (+100) on the **final amount paid**, after any discount. Stock is checked first, so a failed purchase never costs the member any points.

```c
if (m != NULL && m->points >= 1000) {
    int discount = total / 10;
    total -= discount;
    m->points -= 1000;
}
```

Example: Citra (1200 points) buys 1 Milk (Rp25,000) -> Rp2,500 discount, pays Rp22,500, her points go 1200 -> 200 -> 300 (+100 earned).

insert screenshot

### Bonus: Interactive Menu
The static simulation became a continuous `while(1)` loop with a `switch-case` menu that only ends when the user picks **[4] Exit**:

```
[1] View Products | [2] Buy Product | [3] Check Members | [4] Exit
[5] Admin: Update Price | [6] Admin: Update Name
```

Input is validated (non-numeric input, out-of-range product/member numbers, and invalid quantities show an error instead of crashing). Entering `0` as the member number buys as a non-member.

insert screenshot

## Other Test Cases

| Scenario | Result |
|---|---|
| Buy 10 Shampoo (stock is 5) | `[FAILED]` stock message, nothing changes |
| Non-member buys 3 Bread | Sale succeeds, no points awarded |
| Enter `99` or `abc` at the menu | Error message, menu shown again |

insert screenshot

## Notes

- A third member, **Citra** (1200 points), was added to the starting data so the point-redemption discount can be demonstrated immediately. The tutorial's original members start below 1000 points.
