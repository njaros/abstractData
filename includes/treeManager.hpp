#ifndef TREE_MANAGER
# define TREE_MANAGER

# include "utility.hpp"
# include "vector.hpp"
# include "exception.hpp"
# include "functional.hpp"
# include "algorithm.hpp"
# include <cstdlib>
# include <memory>
# include <iostream>
# include <string>
# define BLACK      false
# define RED        true
# define NO_FATHER  0
# define NO_GRANDFATHER 0
# define NO_BROTHER 0
# define NO_UNCLE   0

/* This library is not a class, this is just the necessaries tools to build
 * a class with "red black tree".
 * In my tree, a leaf doesn't contain an inserted element. Here a leaf is here to delimit my tree for use of iterators and reverse iterators
 * At this moment, this library is only compatible with ft::set and ft::map from the 42 project "containers" because of the pairs and iterators,
 * I'll rebuild it generically in the future.
 *
 * Note : Trees are complicated, red black implement is complicated, templates are complicated
 *        templated red black tree is complicated ^ 3
 *
 * SOURCE : https://fr.wikipedia.org/wiki/Arbre_bicolore
 *          https://miashs-www.u-ga.fr/prevert/Prog/Java/CoursJava/arbresRougeNoir.html
 *          a bit from my brain
 *          Thanks to them. */

    template < typename T >
    struct node
    {
        node                *father;
        node                *left;
        node                *right;
        bool                color;
        T                   *content;
    };

//MEMORY

template < typename T, typename allocNode, typename allocValueType >
node<T> *doALeaf(T value, allocNode &alloc, allocValueType &allocT)
{
    node<T> *leaf = alloc.allocate(1);
    leaf->color = BLACK;
    leaf->right = 0;
    leaf->left = 0;
    leaf->father = 0;
    leaf->content = allocT.allocate(1);
    allocT.construct(leaf->content, value);
    return (leaf);
}

template < typename T, typename allocNode, typename allocValueType >
node<T> *newNode(const T &content, allocNode &alloc, allocValueType &allocT)
{
    node<T> *newNode = alloc.allocate(1);
    newNode->color = RED;
    newNode->father = NO_FATHER;
    newNode->right = 0;
    newNode->left = 0;
    newNode->content = allocT.allocate(1);
    allocT.construct(newNode->content, content);
    return (newNode);
}

template < typename T, typename allocNode, typename allocValueType >
void destroyNode(node<T> *n, allocNode &alloc, allocValueType &allocT)
{
    allocT.destroy(n->content);
    allocT.deallocate(n->content, 1);
    alloc.deallocate(n, 1);
}

//GENEALOGY

template < typename T >
bool isNotLeaf(const node<T> *n)
{
    if (!n)
        throw (ft::invalid_argument("isLeaf/isNotLeaf : Null ptr isn't allowed\n"));
    return (n->right);
}

template < typename T >
bool isLeaf(const node<T> *n)
{
	if (!n)
		throw (ft::invalid_argument("isLeaf/isNotLeaf : Null ptr isn't allowed\n"));
    return (!n->right);
}

template < typename T >
node<T>    *father(const node<T> *n)    {return (n->father);}

template < typename T >
node<T>    *grandfather(const node<T> *n)
{
    node<T>    *f = father(n);
    if (f == NO_FATHER)
        return NO_GRANDFATHER;
    return father(f);
}

template < typename  T >
node<T>    *brother(node<T> *n)
{
    node<T>    *f = father(n);

    if (f == NO_FATHER)
        return NO_BROTHER;
    if (n == f->left)
        return f->right;
    return f->left;
}

template < typename  T >
node<T>    *uncle(node<T> *n)
{
    node<T>    *gf = grandfather(n);

    if (gf == NO_GRANDFATHER)
        return NO_UNCLE;
    return brother(father(n));
}

//TREE ROTATIONS (Here is 2 tree modifiers which don't break the tree sort)

