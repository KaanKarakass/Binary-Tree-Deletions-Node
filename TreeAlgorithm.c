#include<stdio.h> 
#include<stdlib.h>
typedef struct NODE_s *NODE;
typedef struct NODE_s{
	NODE left;
	NODE right;
	void* data;
	unsigned long key;
}NODE_t[1];

typedef struct TREE_s{
	NODE root;
}TREE_t[1],*TREE;

TREE tree_init()
{
	TREE t;
	t=(TREE)malloc(sizeof(TREE_t));
	t->root=NULL;
	return t;
}

NODE node_init(unsigned long key,void *data)
{
	NODE node;
	node=(NODE)malloc(sizeof(NODE_t));
	node->data=data;
	node->key=key;
	node->left=NULL;
	node->right=NULL;
	
	return node;
}
void tree_kill(NODE node)
{
	if(node==NULL)
	{
		return;
	}
	tree_kill(node->left);
	tree_kill(node->right);
	free(node);
}

void node_kill(NODE node)
{
	free(node);
}

NODE tree_recursive_insert(NODE node,unsigned long key,void* data)
{
	if(node!=NULL)
	{
		if(key < node->key)
		{
			node->left=tree_recursive_insert(node->left,key,data);
		}
		else if(key > node->key)
		{
			node->right=tree_recursive_insert(node->right,key,data);
		}
		else
		{
			printf("error same key");
		}
	}
	else
	{
		node=node_init(key,data);
	}
	return node;
}

void tree_insert(TREE tree,unsigned long key,void *data)
{
	if(tree->root==NULL)
	{
		tree->root=node_init(key,data);
	}
	else
	{
		tree->root=tree_recursive_insert(tree->root,key,data);
	}
}
void print_tree(NODE node,int count)
{
	int i;
	if(node!=NULL)
	{
		print_tree(node->right,count+3);
		for(i=0;i<count;i++)
		{
			printf(" ");
		}
		printf("%lu\n",node->key);
		print_tree(node->left,count+3);
	}
}
NODE find_max_node(NODE max)
{
	while(max->right!=NULL)
	{
		max=max->right;
	}
	return max;
}
NODE delete_tree_recursive_unbalanced(NODE node,unsigned long key)
{
	if(node==NULL)
	{
		return node;
	}
	if(key < node->key)
	{
		node->left=delete_tree_recursive_unbalanced(node->left,key);
	}
	else if(key > node->key)
	{
		node->right=delete_tree_recursive_unbalanced(node->right,key);
	}
	else
	{
		NODE tmp;
		if(node->left==NULL)
		{
			tmp=node->right;
			free(node->data);
			node_kill(node);
			return tmp;
		}
		else if(node->right==NULL)
		{
			tmp=node->left;
			free(node->data);
			node_kill(node);
			return tmp;
		}
		else 
		{
			NODE left=node->left;
			tmp=find_max_node(node->left);
			tmp->right=node->right;
			free(node->data);
			node_kill(node);
			return left;
		}
	}
	return node;
}
void delete_tree_iterative_balanced(TREE tree,unsigned long key)
{
	NODE parent,node;
	char last=0;
	
	parent=(NODE)tree;
	
	node=tree->root;
	
	while(node!=NULL)
	{
		if(key < node->key)
		{
			parent=node;
			node=node->left;
			last=0;
		}
		else if(key > node->key)
		{
			parent=node;
			node=node->right;
			last=1;
		}
		else
		{
			break;
		}
	}
	if(node==NULL)
	{
		printf("key is not in tree\n");
		return;
	}
	if((node->left==NULL)&&(node->right==NULL))
	{
		free(node->data);
		node_kill(node);
		if(last==0)
		{
			parent->left=NULL;
		}
		else
		{
			parent->right=NULL;
		}
	}
	else if((node->left!=NULL)&&(node->right==NULL))
	{
		if(last==0)
		{
			parent->left=node->left;
		}
		else
		{
			parent->right=node->left;
		}
		free(node->data);
		node_kill(node);
	}
	else if((node->left==NULL)&&(node->right!=NULL))
	{
		if(last==0)
		{
			parent->left=node->right;
		}
		else
		{
			parent->right=node->right;
		}
		free(node->data);
		node_kill(node);
	}
	else
	{
		int control=0;  //if node->left has right child ,control is 1 but if node->left does not has right child ,control is 0. 
		NODE max,prev;
		
		max=node->left;
		prev=max;
		while(max->right!=NULL)
		{
			control=1;   
			prev=max;
			max=max->right;
		}
		
		node->key=max->key;
		node->data=max->data;
		
		if(control==1)
		{
			prev->right=max->left;
			free(max->data);
			node_kill(max);
		}
		else
		{
			node->left=max->left;
			free(max->data);
			node_kill(max);
		}
	}
}
NODE delete_tree_recursive_balanced(NODE node,unsigned long key)
{
	if(node==NULL)
	{
		return node;
	}
	if(key < node->key)
	{
		node->left=delete_tree_recursive_balanced(node->left,key);
	}
	else if(key > node->key)
	{
		node->right=delete_tree_recursive_balanced(node->right,key);
	}
	else 
	{
		NODE max;
		if(node->left==NULL)
		{
			max=node->right;
			free(node->data);
			node_kill(node);
			return max;
		}
		else if(node->right==NULL)
		{
			max=node->left;
			free(node->data);
			node_kill(node);
			return max;
		}
		else
		{
			max=find_max_node(node->left);
			node->key=max->key;
			node->data=max->data;
			node->left=delete_tree_recursive_balanced(node->left,max->key);
		}
	}
	return node;
}
void delete_tree_iterative_unbalanced(TREE tree,unsigned long key)
{
	NODE parent,node;
	char last=0;
	
	parent=(NODE)tree;
	
	node=tree->root;
	while(node!=NULL)
	{
		if(key < node->key)
		{
			parent=node;
			node=node->left;
			last=0;
		}
		else if(key > node->key)
		{
			parent=node;
			node=node->right;
			last=1;
		}
		else
		{
			break;
		}
	}
	if(node==NULL)
	{
		printf("key is not in tree\n");
		return;
	}
	if((node->left==NULL) && (node->right==NULL))
	{
		free(node->data);
		node_kill(node);
		if(last==0)
		{
			parent->left=NULL;	
		}
		else 
		{
			parent->right=NULL;	
		}	
	}
	else if((node->left!=NULL)&&(node->right==NULL))
	{
		if(last==0)
		{
			parent->left=node->left;
		}
		else
		{
			parent->right=node->left;
		}
		free(node->data);
		node_kill(node);
	}
	else if((node->left==NULL)&&(node->right!=NULL))
	{
		if(last==0)
		{
			parent->left=node->right;
		}
		else if(last==1)
		{
			parent->right=node->right;
		}
		free(node->data);
		node_kill(node);
	}
	else
	{
		if(last==0)
		{
			parent->left=node->left;
		}
		else
		{
			parent->right=node->left;
		}
		NODE max=node->left;
		while(max->right!=NULL)
		{
			max=max->right;
		}
		max->right=node->right;
		free(node->data);
		node_kill(node);
	}
}

