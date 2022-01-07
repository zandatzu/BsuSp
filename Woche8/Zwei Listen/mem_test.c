/* Erweitertes Testprogramm zu Aufgabe 8.1: Speicherverwaltung
 * Erstellt von Florian Dittrich (florian.dittrich@uni-duesseldorf.de)
 *
 * Die ueber den Parameter MEM_MANAGER_FILE angegebene Datei
 * wird automatisch eingebunden und darf nicht im gcc-Aufruf
 * mit angegeben werden. Ist der Parameter nicht angegeben,
 * wird als Dateiname "mem_manager.c" angenommen.
 *
 * Kompilieren mit:
 *   gcc mem_test.c -Wall -o test
 * Kompilieren (mit Debug-Ausgaben) mit:
 *   gcc mem_test.c -Wall -o mem_test -DWRAPPER_DUMP
 * Ausfuehren mit:
 *   ./mem_test
 * Weitere Informationen unter
 *   http://localhost:3000/lectures/bsusp/k8a1
 */

#include <signal.h>
#include "mem_test.h"

#if WRAPPER_DUMP
#  undef  WRAPPER_DUMP
#  define WRAPPER_DUMP() wrapper_dump()
#  define TEST_DUMP()    test_dump(block)
#else
#  define WRAPPER_DUMP() ((void)0)
#  define TEST_DUMP()    ((void)0)
#endif
#ifndef MEM_MANAGER_FILE
#  define MEM_MANAGER_FILE "mem_manager.c"
#endif
#ifdef __has_include
#  if __has_include(MEM_MANAGER_FILE)
#    include MEM_MANAGER_FILE
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#define BLOCK_LEN 4
#define A block[0].ptr
#define B block[1].ptr
#define C block[2].ptr
#define D block[3].ptr

typedef struct {
	void *ptr;
	char *chr;
	size_t size;
	char wert;
} mem_block;

void sigsev_action(int signum, siginfo_t *siginfo, void *context) {
	if (signum == SIGSEGV) {
		print_fail(C_HGRAU "Speicherzugriffsfehler beim Zugriff auf Adresse " C_DROT "0x%016" PRIXPTR C_RESET "\n",
				(uintptr_t) siginfo->si_addr);
		exit(EXIT_FAILURE);
	}
}

void test_malloc(mem_block block[]) {
	wrapper_node *found = NULL;
	for (int i = 0; i < BLOCK_LEN; i++) {
		if (!block[i].ptr)
			continue;
		for (wrapper_node *node = wrapper_nodes; node; node = node->next)
			if ((uintptr_t) block[i].ptr >= (uintptr_t) node->ptr && (uintptr_t) block[i].ptr + block[i].size < (uintptr_t) node->ptr + node->size) {
				if (found == NULL)
					found = node;
				else if (found != node) {
					print_fail(C_HGRAU "Alle von mem_alloc allozierten Bloecke muessen im gleichen malloc-Block liegen" C_RESET "\n");
					exit(EXIT_FAILURE);
				}
				goto weiter;
			}
		print_fail(C_HGRAU "Die Allokation %s befindet sich nicht in einem mit malloc allozierten Speicherbereich" C_RESET "\n", block[i].chr);
		exit(EXIT_FAILURE);
	weiter:
		continue;
	}
	if (found)
		print_succ(C_HGRAU "Alle Allokationen befinden sich im Block " C_DGRAU "0x%016" PRIXPTR C_DGRAU " (" C_DCYAN "%zu" C_DGRAU " Bytes)" C_RESET "\n",
		           found->ptr, found->size);
}

void test_fuellen(mem_block *block) {
	print_info(C_HGRAU "Fuelle Block %s mit Daten\n" C_RESET, block->chr);
	memset(block->ptr, block->wert, block->size);
}

void test_integrity(mem_block *block) {
	for (int i = 0; i < BLOCK_LEN; i++) {
		if (!block[i].ptr)
			continue;
		char* pos = (char*) block[i].ptr;
		for (unsigned j = 0; j < block[i].size; j++)
			if (*(pos++) != block[i].wert) {
				print_fail(C_HGRAU "Daten in Allokation %s wurden ueberschrieben (Position %u/%zu)" C_RESET "\n",
				           block[i].chr, j, block[i].size);
				exit(EXIT_FAILURE);
			}
	}
	print_succ(C_HGRAU "Die Daten in allen Allokationen wurden nicht veraendert" C_RESET "\n");
}

void test_disjunkt(mem_block block[]) {
	for (int i = 0; i < BLOCK_LEN; i++)
		for (int j = i+1; j < BLOCK_LEN; j++)
			if (block[i].ptr && (uintptr_t) block[i].ptr + block[i].size >= (uintptr_t) block[j].ptr
			    && (uintptr_t) block[i].ptr < (uintptr_t) block[j].ptr + block[j].size) {
				print_fail(C_HGRAU "Allokationen %s und %s ueberschneiden sich" C_RESET "\n", block[i].chr, block[j].chr);
				exit(EXIT_FAILURE);
			}
	print_succ(C_HGRAU "Allokationen sind alle disjunkt" C_RESET "\n");
}

