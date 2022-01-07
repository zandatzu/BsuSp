/* Musterloesung zu Aufgabe 8.1: Speicherverwaltung
 * Erstellt von Florian Dittrich (florian.dittrich@uni-duesseldorf.de)
 *
 * Dies ist eine Variante der Loesung mit einer Liste, die
 * alle Speicherbloecke enthaelt und einer separaten
 * Freispeicherliste zum schnelleren auffinden freier
 * Bloecke beim Aufruf von mem_alloc.
 *
 * Diese Datei enthaelt keine main-Funktion und kann daher
 * nicht alleine ausgefuehrt werden. Sie kann zusammen mit
 * einem der beigelieferten Testprogramme kompiliert werden.
 *
 * Weitere Informationen unter
 *   http://localhost:3000/lectures/bsusp/k8a1
 */

#include <stdlib.h>
#include <stdio.h>
#include "mem_manager.h"

mem_heap_t* mem_init(int size) {
    if (size < sizeof(mem_block_t))
        return NULL;

    mem_block_t *first = malloc(size);
    first->next = NULL;
    first->nextFree = NULL;
    first->size = size - sizeof(*first);

    mem_heap_t *heap = malloc(sizeof(*heap));
    heap->size = size;
    heap->first = first;
    heap->firstFree = first;

    return heap;
}

void* mem_alloc(mem_heap_t *heap, int size) {
    for (mem_block_t *block = heap->firstFree, *last = NULL; block != NULL;
            last = block, block = block->next)
        if (block->size >= size) {
            mem_block_t *newBlock = block->nextFree;

            // Falls sich dieser Block unterteilen lässt
            if (block->size - size > sizeof(*block)) {
                newBlock = (mem_block_t*) ((char*) (block + 1) + size);
                newBlock->next = block->next;
                newBlock->nextFree = block->nextFree;
                newBlock->size = block->size - size - sizeof(*block);

                block->next = newBlock;
                block->size = size;
            }
            block->nextFree = (void*) -1;

            // Diesen Block aus der Freispeicherliste herausnehmen
            if (last == NULL)
                heap->firstFree = newBlock;
            else
                last->nextFree = newBlock;
            return block->data;
        }
    return NULL;
}

void mem_free(mem_heap_t *heap, void* ptr) {
    for (mem_block_t *block = heap->first, *last = NULL, *lastFree = NULL;
            block != NULL; last = block, block = block->next)
        if (block->nextFree == (void*) -1) {
            if (block->data == ptr) {
                // Diesen Block in die Freispeicherliste einhängen
                if (lastFree) {
                    block->nextFree = lastFree->nextFree;
                    lastFree->nextFree = block;
                } else {
                    block->nextFree = heap->firstFree;
                    heap->firstFree = block;
                }

                // Sofern moeglich mit dem folgenden Block verschmelzen
                if (block->next != NULL && block->next == block->nextFree) {
                    block->size += sizeof(*block) + block->next->size;
                    block->next = block->next->next;
                    block->nextFree = block->nextFree->nextFree;
                }

                // Sofern moeglich mit dem vorherigen Block verschmelzen
                if (last != NULL && last == lastFree) {
                    last->size += sizeof(*block) + block->size;
                    last->next = block->next;
                    last->nextFree = block->nextFree;
                }
                
                break;
            }
        } else
            lastFree = block;
}

void mem_cleanup(mem_heap_t* heap) {
	free(heap->first);
	free(heap);
}

void mem_dump(mem_heap_t *heap) {
    printf("Speicherbloecke:\n");

    int sum = 0, sumFree = 0;
    for (mem_block_t *block = heap->first; block != NULL; block = block->next) {
        printf("  %8d Bytes %s\n", block->size, block->nextFree != (void*) -1 ?"frei":"belegt");

        if (block->nextFree != (void*) -1)
            sumFree += block->size;
        else
            sum += block->size;
    }

    printf("Speicherbelegung:\n"
           "  Gesamtgroesse: %d Bytes\n"
           "  Davon belegt:  %d Bytes\n"
           "  Davon frei:    %d Bytes\n\n", heap->size, sum, sumFree);
}