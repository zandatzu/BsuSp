/* Musterloesung zu Aufgabe 8.1: Speicherverwaltung
 * Erstellt von Florian Dittrich (florian.dittrich@uni-duesseldorf.de)
 *
 * Dies ist eine Variante der Loesung mit nur einer Liste
 * zur Verwaltung aller Speicherbloecke.
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
    first->size = size - sizeof(*first);
    first->free = 1;

    mem_heap_t *heap = malloc(sizeof(*heap));
    heap->size = size;
    heap->first = first;

    return heap;
}

void mem_cleanup(mem_heap_t* heap) {
    free(heap->first);
    free(heap);
}

void* mem_alloc(mem_heap_t *heap, int size) {
    for (mem_block_t *block = heap->first; block != NULL; block = block->next)
        if (block->free && block->size >= size) {
            // Falls sich dieser Block unterteilen lässt
            if (block->size - size > sizeof(*block)) {
                mem_block_t *newBlock = (mem_block_t*) ((char*) (block + 1) + size);
                newBlock->next = block->next;
                newBlock->size = block->size - size - sizeof(*block);
                newBlock->free = 1;

                block->next = newBlock;
                block->size = size;
            }
            block->free = 0;
            return block->data;
        }
    return NULL;
}

void mem_free(mem_heap_t *heap, void* ptr) {
    for (mem_block_t *block = heap->first, *last = NULL; block != NULL;
            last = block, block = block->next)
        if (block->data == ptr) {
            block->free = 1;

            if (block->next != NULL && block->next->free) {
                block->size += sizeof(*block) + block->next->size;
                block->next = block->next->next;
            }

            if (last != NULL && last->free) {
                last->next = block->next;
                last->size += sizeof(*block) + block->size;
            }

            break;
        }
}

void mem_dump(mem_heap_t *heap) {
    printf("Speicherbloecke:\n");

    int sum = 0, sumFree = 0;
    for (mem_block_t *block = heap->first; block != NULL; block = block->next) {
        printf("  %8d Bytes %s\n", block->size, block->free?"frei":"belegt");

        if (block->free)
            sumFree += block->size;
        else
            sum += block->size;
    }

    printf("Speicherbelegung:\n"
           "  Gesamtgroesse: %d Bytes\n"
           "  Davon belegt:  %d Bytes\n"
           "  Davon frei:    %d Bytes\n\n", heap->size, sum, sumFree);
}