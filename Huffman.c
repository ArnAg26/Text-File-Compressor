#include <stdio.h>
#include <stdlib.h>

struct Node{
    char c;
    int frequency;
    struct Node *left;
    struct Node *right;
};

struct Heap{
    int len;
    struct Node** arr;
};

struct Code{
    char c;
    char *arr;
    int len;
};

int printarrayint(int arr[],int n){
    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
}

int printarraychar(char arr[],int n){
    for(int i=0;i<n;i++){
        printf("%c ",arr[i]);
    }
    printf("\n");
}

int printheap(struct Heap* h){
    for(int i=0;i<h->len;i++){
        printf("%c ",h->arr[i]->c);
        printf("%d ",h->arr[i]->frequency);
    }
    printf("\n");
}

void printCodes(struct Code **array,int unique){
    for(int i=0;i<unique;i++){
        printf("%c",array[i]->c);
        for(int j=0;j<array[i]->len;j++){
            printf("%c",array[i]->arr[j]);
        }
    }
}
void LetterWrite(char c,char A[],char B[256][256],int len[],int unique,FILE *fr){
    int idx,length;
    for(int i=0;i<unique;i++){
        if(A[i]==c){
            idx=i;
            length=len[i];
            break;
        }
    }
    int j;
    for(j=0;j<length;j++){
        fprintf(fr,"%c",B[idx][j]);
    }

}
void Decode(int unique,char A[],char B[256][256],int len[],FILE *fp,FILE *fr){
    //fr=fopen("out.txt","a");
    char c;
    while(1){
        c=fgetc(fp);
        if(c==EOF){
            //printf("%d",EOF);
            break;
        }
        LetterWrite(c,A,B,len,unique,fr);
    }
}

void heapify(struct Heap* h,int i){
    int k=i;
    int leftchild=2*i+1;
    int rightchild=2*i+2;
    if(leftchild<h->len && h->arr[leftchild]->frequency<h->arr[k]->frequency)
        k=leftchild;
    if(rightchild<h->len && h->arr[rightchild]->frequency<h->arr[k]->frequency){
        k=rightchild;
    }
    if(k!=i){
        struct Node *p=h->arr[k];
        h->arr[k]=h->arr[i];
        h->arr[i]=p;
        heapify(h,k);
    }
}

struct Heap* BuildHeap(char arr1[],int arr2[],int unique){
    struct Heap* h=(struct Heap*)malloc(sizeof(struct Heap));
    h->len=0;
    h->arr = (struct Node**)malloc(unique*sizeof(struct Node*));
    for(int i=0;i<unique;i++){
        struct Node *p=(struct Node *)malloc(sizeof(struct Node));
        p->c=arr1[i];
        p->frequency=arr2[i];
        p->left=p->right=NULL;
        h->arr[i]=p;
        h->len+=1;
        for(int j=i/2;j>=0;j--){
            heapify(h,j);
            //minHeapify(h,j);
        }
    }
    return h;
}

void ReadFromMetaFile(char C[],char arr[256][256],int len[],int unique,FILE *fp){
    int i=0;
    while(i!=unique){
        char c=fgetc(fp);
        C[i]=c;
        c=fgetc(fp);
        c=fgetc(fp);
        int j=0;
        do{
            c=fgetc(fp);
            if(c=='\n'){
                break;
            }
            //printf("%c",c);
            arr[i][j]=c;
            //printf("%c",c);
            j++;
        }while(c!='\n');
    len[i]=j;  
    i++;
    }
}



struct Node *ExtractMin(struct Heap *h){
    struct Node *min=h->arr[0];
    h->len-=1;
    h->arr[0]=h->arr[h->len];
    heapify(h,0);
    return min;
}

void Insert(struct Heap *h,struct Node *p){
    h->len++;
    int i=h->len-1;
    h->arr[i]=p;
    while(i!=0 && p->frequency<h->arr[(i-1)/2]->frequency){
            h->arr[i]=h->arr[(i-1)/2];
           i=(i-1)/2;

    }
    h->arr[i]=p;
}

void printTree(struct Node *p){
    if(p!=NULL){
        printTree(p->left);
        printf("%c ",p->c);
        printf("%p ",p->left);
        printf("%p ",p->right);
        printf("%d\n",p->frequency);
        printTree(p->right);
    }
}

struct Node *HuffmanTree(struct Heap *h,int unique){
    struct Node *l;
    struct Node *r;
    while(h->len!=1){
        l=ExtractMin(h);
        r=ExtractMin(h);
        struct Node *p=(struct Node *)malloc(sizeof(struct Node));
        p->left=l;
        p->right=r;
        p->c='#';
        p->frequency=l->frequency+r->frequency;
        Insert(h,p);
    }
    return ExtractMin(h);
}

void WriteMetatoFile(char c,char arr[],int n,FILE *fp){
    fprintf(fp,"%c",c);
    fprintf(fp,"->");
    for(int i=0;i<n;i++){
        fprintf(fp,"%c",arr[i]);
    }
    fprintf(fp,"\n");
}

