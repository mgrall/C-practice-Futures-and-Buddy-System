#ifndef LIB_H
#define LIB_H
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define nextPowerOf2(size) ((size_t) pow(2, ceil(log2(size))))
//#define verbalizeState(state) ((state) == FREE ? "FREE" : ((state) == ASSIGNED ? "ASSIGNED" : ((state) == SPLIT ? "SPLIT" : "UNKNOWN")))

/**
 * Status des Knotens
 */
typedef enum {FREE, ASSIGNED, SPLIT} BuddyState;

/**
 * BuddyState zu String
 */
extern const char* BUDDY_STATE_STRING[];
#define verbalizeState(state) (BUDDY_STATE_STRING[(state)])

/**
 * Knoten des Metadatenbaums
 */
typedef struct meta_node {
    struct meta_node* parent;
    struct meta_node* children[2];
    size_t size; /** Größe dieses Knotens */
    BuddyState state;
} MetaNode;

// Öffentliche Funktionen

/**
 * Buddy-System initialisieren
 * @param data Zeiger auf das Feld, in das der Zeiger auf den Datenblock geschrieben werden soll (Basisadresse)
 * @param metaRoot Zeiger auf das Feld, in das die Wurzel des Metadatenbaums geschrieben werden soll
 * @param size Zielgröße des Datenspeichers (data). Muss eine Zweierpotenz sein.
 * @return 0 bei Erfolg, negativ bei Fehler
 */
int init_buddy(void** data, MetaNode* metaRoot, size_t size);

/**
 * Buddy-System zerstören
 * @param data Zeiger auf das Feld, in dem der Zeiger auf den Datenblock steht (Basisadresse)
 * @param metaRoot Zeiger auf das Feld, in dem die Wurzel des Metadatenbaums steht
 */
void destroy_buddy(void** data, MetaNode* metaRoot);

/**
 * Speicherbereich zuweisen
 * @param metaRoot Wurzel des zu benutzenden Metadatenbaums, darf auch ein Teilbaum sein
 * @param size Benötigte Größe, wird auf eine ganze Zweierpotenz aufgerundet
 * @return Zeiger auf zugewiesenen Metadatenknoten
 */
MetaNode* assign_buddy(MetaNode* metaRoot, size_t size);

/**
 * Speicherbereich freigeben
 * @param node Zeiger auf freizugebenden Speicherbereich.
 */
void free_buddy(MetaNode* node);

/**
 * Zeiger auf zugewiesenen Speicherbereich erhalten
 * @param data Basisadresse des Datenblocks
 * @param node Zeiger auf Metadatenknoten, der zu dem Speicherbereich gehört, der zugegriffen werden soll
 * @return Zeiger auf zugewiesenen Speicherbereich
 */
void* buddy_ptr(void* data, MetaNode* node);

// Bereits implementiert

/**
 * Belegung grafisch darstellen
 * @param root Wurzel des darzustellenden (Teil-) Baums
 */
void visualize_buddies(const MetaNode* root);

// Private Helper, müssen nicht benutzt werden

/**
 * Metadatenknoten initialisieren
 * @param meta zu initialisierender Knoten
 * @param parent einzutragender Elternknoten
 * @param size einzutragende Größe. Falls 0, wird die Hälfte der Größe des Elternknotens verwendet.
 */
void init_meta(MetaNode* meta, MetaNode* parent, size_t size);

/**
 * Metadaten(teil)baum rekursiv zerstören
 * @param metaRoot Wurzel des zu zerstörenden Metadaten-(teil-)baums
 */
void destroy_meta(MetaNode* metaRoot);

/**
 * Knoten so oft teilen, bis er auf die gegebene Größe passt.
 * @param node zu teilender Knoten
 * @param size Zielgröße, wird intern auf Zweierpotenz aufgerundet
 * @return passender Kindknoten; falls gegebener Knoten bereits passt, dieser. Falls durch Teilen kein passender Knoten erzeugt werden kann, NULL.
 */
MetaNode* splitNodeToSize(MetaNode* node, size_t size);

/**
 * Knoten mit Geschwistern rekursiv nach oben zusammenfassen, falls möglich
 * @param node Ausgangspunkt
 * @return Nach Zusammenfassung erhaltener Knoten
 */
MetaNode* tryMergeParents(MetaNode* node);

/**
 * Möglichst gut passenden, bestehenden Knoten finden
 * @param searchRoot Wurzel des zu durchsuchenden (Teil-)Baums
 * @param size Zielgröße, nach der gesucht werden soll
 * @return Bestmöglicher Knoten. Falls erfolglos, NULL
 */
MetaNode* best_fit(MetaNode* searchRoot, size_t size);

// Private Helper, bereits implementiert
/**
 * Teilbaum auf gegebener Ebene rekursiv grafisch darstellen
 * @param node Wurzel des darzustellenden Teilbaums
 * @param level Ebene, auf der sich der übergebene Knoten befindet (für Einrückung)
 */
void visualize_buddies_helper(const MetaNode* node, int level);
#endif
