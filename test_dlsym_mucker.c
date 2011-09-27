#include <stdio.h>

static void test_dlsym_mucker_init(void) __attribute__ ((constructor));

void print_socket(void);

void test_dlsym_mucker_init(void) {
    printf("test_dlsym_mucker init\n");
    
    print_socket();

    return;
}

