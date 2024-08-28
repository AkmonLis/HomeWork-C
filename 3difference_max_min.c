#include <stdio.h>
#include <stdlib.h>




typedef struct tree
{
 int key;
 struct tree *left;
 struct tree *right;
 struct tree *parent;
}tree;


void insert(tree **root,int key, tree *pt) 
{
	if(!(*root)) 
	{
		*root=malloc(sizeof(tree));
		(*root)->key=key;
		(*root)->parent=pt;
		(*root)->left=(*root)->right= NULL;
		}
	else if( key < (*root)->key)
	{
		insert( &((*root)->left) ,key, *root );
	}
	else
	{
		insert( &((*root)->right),key,*root);
	}
}

int FindMax(tree *tr)
{
	if (tr == NULL)
	{
		return 0;
	}
	while (tr->right != NULL)
	{
		tr = tr->right;
	}
	return tr->key;
}


int FindMin(tree *tr)
{
	if (tr == NULL)
	{
		return 0;
	}
	while (tr->left != NULL)
	{
		tr = tr->left;
	}
	return tr->key;
}


int main(int argc, char **argv)
{
	tree *tr = NULL;
	insert(&tr,10,NULL);
	insert(&tr,5, NULL);
	insert(&tr,15,NULL);
	insert(&tr,3, NULL);
	insert(&tr,7 ,NULL);
	insert(&tr,13,NULL);
	insert(&tr,18,NULL);
	insert(&tr,1, NULL);
	insert(&tr,6 ,NULL);
	
	printf("%d", (FindMax(tr) - FindMin(tr)));
	
	return 0;
}

