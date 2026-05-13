#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INF 1000000000

int taoSoNgauNhien(int min, int max) {
    return min + rand() % (max - min + 1);
}

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

void taoDoThiLienThongNgauNhien(int **a, int n) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            a[i][j] = INF;
        }
        a[i][i] = 0;
    }

    for (i = 0; i < n - 1; i++) {
        int w = taoSoNgauNhien(1, 20);
        a[i][i + 1] = w;
        a[i + 1][i] = w;
    }

    for (i = 0; i < n; i++) {
        for (j = i + 2; j < n; j++) {
            int coCanh = taoSoNgauNhien(0, 1);
            if (coCanh) {
                int w = taoSoNgauNhien(1, 20);
                a[i][j] = w;
                a[j][i] = w;
            }
        }
    }
}

void inMaTran(int **a, int n) {
    int i, j;

    printf("\nMa tran trong so cua do thi:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (a[i][j] >= INF) {
                printf("%4s", "INF");
            } else {
                printf("%4d", a[i][j]);
            }
        }
        printf("\n");
    }
}

int laDoThiEuler(int **a, int n) {
    int i, j;

    for (i = 0; i < n; i++) {
        int bac = 0;
        for (j = 0; j < n; j++) {
            if (i != j && a[i][j] < INF) {
                bac++;
            }
        }
        if (bac % 2 != 0) {
            return 0;
        }
    }

    return 1;
}

void inKetQuaEuler(int **a, int n) {
    int i, j;

    printf("\nKiem tra tinh chat Euler:\n");
    for (i = 0; i < n; i++) {
        int bac = 0;
        for (j = 0; j < n; j++) {
            if (i != j && a[i][j] < INF) {
                bac++;
            }
        }
        printf("- Dinh %d co bac %d\n", i + 1, bac);
    }

    if (laDoThiEuler(a, n)) {
        printf("=> Do thi la do thi Euler vi tat ca dinh deu co bac chan.\n");
    } else {
        printf("=> Do thi KHONG la do thi Euler vi ton tai dinh co bac le.\n");
    }
}

void dijkstra(int **a, int n, int start, int finish, int *dist, int *prev) {
    int i, step;
    int *used = (int *)calloc(n, sizeof(int));

    for (i = 0; i < n; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (step = 0; step < n; step++) {
        int u = -1;
        int best = INF;

        for (i = 0; i < n; i++) {
            if (!used[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }

        if (u == -1) {
            break;
        }

        used[u] = 1;
        for (i = 0; i < n; i++) {
            if (!used[i] && a[u][i] < INF && dist[u] + a[u][i] < dist[i]) {
                dist[i] = dist[u] + a[u][i];
                prev[i] = u;
            }
        }
    }

    free(used);
}

void inDuongDi(int *prev, int n, int start, int finish) {
    int count = 0;
    int cur = finish;
    int *path = (int *)malloc(n * sizeof(int));

    while (cur != -1) {
        path[count++] = cur;
        if (cur == start) {
            break;
        }
        cur = prev[cur];
    }

    if (path[count - 1] != start) {
        printf("Khong tim thay duong di.\n");
        free(path);
        return;
    }

    printf("Duong di ngan nhat: ");
    while (count > 0) {
        count--;
        printf("%d", path[count] + 1);
        if (count > 0) {
            printf(" -> ");
        }
    }
    printf("\n");

    free(path);
}

void xuLyDijkstra(int **a, int n) {
    int start, finish;
    int *dist = (int *)malloc(n * sizeof(int));
    int *prev = (int *)malloc(n * sizeof(int));

    printf("\nTim duong di ngan nhat bang Dijkstra\n");
    printf("Nhap dinh bat dau (1..%d): ", n);
    scanf("%d", &start);
    printf("Nhap dinh ket thuc (1..%d): ", n);
    scanf("%d", &finish);

    start--;
    finish--;

    if (start < 0 || start >= n || finish < 0 || finish >= n) {
        printf("Du lieu dinh khong hop le.\n");
        free(dist);
        free(prev);
        return;
    }

    dijkstra(a, n, start, finish, dist, prev);

    if (dist[finish] >= INF) {
        printf("Khong co duong di tu dinh %d den dinh %d.\n", start + 1, finish + 1);
    } else {
        printf("Tong trong so nho nhat tu dinh %d den dinh %d la: %d\n",
               start + 1, finish + 1, dist[finish]);
        inDuongDi(prev, n, start, finish);
    }

    free(dist);
    free(prev);
}

void prim(int **a, int n, int *parent, int *total) {
    int i, step;
    int *selected = (int *)calloc(n, sizeof(int));
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
            if (!selected[i] && best[i] < minCost) {
                minCost = best[i];
                u = i;
            }
        }

        if (u == -1) {
            break;
        }

        selected[u] = 1;
        *total += best[u];

        for (i = 0; i < n; i++) {
            if (!selected[i] && a[u][i] < best[i]) {
                best[i] = a[u][i];
                parent[i] = u;
            }
        }
    }

    free(selected);
    free(best);
}

void inCayPhuNhoNhat(int **a, int n) {
    int i;
    int total;
    int *parent = (int *)malloc(n * sizeof(int));

    prim(a, n, parent, &total);

    printf("\nCay phu nho nhat theo Prim:\n");
    for (i = 1; i < n; i++) {
        if (parent[i] != -1) {
            printf("- Noi dinh %d voi dinh %d, trong so = %d\n",
                   parent[i] + 1, i + 1, a[parent[i]][i]);
        }
    }
    printf("Tong trong so cua cay phu nho nhat: %d\n", total);

    free(parent);
}

int nhapSoDinh(void) {
    int n;
    printf("Nhap so dinh cua do thi (n >= 2): ");
    scanf("%d", &n);
    return n;
}

void xuLyChuongTrinh(int n, int **a) {
    taoDoThiLienThongNgauNhien(a, n);
    inMaTran(a, n);
    inKetQuaEuler(a, n);
    xuLyDijkstra(a, n);
    inCayPhuNhoNhat(a, n);
}

int main(void) {
    int n = nhapSoDinh();
    int **a;

    if (n < 2) {
        printf("So dinh khong hop le.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));
    a = capPhatMaTran(n);
    if (a == NULL) {
        printf("Khong du bo nho de tao ma tran.\n");
        return 1;
    }

    xuLyChuongTrinh(n, a);

    giaiPhongMaTran(a, n);
    return 0;
}
