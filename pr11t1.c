#include <stdio.h>
#include <pthread.h>
#include <string.h>
#define NUM_EMPLOYEES 2

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
    to->number = from->number;
    to->id = from->id;
    strcpy(to->first_name, from->first_name);
    strcpy(to->last_name, from->last_name);
    strcpy(to->department, from->department);
    to->room_number = from->room_number;
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
    pthread_t t1, t2;
    int num1 = 1, num2 = 2;
    copy_employee(&employees[0], &employee_of_the_day);
    pthread_create(&t1, NULL, do_loop, (void*)&num1);
    pthread_create(&t2, NULL, do_loop, (void*)&num2);
    
    struct employee local;
    for (int i = 0; i < 60000; i++) {
        copy_employee(&employee_of_the_day, &local);
        struct employee* worker = &employees[local.number-1];
        
        if (local.id != worker->id) {
            printf("Ошибка: несовпадение 'id', %d != %d (итерация %d)\n",
                local.id, worker->id, i);
            return 0;
        }
        if (strcmp(local.first_name, worker->first_name) != 0) {
            printf("Ошибка: несовпадение 'first_name', %s != %s (итерация %d)\n",
                local.first_name, worker->first_name, i);
            return 0;
        }
        if (strcmp(local.last_name, worker->last_name) != 0) {
            printf("Ошибка: несовпадение 'last_name', %s != %s (итерация %d)\n",
                local.last_name, worker->last_name, i);
            return 0;
        }
        if (strcmp(local.department, worker->department) != 0) {
            printf("Ошибка: несовпадение 'department', %s != %s (итерация %d)\n",
                local.department, worker->department, i);
            return 0;
        }
        if (local.room_number != worker->room_number) {
            printf("Ошибка: несовпадение 'room_number', %d != %d (итерация %d)\n",
                local.room_number, worker->room_number, i);
            return 0;
        }
    }
    
    printf("Все данные сотрудников оставались согласованными!\n");
    return 0;
}