int main()
{
	TREE tree1;
	tree1=tree_init();
	tree_insert(tree1,55,NULL);
	tree_insert(tree1,35,NULL);
	tree_insert(tree1,27,NULL);
	tree_insert(tree1,21,NULL);
	tree_insert(tree1,29,NULL);
	tree_insert(tree1,37,NULL);
	tree_insert(tree1,36,NULL);
	tree_insert(tree1,78,NULL);
	tree_insert(tree1,68,NULL);
	tree_insert(tree1,89,NULL);
	tree_insert(tree1,90,NULL);
	tree_insert(tree1,69,NULL);
	print_tree(tree1->root,0);
	unsigned long w_key;
	NODE p;
	p=(NODE)tree1;
	int a;
	while(a!=5)
	{
		printf("\n**********ENTER '1' FOR ITERATIVE UNBALANCED\n");
		printf("\n**********ENTER '2' FOR ITERATIVE BALANCED\n");
		printf("\n**********ENTER '3' FOR RECURSIVE UNBALANCED\n");
		printf("\n**********ENTER '4' FOR RECURSIVE BALANCED\n");
		printf("\n**********ENTER '5' FOR EXIT\n");
		scanf("%d",&a);
		switch(a)
		{
			case 1:
				printf("enter want to you delete key\n");
				scanf("%lu",&w_key);
				delete_tree_iterative_unbalanced(tree1,w_key);
				printf("after delete %lu\n\n",w_key);
				print_tree(tree1->root,0);
				break;
			case 2:
				printf("enter want to you delete key\n");
				scanf("%lu",&w_key);
				delete_tree_iterative_balanced(tree1,w_key);
				printf("after delete %lu\n\n",w_key);
				print_tree(tree1->root,0);
				break;
			case 3:
				printf("enter want to you delete key\n");
				scanf("%lu",&w_key);
				delete_tree_recursive_unbalanced(p,w_key);
				printf("after delete %lu\n\n",w_key);
				print_tree(tree1->root,0);
				break;
			case 4:
				printf("enter want to you delete key\n");
				scanf("%lu",&w_key);
				delete_tree_recursive_balanced(p,w_key);
				printf("after delete %lu\n\n",w_key);
				print_tree(tree1->root,0);
				break;
		}
	}
	tree_kill(tree1->root);
}