template < typename T >
void    leftRotation(node<T> *x, node<T> **root)
{
    node<T>    *y = x->right;

    x->right = y->left;
    if (isNotLeaf(y->left))
        y->left->father = x;
    y->father = x->father;
    if (y->father == NO_FATHER)
        *root = y;
    else
        if (x == x->father->left)
            x->father->left = y;
        else
            x->father->right = y;
    y->left = x;
    x->father = y;
}

template < typename T >
void    rightRotation(node<T> *x, node<T> **root)
{
    node<T>    *y = x->left;

    x->left = y->right;
    if (isNotLeaf(y->right))
        y->right->father = x;
    y->father = x->father;
    if (y->father == NO_FATHER)
        *root = y;
    else
        if (x == x->father->right)
            x->father->right = y;
        else
            x->father->left = y;
    y->right = x;
    x->father = y;
}

//NODE INSERTION

template < typename T, typename Compare >
ft::pair< node<T>*, bool >    recursiveInsert(node<T> *root, node<T> *n, Compare compare, bool onlyLeft, bool onlyRight)
{
    bool    nIsHigher = compare(root->content->first, n->content->first);
    if (!nIsHigher) {
        if (isNotLeaf(root->left))
            return recursiveInsert<T, Compare>(root->left, n, compare, onlyLeft, false);
    }
    else if (isNotLeaf( root->right))
		return recursiveInsert<T, Compare>(root->right, n, compare, false, onlyRight);
    if (nIsHigher) {
        node<T> *leaf = root->right;
        root->right = n;
        n->right = leaf;
        n->left = leaf;
        if (onlyRight)
			leaf->father = n;
    }
    else {
        node<T> *leaf = root->left;
        root->left = n;
        n->right = leaf;
        n->left = leaf;
        if (onlyLeft)
            leaf->left = n;
    }
    n->father = root;
    return (ft::make_pair(n, true));
}

template < typename T, typename Compare >
node<T>*    recursiveInsertMultimap(node<T>* root, node<T>* n, Compare compare, bool onlyLeft, bool onlyRight)
{
    bool    nIsHigherOrEqual = !compare(n->content->first, root->content->first);
    if (!nIsHigherOrEqual) {
        if (isNotLeaf(root->left))
            return recursiveInsertMultimap<T, Compare>(root->left, n, compare, onlyLeft, false);
    }
    else if (isNotLeaf(root->right))
        return recursiveInsertMultimap<T, Compare>(root->right, n, compare, false, onlyRight);
    if (nIsHigherOrEqual) {
        node<T>* leaf = root->right;
        root->right = n;
        n->right = leaf;
        n->left = leaf;
        if (onlyRight)
            leaf->father = n;
    }
    else {
        node<T>* leaf = root->left;
        root->left = n;
        n->right = leaf;
        n->left = leaf;
        if (onlyLeft)
            leaf->left = n;
    }
    n->father = root;
    return n;
}

template < typename T, typename Compare >
ft::pair< node<T>*, bool >    recursiveInsertSet(node<T> *root, node<T> *n, Compare compare, bool onlyLeft, bool onlyRight)
{
    bool    nIsHigher = compare(*(root->content), *(n->content));
    if (!nIsHigher) {
        if (isNotLeaf(root->left))
            return recursiveInsertSet<T, Compare>(root->left, n, compare, onlyLeft, false);
    }
    else if (isNotLeaf( root->right))
        return recursiveInsertSet<T, Compare>(root->right, n, compare, false, onlyRight);
    if (nIsHigher) {
        node<T> *leaf = root->right;
        root->right = n;
        n->right = leaf;
        n->left = leaf;
        if (onlyRight)
            leaf->father = n;
    }
    else {
        node<T> *leaf = root->left;
        root->left = n;
        n->right = leaf;
        n->left = leaf;
        if (onlyLeft)
            leaf->left = n;
    }
    n->father = root;
    return (ft::make_pair(n, true));
}

