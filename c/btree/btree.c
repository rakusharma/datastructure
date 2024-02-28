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

struct item *pop(struct item **root)
{
    struct item *t = *root;
    *root = (*root)->next;
    return t;
}


int stack_len(struct item *top)
{
    struct item *i = top;
    int n = 0;
    while (i != NULL) {
        ++n;
        i = i->next;
    }
    //printf("stack len %4d\n", n);
    return n;
}
void print_stack(struct item *top)
{
    struct item *i = top;

    printf("*stack -> %p\n ", i);
    while (i != NULL) {
       // printf("ptr -> %p val ->%d\n", i->ptr, ((struct node*)(i->ptr))->d);
        printf("item val ->%d ", ((struct node*)(i->ptr))->d);
        i = i->next;
    }
    printf("\nstack ->endsi\n");
}


struct node *alloc(int d)
{
    struct node *t;
    t  = malloc(sizeof(struct node));
    t->d = d;
    t->l = t->r = NULL;
    return t;
}

struct node *add_node(int d, struct node *node)
{
    if (node == NULL)
        return alloc(d);
    else if (d > node->d) 
        node->r = add_node(d, node->r);
    else if (d < node->d)
        node->l = add_node(d, node->l); 
    return node;
}

/*in order print*/
void dfs_in_order(struct node *root)
{
    if(root == NULL) return;
    // stack = push(root, stack);
    dfs_in_order(root->l);
    printf(" %d ", root->d);
    dfs_in_order(root->r);
}

/*non recursive in*/
void *add_node_nr(int d, struct node **root)
{

    struct node **cur = root;

    while (1) {
        if (*cur == NULL) {*cur =  alloc(d);break;}
        if (d < (*cur)->d) 
            cur = &(*cur)->l;
        else cur = &(*cur)->r;
    }
}

void dfs_in_order_nr(struct node *root)
{
    struct node *cur = root;
    struct item *stack = NULL, *item;
    int *p[100] = {0x00};
    int i = 0;    

    printf("\ndfs in order non recursive => ");

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
        p[x] != NULL ? printf(" %d  ", ((struct node*)p[x])->d): (x = x);
}


/*
 * pushing stack and analyzing does not work here
void dfs_in_order_nr(struct node *root)
{
    int *visit[100] = {0x00};
    struct item *stack = NULL, *item;
    struct node *cur = root;
    int i = 0;

    stack = push(&cur, &stack);

    while (stack_len(stack) != 0) {
        cur = pop(&stack);
        if (cur->l == NULL && cur->r == NULL) {
            visit[i++] = cur;
        }
        if (cur->l) {
            stack = push(&cur, &stack);
            stack = push(&(cur->l), &stack);
        }
        if (cur->r) {
            stack = push(&cur, &stack);
            stack = push(&(cur->r), &stack);
        }



    }



}
*/
void dfs_pre_order_nr(struct node *root)
{
    int *visit[100] = {0x00};
    struct item *stack = NULL, *item;
    struct node *cur = root;
    int i = 0;
    stack = push(&cur, &stack);
    //visit[i++] = cur;
     //print_stack(stack);

    while(stack_len(stack) != 0) {

        item = pop(&stack);
        cur = (struct node*)(item->ptr);
        visit[i++] = cur;

        if(cur->r)stack = push(&(cur->r), &stack);
        if(cur->l)stack = push(&(cur->l), &stack);

        //print_stack(stack);
    }

    printf("\npreoder node -> ");
    for (int j = 0; j < i; ++j)
        printf(" %2d ", ((struct node*)visit[j])->d);


}

struct _item {
   void *ptr;
   int visited;
   struct _item *next;
};
struct _item *_push(void **p, struct item **root, int visited)
{
    struct _item *x = malloc(sizeof(struct item));
    x->ptr = *p;
    x->visited = visited;
    if(*root != NULL){
        x->next = *root;
        return x;
    }
    x->next = NULL;
    return x;
}

struct item *_pop(struct _item **root)
{
    struct _item *t = *root;
    *root = (*root)->next;
    return t;
}


