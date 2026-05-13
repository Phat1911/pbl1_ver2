#include <stdio.h>
#include <stdlib.h>

#define INF 1000000000

typedef struct {
    int u;
    int v;
    int w;
} Edge;

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

void inTomTatLyThuyet(int n) {
    printf("\nTOM TAT LY THUYET\n");
    printf("1. Cay phu nho nhat la cay phu co tong trong so nho nhat.\n");
    printf("2. Kruskal sap xep canh theo thu tu tang dan cua trong so.\n");
    printf("3. Moi buoc chon canh nho nhat neu canh do khong tao chu trinh.\n");
    printf("4. Khi da chon du n-1 canh thi thu duoc cay phu nho nhat.\n");
    printf("Nhap ma tran trong so %d x %d.\n", n, n);
    printf("Quy uoc: 0 neu 2 dinh khong noi nhau, so duong neu co noi nhau.\n");
}

int timGoc(int *root, int x) {
    while (root[x] != x) {
        x = root[x];
    }
    return x;
}

void hopNhom(int *root, int *rank, int a, int b) {
    int ra = timGoc(root, a);
    int rb = timGoc(root, b);

    if (ra == rb) {
        return;
    }

    if (rank[ra] < rank[rb]) {
        root[ra] = rb;
    } else if (rank[ra] > rank[rb]) {
        root[rb] = ra;
    } else {
        root[rb] = ra;
        rank[ra]++;
    }
}

int demCanh(int **a, int n) {
    int i, j;
    int count = 0;

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (a[i][j] < INF) {
                count++;
            }
        }
    }

    return count;
}

void taoDanhSachCanh(int **a, int n, Edge *edges) {
    int i, j;
    int k = 0;

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (a[i][j] < INF) {
                edges[k].u = i;
                edges[k].v = j;
                edges[k].w = a[i][j];
                k++;
            }
        }
    }
}

int soSanhCanh(const void *p1, const void *p2) {
    const Edge *a = (const Edge *)p1;
    const Edge *b = (const Edge *)p2;
    return a->w - b->w;
}

int kruskal(int **a, int n, Edge *tree, int *total) {
    int edgeCount = demCanh(a, n);
    int chosen = 0;
    int i;
    int *root = (int *)malloc(n * sizeof(int));
    int *rank = (int *)calloc(n, sizeof(int));
    Edge *edges = (Edge *)malloc(edgeCount * sizeof(Edge));

    for (i = 0; i < n; i++) {
        root[i] = i;
    }

    taoDanhSachCanh(a, n, edges);
    qsort(edges, edgeCount, sizeof(Edge), soSanhCanh);

    *total = 0;
    for (i = 0; i < edgeCount && chosen < n - 1; i++) {
        int ru = timGoc(root, edges[i].u);
        int rv = timGoc(root, edges[i].v);

        if (ru != rv) {
            tree[chosen++] = edges[i];
            *total += edges[i].w;
            hopNhom(root, rank, ru, rv);
        }
    }

    free(root);
    free(rank);
    free(edges);
    return chosen;
}

void inKetQuaKruskal(Edge *tree, int chosen, int n, int total) {
    int i;

    if (chosen != n - 1) {
        printf("\nDo thi khong lien thong, khong tao duoc cay phu nho nhat day du.\n");
        return;
    }

    printf("\nCac canh trong cay phu nho nhat:\n");
    for (i = 0; i < chosen; i++) {
        printf("- %d -- %d, trong so = %d\n", tree[i].u + 1, tree[i].v + 1, tree[i].w);
    }
    printf("Tong trong so nho nhat: %d\n", total);
}

int nhapSoDinh(void) {
    int n;
    printf("Nhap so dinh cua do thi: ");
    scanf("%d", &n);
    return n;
}

void xuLyChuongTrinh(int n, int **a, Edge *tree) {
    int chosen, total;
    inTomTatLyThuyet(n);
    nhapMaTranTrongSo(a, n);
    chosen = kruskal(a, n, tree, &total);
    inKetQuaKruskal(tree, chosen, n, total);
}

int main(void) {
    int n = nhapSoDinh();
    int **a;
    Edge *tree;

    if (n < 1) {
        printf("So dinh khong hop le.\n");
        return 1;
    }

    a = capPhatMaTran(n);
    tree = (Edge *)malloc((n - 1) * sizeof(Edge));
    if (a == NULL || tree == NULL) {
        printf("Khong du bo nho.\n");
        giaiPhongMaTran(a, n);
        free(tree);
        return 1;
    }

    xuLyChuongTrinh(n, a, tree);

    giaiPhongMaTran(a, n);
    free(tree);
    return 0;
}