template < typename T, typename Compare >
node<T>*   recursiveInsertMultiset(node<T>* root, node<T>* n, Compare compare, bool onlyLeft, bool onlyRight)
{
    bool    nIsHigherOrEqual = !compare(*(n->content), *(root->content));
    if (!nIsHigherOrEqual) {
        if (isNotLeaf(root->left))
            return recursiveInsertMultiset<T, Compare>(root->left, n, compare, onlyLeft, false);
    }
    else if (isNotLeaf(root->right))
        return recursiveInsertMultiset<T, Compare>(root->right, n, compare, false, onlyRight);
    if (nIsHigherOrEqual) {
        node<T>* leaf = root->right;
        root->right = n;
        n->right = leaf;
        n->left = leaf;
        if (onlyRight)
            leaf->father = n;
    }
    else {
        node<T>* leaf = root->left;
        root->left = n;
        n->right = leaf;
        n->left = leaf;
        if (onlyLeft)
            leaf->left = n;
    }
    n->father = root;
    return n;
}


template < typename T >
void doFix1(node<T> *n, node<T> **root)
{
    father(n)->color = BLACK;
    uncle(n)->color = BLACK;
    grandfather(n)->color = RED;
    fixTree(grandfather(n), root);
}

template < typename T >
void doFix3(node<T> *n, node<T> **root)
{
    node<T> *f = father(n);
    node<T> *g = grandfather(n);

    if (n == f->left)
        rightRotation(g, root);
    else
        leftRotation(g, root);
    f->color = BLACK;
    g->color = RED;
}

template < typename T >
void doFix2(node<T> *n, node<T> **root)
{
    node<T> *f = father(n);
    node<T> *g = grandfather(n);

    if (g && isNotLeaf(g->left) && n == g->left->right) {
        leftRotation(f, root);
        n = n->left;
    }
    else if (g && isNotLeaf(g->right) && n == g->right->left) {
        rightRotation(f, root);
        n = n->right;
    }
    doFix3(n, root);
}

template < typename T >
void fixTree(node<T> *n, node<T> **root)
{
    if (!father(n))
        n->color = BLACK;
    else if (father(n)->color == BLACK)
        ;
    else if (isNotLeaf(uncle(n)) && uncle(n)->color == RED)
        doFix1(n, root);
    else
        doFix2(n, root);
}

template < typename T, typename Compare>
ft::pair< node<T>*, bool >    insertNode(node<T> **root, node<T> *n, node<T> *leaf, Compare compare)
{
    ft::pair< node<T>*, bool >  result;

    if (!*root)
    {
        leaf->father = n;
		leaf->left = n;
        n->left = leaf;
        n->right = leaf;
        n->color = BLACK;
        return (ft::make_pair<node<T> *, bool>(n, true));
    }
    else
        result = recursiveInsert<T, Compare>(*root, n, compare, true, true);
    if (result.second)
        fixTree(n, root);
    return result;
}

template < typename T, typename Compare>
node<T>*    insertNodeMultimap(node<T>** root, node<T>* n, node<T>* leaf, Compare compare)
{
    node<T>*  result;

    if (!*root)
    {
        leaf->father = n;
        leaf->left = n;
        n->left = leaf;
        n->right = leaf;
        n->color = BLACK;
        return n;
    }
    else
        result = recursiveInsertMultimap<T, Compare>(*root, n, compare, true, true);
    fixTree(n, root);
    return result;
}

template < typename T, typename Compare>
ft::pair< node<T>*, bool >    insertNodeSet(node<T> **root, node<T> *n, node<T> *leaf, Compare compare)
{
    ft::pair< node<T>*, bool >  result;

    if (!*root)
    {
		leaf->father = n;
		leaf->left = n;
        n->left = leaf;
        n->right = leaf;
        n->color = BLACK;
        return (ft::make_pair<node<T> *, bool>(n, true));
    }
    else
        result = recursiveInsertSet<T, Compare>(*root, n, compare, true, true);
    if (result.second)
        fixTree(n, root);
    return result;
}

