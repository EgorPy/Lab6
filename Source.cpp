#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct Date {
    int day;
    char* month;
    int year;
};

struct FIO {
    char* surname;
    char* name;
};

struct Student {
    struct FIO fio;
    struct Date birth;
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
    printf("Фамилия: "); s.fio.surname = inputString();
    printf("Имя: "); s.fio.name = inputString();
    printf("Направление: "); s.direction = inputString();
    printf("Номер группы: "); scanf("%d", &s.group); clearInputBuffer();
    printf("Дата рождения (день месяц год): ");
    scanf("%d", &s.birth.day); clearInputBuffer();
    s.birth.month = inputString();
    scanf("%d", &s.birth.year); clearInputBuffer();

    *arr = (struct Student*)realloc(*arr, (*n + 1) * sizeof(struct Student));
    (*arr)[*n] = s;
    (*n)++;
}

void printTable(struct Student* arr, int n) {
    printf("\n---------------------------------------------------------------\n");
    printf("%-15s %-15s %-10s %-10s %-10s %-10s\n", 
           "Фамилия", "Имя", "Направление", "Группа", "Дата", "Год");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-15s %-15s %-10s %-10d %-2d %-10s %-4d\n",
               arr[i].fio.surname, arr[i].fio.name, arr[i].direction,
               arr[i].group, arr[i].birth.day, arr[i].birth.month, arr[i].birth.year);
    }
}

void findYoungestOldest(struct Student* arr, int n) {
    if (n == 0) return;
    int youngest = 0, oldest = 0;
    for (int i = 1; i < n; i++) {
        struct Date d1 = arr[i].birth;
        struct Date y = arr[youngest].birth;
        struct Date o = arr[oldest].birth;
        if (d1.year > y.year || 
            (d1.year == y.year && d1.month[0] > y.month[0]) ||
            (d1.year == y.year && strcmp(d1.month, y.month) == 0 && d1.day > y.day)) {
            youngest = i;
        }
        if (d1.year < o.year || 
            (d1.year == o.year && d1.month[0] < o.month[0]) ||
            (d1.year == o.year && strcmp(d1.month, o.month) == 0 && d1.day < o.day)) {
            oldest = i;
        }
    }
    printf("Самый молодой студент: %s %s, %d %s %d\n",
           arr[youngest].fio.surname, arr[youngest].fio.name,
           arr[youngest].birth.day, arr[youngest].birth.month, arr[youngest].birth.year);
    printf("Самый старший студент: %s %s, %d %s %d\n",
           arr[oldest].fio.surname, arr[oldest].fio.name,
           arr[oldest].birth.day, arr[oldest].birth.month, arr[oldest].birth.year);
}

void saveToFile(struct Student* arr, int n, const char* filename) {
    FILE* f = fopen(filename, "wb");
    fwrite(&n, sizeof(int), 1, f);
    for (int i = 0; i < n; i++) {
        int len = strlen(arr[i].fio.surname) + 1;
        fwrite(&len, sizeof(int), 1, f);
        fwrite(arr[i].fio.surname, sizeof(char), len, f);

        len = strlen(arr[i].fio.name) + 1;
        fwrite(&len, sizeof(int), 1, f);
        fwrite(arr[i].fio.name, sizeof(char), len, f);

        len = strlen(arr[i].direction) + 1;
        fwrite(&len, sizeof(int), 1, f);
        fwrite(arr[i].direction, sizeof(char), len, f);

        fwrite(&arr[i].group, sizeof(int), 1, f);
        fwrite(&arr[i].birth.day, sizeof(int), 1, f);

        len = strlen(arr[i].birth.month) + 1;
        fwrite(&len, sizeof(int), 1, f);
        fwrite(arr[i].birth.month, sizeof(char), len, f);

        fwrite(&arr[i].birth.year, sizeof(int), 1, f);
    }
    fclose(f);
}

struct Student* loadFromFile(int* n, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    fread(n, sizeof(int), 1, f);
    struct Student* arr = (struct Student*)malloc((*n) * sizeof(struct Student));
    for (int i = 0; i < *n; i++) {
        int len;
        fread(&len, sizeof(int), 1, f);
        arr[i].fio.surname = (char*)malloc(len); fread(arr[i].fio.surname, sizeof(char), len, f);

        fread(&len, sizeof(int), 1, f);
        arr[i].fio.name = (char*)malloc(len); fread(arr[i].fio.name, sizeof(char), len, f);

        fread(&len, sizeof(int), 1, f);
        arr[i].direction = (char*)malloc(len); fread(arr[i].direction, sizeof(char), len, f);

        fread(&arr[i].group, sizeof(int), 1, f);
        fread(&arr[i].birth.day, sizeof(int), 1, f);

        fread(&len, sizeof(int), 1, f);
        arr[i].birth.month = (char*)malloc(len); fread(arr[i].birth.month, sizeof(char), len, f);

        fread(&arr[i].birth.year, sizeof(int), 1, f);
    }
    fclose(f);
    return arr;
}

int main() {
    setlocale(LC_ALL, "Russian");
    struct Student* students = NULL;
    int n = 0;
    const char* filename = "students.dat";

    students = loadFromFile(&n, filename);
    if (!students) {
        printf("Файл базы данных вуза не обнаружен. Ввод с клавиатуры.\n");
        do {
            printf("Введите число студентов (не меньше 8): ");
            scanf("%d", &n); clearInputBuffer();
        } while (n < 1);
        students = (struct Student*)malloc(n * sizeof(struct Student));
        for (int i = 0; i < n; i++) {
            printf("\nСтудент %d:\n", i + 1);
            addStudent(&students, &i); i--;
        }
    } else {
        printf("База данных студентов вуза найдена и загружена");
    }

    int choice = 0;
    while (1) {
        printf("\n1. Добавить студента\n2. Вывести таблицу\n3. Найти самого молодого/старшего\n4. Выход\nВведите выбор: ");
        scanf("%d", &choice); clearInputBuffer();
        if (choice == 1) addStudent(&students, &n);
        else if (choice == 2) printTable(students, n);
        else if (choice == 3) findYoungestOldest(students, n);
        else if (choice == 4) break;
        else printf("Неверный выбор\n");
    }

    saveToFile(students, n, filename);

    for (int i = 0; i < n; i++) {
        free(students[i].fio.surname);
        free(students[i].fio.name);
        free(students[i].direction);
        free(students[i].birth.month);
    }
    free(students);

    return 0;
}
