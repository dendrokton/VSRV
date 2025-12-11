#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

// Семафоры для деталей и модуля
sem_t sem_a;    // Деталь A
sem_t sem_b;    // Деталь B
sem_t sem_c;    // Деталь C
sem_t sem_ab;   // Модуль (A+B)
sem_t sem_final; // Готовое изделие
sem_t mutex;    // Мьютекс для вывода

int produced_count = 0;
const int TOTAL_PRODUCTS = 5; // Количество изделий для производства

// Функция для производства детали A
void* produce_a(void* arg) {
    while (produced_count < TOTAL_PRODUCTS) {
        sleep(1); // Производство детали A занимает 1 секунду
        
        sem_wait(&mutex);
        printf("Произведена деталь A\n");
        sem_post(&mutex);
        
        sem_post(&sem_a); // Увеличиваем счетчик деталей A
    }
    return NULL;
}

// Функция для производства детали B
void* produce_b(void* arg) {
    while (produced_count < TOTAL_PRODUCTS) {
        sleep(2); // Производство детали B занимает 2 секунды
        
        sem_wait(&mutex);
        printf("Произведена деталь B\n");
        sem_post(&mutex);
        
        sem_post(&sem_b); // Увеличиваем счетчик деталей B
    }
    return NULL;
}

// Функция для производства детали C
void* produce_c(void* arg) {
    while (produced_count < TOTAL_PRODUCTS) {
        sleep(3); // Производство детали C занимает 3 секунды
        
        sem_wait(&mutex);
        printf("Произведена деталь C\n");
        sem_post(&mutex);
        
        sem_post(&sem_c); // Увеличиваем счетчик деталей C
    }
    return NULL;
}

// Функция для сборки модуля из деталей A и B
void* assemble_ab(void* arg) {
    while (produced_count < TOTAL_PRODUCTS) {
        // Ждем, пока появятся обе детали A и B
        sem_wait(&sem_a);
        sem_wait(&sem_b);
        
        sleep(1); // Сборка модуля занимает 1 секунду
        
        sem_wait(&mutex);
        printf("Собран модуль (A+B)\n");
        sem_post(&mutex);
        
        sem_post(&sem_ab); // Увеличиваем счетчик модулей
    }
    return NULL;
}

// Функция для сборки готового изделия
void* assemble_final(void* arg) {
    while (produced_count < TOTAL_PRODUCTS) {
        // Ждем, пока появятся модуль (A+B) и деталь C
        sem_wait(&sem_ab);
        sem_wait(&sem_c);
        
        sleep(2); // Финальная сборка занимает 2 секунды
        
        sem_wait(&mutex);
        produced_count++;
        printf("=== Собрано изделие %d (C + (A+B)) ===\n", produced_count);
        sem_post(&mutex);
        
        sem_post(&sem_final); // Увеличиваем счетчик готовых изделий
    }
    return NULL;
}

// Функция для расчета оптимального плана исполнения
void calculate_schedule() {
    printf("\n=== Анализ и расчет оптимального расписания ===\n");
    
    // Время производства каждой детали
    int t_a = 1;  // Деталь A
    int t_b = 2;  // Деталь B  
    int t_c = 3;  // Деталь C
    int t_ab = 1; // Сборка модуля
    int t_final = 2; // Финальная сборка
    
    // Критический путь (максимальное время производства)
    int critical_path = t_c > (t_a + t_b + t_ab) ? t_c : (t_a + t_b + t_ab);
    critical_path += t_final;
    
    printf("Время производства детали A: %d сек\n", t_a);
    printf("Время производства детали B: %d сек\n", t_b);
    printf("Время производства детали C: %d сек\n", t_c);
    printf("Время сборки модуля (A+B): %d сек\n", t_ab);
    printf("Время финальной сборки: %d сек\n", t_final);
    printf("Критический путь: %d секунд\n", critical_path);
    
    // Расчет оптимального количества параллельных процессов
    float utilization_a = (float)t_a / critical_path;
    float utilization_b = (float)t_b / critical_path;
    float utilization_c = (float)t_c / critical_path;
    
    printf("\nЗагрузка производственных линий:\n");
    printf("Линия A: %.1f%%\n", utilization_a * 100);
    printf("Линия B: %.1f%%\n", utilization_b * 100);
    printf("Линия C: %.1f%%\n", utilization_c * 100);
    
    // Рекомендации по расписанию
    printf("\nРекомендации по оптимальному расписанию:\n");
    printf("1. Запускать производство детали C первым (дольше всего)\n");
    printf("2. Производство A и B можно запускать параллельно\n");
    printf("3. Сборку модуля (A+B) начинать сразу при наличии компонентов\n");
    printf("4. Максимальная производительность: 1 изделие в %d секунд\n", critical_path);
}

int main() {
    pthread_t thread_a, thread_b, thread_c, thread_ab, thread_final;
    
    // Инициализация семафоров
    sem_init(&sem_a, 0, 0);
    sem_init(&sem_b, 0, 0);
    sem_init(&sem_c, 0, 0);
    sem_init(&sem_ab, 0, 0);
    sem_init(&sem_final, 0, 0);
    sem_init(&mutex, 0, 1);
    
    printf("=== Запуск производственной линии ===\n");
    printf("Цель: произвести %d изделий\n\n", TOTAL_PRODUCTS);
    
    // Расчет оптимального расписания
    calculate_schedule();
    
    printf("\n=== Начало производства ===\n");
    
    // Создание потоков для производства деталей и сборки
    pthread_create(&thread_a, NULL, produce_a, NULL);
    pthread_create(&thread_b, NULL, produce_b, NULL);
    pthread_create(&thread_c, NULL, produce_c, NULL);
    pthread_create(&thread_ab, NULL, assemble_ab, NULL);
    pthread_create(&thread_final, NULL, assemble_final, NULL);
    
    // Ожидание завершения производства заданного количества изделий
    for (int i = 0; i < TOTAL_PRODUCTS; i++) {
        sem_wait(&sem_final);
    }
    
    // Даем потокам время завершиться
    sleep(5);
    
    // Уничтожение семафоров
    sem_destroy(&sem_a);
    sem_destroy(&sem_b);
    sem_destroy(&sem_c);
    sem_destroy(&sem_ab);
    sem_destroy(&sem_final);
    sem_destroy(&mutex);
    
    printf("\n=== Производство завершено ===\n");
    printf("Изготовлено %d изделий\n", TOTAL_PRODUCTS);
    
    return 0;
}