// I did many assignments for my course "Data Structures And Algorithms" one of which was the following:

// A perfectly balanced BST is a BST where for every node v the difference between the height of the left and
// right subtree of v is at most 1. A nearly balanced BST is a BST such that for every node v in the tree,
// max{ |subtree(left(v))|,|subtree(right(v))| } ≤ 3/4 |subtree(v)|.
// In this assignment, you have to code up these variants, and compare their performances on random input
// sequences. You have to :
// 1. Implement a function used to insert a given value into the BST
// 2. Implement a search function for a given value
// 3. Checking perfectly balanced condition at a node
// 4. Checking nearly balanced condition at a node
// 5. Creating a BST from a sorted array (this must be done in O(n) time, where n is the size of the array)
// 6. Creating a sorted array from a BST (this must also be done in O(n) time, where n is the number of
// nodes in the BST)




#include<stdio.h>
#include<stdlib.h>
#define N 200005
int TOBALANCE=1; // TOBALANCE is 1 when the condition is Perfectly balanced, 0 otherwise. By changing this constant you can change balance condition.
int NEARLY_BALANCED = 0;
int PERFECTLY_BALANCED = 1;
double BALANCE_RATIO = 0.75; //You may change this ratio.

/* structure of a node of the binary tree.
val: value at the node
size: size of subtree rooted at node
left,right:pointers to right and left sub tree 
*/
typedef struct node{
  int val;
  int size; 
  struct node *left;
  struct node *right;  
} Node;


/*
This function takes a value and initialises and returns a new node whose value is set to the given parameter.
*/
Node* init(int val){
  Node *curr = (Node *)malloc(sizeof(Node));
  curr->val=val;
  curr->size=1;
  curr->left=curr->right=NULL;
  return curr;
}
//done

/*
SortedArrayFromBST: This function takes as input a node, pointer to an array, and an integer that denotes 
the next free position in the array. This function fills in the array all the values in the tree whose root
 is the given node i.e. output array finally contains the elements in increasing order.
This function must return the number of array positions it has filled. i.e. if initial value of parameter fst = p, then after returning from 
the function the final value must increase by the size of subtree i.e p+curr->size.
*/
int height(Node* curr){
    if(curr==NULL){
        return -1;
    }
    int left= height(curr->left);
    int right = height(curr->right);
    int max = (left>right)?left:right;
    return max+1;
}

int SortedArrayFromBST(Node *curr,int output[],int fst){ //Check if fst thing works
  if(curr == NULL){
      return fst;
  }
    fst=SortedArrayFromBST(curr->left,output, fst);
    output[fst]= curr->val;
    fst=fst+1;
    fst=SortedArrayFromBST(curr->right,output,fst);
    return fst;
}
/*
This function creates a BST from an array. It takes as parameters input array and the length of array. It returns the root of the BST
that is formed by the array
*/
Node *BSTFromSortedArray(int input[], int len){
  if(len<=0){
    return NULL;
  }
  if(len==1){
    return init(*(input));
  }
  int mid = len/2;
  Node *curr=init(*(input+mid));
  curr->left = BSTFromSortedArray(input, mid);
  curr->right = BSTFromSortedArray(input+mid+1, len-mid-1);
  int left=0;int right=0;
  if(curr->left!=NULL){
    left = curr->left->size;
  }
  if(curr->right!=NULL){
    right = curr->right->size;
  }
  curr->size = left+right+1;
  return curr;
}

/*
This function recursively deletes all the nodes in the tree whose root in pointed by curr
*/
void deleteNode(Node *curr){
  if(curr==NULL) return;
  deleteNode(curr->left); 
  deleteNode(curr->right);
  free(curr);
}

/*
This function returns true if the current node satisfies the nearly balanced condition and false otherwise
*/
int checkNearBalance(Node *curr){
  int left = 0;
  int right = 0;
  if(curr == NULL){
    return 1;
  }
  if(curr->left != NULL){
    left = curr->left->size;
  }
  if(curr->right != NULL){
    right = curr->right->size;
  }
  double temp=BALANCE_RATIO*(curr->size);
  int max =(int)temp;
   int check = 0;
  if(left>=right){
    if(left<=max){
      check = 1;
    }
  }
  else{
    if(right<=max){
      check = 1;
    }
  }
  return check;
}

/*
This function returns true if the current node satisfies the perfectly balanced condition and false otherwise
*/
int checkPerfectBalance(Node *curr){
   if(curr == NULL){
     return 1;
   }
   int l=0;
   int r=0;
   if(curr->left!=NULL){
     l = curr->left->size;
   }
   if(curr->right!=NULL){
     r = curr->right->size;
   }
   int diff = l-r;
   if(diff==0||diff==1||diff==-1){
     return 1;
   }
   return 0;
}

/*
This function takes as input a pointer to a node and inserts a node in the subtree of the node depending on the value.
out[] array is used as a helper array which will be used when the BST does not satisfy the balanced condition.

-when the insert returns then the tree must satisfy the balanced condition
*/

//just a utility function used for debugging purposes
void insert(Node **curr,int val,int out[]){
  if((*curr)->val>val){
    if((*curr)->left == NULL){
      (*curr)->size++;
      (*curr)->left= init(val);
    }
    else{
      (*curr)->size++;
      insert(&((*curr)->left),val,out);
    }
  }
  else{
    if((*curr)->right == NULL){
      (*curr)->size++;
      (*curr)->right = init(val);
    }
    else{
      (*curr)->size++;
      insert(&((*curr)->right),val,out);
    }
  }
  if(((TOBALANCE==PERFECTLY_BALANCED)&&(checkPerfectBalance((*curr))))||((TOBALANCE==NEARLY_BALANCED)&&(checkNearBalance((*curr))==0))){
    int len = SortedArrayFromBST((*curr), out, 0);
    (*curr) = BSTFromSortedArray(out, len);
  }
  
  return;
}

/*
This function takes as input a node pointer and a val and searches for a node which has value val in the subtree of node.
It returns true if the value is found, false otherwise.
*/
int search(Node *curr,int val){
  if(curr->val == val){
    return 1;
  }
  if(curr->val<val){
    return search(curr->right, val);
  }
  return search(curr->left, val);
  return 0;
}

int main(){
  Node *root = NULL;
  Node *r2;
  int out[N];
  int queries; scanf("%d",&queries);
  while(queries--){
    int c,val; scanf("%d%d",&c,&val);
    if(c==1){
      //insert
      if(root){
        insert(&root,val,out);
      }else{
        root = init(val);

      }
    }else{
      //search query
      if(search(root,val)) printf("Yes\n"); else printf("No\n");
    }
  }
  /*if(checkPerfectBalance(root)){
    printf("Yes, it is Perfectly balanced\n");
  }
  else{
    printf("No, it is not perfectly balanced.\n");
  }
  if(checkNearBalance(root)){
    printf("Yes, it is Nearly balanced\n");
  }
  else {
    printf("No, it is not nearly balanced\n");
  }*/
  return 0;
}