int _stack_len(struct _item *top)
{
    struct _item *i = top;
    int n = 0;
    while (i != NULL) {
        ++n;
        i = i->next;
    }
    //printf("stack len %4d\n", n);
    return n;
}
void _print_stack(struct _item *top)
{
    struct _item *i = top;

    printf("*stack -> %p\n ", i);
    while (i != NULL) {
       // printf("ptr -> %p val ->%d\n", i->ptr, ((struct node*)(i->ptr))->d);
        printf("item val ->%d ", ((struct node*)(i->ptr))->d);
        i = i->next;
    }
    printf("\nstack ->ends\n");
}



/*1->2, 1->3  => 2, 3, 1*/

#define TRUE 1
#define FALSE 0
void dfs_post_order_nr(struct node *root)
{
    int *visit[100] = {0x00};
    struct _item *stack = NULL, *item;
    struct node *cur = root;
    int i = 0;
    stack = _push(&cur, &stack, FALSE);


    while(_stack_len(stack) != 0) {
        item =_pop(&stack);
        cur = (struct node*)(item->ptr);
        if(item->visited) {
        visit[i++] = cur;
        } 
        else {
            /*This is where we are visiting*/
            stack = _push(&cur, &stack, TRUE);

           if (cur->r)  stack = _push(&(cur->r), &stack, FALSE);
            if (cur->l) stack = _push(&(cur->l), &stack, FALSE);
        }



   }

    printf("\ndfs post order -> ");
    for (int j = 0; j < i; ++j)
        printf(" %2d ", ((struct node*)visit[j])->d);

 
 
}
void dfs_in_order_nr_v1(struct node *root)
{
    int *visit[100] = {0x00};
    struct _item *stack = NULL, *item;
    struct node *cur = root;
    int i = 0;
    stack = _push(&cur, &stack, FALSE);


    while(_stack_len(stack) != 0) {
        item =_pop(&stack);
        cur = (struct node*)(item->ptr);
        if(item->visited) {
        visit[i++] = cur;
        } 
        else {

           if (cur->r)  stack = _push(&(cur->r), &stack, FALSE);
            stack = _push(&cur, &stack, TRUE);
            if (cur->l) stack = _push(&(cur->l), &stack, FALSE);
        }



   }

    printf("\ndfs in order v1-> ");
    for (int j = 0; j < i; ++j)
        printf(" %2d ", ((struct node*)visit[j])->d);

 
 
}

void dfs_pre_order_nr_v1(struct node *root)
{
    int *visit[100] = {0x00};
    struct _item *stack = NULL, *item;
    struct node *cur = root;
    int i = 0;
    stack = _push(&cur, &stack, FALSE);


    while(_stack_len(stack) != 0) {
        item =_pop(&stack);
        cur = (struct node*)(item->ptr);
        if(item->visited) {
        visit[i++] = cur;
        } 
        else {

           if (cur->r)  stack = _push(&(cur->r), &stack, FALSE);
            if (cur->l) stack = _push(&(cur->l), &stack, FALSE);
            stack = _push(&cur, &stack, TRUE);
        }



   }

    printf("\ndfs in order v1-> ");
    for (int j = 0; j < i; ++j)
        printf(" %2d ", ((struct node*)visit[j])->d);

 
 
}



int main()
{
    struct node *root = NULL;

    add_node_nr(8, &root);
    add_node_nr(3, &root);
    add_node_nr(10, &root);
    add_node_nr(1, &root);
    add_node_nr(6, &root);
    add_node_nr(4, &root);
    add_node_nr(7, &root);
    add_node_nr(14, &root);
    add_node_nr(13, &root);

    printf("dfs in order recursive  => ");
    dfs_in_order(root);
    dfs_in_order_nr(root);
    dfs_in_order_nr_v1(root);

    dfs_pre_order_nr(root);
    dfs_pre_order_nr_v1(root);
    
    dfs_post_order_nr(root);
    
    //print_stack(stack);

    return 0;
}
