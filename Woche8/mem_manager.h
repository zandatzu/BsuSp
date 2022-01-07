/* Datenstruktur, in der die Verwaltungsdaten des Heaps gespeichert werden,
 * beispielsweise die Groesse des Heaps, Zeiger auf die Liste der Speicherbloecke
 * bzw. auf die Freispeicherliste und auf die der bereits belegten Speicherbloecke.
 */
typedef struct {
    // Hier Code einfuegen
    struct mem_heap_t *next;
    char data[];
    int free;

} mem_heap_t;

/* Heap anlegen und initialisieren
 * -------------------------------
 * Parameter:
 *   size: Groesse des Speichers, der fuer den Heap belegt werden soll und dann
 *         zur Allokation von allozierte Speicherbloecken zur Verfuegung steht
 * Rueckgabewert:
 *   Zeiger auf eine Datenstruktur, die den Heap verwaltet und die allen anderen
 *   Funktionen uebergeben werden muss.
 */
mem_heap_t* mem_init(int size);

/* Heap freigeben
 * --------------
 * Parameter:
 *   heap: Zeiger auf die Verwaltungsdaten des Heaps, der freigegeben werden soll
 * Anmerkungen:
 *   Diese Funktion gibt allen Speicher wieder frei, der von ff_init belegt wurde.
 */
void mem_cleanup(mem_heap_t* heap);

/* Speicherblock allozieren
 * --------------------------
 * Parameter:
 *   heap: Zeiger auf die Verwaltungsdaten des Heaps, auf dem der Speicherblock
 *         alloziert werden soll
 *   size: Groesse des nutzbaren Speichers, der alloziert werden soll
 *         (also ohne den Header)
 * Rueckgabewert:
 *   Ein Zeiger auf einen mindestens size Bytes grossen Speicherblock
 *   oder NULL, falls kein Speicherblock alloziert werden konnte.
 */
void* mem_alloc(mem_heap_t *heap, int size);

/* Speicherblock freigeben
 * -------------------------
 * Parameter:
 *   heap: Zeiger auf die Verwaltungsdaten des Heaps, auf dem der freizugebende
 *         Speicherblock liegt
 *   ptr:  Zeiger auf den Speicherblock, der freigegeben werden soll
 * Anmerkungen:
 *   Falls NULL als ptr uebergeben wird, geschieht nichts.
 */
void mem_free(mem_heap_t *heap, void* ptr);

/* Alle Speicherbloecke auflisten
 * -------------------------------
 * Parameter:
 *   heap:  Der Heap, dessen Belegung ausgegeben werden soll
 * Anmerkungen:
 *   Ausgegeben wird eine Liste mit allen freien und belegten Speicherbloecken sowie
 *   deren Groesse und die Menge des bereits belegten und des noch freien Speichers.
 */
void mem_dump(mem_heap_t *heap);