void Coding(struct Node *head,char arr[],int n,FILE *fp){
    if(head->left!=NULL){
        arr[n]='0';
        int N=n+1;
        Coding(head->left,arr,N,fp);
    }
    if(head->right!=NULL){
        arr[n]='1';
        int N=n+1;
        Coding(head->right,arr,N,fp);
    }
    if(head->right==NULL && head->left==NULL){
        printf("%c->",head->c);
        for(int i=0;i<n;i++){
            printf("%c",arr[i]);
        }
        WriteMetatoFile(head->c,arr,n,fp);
        printf("\n");
    }
}

void Metadata(char arr[],int frequency[],int unique){
    FILE *fp;
    fp=fopen("metadata.txt","w");
    for(int i=0;i<unique;i++){
        fprintf(fp,"%c",arr[i]);
        fprintf(fp,"%d",frequency[i]);
        fprintf(fp,";");
    }
    
    fclose(fp);
}

struct Node *MakeDecompressionTree(int unique,char C[],int frequency[]){
    FILE *fp;
    fp=fopen("metadata.txt","r");
    int i=0;
    char c;
    for(int i=0;i<unique;i++){
        c=fgetc(fp);
        C[i]=c;
        frequency[i]=0;
        while(c!=';'){
            c=fgetc(fp);
            if(c==';'){
                break;
            }
            int num=c-'0';
            frequency[i]=frequency[i]*10+ num;
        }
    }
//printf("1");
fclose(fp);
struct Heap *h=BuildHeap(C,frequency,unique);
struct Node *H=HuffmanTree(h,unique);
//printTree(H);
return H;
}


void FileDecode(struct Node *root,int unique,char arr[256][256],int len[],char C[],FILE *fp,FILE *fr){
    char c;
    int i=0;
    while(i!=unique){
        c=fgetc(fp);
        while(c!='\n'){
            c=fgetc(fp);
        }
        i++;
    }
    //printf("%c",c);
    struct Node* current=root;
    while(c!=EOF){
        c=fgetc(fp);
        if(c=='0'){
            //printf("0");
        current=current->left;
            if(current->left==NULL && current->right==NULL){
                fprintf(fr,"%c",current->c);
                current=root;
            }
    }
        else if(c=='1'){
            //printf("1");
            current=current->right;
            if(current->right==NULL && current->left==NULL){
                fprintf(fr,"%c",current->c);
                current=root;
            }
        }
    }
}
int main()

{   char fname[128];
    printf("Input File");
    scanf("%s",&fname);
    char gname[128];
    printf("Output File");
    scanf("%s",&gname);
    int x;
    printf("Enter preferred mode of file ");
    scanf("%d",&x);
    if(x==0){
    
    //char gname[]="out.txt";
   
   int num;
   FILE *fp;
   fp = fopen(fname,"r");
   char c=fgetc(fp);
   int noOfChar=0;
   while (c!=EOF){
    noOfChar+=1;
    c=fgetc(fp);
   }
   fclose(fp);
   FILE *fr;
   fr=fopen(fname,"r");
   char array[noOfChar];
   int unique=0;
   int frequency[noOfChar];
   for(int i=0;i<noOfChar;i++){
        frequency[i]=0;
        char c=fgetc(fr);
        int flag=0;
        for(int j=0;j<unique;j++){
            if(array[j]==c){
                //printarraychar(array,unique);
                frequency[j]+=1;
                flag=1;
                break;
            }
        }
        if(flag==0){
            array[unique]=c;
            frequency[unique]+=1;
            unique+=1;
        }
   }
   fclose(fr);

   struct Heap *h=BuildHeap(array,frequency,unique);
   struct Node *H=HuffmanTree(h,unique);
    char arr[256];
   struct Code* a[256];
   char A[256];
   char B[256][256];
   int len[256];
   
    //FILE *fp,*fr;
    fp=fopen(gname,"w");
    Coding(H,arr,0,fp);
    fclose(fp);
    fp=fopen(gname,"r");
   ReadFromMetaFile(A,B,len,unique,fp);
   fclose(fp);
   fp=fopen(fname,"r");
   fr=fopen(gname,"a");
   Decode(unique,A,B,len,fp,fr);
   fclose(fp);
   fclose(fr);
   Metadata(array,frequency,unique);

   }
   else{
    char arr[256];
   struct Code* a[256];
   char A[256];
   char B[256][256];
   int len[256];
    FILE *fp;
    fp=fopen("metadata.txt","r");

    int i=0;
    char c;
    int ctr=0;
    while(c!=EOF){
        c=fgetc(fp);
        if(c==';'){
            ctr++;
        }
    }
    //printf("%d",ctr);

    fclose(fp);
    

    struct Node *root=(struct Node *)malloc(sizeof(struct Node));
   root->c='#';
   root->frequency=0;
   root->left=NULL;
   root->right=NULL;
   //FILE *fp;
    FILE *fr;
    fp=fopen(fname,"r");
    fr=fopen(gname,"w");
   
   
   char C[256];
   int f[256];
   struct Node *t=MakeDecompressionTree(ctr,C,f);
   FileDecode(t,ctr,B,len,A,fp,fr);
   fclose(fp);
   fclose(fr);
}

}
