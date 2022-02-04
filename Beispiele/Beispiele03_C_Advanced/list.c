#include <stdlib.h>
#include <stdio.h>

struct element_prototype {
    int value;
    struct element_prototype * next;
};

typedef struct element_prototype element_type;

element_type *head;

void insert_element(int val) {
    element_type  *el;

    el = (element_type *) malloc(sizeof(element_type));

    if (el == NULL) {
        printf("error: malloc failed\n");
        return ;
    }

    el->value = val;
    el->next = NULL;

    if (head!=NULL) {
      el->next = head;
    }
    head = el;
}

void remove_element(int v) {
    element_type  *del  = head;
    element_type  *prev = head;

    printf("remove_element(%d)\n", v);
    
    if (head->value == v) {
        head = head->next;
        free( del );
        return ;
    }
    
    while ( prev->next != NULL) {
        if (prev->next->value == v ) {
            del = prev->next;
            prev->next = prev->next->next;
            free ( del );
            return ;
        } else {
            prev = prev->next;
        }
    }
    
    if (prev->next == NULL) {
        printf("error: remove failed\n");
        return ;
    }
}

void remove_element_v2(int v) {
    element_type  **curr  = &head;
    element_type  *entry;

    printf("remove_element_v2(%d)\n", v);
    
    while ( *curr != NULL) {
        entry = *curr;
        if (entry->value == v) {
            *curr = entry->next;
            free(entry);
            return ;
        } else {
            curr = &entry->next;
        }
    }
    
    
}
void dump_list() {
    printf("dump_list: ");
    
	element_type *pos = head;
	while (pos != NULL) {
		printf("%d", pos->value);
		pos = pos->next;
    
        if (pos!=NULL)
            printf(" -> ");
	}
    printf("\n");
}

int main() {
    insert_element(1);
    insert_element(2);
    insert_element(3);
    insert_element(4);
    insert_element(5);
    dump_list();

    remove_element_v2(5);
    dump_list();
}
