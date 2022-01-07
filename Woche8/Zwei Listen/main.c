/* Einfaches Testprogramm zu Aufgabe 8.1: Speicherverwaltung
 * Erstellt von Florian Dittrich (florian.dittrich@uni-duesseldorf.de)
 *
 * Kompilieren mit:
 *   gcc mem_manager.c main.c -Wall -o main
 * Ausfuehren mit:
 *   ./main
 * Weitere Informationen unter
 *   http://localhost:3000/lectures/bsusp/k8a1
 */

#include <stdio.h>
#include "mem_manager.h"

int main() {
    printf("1.: Erstelle leeren Heap (1 MB)\n\n");
    mem_heap_t *heap = mem_init(1024*1024);
    mem_dump(heap);

    printf("2.: A = alloc(1 KB)\n\n");
    void* A = mem_alloc(heap, 1024);
    mem_dump(heap);

    printf("3.: B = alloc(4 KB)\n\n");
    void* B = mem_alloc(heap, 4096);
    mem_dump(heap);

    printf("4.: C = alloc(2 KB)\n\n");
    void* C = mem_alloc(heap, 2048);
    mem_dump(heap);

    printf("5.: alloc(1 MB) liefert ");
    void* D = mem_alloc(heap, 1024*1024);
    printf("%p\n\n", D);
    mem_dump(heap);

    printf("6.: free(B)\n\n");
    mem_free(heap, B);
    mem_dump(heap);

    printf("7.: free(C)  (sollte mit B verschmelzen)\n\n");
    mem_free(heap, C);
    mem_dump(heap);

    printf("8.: free(A)  (sollte zu einem einzigen Speicherblock verschmelzen)\n\n");
    mem_free(heap, A);
    mem_dump(heap);

    printf("9.: Gebe Heap wieder frei\n\n");
    mem_cleanup(heap);
}