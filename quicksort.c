void swap(game_detail *a, game_detail *b) {
    game_detail temp = *a;
    *a = *b;
    *b = temp;
}

int partition(game_detail arr[], int low, int high) {
    int pivot = arr[high].active_users;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].active_users > pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void qSort(game_detail arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        qSort(arr, low, pi - 1);
        qSort(arr, pi + 1, high);
    }
}