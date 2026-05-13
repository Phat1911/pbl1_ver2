#include <stdio.h>
#include <stdlib.h>

#define INF 1000000000

int **capPhatMaTran(int n) {
    int i;
    int **a = (int **)malloc(n * sizeof(int *));
    if (a == NULL) {
        return NULL;
    }

    for (i = 0; i < n; i++) {
        a[i] = (int *)malloc(n * sizeof(int));
        if (a[i] == NULL) {
            while (i > 0) {
                free(a[i - 1]);
                i--;
            }
            free(a);
            return NULL;
        }
    }

    return a;
}

void giaiPhongMaTran(int **a, int n) {
    int i;
    if (a == NULL) {
        return;
    }
    for (i = 0; i < n; i++) {
        free(a[i]);
    }
    free(a);
}

void nhapMaTranTrongSo(int **a, int n) {
    int i, j, f = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &a[i][j]);
            if (a[i][j] < 0) f = 1;
            if (i == j) {
                a[i][j] = 0;
            } else if (a[i][j] == 0) {
                a[i][j] = INF;
            }
        }
    }
    if (f) {
        printf("Trong ma tran cua ban co gia tri khong hop le, vui long nhap lai\n");
        nhapMaTranTrongSo(a, n);
    }
}

void inTomTatLyThuyet(void) {
    printf("\nTOM TAT LY THUYET\n");
    printf("1. Cay phu la tap canh noi du tat ca dinh va khong tao chu trinh.\n");
    printf("2. Cay phu nho nhat la cay phu co tong trong so nho nhat.\n");
    printf("3. Prim xay cay bang cach mo rong tung buoc tu mot dinh ban dau.\n");
    printf("4. Moi buoc chon canh nho nhat de noi them 1 dinh moi vao cay.\n");
}

void prim(int **a, int n, int *parent, int *total) {
    int i, step;
    int *picked = (int *)calloc(n, sizeof(int));
    int *best = (int *)malloc(n * sizeof(int));

    for (i = 0; i < n; i++) {
        best[i] = INF;
        parent[i] = -1;
    }

    best[0] = 0;
    *total = 0;

    for (step = 0; step < n; step++) {
        int u = -1;
        int minCost = INF;

        for (i = 0; i < n; i++) {
            if (!picked[i] && best[i] < minCost) {
                minCost = best[i];
                u = i;
            }
        }

        if (u == -1) {
            break;
        }

        picked[u] = 1;
        *total += best[u];

        for (i = 0; i < n; i++) {
            if (!picked[i] && a[u][i] < best[i]) {
                best[i] = a[u][i];
                parent[i] = u;
            }
        }
    }

    free(picked);
    free(best);
}

int laLienThongTheoPrim(int *parent, int n) {
    int i;
    for (i = 1; i < n; i++) {
        if (parent[i] == -1) {
            return 0;
        }
    }
    return 1;
}

void inKetQuaPrim(int **a, int *parent, int n, int total) {
    int i;

    if (!laLienThongTheoPrim(parent, n)) {
        printf("\nDo thi khong lien thong, khong tao duoc cay phu nho nhat day du.\n");
        return;
    }

    printf("\nCac canh trong cay phu nho nhat:\n");
    for (i = 1; i < n; i++) {
        printf("- %d -- %d, trong so = %d\n", parent[i] + 1, i + 1, a[parent[i]][i]);
    }
    printf("Tong trong so nho nhat: %d\n", total);
}

int nhapSoDinh(void) {
    int n;
    printf("Nhap so dinh cua do thi: ");
    scanf("%d", &n);
    return n;
}

void xuLyChuongTrinh(int n, int **a, int *parent) {
    int total;
    inTomTatLyThuyet();
    printf("Nhap ma tran trong so %d x %d.\n", n, n);
    printf("Quy uoc: 0 neu 2 dinh khong noi nhau, so duong neu co noi nhau.\n");
    nhapMaTranTrongSo(a, n);
    prim(a, n, parent, &total);
    inKetQuaPrim(a, parent, n, total);
}

int main(void) {
    int n = nhapSoDinh();
    int **a;
    int *parent;

    if (n < 1) {
        printf("So dinh khong hop le.\n");
        return 1;
    }

    a = capPhatMaTran(n);
    parent = (int *)malloc(n * sizeof(int));
    if (a == NULL || parent == NULL) {
        printf("Khong du bo nho.\n");
        giaiPhongMaTran(a, n);
        free(parent);
        return 1;
    }

    xuLyChuongTrinh(n, a, parent);

    giaiPhongMaTran(a, n);
    free(parent);
    return 0;
}
