#include <stdio.h>
#include <pthread.h>

// Thread function for adding two numbers
void* sum_numbers(void* arg) {
    int sum = 6 * 2 + 1;
    return (void*)(size_t)sum;
}

// Thread function to return the string "Hello World!"
void* hello_world(void* arg) {
    return "Hello World!";
}

// Thread function to return the thread ID
void* thread_id(void* arg) {
    pthread_t id = pthread_self();
    return (void*)(size_t)id;
}

int main() {
    pthread_t thread1, thread2, thread3;
    void *result1, *result2, *result3;

    // Creating threads
    pthread_create(&thread1, NULL, sum_numbers, NULL);
    pthread_create(&thread2, NULL, hello_world, NULL);
    pthread_create(&thread3, NULL, thread_id, NULL);

    // Waiting for threads to finish and retrieving results
    pthread_join(thread1, &result1);
    pthread_join(thread2, &result2);
    pthread_join(thread3, &result3);

    // Printing the results from each thread
    printf("Sum of numbers (6 * 2 + 1): %d\n", (int)(size_t)result1);
    printf("Thread 2 message: %s\n", (char*)result2);
    printf("Thread 3 ID: %lu\n", (unsigned long)result3);

    return 0;
}
