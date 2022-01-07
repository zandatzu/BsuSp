/* Musterloesung zu Aufgabe 8.1: Speicherverwaltung
 * Erstellt von Florian Dittrich (florian.dittrich@uni-duesseldorf.de)
 *
 * Dies ist eine Variante der Loesung mit zwei getrennten Listen
 * fuer freie und belegte Speicherbloecke.
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

mem_heap_t *mem_init(int size) {
    if (size < sizeof(mem_block_t))
        return NULL;

    mem_block_t *firstBlock = malloc(size);
    firstBlock->next = NULL;
    firstBlock->size = size - sizeof(*firstBlock);

    mem_heap_t *heap = malloc(sizeof(*heap));
    heap->size = size;
    heap->free = firstBlock;
    heap->occupied = NULL;

    return heap;
}

void mem_cleanup(mem_heap_t* heap) {
    if (heap->occupied && heap->free > heap->occupied) {
        free(heap->occupied);
    } else {
        free(heap->free);
    }

    free(heap);
}

void* mem_alloc(mem_heap_t *heap, int size) {
    /* Die Freispeicherliste nach einem freien Speicherblock durchsuchen,
     * der fuer diese Anforderung gross genug ist.
     */
    for (mem_block_t *currentBlock = heap->free, *lastBlock = NULL;
            currentBlock != NULL; currentBlock = currentBlock->next) {
        // Falls der aktuell betrachtete Block gross genug ist
        if (currentBlock->size + sizeof(*currentBlock) >= size) {
            /* Falls noch genug Platz fuer einen Blockheader uebrig bleibt,
             * den betrachteten Block aufteilen und den verbleibenden Rest
             * in der Freispeicherliste belassen.
             */
            if (currentBlock->size + 2*sizeof(*currentBlock) > size) {
                mem_block_t* newBlock = (mem_block_t*) (currentBlock->data + size);
                newBlock->next = currentBlock->next;
                newBlock->size = currentBlock->size - sizeof(*newBlock) - size;
                currentBlock->size = size;

                if (lastBlock == NULL) {
                    heap->free = newBlock;
                } else {
                    lastBlock->next = newBlock;
                }
            /* Falls nicht mehr genug Platz ist, diesen Block komplett aus der
             * Freispeicherliste nehmen.
             */
            } else {
                if (lastBlock == NULL) {
                    heap->free = currentBlock->next;
                } else {
                    lastBlock->next = currentBlock->next;
                }
            }

            /* Den gerade allozierten in die Liste der belegten Speicherbloecke
             * einfuegen.
             */
            currentBlock->next = NULL;
            mem_block_t *lastOccupiedBlock = NULL;

            /* Die Liste der belegten Speicherbloecke durchgehen und nach der
             * passenden Lsite zum einfuegen suchen, damit alle Bloecke dort
             * in der gleichen Reihenfolge wie im Speicher liegen. Dies
             * erleichtert in mem_free das Verschmelzen von nebeneinanderliegen,
             * freien Speicherbloecken.
             */
            for (mem_block_t *occupiedBlock = heap->occupied;
                    occupiedBlock != NULL; occupiedBlock = occupiedBlock->next) {
                if (occupiedBlock > currentBlock) {
                    currentBlock->next = occupiedBlock;
                    break;
                }

                lastOccupiedBlock = occupiedBlock;
            }

            /* Falls der neue Block vor das erste Element der Liste gesetzt
             * werden soll, muss der Zeiger in der Verwaltungsdatenstruktur des
             * Heaps aktualisiert werden.
             */
            if (lastOccupiedBlock == NULL) {
                heap->occupied = currentBlock;
            } else {
                lastOccupiedBlock->next = currentBlock;
            }

            return currentBlock->data;
        }

        lastBlock = currentBlock;
    }

    return NULL;
}

void mem_free(mem_heap_t *heap, void* ptr) {
    if (ptr == NULL)
        return;

    /* Die Liste der belegten Seicherbloecken durchgehen, bis der angegebene
     * Block gefunden wurde oder nicht mehr vorkommen kann.
     */
    mem_block_t *currentBlock = heap->occupied;
    for (mem_block_t *lastBlock = NULL; ; currentBlock = currentBlock->next) {
        /* Falls der gesuchte Block nicht mehr vorkommen kann,
         * die Suche abbrechen.
         */
        if (currentBlock == NULL || currentBlock->data > (char*) ptr)
            return;

        /* Bei einem Treffer den aktuellen Block aus der Liste der belegten
         * Speicherbloecke rausnehmen und die Schleife verlassen.
         */
        if (currentBlock->data == ptr) {
            if (lastBlock == NULL) {
                heap->occupied = currentBlock->next;
            } else {
                lastBlock->next = currentBlock->next;
            }

            break;
        }

        lastBlock = currentBlock;
    }

    // Der entnommene Block wird nun in die Freispeicherliste eingefuegt
    currentBlock->next = NULL;
    mem_block_t *freeBlock = heap->free, *lastBlock = NULL;
    for (; freeBlock != NULL; freeBlock = freeBlock->next) {
        if (freeBlock > currentBlock) {
            currentBlock->next = freeBlock;
            break;
        }

        lastBlock = freeBlock;
    }

    if (lastBlock == NULL) {
        heap->free = currentBlock;
    } else {
        lastBlock->next = currentBlock;
    }

    /* Falls moeglich, verschmelze den gerade freigegebenen Speicherblock
     * mit dem vorherigen Block in der Freispeicherliste
     */
    if (lastBlock != NULL &&
            lastBlock->data + lastBlock->size == (char*) currentBlock) {
        lastBlock->size += sizeof(*currentBlock) + currentBlock->size;
        lastBlock->next = currentBlock->next;
        currentBlock = lastBlock;
    }

    /* Falls moeglich, verschmelze den gerade freigegebenen oder verschmolzenen
     * Speicherblock dem folgenden Block in der Freispeicherliste
     */
    if (currentBlock->data + currentBlock->size == (char*) currentBlock->next) {
        currentBlock->size += sizeof(*currentBlock) + currentBlock->next->size;
        currentBlock->next = currentBlock->next->next;
    }
}

void mem_dump(mem_heap_t *heap) {
    mem_block_t *freeBlock = heap->free, *occupiedBlock = heap->occupied;
    int freeSize = 0, occupiedSize = 0;
    printf("Auflistung der Speicherbloecke des Heaps mit %d Bytes Gesamtgroesse:\n",
        heap->size);

    while (freeBlock != NULL || occupiedBlock != NULL) {
        /* Der aktuelle Block in der Freispeicherliste liegt im Speicher vor
         * dem aktuell betrachteten Block in der Liste der belegten Bloecke,
         * gebe daher den Block aus der Freispeicherliste aus.
         */
        if (freeBlock != NULL &&
                (freeBlock < occupiedBlock || occupiedBlock == NULL)) {
            printf("- Freier Block:   %d Bytes\n", freeBlock->size);
            freeSize += freeBlock->size;
            freeBlock = freeBlock->next;
        } else {
            printf("- Belegter Block: %d Bytes\n", occupiedBlock->size);
            occupiedSize += occupiedBlock->size;
            occupiedBlock = occupiedBlock->next;
        }
    }

    printf("\nFreier Speicher insgesamt: %d\n", freeSize);
    printf("Belegter Speicher insgesamt: %d\n\n", occupiedSize);
}