template < typename T, typename Compare>
node<T>* insertNodeMultiset(node<T>** root, node<T>* n, node<T>* leaf, Compare compare)
{
    node<T>* result;

    if (!*root)
    {
        leaf->father = n;
        leaf->left = n;
        n->left = leaf;
        n->right = leaf;
        n->color = BLACK;
        return n;
    }
    else
        result = recursiveInsertMultiset<T, Compare>(*root, n, compare, true, true);
    fixTree(n, root);
    return result;
}

//NODE DELETION

template < typename T >
void    fixTreeDelete(node<T> *n, node<T> **root)
{
    node<T> *y;

    while (n->father != NO_FATHER && n->color == BLACK)
    {
        if (n == n->father->left)
        {
            y = brother(n);
            if (y->color == RED)
            {
                y->color = BLACK;
                n->father->color = RED;
                leftRotation(n->father, root);
                y = n->father->right;
            }
            if ((y->left->color == BLACK) && (y->right->color == BLACK))
            {
                y->color = RED;
                n = n->father;
            }
            else
            {
                if (y->right->color == BLACK)
                {
                    y->left->color = BLACK;
                    y->color = RED;
                    rightRotation(y, root);
                    y = n->father->right;
                }
                y->color = n->father->color;
                n->father->color = BLACK;
                y->right->color = BLACK;
                leftRotation(n->father, root);
                break ;
            }
        }
        else
        {
            y = brother(n);
            if (y->color == RED)
            {
                y->color = BLACK;
                n->father->color = RED;
                rightRotation(n->father, root);
                y = n->father->left;
            }
            if ((y->right->color == BLACK) && (y->left->color == BLACK))
            {
                y->color = RED;
                n = father(n);
            }
            else
            {
                if (y->left->color == BLACK)
                {
                    y->right->color = BLACK;
                    y->color = RED;
                    leftRotation(y, root);
                    y = n->father->left;
                }
                y->color = n->father->color;
                n->father->color = BLACK;
                y->left->color = BLACK;
                rightRotation(n->father, root);
                break;
            }
        }
    }
    n->color = BLACK;
}

template < typename T, typename allocNode, typename allocValueType >
void    deleteNode(node<T> *n, node<T> **root, allocNode &alloc, allocValueType &allocT)
{
    node<T>* replace;
    node<T>* child;
    node<T>* temp;

    if (!n)
        return ;
    if (isLeaf((*root)->right) && isLeaf((*root)->left)) {
        destroyNode(*root, alloc, allocT);
        return ;
    }
    replace = n;
    if (isNotLeaf(replace->left) && isNotLeaf(replace->right))
    {
        replace = replace->right;
        while (isNotLeaf(replace->left))
            replace = replace->left;
    }
    if (n != replace) {

        //FATHER EXCHANGE
        if (n->father != NO_FATHER)
        {
            if (n == n->father->left)
                n->father->left = replace;
            else
                n->father->right = replace;
        }
        if (replace == replace->father->right)
            replace->father->right = n;
        else
            replace->father->left = n;
        temp = n->father;
        n->father = replace->father;
        replace->father = temp;

        //LEFT EXCHANGE
        if (isNotLeaf(n->left))
            n->left->father = replace;
        if (isNotLeaf(replace->left))
            replace->left->father = n;
        temp = n->left;
        n->left = replace->left;
        replace->left = temp;

        //RIGHT EXCHANGE
        if (isNotLeaf(n->right))
            n->right->father = replace;
        if (isNotLeaf(replace->right))
            replace->right->father = n;
        temp = n->right;
        n->right = replace->right;
        replace->right = temp;
    }
    if (isNotLeaf(n->left))
        child = n->left;
    else
        child = n->right;
    child->father = n->father;
    if (n->father == NO_FATHER)
        *root = child;
    else
    {
        if (n == n->father->left)
            n->father->left = child;
        else
            n->father->right = child;
    }
    if (n->color == BLACK)
        fixTreeDelete(child, root);
    destroyNode(n, alloc, allocT);
}

