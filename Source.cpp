#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct Student {
    char* surname;
    char* name;
    char* direction;
    int group;
};

char* inputString() {
    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), stdin)) return NULL;
    buffer[strcspn(buffer, "\n")] = 0;
    char* s = (char*)malloc(strlen(buffer) + 1);
    strcpy(s, buffer);
    return s;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void addStudent(struct Student** arr, int* n) {
    struct Student s;
    printf("Введите фамилию: ");
    s.surname = inputString();
    printf("Введите имя: ");
    s.name = inputString();
    printf("Введите направление (например ПИН): ");
    s.direction = inputString();
    printf("Введите номер группы: ");
    scanf("%d", &s.group);
    clearInputBuffer();

    *arr = (struct Student*)realloc(*arr, (*n + 1) * sizeof(struct Student));
    (*arr)[*n] = s;
    (*n)++;
}

void printTable(struct Student* arr, int n) {
    printf("\n-------------------------------------------------------------\n");
    printf("%-15s %-15s %-15s %-10s\n", "Фамилия", "Имя", "Направление", "Группа");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf(arr[i].surname);

        printf("%-15s %-15s %-15s %-10d\n",
               arr[i].surname, arr[i].name, arr[i].direction, arr[i].group);
    }
}

void findByDirection(struct Student* arr, int n) {
    printf("Введите направление: ");
    char* dir = inputString();
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].direction, dir) == 0) {
            printf("%s %s, группа %d\n", arr[i].surname, arr[i].name, arr[i].group);
            found = 1;
        }
    }
    if (!found) printf("Нет студентов по данному направлению.\n");
    free(dir);
}

int cmpStudents(const void* a, const void* b) {
    const struct Student* s1 = (const struct Student*)a;
    const struct Student* s2 = (const struct Student*)b;
    return strcmp(s1->surname, s2->surname);
}

void findGroupSorted(struct Student* arr, int n) {
    printf("Введите направление: ");
    char* dir = inputString();
    printf("Введите номер группы: ");
    int g;
    scanf("%d", &g);
    clearInputBuffer();

    struct Student* result = NULL;
    int m = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].direction, dir) == 0 && arr[i].group == g) {
            result = (struct Student*)realloc(result, (m + 1) * sizeof(struct Student));
            result[m] = arr[i];
            m++;
        }
    }

    if (m == 0) {
        printf("Нет студентов в группе %s-%d.\n", dir, g);
        free(dir);
        return;
    }

    qsort(result, m, sizeof(struct Student), cmpStudents);

    printf("\nСтуденты группы %s-%d (отсортировано):\n", dir, g);
    for (int i = 0; i < m; i++) {
        printf("%s %s\n", result[i].surname, result[i].name);
    }

    free(result);
    free(dir);
}

int main() {
    setlocale(LC_ALL, "Russian");

    struct Student* students = NULL;
    int n = 0;

    do {
        printf("Введите число студентов n (не меньше 8): ");
        scanf("%d", &n);
        clearInputBuffer();
    } while (n < 1);

    students = (struct Student*)malloc(n * sizeof(struct Student));

    for (int i = 0; i < n; i++) {
        printf("\nСтудент %d:\n", i + 1);
        printf("Фамилия: ");
        students[i].surname = inputString();
        printf("Имя: ");
        students[i].name = inputString();
        printf("Направление: ");
        students[i].direction = inputString();
        printf("Номер группы: ");
        scanf("%d", &students[i].group);
        clearInputBuffer();
    }

    int choice = 0;
    while (1) {
        printf("\n================ МЕНЮ ================\n");
        printf("1. Добавить студента\n");
        printf("2. Вывести таблицу\n");
        printf("3. Найти по направлению\n");
        printf("4. Найти группу и отсортировать\n");
        printf("5. Выход\n");
        printf("Введите выбор: ");

        scanf("%d", &choice);
        clearInputBuffer();

        if (choice == 1) addStudent(&students, &n);
        else if (choice == 2) printTable(students, n);
        else if (choice == 3) findByDirection(students, n);
        else if (choice == 4) findGroupSorted(students, n);
        else if (choice == 5) break;
        else printf("Неверный выбор.\n");
    }

    for (int i = 0; i < n; i++) {
        free(students[i].surname);
        free(students[i].name);
        free(students[i].direction);
    }
    free(students);

    return 0;
}
