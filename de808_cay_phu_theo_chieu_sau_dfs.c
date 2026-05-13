#include <stdio.h>
#include <stdlib.h>

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

void nhapMaTranKe(int **a, int n) {
    int i, j, f = 0;
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            int x;
            scanf("%d", &a[i][j]);
            if (a[i][j] && a[i][j] != 1) f = 1;
        }
    }
    if (f) {
        printf("Trong ma tran cua ban co gia tri khong hop le, vui long nhap lai\n");
        nhapMaTranKe(a, n);
    }
}

void inTomTatLyThuyet(void) {
    printf("\nTOM TAT LY THUYET\n");
    printf("1. Cay la do thi lien thong va khong co chu trinh.\n");
    printf("2. Cay phu la cay chua tat ca cac dinh cua do thi ban dau.\n");
    printf("3. DFS di sau theo mot nhanh roi moi quay lui de tim nhanh khac.\n");
    printf("4. Cac canh duoc chon trong luc DFS se tao thanh cay phu DFS.\n");
}

void dfsCayPhu(int **a, int n, int start, int *parent) {
    int top = -1;
    int i;
    int *stack = (int *)malloc(n * sizeof(int));
    int *visited = (int *)calloc(n, sizeof(int));

    for (i = 0; i < n; i++) {
        parent[i] = -1;
    }

    stack[++top] = start;
    visited[start] = 1;

    while (top >= 0) {
        int u = stack[top--];
        for (i = n - 1; i >= 0; i--) {
            if (a[u][i] != 0 && !visited[i]) {
                visited[i] = 1;
                parent[i] = u;
                stack[++top] = i;
            }
        }
    }

    free(stack);
    free(visited);
}

int demDinhDaNoi(int *parent, int n, int start) {
    int i;
    int count = 1;
    for (i = 0; i < n; i++) {
        if (i != start && parent[i] != -1) {
            count++;
        }
    }
    return count;
}

void inKetQuaCayPhu(int *parent, int n, int start) {
    int i;

    printf("\nCac canh trong cay phu theo chieu sau:\n");
    for (i = 0; i < n; i++) {
        if (parent[i] != -1) {
            printf("- %d -> %d\n", parent[i] + 1, i + 1);
        }
    }

    if (demDinhDaNoi(parent, n, start) == n) {
        printf("=> Da tao duoc cay phu DFS vi do thi lien thong tu dinh bat dau.\n");
    } else {
        printf("=> Khong tao duoc cay phu day du do do thi khong lien thong.\n");
    }
}

void inGoiYSuDung(int n) {
    printf("\nHUONG DAN NHANH\n");
    printf("- Mỗi hang la thong tin noi cua 1 dinh.\n");
    printf("- Dinh duoc danh so tu 1 den %d.\n", n);
    printf("- Vi du: neu hang 2 cot 5 bang 1, nghia la dinh 2 noi voi dinh 5.\n");
    printf("Nhap ma tran %d x %d (0 la khong noi, 1 la co noi):\n", n, n);
}

int nhapSoDinh(void) {
    int n;
    printf("Nhap so dinh cua do thi: ");
    scanf("%d", &n);
    return n;
}

int nhapDinhBatDau(int n) {
    int start;
    printf("Nhap dinh bat dau de tao cay phu DFS (1..%d): ", n);
    scanf("%d", &start);
    return start - 1;
}

void xuLyChuongTrinh(int n, int **a, int *parent) {
    int start;
    inTomTatLyThuyet();
    inGoiYSuDung(n);
    nhapMaTranKe(a, n);
    start = nhapDinhBatDau(n);
    if (start < 0 || start >= n) {
        printf("Dinh bat dau khong hop le.\n");
        return;
    }
    dfsCayPhu(a, n, start, parent);
    inKetQuaCayPhu(parent, n, start);
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
