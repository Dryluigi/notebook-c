#include <stdio.h>
#include <stdlib.h>
#define Q_MAX 20

typedef struct{
    int id_note, tags_len;
    char title[50];
    char content[200];
    char tags[20][20];
}Note;

typedef struct Simpul Node;
struct Simpul{
    Note note;
    Node *next, *prev;
};

typedef struct{
    Note note[Q_MAX];
    int head, tail, len, isFull, isEmpty;
}Queue;

void menu();
void menu_show_note();
void menu_delete();
void create_note();
void show_note_id();
void show_all_note();
void show_note(Note);
void search_note_tag();
void delete_note_id();
void delete_all();
void delete_note(int);
void create_node(Note);
void insert_node(Note);
Note *search_note_id(int);
int compr_str(char *, char *);
void enqueue(Queue *, Note *);
Note dequeue(Queue *);

int id = 0;
Node *head = NULL, *tail = NULL, *p = NULL;

int main()
{
    int choice;

    while(1){
        menu();
        scanf("%d", &choice);
        switch(choice){
            case 1: create_note(); break;
            case 2:
                if(head != NULL)
                    menu_show_note();
                else
                    puts("\nNote book masih kosong!\n");
                break;
            case 3:
                if(head != NULL)
                    search_note_tag();
                else
                    puts("\nNote book masih kosong!\n");
                break;
            case 4:
                if(head != NULL)
                    menu_delete();
                else
                    puts("\nNote book masih kosong!\n");
                break;
            case 5: exit(0); break;
            default:
                puts("\nInput salah!\n");
                break;
        }
    }
    return 0;
}

void menu(){
    puts("========================");
    puts("Menu Notebook");
    puts("========================");
    puts("1.Buat note baru");
    puts("2.Lihat note");
    puts("3.Cari note dengan tag");
    puts("4.Hapus note");
    puts("5.Exit");
    printf("Input pilihan anda : ");
}

void menu_show_note(){
    int choice;

    puts("========================");
    puts("Menu Tampilkan Note");
    puts("========================");
    puts("1.Tampilkan semua notes");
    puts("2.Tampilkan note berdasarkan ID");
    puts("3.Cancel");
    printf("Input pilihan anda : ");
    scanf("%d", &choice);
    switch(choice){
        case 1: show_all_note(); break;
        case 2: show_note_id(); break;
        case 3: break;
        default:
            puts("\nInput salah!\n");
            menu_show_note();
            break;
    }
}

void menu_delete(){
    int choice;

    puts("========================");
    puts("Menu Delete Note");
    puts("========================");
    puts("1.Delete berdasarkan ID");
    puts("2.Delete semua notes");
    puts("3.Cancel");
    printf("Input pilihan anda : ");
    scanf("%d", &choice);
    switch(choice){
        case 1: delete_note_id(); break;
        case 2: delete_all(); break;
        case 3: break;
        default:
            puts("\nInput salah!\n");
            menu_delete();
            break;
    }
}

void create_note(){
    char choice;
    int i = 0;
    Note temp;

    fflush(stdin);
    printf("Masukkan judul note : ");
    gets(temp.title);
    puts("Masukkan isi note : (Gunakan ';' untuk mengakhiri isi note)");
    scanf("%[^;]s", temp.content);
    fflush(stdin);
    printf("Ingin menambahkan tag ? (y/n) : ");
    choice = getchar();
    fflush(stdin);
    if(choice == 'Y' || choice == 'y'){
        while(choice == 'Y' || choice == 'y'){
            printf("Masukkan tag : ");
            gets(temp.tags[i++]);
            printf("Ingin menambahkan tag lagi ? (y/n) : ");
            fflush(stdin);
            choice = getchar();
            fflush(stdin);
        }
    }
    temp.id_note = id++;
    temp.tags_len = i;
    insert_node(temp);
}

void show_note_id(){
    int id;
    Note *nt;

    printf("Masukkan ID note : ");
    scanf("%d", &id);
    nt = search_note_id(id);
    if(nt != NULL){
        puts("========================");
        puts("Note");
        puts("========================");
        show_note(*nt);
        puts("\n========================");
    }
}

void show_all_note(){
    Node *point;

    point = head;
    puts("========================");
    puts("List Note");
    puts("========================");
    do{
        show_note(point->note);
        puts("\n========================");
        point = point->next;
    }while(point != NULL);
}

