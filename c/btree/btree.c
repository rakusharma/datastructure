#include<stdio.h>
#include<malloc.h>

/*DFS -> stack based*/
struct node {
    int d;
    struct node *l;
    struct node *r;
};
/*stack*/

struct item {
    void *ptr;
    struct item *next;
}item_t;

struct item *push(void **p, struct item **root)
{
    struct item *x = malloc(sizeof(struct item));
    x->ptr = *p;
    if(*root != NULL){
        x->next = *root;
        return x;
    }
    x->next = NULL;
    return x;
}


int stack_len(struct item *top)
{
    struct item *i = top;
    int n = 0;
    while (i != NULL) {
        ++n;
        i = i->next;
    }

    printf("stack len %4d\n", n);

    return n;

}
void print_stack(struct item *top)
{
    struct item *i = top;
    printf("*stack -> %p\n ", i);
    while (i != NULL) {
        printf("ptr -> %p val ->%d\n", i->ptr, ((struct node*)(i->ptr))->d);
        i = i->next;
    }
    printf("*stack ->ends \n ");
}
struct item *pop(struct item **root)
{
    struct item *t = *root;

    *root = (*root)->next;

    return t;
}


struct node *alloc(int d)
{
    struct node *t;
    t  = malloc(sizeof(struct node));
    t->d = d;
    t->l = t->r = NULL;
    return t;
}

struct node *in(int d, struct node *node)
{
    if (node == NULL)
        return alloc(d);
    else if (d > node->d) 
        node->r = in(d, node->r);
    else if (d < node->d)
        node->l = in(d, node->l); 

    return node;
}

/*in order print*/
void print_io(struct node *root)
{
    if(root == NULL) return;
    // stack = push(root, stack);
    print_io(root->l);
    printf(" :%d: ", root->d);
    print_io(root->r);
}

void *in_nr(int d, struct node **root)
{

    struct node **cur = root;

    while (1) {
        if (*cur == NULL) {*cur =  alloc(d);break;}
        if (d < (*cur)->d) 
            cur = &(*cur)->l;
        else cur = &(*cur)->r;
    }
}

void print_io_nr(struct node *root)
{
    struct node *cur = root;
    struct item *stack = NULL, *item;
    int *p[100] = {0x00};
    int i = 0;    

    while(1) {
        if (cur != NULL) {
            stack = push(&cur, &stack);
            cur = cur->l;
        } else {
            if(stack_len(stack) == 0)break;
            //pop
            item = pop(&stack);
            cur = (struct node*)(item->ptr);
            p[i++] = cur;
            cur = cur->r;
        }
    }
    for (int x = 0; x < 10; ++x) 
        p[x] != NULL ? printf(" :%d : ", ((struct node*)p[x])->d): (x = x);

}


int main()
{
    struct node *root = NULL;
    in_nr(4, &root);

    in_nr(6, &root);
    in_nr(8, &root);
    in_nr(12, &root);
    in_nr(15, &root);

    print_io(root);
    print_io_nr(root);
    //print_stack(stack);

    return 0;
}
