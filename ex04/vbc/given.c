#include <stdio.h>
#include <stdlib.h> // change this to <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

static node *parse_expr(char **s);
static node *parse_term(char **s);
static node *parse_factor(char **s);

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

//...
static node *parse_number(char **s)
{
	if (isdigit((unsigned char)(**s))) {
		node lit = { .type = VAL, .val = **s - '0', .l = NULL, .r = NULL };
		(*s)++;
		return new_node(lit);
	}
	unexpected(**s);
	return NULL;
}

static node *parse_factor(char **s)
{
	if (**s == '\0')
	{
		unexpected('\0');
		return NULL;
	}
	if (accept(s, '('))
	{
		node *inside = parse_expr(s);
		if (!inside)
			return NULL;
		if (!expect(s, ')'))
		{
			destroy_tree(inside);
			return NULL;
		}
		return inside;
	}
	return parse_number(s);
}

static node *parse_term(char **s)
{
	node *lhs = parse_factor(s);
	if (!lhs) return NULL;

	while (accept(s, '*'))
	{
		node *rhs = parse_factor(s);
		if (!rhs)
		{
			destroy_tree(lhs);
			return NULL;
		}
		node nn = { .type = MULTI, .l = lhs, .r = rhs };
		node *parent = new_node(nn);
		if (!parent)
		{
			destroy_tree(lhs);
			destroy_tree(rhs);
			return NULL;
		}
		lhs = parent;
	}
	return lhs;
}

node    *parse_expr(char **s)
{
    node *lhs = parse_term(s);
    if (!lhs) return NULL;

    while (accept(s, '+'))
    {
	    node *rhs = parse_term(s);
	    if (!rhs) {
		    destroy_tree(lhs);
		    return NULL;
	    }
	    node nn = { .type = ADD, .l = lhs, .r = rhs };
	    node *parent = new_node(nn);
	    if (!parent) {
		    destroy_tree(lhs);
		    destroy_tree(rhs);
		    return NULL;
	    }
	    lhs = parent;
    }
    return lhs;
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);

    char *p = argv[1];
    node *tree = parse_expr(&p);
    if (!tree)
        return (1);

    if (*p != '\0') {
	    unexpected(*p);
	    destroy_tree(tree);
	    return 1;
    }

    if (printf("%d\n", eval_tree(tree)) < 0)
    {
	    destroy_tree(tree);
	    return 1;
    }
    destroy_tree(tree);
    return 0;
}