template < typename T, typename allocNode, typename allocValueType >
void	recursiveBurnTheTree(node<T> *n, allocNode &alloc, allocValueType &allocT) {
    if (isLeaf(n))
        return ;
    node<T> *right;
    node<T> *left;
    right = n->right;
    left = n->left;
    destroyNode(n, alloc, allocT);
    recursiveBurnTheTree(right, alloc, allocT);
    recursiveBurnTheTree(left, alloc, allocT);
}

//TREE TOOLS

template < typename T >
node<T> *getRoot(node<T> *n)
{
    if (!n)
        return (n);
    while (n->father)
        n = n->father;
    return n;
}

template < typename T >
node<T> *getSuccessor(node<T> *n)
{
    node<T> *f;
    node<T> *leaf;

    if (!n)
        return (NULL);
    if (isNotLeaf(n->right)) {
        n = n->right;
        while (isNotLeaf(n->left))
            n = n->left;
        return (n);
    }
    else
    {
        leaf = n->right;
        f = father(n);
        while (f && n == f->right)
        {
            n = f;
            f = father(n);
        }
        if (f)
            return (f);
        return (leaf);
    }
}

template < typename T >
const node<T> *getSuccessor(const node<T> *n)
{
	node<T> *f;
	node<T> *leaf;

	if (!n)
		return (NULL);
	if (isNotLeaf(n->right)) {
		n = n->right;
		while (isNotLeaf(n->left))
			n = n->left;
		return (n);
	}
	else
	{
		leaf = n->right;
		f = father(n);
		while (f && n == f->right)
		{
			n = f;
			f = father(n);
		}
		if (f)
			return (f);
		return (leaf);
	}
}


template < typename T >
node<T> *getPredecessor(node<T> *n)
{
    node<T> *f;
    node<T> *leaf;

    if (!n)
        return (NULL);
    if (isNotLeaf(n->left)) {
        n = n->left;
        while (isNotLeaf(n->right))
            n = n->right;
        return (n);
    }
    else
    {
        leaf = n->left;
        f = father(n);
        while (f && n == f->left)
        {
            n = f;
            f = father(n);
        }
        if (f)
            return (f);
        return (leaf);
    }
}

template < typename T >
const node<T> *getPredecessor(const node<T> *n)
{
	node<T> *f;
	node<T> *leaf;

	if (!n)
		return (NULL);
	if (isNotLeaf(n->left)) {
		n = n->left;
		while (isNotLeaf(n->right))
			n = n->right;
		return (n);
	}
	else
	{
		leaf = n->left;
		f = father(n);
		while (f && n == f->left)
		{
			n = f;
			f = father(n);
		}
		if (f)
			return (f);
		return (leaf);
	}
}

template < typename T >
node<T> *getMaxNode(node<T> *root)
{
    if (!root)
        return (root);
    while (isNotLeaf(root->right))
        root = root->right;
    return (root);
}

template < typename T >
node<T> *getMinNode(node<T> *root)
{
    if (!root)
        return (root);
    while (isNotLeaf(root->left))
        root = root->left;
    return (root);
}

template < typename T >
node<T> *getMaxNodeNoLeaf(node<T> *root)
{
    if (!root)
        return (root);
    while (root->right)
        root = root->right;
    return (root);
}

template < typename T >
node<T> *getMinNodeNoLeaf(node<T> *root)
{
    if (!root)
        return (root);
    while (root->left)
        root = root->left;
    return (root);
}

template <typename T >
size_t  recursiveGetHigh(node<T> *n, size_t t)
{
    if (isLeaf(n))
        return (t);
    return (ft::max(recursiveGetHigh(n->left, t + 1), recursiveGetHigh(n->right, t + 1)));
}

