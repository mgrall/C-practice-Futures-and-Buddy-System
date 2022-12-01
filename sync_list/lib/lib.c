#include <lib.h>

const char* BUDDY_STATE_STRING[] = {
    [FREE] = "FREE",
    [ASSIGNED] = "ASSIGNED",
    [SPLIT] = "SPLIT"
};

int init_buddy(void** data, MetaNode* metaRoot, size_t size) {

    // Überprüft ob @param size eine Zweierpotenz ist
    if(!(ceil(log2(size)) == floor(log2(size)))){
        printf("\nZielgröße des Datenspeichers (data) muss eine Zweierpotenz sein.\n");
        return -1;
    }

    // Versuche Speicherplatz zu allokieren
    *data = malloc(size);
    if(!*data) return -1;

    // Alle Werte initialisieren
    metaRoot->state = FREE;
    metaRoot->size = size;
    metaRoot->parent = NULL;
    metaRoot->children[0] = NULL;
    metaRoot->children[1] = NULL;

    return 0;

}

void destroy_buddy(void** data, MetaNode* metaRoot) {

    // Datenspeicher freigeben
    free(*data);
    // MetaNode Baum zerstören
    destroy_meta(metaRoot->children[0]);
    destroy_meta(metaRoot->children[1]);
    free(&metaRoot);
}

MetaNode* assign_buddy(MetaNode* metaRoot, size_t size) {

    // Berechne nächste Zweierpotenz
    int s = nextPowerOf2(size);

    // finds minimal existing FREE node
    MetaNode* n = best_fit(metaRoot, s);

    // Return NULL wenn es keinen Platz mehr gab
    if(n==NULL){
        printf("\nKein passenden Platz im Speicher vorhanden.\n");
        return NULL;
    }

    // Splits node if needed
    MetaNode* m = splitNodeToSize(n, s);

    m->state = ASSIGNED;

    return m;
}

void free_buddy(MetaNode* node) {

    node->state = FREE;

    // Wenn es Kinderknoten gibt, diese auch alle rekursiv auf FREE setzen und anschließend mergen
    if(node->state == SPLIT){
        free_buddy(node->children[0]);
        free_buddy(node->children[1]);
    } else if(node->parent) tryMergeParents(node->parent);

}

void* buddy_ptr(void* data, MetaNode* node) {
    // Check ob es einen Parent-Node Gibt
    if(node->parent){
        // Größe des linken Nachbarknoten zum offset addieren
        if (node->parent->children[1] == node) {
            data = (void*) (char*) data + node->size;  
        }
        buddy_ptr(data, node->parent);
    }
    // Basisadresse + Offset
    return data;
}

// Helper (gegeben)
void visualize_buddies_helper(const MetaNode* node, int level) {
    for (int i = 0; i < level; ++i) {
        printf("| ");
    }

    printf("%s: %lu bytes\n", verbalizeState(node->state), node->size);

    if (node->state == SPLIT) {
        visualize_buddies_helper(node->children[0], level + 1);
        visualize_buddies_helper(node->children[1], level + 1);
    }
}

// Helper (gegeben)
void visualize_buddies(const MetaNode* root) {
    visualize_buddies_helper(root, 0);
}

// Helper
void init_meta(MetaNode* meta, MetaNode* parent, size_t size) {
    // Elternknoten setzen
    meta->parent = parent;
    // Falls size = 0, setze auf hälfte des Elternknotens
    if(size) {
        meta->size = size;
    } else {
        meta->size = parent->size / 2;
    }
    meta->state = FREE;
}

// Helper
void destroy_meta(MetaNode* metaRoot) {
    // Wenn Kinder vorhanden sind, diese rekursiv zerstören
    if (metaRoot->state == SPLIT){
        destroy_meta(metaRoot->children[0]);
        destroy_meta(metaRoot->children[1]);
    }
    free(metaRoot);
}

// Helper
MetaNode* splitNodeToSize(MetaNode* node, size_t size){
    // Berechne nächste 2er-Potenz
    int n = nextPowerOf2(size);

    // Wenn initialer Node zu klein ist return NULL
    if (n > node->size) return NULL;

    // Wenn Node groß genug zum splitten ist
    if (node->size / 2 >= n) {
        node->state = SPLIT;
        // Erstelle Kinder Struckturen und versuche speicher zu allokieren 
        MetaNode* child1;
        child1 = malloc(sizeof(MetaNode));
        MetaNode* child2;
        child2 = malloc(sizeof(MetaNode));
        node->children[0] = child1;
        node->children[1] = child2;

        // Split Node
        init_meta(node->children[0], node, 0);
        init_meta(node->children[1], node, 0);

        // Check ob man weiter splitten muss rekursiv
        return splitNodeToSize(node->children[0], size);
    }
    // Size passt in den momentanen Node
    return node;
}

// Helper
MetaNode* tryMergeParents(MetaNode* node){
    // Check ob beide Kinder FREE sind
    if(node->children[0] && node->children[1]){
        if(node->children[0]->state == FREE && node->children[1]->state == FREE) {
            destroy_meta(node->children[0]);
            destroy_meta(node->children[1]);
            node->state = FREE;
            node->children[0] = NULL;
            node->children[1] = NULL;
            if(node->parent) tryMergeParents(node->parent);
        }
    }
    return node;
}

// Helper
MetaNode* best_fit(MetaNode* searchRoot, size_t size){

    // Berechne nächste 2er-Potenz
    int n = nextPowerOf2(size);
    // Wenn initialer Node zu klein ist return NULL
    if (n > searchRoot->size || searchRoot->state == ASSIGNED) return NULL;
    
    if (searchRoot->state == SPLIT){
        // search children
        MetaNode* p1 = best_fit(searchRoot->children[0], size);
        MetaNode* p2 = best_fit(searchRoot->children[1], size);
        if (p1) return p1;
        return p2;

    }

    // state == FREE
    return searchRoot;

}