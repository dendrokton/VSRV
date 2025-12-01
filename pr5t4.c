#include <stdio.h>
#include <limits.h>

int main() {
    int size;
    int count = 0;
    int sum = 0;
    int min = INT_MAX;
    int max = INT_MIN;
    double average = 0.0;

    printf("Введите размер массива: ");
    scanf("%d", &size);

    int arr[size];
    printf("Введите элементы массива:\n");
    for (int i = 0; i < size; i++) {
        printf("Элемент [%d]: ", i);
        scanf("%d", &arr[i]);
        if (arr[i] > 0 && arr[i] % 2 == 0) {
            count++;
            sum += arr[i];

            if (arr[i] < min) {
                min = arr[i];
            }
            if (arr[i] > max) {
                max = arr[i];
            }
        }
    }

    if (count > 0) {
        average = (double)sum / count;
    } else {
        min = 0;
        max = 0;
        average = 0.0;
    }

    if (count > 0) {
        printf("Минимальное значение: %d\n", min);
        printf("Максимальное значение: %d\n", max);
        printf("Среднее арифметическое: %.2f\n", average);
    } else {
        printf("\nЧётных положительных элементов в массиве не найдено\n");
    }

    return 0;
}