void test_dump(mem_block *block) {
	for (int i = 0; i < BLOCK_LEN; i++) {
		if (block[i].ptr)
			goto weiter;
	}
	print_dump(C_HGRAU "Es sind aktuell Speicherbereiche mit " C_DCYAN "mem_alloc" C_HGRAU " alloziert" C_RESET "\n");
	return;
weiter:
	print_dump(C_HGRAU "Folgende Speicherbereiche sind aktuell mit " C_DCYAN "mem_alloc" C_HGRAU " alloziert:\n" C_RESET);

	for (int i = 0; i < BLOCK_LEN; i++) {
		if (!block[i].ptr)
			continue;
		print_space(C_DCYAN "  %s" C_HGRAU " = " C_DCYAN "0x%016" PRIXPTR C_DGRAU " (" C_DCYAN "%10zu" C_DGRAU " Bytes)" C_RESET "\n",
			            block[i].chr, block[i].ptr, block[i].size);
	}
}

#define TEST_NULL(ptr) \
	if (ptr) { \
		print_fail(C_HGRAU "Die Allokation von " #ptr " haette fehlschlagen sollen" C_RESET "\n"); \
		exit(EXIT_FAILURE); \
	} else \
		print_succ(C_HGRAU "Allokation erfolgreich abgelehnt" C_RESET "\n");
#define TEST_NNULL(ptr) \
	if (ptr) \
		print_succ(C_HGRAU "Allokation erfolgreich" C_RESET "\n"); \
	else { \
		print_fail(C_HGRAU "Allokation fehlgeschlagen" C_RESET "\n"); \
		exit(EXIT_FAILURE); \
	}
#pragma GCC diagnostic pop

int main() {
	mem_heap_t *heap;
	mem_block block[BLOCK_LEN] = {
		{.size = 1024, .chr = "A", .wert = 42},
		{.size = 2048, .chr = "B", .wert = 63},
		{.size = 2048, .chr = "C", .wert = 87},
		{.size = 3096, .chr = "D", .wert = 91}
	};

	struct sigaction act = {0};
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SA_SIGINFO);
	sigaddset(&act.sa_mask, SA_RESTART);
	act.sa_sigaction = &sigsev_action;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &act, NULL);
	
	print_aufruf(heap = mem_init(4096));
	if (heap)
		print_succ(C_HGRAU "Heap erfolgreiche erstellt" C_RESET "\n");
	else {
		print_fail(C_HGRAU "Heap konnte nicht erstellt werden" C_RESET "\n");
		exit(EXIT_FAILURE);
	}
	WRAPPER_DUMP();
	TEST_DUMP();

	print_aufruf(A = mem_alloc(heap, 1024));
	TEST_NNULL(A);
	WRAPPER_DUMP();
	TEST_DUMP();
	test_malloc(block);
	test_fuellen(block + 0);

	print_aufruf(B = mem_alloc(heap, 2048));
	TEST_NNULL(B);
	WRAPPER_DUMP();
	TEST_DUMP();
	test_disjunkt(block);
	test_malloc(block);
	test_fuellen(block + 1);
	test_integrity(block);

	print_aufruf(C = mem_alloc(heap, 2048));
	TEST_NULL(C);
	WRAPPER_DUMP();
	TEST_DUMP();
	test_disjunkt(block);
	test_malloc(block);
	test_integrity(block);

	print_aufruf(mem_free(heap, B));
	B = NULL;
	WRAPPER_DUMP();
	TEST_DUMP();
	test_integrity(block);

	print_aufruf(B = mem_alloc(heap, 2048));
	TEST_NNULL(B);
	WRAPPER_DUMP();
	TEST_DUMP();
	test_disjunkt(block);
	test_malloc(block);
	test_fuellen(block + 1);
	test_integrity(block);

	print_aufruf(mem_free(heap, B));
	B = NULL;
	WRAPPER_DUMP();
	TEST_DUMP();
	test_integrity(block);

	print_aufruf(mem_free(heap, A));
	A = NULL;
	WRAPPER_DUMP();
	TEST_DUMP();
	test_integrity(block);

	print_aufruf(D = mem_alloc(heap, 3096));
	TEST_NNULL(D);
	WRAPPER_DUMP();
	TEST_DUMP();
	test_disjunkt(block);
	test_malloc(block);
	test_fuellen(block + 3);
	test_integrity(block);

	print_aufruf(mem_free(heap, D));
	D = NULL;
	WRAPPER_DUMP();
	TEST_DUMP();
	test_integrity(block);

	print_aufruf(mem_free(heap, C));
	C = NULL;
	WRAPPER_DUMP();
	TEST_DUMP();
	
	print_aufruf(mem_cleanup(heap));
	if (wrapper_dump()) {
		print_fail(C_HGRAU "Einige allozierte Bloecke wurden nicht wieder freigegeben" C_RESET "\n");
		exit(EXIT_FAILURE);
	} else
		print_succ(C_HGRAU "Alle allozierten Bloecke wurden wieder freigegeben" C_RESET "\n");
}

#  else
#    error "Erstellen Sie fuer diese Aufgabe Ihr Programm in einer Datei 'mem_manager.c' im gleichen Ordner wie diese Datei."
#  endif
#endif