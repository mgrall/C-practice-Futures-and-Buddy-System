#include <stdio.h>
#include <lib.h>

int main() {
    void* buddy_data;
    MetaNode buddy_meta;

    if (init_buddy(&buddy_data, &buddy_meta, 1024) < 0) {
        printf("init fail\n");
        return -1;
    }
    MetaNode* pos25 = assign_buddy(&buddy_meta, 25);
    MetaNode* pos111 = assign_buddy(&buddy_meta, 111);
    MetaNode* pos141 = assign_buddy(&buddy_meta, 141);

    *((uint32_t*) buddy_ptr(buddy_data, pos111)) = 42;

    MetaNode* pos17 = assign_buddy(&buddy_meta, 17);

    visualize_buddies(&buddy_meta);
    printf("\n");

    free_buddy(pos25);
    free_buddy(pos17);

    visualize_buddies(&buddy_meta);
    printf("\n");

    free_buddy(pos141);

    visualize_buddies(&buddy_meta);
    printf("\n");

    destroy_buddy(&buddy_data, &buddy_meta);

    return 0;
}
