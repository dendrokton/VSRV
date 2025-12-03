#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define NUM_EMPLOYEES 2
pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;

struct employee {
    int number;
    int id;
    char first_name[20];
    char last_name[30];
    char department[30];
    int room_number;
};

struct employee employees[] = {
    { 1, 12345678, "danny", "coresh", "Accounting", 101 },
    { 2, 87654321, "misha", "levyn", "Programmers", 202 }
};

struct employee employee_of_the_day;

void copy_employee(struct employee* from, struct employee* to)
{
    pthread_mutex_lock(&a_mutex);
    to->number = from->number;
    to->id = from->id;
    strcpy(to->first_name, from->first_name);
    strcpy(to->last_name, from->last_name);
    strcpy(to->department, from->department);
    to->room_number = from->room_number;
    pthread_mutex_unlock(&a_mutex);
}

void* do_loop(void* data)
{
    int my_num = *((int*)data);
    while (1) {
        copy_employee(&employees[my_num-1], &employee_of_the_day);
    }
    return NULL;
}

int main()
{
    pthread_t p1, p2;
    int num1 = 1, num2 = 2;
    copy_employee(&employees[0], &employee_of_the_day);
    pthread_create(&p1, NULL, do_loop, (void*)&num1);
    pthread_create(&p2, NULL, do_loop, (void*)&num2);
    
    struct employee e;
    struct employee* worker;
    for (int i = 0; i < 60000; i++) {
        copy_employee(&employee_of_the_day, &e);
        worker = &employees[e.number - 1];
        
        if (e.id != worker->id) {
            printf("Ошибка: несовпадение 'id'\n");
            return 0;
        }
        if (strcmp(e.first_name, worker->first_name) != 0) {
            printf("Ошибка: несовпадение 'first_name'\n");
            return 0;
        }
        if (strcmp(e.last_name, worker->last_name) != 0) {
            printf("Ошибка: несовпадение 'last_name'\n");
            return 0;
        }
        if (strcmp(e.department, worker->department) != 0) {
            printf("Ошибка: несовпадение 'department'\n");
            return 0;
        }
        if (e.room_number != worker->room_number) {
            printf("Ошибка: несовпадение 'room_number'\n");
            return 0;
        }
    }
    
    printf("Данные сотрудников оставались согласованными на всех итерациях\n");
    return 0;
}