template < typename T >
size_t  getHigh(node<T> *root)
{
    if (!root)
        return (0);
    else
        return recursiveGetHigh(root, 0);
}

template < class Size >
void	fillString(std::string &str, const Size s)
{
	if ((s / 10) > 0)
	{
		fillString(str, s / 10);
		fillString(str, s % 10);
	}
	if ((s / 10) == 0)
		str += s + '0';
}

template < class Size >
std::string	size_tToString(const Size s)
{
	std::string	str;

	fillString(str, s);
	return (str);
}

template < typename  T >
void recursiveDisplayTree(node<T> *n, size_t size, size_t depth, ft::vector<std::string> &lines) {
    if (isLeaf(n)) {
        if (depth <= size) {
            size_t toBlank = 1;
            size_t blankDepth = depth;
            while (blankDepth <= size) {
                size_t blank = toBlank;
                while (blank--)
                    lines[blankDepth - 1] += "   ";
                toBlank++;
                blankDepth++;
            }
        }
    }
    else
    {
        size_t  blankDepth = depth;
        while (blankDepth < size)
        {
            lines[depth - 1] += "  ";
            blankDepth++;
        }
        if (n->color == RED) {
            lines[depth - 1] += "\e[31m ";
            lines[depth - 1] += size_tToString(n->content->first);
            lines[depth - 1] += " \e[0m";
        }
        else {
            lines[depth - 1] += " ";
            lines[depth - 1] += size_tToString(n->content->first);
            lines[depth - 1] += " ";
        }
        blankDepth = depth;
        while (blankDepth < size)
        {
            lines[depth - 1] += "  ";
            blankDepth++;
        }
        recursiveDisplayTree(n->left, size, depth + 1, lines);
        recursiveDisplayTree(n->right, size, depth + 1, lines);
    }
}

template < typename T >
void displayTree(node<T> *n) {
    size_t size = getHigh(n);

    ft::vector<std::string> lines(size);
    ft::vector<std::string>::iterator it = lines.begin();
    if (size)
        recursiveDisplayTree(n, size, 1, lines);
    while (it != lines.end()) {
        std::cout << *it << std::endl;
        it++;
    }
}

template < typename  T >
void recursiveDisplayTreeSet(node<T> *n, size_t size, size_t depth, ft::vector<std::string> &lines) {
    if (isLeaf(n)) {
        if (depth <= size) {
            size_t toBlank = 1;
            size_t blankDepth = depth;
            while (blankDepth <= size) {
                size_t blank = toBlank;
                while (blank--)
                    lines[blankDepth - 1] += "   ";
                toBlank++;
                blankDepth++;
            }
        }
    }
    else
    {
        size_t  blankDepth = depth;
        while (blankDepth < size)
        {
            lines[depth - 1] += "  ";
            blankDepth++;
        }
        if (n->color == RED) {
            lines[depth - 1] += "\e[31m ";
            lines[depth - 1] += size_tToString(*(n->content));
            lines[depth - 1] += " \e[0m";
        }
        else {
            lines[depth - 1] += " ";
            lines[depth - 1] += size_tToString(*(n->content));
            lines[depth - 1] += " ";
        }
        blankDepth = depth;
        while (blankDepth < size)
        {
            lines[depth - 1] += "  ";
            blankDepth++;
        }
        recursiveDisplayTreeSet(n->left, size, depth + 1, lines);
        recursiveDisplayTreeSet(n->right, size, depth + 1, lines);
    }
}

template < typename T >
void displayTreeSet(node<T> *n) {
    size_t size = getHigh(n);

    ft::vector<std::string> lines(size);
    ft::vector<std::string>::iterator it = lines.begin();
    if (size)
        recursiveDisplayTreeSet(n, size, 1, lines);
    while (it != lines.end()) {
        std::cout << *it << std::endl;
        it++;
    }
}
#endif