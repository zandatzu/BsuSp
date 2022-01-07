/* malloc-Wrapper zu Aufgabe 8.1: Speicherverwaltung
 * Erstellt von Florian Dittrich (florian.dittrich@uni-duesseldorf.de)
 *
 * Einbinden mit
 *   #include "mem_test.h"
 * Verwendung:
 *   Das Einbinden dieser Datei startet eine Ueberwachung aller
 *   malloc/callc/realloc/free-Aufrufe im Programm. Die Zugriffe
 *   koennen auf der Konsole ausgegeben werden, wenn mit
 *   -DWRAPPER_DUMP kompiliert wird. Mit der Funktion wrapper_dump
 *   lassen sich ausserdem alle aktuelle allozierten Bloecke
 *   mit Speicheradresse und Groesse auf der Konsole ausgeben.
 * Weitere Informationen unter
 *   http://localhost:3000/lectures/bsusp/k8a1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#ifndef ALLOC_WRAPPER_LIB
#define ALLOC_WRAPPER_LIB
#ifdef malloc
#error "Es ist bereits ein Wrapper für 'malloc' vorhanden."
#endif
#ifdef free
#error "Es ist bereits ein Wrapper für 'free' vorhanden."
#endif
#ifdef calloc
#error "Es ist bereits ein Wrapper für 'calloc' vorhanden."
#endif
#ifdef realloc
#error "Es ist bereits ein Wrapper für 'realloc' vorhanden."
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#define C_DROT  "\e[0;31m"
#define C_DBLAU "\e[0;34m"
#define C_DLILA "\e[0;35m"
#define C_DCYAN "\e[0;36m"
#define C_HGRAU "\e[0;37m"
#define C_DGRAU "\e[1;30m"
#define C_HROT  "\e[1;31m"
#define C_HGRUN "\e[1;32m"
#define C_HBLAU "\e[1;34m"
#define C_HLILA "\e[1;35m"
#define C_HCYAN "\e[1;36m"
#define C_WEISS "\e[1;37m"
#define C_RESET "\e[0m"
#define macro_print(clr_sig) \
	C_WEISS "[" clr_sig C_WEISS "] "
#define print_dump(...) \
	printf( macro_print(C_HCYAN "DUMP") __VA_ARGS__)
#define print_fail(...) \
	printf( macro_print(C_HROT  "FAIL") __VA_ARGS__)
#define print_info(...) \
	printf( macro_print(C_HBLAU "INFO") __VA_ARGS__)
#define print_succ(...) \
	printf( macro_print(C_HGRUN "SUCC") __VA_ARGS__)
#define print_wrap(...) \
	printf( macro_print(C_HLILA "WRAP") __VA_ARGS__)
#define print_space(...) \
	printf("       " __VA_ARGS__);
#define print_aufruf(func) \
	(print_info(C_HGRAU "Aufruf von " C_HBLAU "'" #func "'" C_RESET "\n"), func)

typedef struct wrapper_node {
	struct wrapper_node *next;
	void *ptr;
	char *file;
	size_t size;
	int line;
} wrapper_node;

wrapper_node *wrapper_nodes;

void *wrapper_eintragen(int line, char* file, void *ptr, size_t size) {
	wrapper_node *node = malloc(sizeof(*node));
	node->ptr = ptr;
	node->size = size;
	node->line = line;
	node->file = file;
	node->next = wrapper_nodes;
	wrapper_nodes = node;

	#if WRAPPER_DUMP
	print_wrap(C_HGRAU "Speicher angefordert: " C_DLILA "%zu" C_DGRAU " Bytes in " C_DLILA "%s:%d" C_RESET "\n",
			size, file, line);
	#endif
	return ptr;
}

void *wrapper_austragen(int line, char *file, void* ptr) {
	wrapper_node *node = wrapper_nodes, *prev = NULL;
	for (; node; prev = node, node = node->next)
		if (node->ptr == ptr)
			goto weiter;
	print_fail(C_HGRAU "Freizugebender Speicher konnte nicht gefunden werden\n" C_RESET);
	exit(EXIT_FAILURE);
weiter:
	if (prev)
		prev->next = node->next;
	else
		wrapper_nodes = node->next;

	#if WRAPPER_DUMP
	print_wrap(C_HGRAU "Speicher freigegeben: " C_DLILA "%zu" C_DGRAU " Bytes in " C_DLILA "%s:%d" C_DGRAU
			" (alloziert in " C_DLILA "%s:%d" C_DGRAU ")" C_RESET "\n", node->size, file, line, node->file, node->line);
	#endif
	free(node);
	return ptr;
}

int wrapper_dump() {
	#if WRAPPER_DUMP
	if (wrapper_nodes) {
		print_dump(C_HGRAU "Folgende Speicherbereiche sind aktuell mit " C_DCYAN "malloc" C_HGRAU " alloziert:\n" C_RESET);
		for (wrapper_node *node = wrapper_nodes; node; node = node->next)
			print_space(C_DCYAN "  0x%016" PRIXPTR C_DGRAU " (" C_DCYAN "%10zu" C_DGRAU " Bytes) alloziert in " C_DCYAN "%s:%d" C_RESET "\n",
			            (uintptr_t) node->ptr, node->size, node->file, node->line);
		return 1;
	}
	print_dump(C_HGRAU "Es sind aktuell keine Speicherbereiche mit " C_DCYAN "malloc" C_HGRAU " alloziert" C_RESET "\n");
	return 0;
	#else
	return wrapper_nodes != NULL;
	#endif
}

void *malloc_wrap(int line, char* file, size_t size) {
	return wrapper_eintragen(line, file, malloc(size), size);
}

void free_wrap(int line, char* file, void *ptr) {
	return free(wrapper_austragen(line, file, ptr));
}

void *calloc_wrap(int line, char* file, size_t nmemb, size_t size) {
	return wrapper_eintragen(line, file, calloc(nmemb, size), size);
}

void *realloc_wrap(int line, char* file, void *ptr, size_t size) {
	return wrapper_eintragen(line, file, realloc(wrapper_austragen(line, file, ptr), size), size);
}

#pragma GCC diagnostic pop
#define malloc(size) \
	malloc_wrap(__LINE__, __FILE__, size)
#define free(ptr) \
	free_wrap(__LINE__, __FILE__, ptr)
#define calloc(nmemb, size) \
      	calloc_wrap(__LINE__, __FILE__, nmemb, size)
#define realloc(ptr, size) \
       	realloc_wrap(__LINE__, __FILE__, ptr, size)
#endif