#include <stdio.h>
#include <stdlib.h>
#define BLOCK_SIZE 16
#define ROUNDS 5
#define KEY_SIZE 32

unsigned short sbox[16] = {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7};      //HARD CODED SBOX
unsigned short sbox_inv[16] = {14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5};  //HARD CODED INVERSE OF SBOX
unsigned short pbox_rev[16] = {5,8,14,7,10,0,12,2,15,6,13,3,9,4,1,11};      //HARD CODED PERMUTATIONS BOX
unsigned short pbox[16] = {5,14,7,11,13,0,9,3,1,12,4,15,6,10,2,8};  //HARD CODED INV_PERMUTATION BOX

unsigned short xor(unsigned short i,unsigned short j) {
  if(i!=j) return 1;
  return 0;
}
void substitution(char *text,unsigned short *values);
void permutation(char *text,unsigned short *values);
void keySchedule(char *key,unsigned short *keys);
void round_encrypt(unsigned short *keys,char *text);
void round_decrypt(unsigned short *keys,char *text);
void encrypt(unsigned short *keys);
void decrypt(unsigned short *keys);

void main() {
  int option;
  char key[33];
  printf("Enter the 32 bit key (only binary key allowed) - \n");
  scanf("%s",key);
  unsigned short keys[ROUNDS];
  keySchedule(key,keys);
  printf("\noption = 0,exit\n");
  printf("\noption = 1,take a plain text(BINARY digits's only) return the cipher text and come to loop again\n");
  printf("\noption = 2,take a cipher text(BINARY digit's only) return the plain text and come to loop again\n");
  option=1;
  while(option!=0) {
    printf("\n\nEnter a option - ");
    scanf("%d",&option);
    if(option==0) break;
    if(option==1) encrypt(keys);
    if(option==2) decrypt(keys);
    if((option<0)||(option>2)) {
        option=1;
        printf("\nEnter option = 0 or 1 or 2 only");
    }
  }
}
void substitution(char *text,unsigned short *values) {
  unsigned short input = 0,x;
  int i,j;
  for(i=0;i<BLOCK_SIZE;i+=4) {
    x = 0;
    for(j=i;j<i+4;j++) {
      x *= 2;
      x += text[j]-'0';
    }
    x = values[x];
    input *= 16;
    input += x;
  }
  for(i=BLOCK_SIZE-1;i>=0;i--) {
    text[i] = (input%2) + '0';
    input /= 2;
  }
}
void permutation(char *text,unsigned short *values){
  unsigned short i,m;
  char word[BLOCK_SIZE];
  for(i=0;i<BLOCK_SIZE;i++) word[i] = text[i];
  for(i=0;i<BLOCK_SIZE;i++) {
    m = values[i];
    text[i] = word[m];
  }
}
void keySchedule(char *key,unsigned short *keys) {
  unsigned short i,j,x,initial;
  for(i=0;i<11;i++) {   // 11 = block_size-sbox_size-1 = 16-4-1
    initial += key[i] - '0';
    initial *= 2;
  }
  initial += key[i];
  for(i=0;i<5;i++) {    // 5 = no of rounds
    x = 0;
    for(j=4*i+12;j<4*i+16;j++) {
      x *= 2;
      x += key[j] - '0';
    }
    initial *= 16;
    keys[i] = initial + x;
    initial = keys[i];
  }
}
void round_encrypt(unsigned short *keys,char *text) {
  unsigned short key;
  int i,j;
  for(j=0;j<ROUNDS-1;j++) {
    key = keys[j];
    //XOR with the key
    for(i=BLOCK_SIZE-1;i>=0;i--) {
      text[i] = xor(text[i]-'0',key%2) + '0';
      key /= 2;
    }
    //SBOX Substitution
    substitution(text,sbox);
    //Permutation
    if(j!=ROUNDS-2) permutation(text,pbox);
  }
  key = keys[j];
  //XOR with the key
  for(i=BLOCK_SIZE-1;i>=0;i--) {
    text[i] = xor(text[i]-'0',key%2) + '0';
    key /= 2;
  }
}
void round_decrypt(unsigned short *keys,char *text) {
  unsigned short key;
  int i,j;
  j = ROUNDS-1;
  key = keys[j];
  //XOR with the key
  for(i=BLOCK_SIZE-1;i>=0;i--) {
    text[i] = xor(text[i]-'0',key%2) + '0';
    key /= 2;
  }
  for(j=ROUNDS-2;j>=0;j--) {
    //Reversing the permutation
    if(j!=ROUNDS-2) permutation(text,pbox_rev);
    //Inverting the Substitution
    substitution(text,sbox_inv);
    //XOR with the Key
    key = keys[j];
    for(i=BLOCK_SIZE-1;i>=0;i--) {
      text[i] = xor(text[i]-'0',key%2) + '0';
      key /= 2;
    }
  }
}
void encrypt(unsigned short *keys) {
  int msg,i,j;
  char *message,text[BLOCK_SIZE];
  printf("Enter the size of plain text (it should be a multiple of 16 bits only) - ");
  scanf("%d",&msg);
  if(msg%16 != 0) {
    printf("\nThe size should be a multiple of 16 bits only ! try again\n");
    return;
  }
  message = (char *) malloc((msg+1)*sizeof(char));
  printf("\nEnter the Plain text (should consist of only BINARY digit's) - \n");
  scanf("%s",message);
  for(i=0;i<msg;i+=BLOCK_SIZE) {
    for(j=i;j<i+BLOCK_SIZE;j++) text[j-i] = message[j];
    round_encrypt(keys,text);
    for(j=i;j<i+BLOCK_SIZE;j++) message[j] = text[j-i];
  }
  printf("\nThe Cipher text is - \n%s\n\n",message);
}
void decrypt(unsigned short *keys) {
  int msg,i,j;
  char *message,text[BLOCK_SIZE];
  printf("Enter the size of cipher text (it should be a multiple of 16 bits only) - ");
  scanf("%d",&msg);
  if(msg%16 != 0) {
    printf("\nThe size should be a multiple of 16 bits only ! try again\n");
    return;
  }
  message = (char *) malloc((msg+1)*sizeof(char));
  printf("\nEnter the Cipher text (should consist of only BINARY digit's) - \n");
  scanf("%s",message);
  for(i=0;i<msg;i+=BLOCK_SIZE) {
    for(j=i;j<i+BLOCK_SIZE;j++) text[j-i] = message[j];
    round_decrypt(keys,text);
    for(j=i;j<i+BLOCK_SIZE;j++) message[j] = text[j-i];
  }
  printf("\nThe plain text is - \n%s\n\n",message);
}