void show_note(Note nt){
    int i;

    printf("Note ID : %d\n", nt.id_note);
    printf("> %s <\n", nt.title);
    printf("%s\n", nt.content);
    printf("\nTags : ");
    if(nt.tags_len > 0){
        for(i=0; i<nt.tags_len; i++)
            printf("#%s ", nt.tags[i]);
    } else {
        printf("-");
    }
}

void search_note_tag(){
    char tag[20];
    Queue tag_q = { .head = 0, .tail = 0, .len = 0, .isFull = 0, .isEmpty = 1 };
    Node *point;
    int i, n;

    printf("\nMasukkan tag yang ingin dicari : ");
    fflush(stdin);
    gets(tag);
    fflush(stdin);
    point = head;
    while(point != NULL){
        for(i=0; i<point->note.tags_len; i++){
            if(compr_str(tag, point->note.tags[i]) == 1){
                enqueue(&tag_q, &(point->note));
                break;
            }
        }
        point = point->next;
    }
    if(tag_q.isEmpty != 1){
        puts("========================");
        puts("List Notebook yang Sesuai");
        puts("========================");
        n = tag_q.len;
        for(i=0; i<n; i++){
            show_note(dequeue(&tag_q));
            puts("\n========================");
        }
    } else {
        puts("\nTidak didapat hasil yang cocok\n");
    }
}

void delete_note_id(){
    int id;
    Note *nt;

    printf("Masukkan ID note : ");
    scanf("%d", &id);
    nt = search_note_id(id);
    if(nt != NULL){
        delete_note(id);
        puts("\nDelete berhasil!\n");
    } else {
        puts("\nDelete gagal!\n");
    }
}

void delete_all(){
    while(head != NULL){
        delete_note(tail->note.id_note);
    }
    puts("\nDelete berhasil!\n");
}

void delete_note(int id){
    Node *point;

    point = head;
    if(point->next == NULL && point->note.id_note == id){
        free(point);
        head = NULL;
        tail = NULL;
    } else {
        while(point->next != NULL && point->note.id_note != id)
            point = point->next;
        if(head->note.id_note == id){
            head = head->next;
            head->prev = NULL;
            free(point);
        }
        else if(point->next == NULL){
            tail = tail->prev;
            tail->next = NULL;
            free(point);
        } else {
            point->prev->next = point->next;
            point->next->prev = point->prev;
            free(point);
        }
    }
}

void create_node(Note nt){
    p = (Node *)malloc(sizeof(Node));
    if(p == NULL){
        puts("\nMemory error!\n");
        exit(0);
    }
    p->note = nt;
    p->next = NULL;
    p->prev = NULL;
}

void insert_node(Note nt){
    create_node(nt);
    if(head == NULL){
        head = p;
        tail = p;
    } else {
        p->prev = tail;
        tail->next = p;
        tail = p;
    }
}

Note *search_note_id(int id){
    Node *point;

    point = head;
    while(point->next != NULL && point->note.id_note != id)
        point = point->next;
    if(point->note.id_note == id){
        return &(point->note);
    } else {
        puts("\nID tidak ditemukan!\n");
        return NULL;
    }
}

int compr_str(char *a, char *b){
    while(*a != '\0' && *b != '\0'){
        if(*a >= 65 && *a <= 90)
            *a += 32;
        if(*b >= 65 && *b <= 90)
            *b += 32;
        if(*a == *b){
            a++;
            b++;
        } else {
            return 0;
        }
    }
    if(*a == '\0' && *b != '\0')
        return 0;
    if(*b == '\0' && *a != '\0')
        return 0;
    return 1;
}

void enqueue(Queue *q, Note *nt){
    if(q->isFull == 0){
        q->note[q->tail++] = *nt;
        q->isEmpty = 0;
        q->len++;
        q->tail %= Q_MAX;
        if(q->head == q->tail)
            q->isFull = 1;
    } else {
        puts("\nQueue penuh!\n");
    }
}

Note dequeue(Queue *q){
    Note temp;
    if(q->isEmpty == 0){
        temp = q->note[q->head++];
        q->isFull = 0;
        q->len--;
        q->head %= Q_MAX;
        if(q->head > q->tail){
            q->isEmpty = 1;
            q->head--;
        }
    } else {
        puts("\nQueue kosong!\n");
    }
    return temp;
}
