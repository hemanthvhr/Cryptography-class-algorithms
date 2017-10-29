#include <stdio.h>
#include <stdlib.h>
#define FILL1 'x'
#define FILL2 'q'
#define FILL3 'z'
#define OMITED 'j'
#define INSTEAD 'i'
#define TOTAL 26

void encrypt(int *tracking,char **table);
void decrypt(int *tracking,char **table);

void main() {
  char *word,temp;
  int n,i,j,x;
  int tracking[TOTAL];
  char **table = (char **) malloc(5*sizeof(char*));
  for(i=0;i<5;i++) table[i] = (char *) malloc(5*sizeof(char));
  for(i=0;i<TOTAL;i++) tracking[i] = -1;
  printf("Enter the no of letters in the key without space is - ");
  scanf("%d",&n);
  //printf("\nok-0 n=%d\n",n);
  printf("\nEnter the KEY without space - ");
  for(i=0;i<n;i++) {
    temp = getchar();//getchar();
    //printf("temp = %c\n",temp);
    temp = tolower(temp);
    x = temp - 'a';
    if((tracking[x] != -1)||(temp == OMITED)) {
        i--;
        continue;
    }
    tracking[x] = i;
    table[i/5][i%5] = temp;
  }
  //printf("\nok1\n");
  n = i;
  for(i=0;i<TOTAL;i++) {
    if((tracking[i] >= 0)||(i == (OMITED - 'a'))) continue;
    tracking[i] = n;
    table[n/5][n%5] = 'a' + i;
    n++;
  }
  printf("\nThe key table is - \n");
  for(i=0;i<5;i++) {
    for(j=0;j<5;j++) {
        printf("%c ",table[i][j]);
    }
    printf("\n");
  }
  tracking[OMITED - 'a'] = tracking[INSTEAD - 'a'];
  printf("\noption = 0,exit\n");
  printf("\noption = 1,take a plain text return the cipher text and come to loop again\n");
  printf("\noption = 2,take a cipher text return the plain text and come to loop again\n");
  i=1;
  while(i!=0) {
    printf("\n\nEnter a option - ");
    scanf("%d",&i);
    if(i==0) break;
    if(i==1) encrypt(tracking,table);
    if(i==2) decrypt(tracking,table);
    if((i<0)||(i>2)) {
        i=1;
        printf("\nEnter option = 0 or 1 or 2 only");
    }
  }
}
void encrypt(int *tracking,char **table) {
  int n,i,j;
  int p,q;
  char *text,*word,temp,x,y;
  printf("\nEnter the no of letters in plain text without space - ");
  scanf("%d",&n);
  printf("\nEnter the Plain text without space - \n");
  word = (char *) malloc((n+1)*sizeof(char));
  scanf("%s",word);
  text = (char *) malloc((2*n+1)*sizeof(char));
  temp = word[0];
  temp = tolower(temp);
  text[0] = temp;
  j=1;
  for(i=1;i<n;i++) {
    temp = word[j];
    temp = tolower(temp);
    if(temp == text[i-1]) {
        switch(temp) {
            case FILL1 : text[i] = FILL2;break;
            case FILL2 : text[i] = FILL3;break;
            case FILL3 : text[i] = FILL1;break;
            default : text[i] = FILL1;
        }
        i++;
        n++;
    }
    text[i] = temp;
    j++;
  }
  if(n%2) {
    temp = FILL1;
    if(temp == text[i-1]) temp = FILL2;
    text[i] = temp;
    i++;
    n++;
  }
  text[i] = '\0';
  for(i=0;i<n;i+=2){
    x = text[i];p = tracking[x-'a'];
    y = text[i+1];q = tracking[y-'a'];
    if((p/5) == (q/5)) {
        x = table[p/5][(p%5 + 1)%5];
        y = table[q/5][(q%5 + 1)%5];
    }
    if((p%5) == (q%5)) {
        x = table[(p/5 + 1)%5][p%5];
        y = table[(q/5 + 1)%5][q%5];
    }
    if(((p/5)!=(q/5))&&((p%5)!=(q%5))) {
        x = table[p/5][q%5];
        y = table[q/5][p%5];
    }
    text[i] = x;
    text[i+1] = y;
  }
  printf("\nThe Cipher text is - %s\n",text);
  free(text);
  free(word);
}
void decrypt(int *tracking,char **table) {
  int n,i;
  int p,q;
  char *text,x,y;
  printf("\nEnter the no of letters in cipher text without space - ");
  scanf("%d",&n);
  while(n%2==1) {
    printf("\nEnter the no of letters in a valid cipher text - ");
    scanf("%d",&n);
  }
  printf("\nEnter the cipher text without space - \n");
  text = (char *) malloc((n+1)*sizeof(char));
  scanf("%s",text);
  for(i=0;i<n;i+=2){
    x = text[i];p = tracking[x-'a'];
    y = text[i+1];q = tracking[y-'a'];
    if(x==y) {
        printf("\nEnter a valid cipher text and try again !!!\n");
        return;
    }
    if((p/5) == (q/5)) {
        x = table[p/5][(p%5 - 1)%5];
        y = table[q/5][(q%5 - 1)%5];
    }
    if((p%5) == (q%5)) {
        x = table[(p/5 - 1)%5][p%5];
        y = table[(q/5 - 1)%5][q%5];
    }
    if(((p/5)!=(q/5))&&((p%5)!=(q%5))) {
        x = table[p/5][q%5];
        y = table[q/5][p%5];
    }
    text[i] = x;
    text[i+1] = y;
  }
  printf("\nThe Approximate Plain text is - %s\n",text);
  free(text);
}



