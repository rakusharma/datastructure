#include<stdio.h>
#include<malloc.h>

struct node {
    int d;
    struct node *l;
    struct node *r;
};

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

/*queue*/

struct _item {
    void *ptr;
    int visited;
    struct _item *next;
};
//struct _item *_enqueue(void **p, struct _item **root, int visited)
void *_enqueue(void **p, struct _item **root, int visited)
{

    struct _item *x = malloc(sizeof(struct _item));
    x->ptr = *p;
    x->visited = visited;
    x->next = NULL;
    if (*root == NULL)
        *root = x;
    else {
        struct _item *itr = *root;
        while (itr->next != NULL){
            itr = itr->next;
        }
        itr->next = x;
    }
}

struct _item *_pop(struct _item **root)
{
    struct _item *t = *root;
    *root = (*root)->next;
    return t;
}


int _q_len(struct _item *queue)
{
    struct _item *i = queue;
    int n = 0;
    while (i != NULL) {
        ++n;
        i = i->next;
    }
    return n;
}
void _q_print(struct _item *queue)
{
    struct _item *itr = queue;
    while (itr != NULL) {
        printf(" [%d] ", ((struct node*)(itr->ptr))->d);
        itr = itr->next;
    }
}

#define TRUE 1
#define FALSE 0
struct tree_node {
    int level;
    struct _item *q;
    int nodes;
};

void bfs(struct node *root)
{
    int *visit[100] = {0x00};
    struct node *child[100] = {0x00};
    struct _item *queue = NULL, *item;
    struct node *cur = root;
    int i = 0, len;
    struct tree_node tn[10] = {{0, NULL}};
    int level = 0;
    int no_of_child = 0;

    _enqueue(&cur, &queue, FALSE);

    while((len = _q_len(queue)) != 0) {
        tn[level].nodes = len;
        for (int j = 0; j < len; ++j) {
            item =_pop(&queue);
            cur = (struct node*)(item->ptr);
            visit[i++] = cur;
            if (cur->l == NULL && cur->r == NULL) 
                child[no_of_child++] = cur;
            tn[level].level = level;
            _enqueue(&cur, &tn[level].q, FALSE);
            if (cur->l) _enqueue(&(cur->l), &queue, FALSE);
            if (cur->r)  _enqueue(&(cur->r), &queue, FALSE);
        }
        ++level;
    }

    for (int j = 0; j < level; ++j) {
        printf("level=> %d\n", tn[j].level);
        printf("nodes=> %d\n", tn[j].nodes);
        _q_print(tn[j].q);
    }


    printf("\n bfs-> ");
    for (int j = 0; j < i; ++j)
        printf(" %2d ", ((struct node*)visit[j])->d);
    printf("\n child-> ");
    for (int j = 0; j < no_of_child; ++j)
        printf(" %2d ", ((struct node*)child[j])->d);

    //problem = create list of elements to the node 
    //Sol> Get all the child nodes, and get the connected parent to the root

    struct _item *path[100] = {NULL};

    for (int j = 0; j < no_of_child; ++j) {
        struct node *c = child[j];
        struct node *parent;
        int total_nodes = i;
        _enqueue(&c, &path[j], FALSE);
        printf("c ->%d\n", c->d);
        struct node *child_itr = c;
        //Get parent of node till root
        do {
            for (int i = 0; i < total_nodes; ++i) {

                if (child_itr == (struct node*)visit[i])
                    continue;
                if (((struct node*)visit[i])->l == child_itr || ((struct node*)visit[i])->r == child_itr ) {
                    parent = (struct node*)visit[i];
                    _enqueue(&parent, &path[j], FALSE);
                    child_itr = parent;
                    break;
                }
            }
        } while(parent != root);
    }

    //print path
    for(int x = 0; x < no_of_child; ++x) {
        printf("path => ");
        _q_print(path[x]);
        printf("\n");
    } 
}



int main()
{
    struct node *root = NULL;

#if 0
    add_node_nr(10, &root);
    add_node_nr(6, &root);
    add_node_nr(18, &root);
    add_node_nr(4, &root);
    add_node_nr(8, &root);
    add_node_nr(15, &root);
    add_node_nr(21, &root);
    add_node_nr(22, &root);
#endif
#if 1
    add_node_nr(8, &root);
    add_node_nr(3, &root);
    add_node_nr(10, &root);
    add_node_nr(1, &root);
    add_node_nr(6, &root);
    add_node_nr(4, &root);
    add_node_nr(7, &root);
    add_node_nr(14, &root);
    add_node_nr(13, &root);
#endif
    bfs(root); 


    return 0;
}
