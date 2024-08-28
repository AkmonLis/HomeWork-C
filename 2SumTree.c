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


int heightTree(tree* p)
{
	if (p == NULL)
	{
		return 0;
	}
	else 
	{
		int lheight = heightTree(p->left);
		int rheight = heightTree(p->right);
		
		if (lheight > rheight)
		{
			return (lheight + 1);
		}
		else
		{
			return (rheight + 1);
		}
	}
}
void sumCurrentLevel(tree* root, int level,int* sum)
{
	if (root == NULL)
	{
		return;
	}
	
	if (level == 1)
	{
		(*sum) = (*sum) + root->key;
	}
	else if (level > 1) 
	{
		sumCurrentLevel(root->left, level - 1, sum);
		sumCurrentLevel(root->right, level - 1, sum);
	}
}


void printSUM(tree* root)
{
	int h = heightTree(root);
	int sum = 0;
	for (int i = 1; i <= h; i++)
	{
		sumCurrentLevel(root, i,&sum);
	}
	printf("%d", sum);
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
	printSUM(tr);
	
	return 0;
}

