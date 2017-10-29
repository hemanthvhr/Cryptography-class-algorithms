#include <iostream>
#define ROUNDS 16
#define MSG_SIZE 64
#define KEY_SIZE 64
#define EKEY_SIZE 56
#define ROUND_KEY 48
//0000000100100011010001010110011110001001101010111100110111101111
int leftshifts[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
int totShift[16] =   {1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28};
int PC1[56] =  {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51,
                  43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7,
                  62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20,
                  12, 4};
int PC2[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
                 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
                 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

int PERMUTE[32] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
int InitialP[64] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54,
                      46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41,
                      33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29,
                      21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
int ReverseP[64] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6,
 					  46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12,
 					  52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18,
                      58, 26, 33, 1, 41, 9, 49, 17, 57, 25};
int Sbox[8][64] = {
                                {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4,
                                 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 4, 1, 14, 8, 13, 6, 2, 11,
                                 15, 12, 9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14,
                                 10, 0, 6, 13},
                                {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7,
                                 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 0, 14, 7, 11, 10, 4, 13, 1,
                                 5, 8, 12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12,
                                 0, 5, 14, 9},
                                {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9,
                                 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 13, 6, 4, 9, 8, 15, 3, 0,
                                 11, 1, 2, 12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3,
                                 11, 5, 2, 12},
                                {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5,
                                 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 10, 6, 9, 0, 12, 11, 7, 13,
                                 15, 1, 3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12,
                                 7, 2, 14},
                                {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12,
                                 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 4, 2, 1, 11, 10, 13, 7, 8, 15,
                                 9, 12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10,
                                 4, 5, 3},
                                {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7,
                                 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 9, 14, 15, 5, 2, 8, 12, 3, 7, 0,
                                 4, 10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
                                {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4,
                                 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 1, 4, 11, 13, 12, 3, 7, 14, 10, 15,
                                 6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
                                {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10,
                                 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10,
                                 13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
                             };
int Expansion[48] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13,
                      12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22,
                      23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};

using namespace std;

int xored(int i,int j) {
    if(i==j) return 0;
    return 1;
}

void permute(int *input,int *table,int *output,int n) {
  int *temp = new int[n];
  for(int i=0;i<n;i++) {
    temp[i] = input[table[i]-1];
  }
  for(int i=0;i<n;i++) {
    output[i] = temp[i];
  }
  delete[] temp;
}

void keySchedule(int *key,int **keys) {
  int ekey[56]; //initial effective key
  int buffer[56];
  permute(key,PC1,ekey,56); //PC-1 permutation
  for(int i=0;i<ROUNDS;i++) {
    for(int j=0;j<56;j++) {
        int k = j/28;
        buffer[j] = ekey[28*k + ((j-k*28) + totShift[i])%28]; //circular left shift
    }
    permute(buffer,PC2,keys[i],48); //PC-2 permutation
  }
}

class keySystem {
    int key[KEY_SIZE];
public:
    int *keys[ROUNDS];
    keySystem(string& keyer) {
        for(int i=0;i<ROUNDS;i++) keys[i] = new int[ROUND_KEY];
        for(int i=0;i<keyer.size();i++) key[i] = keyer.at(i) -'0' ;
        keySchedule(key,keys);
    }
    ~keySystem() {for(int i=0;i<ROUNDS;i++) delete[] keys[i];}
};

void funcE(int *right,int *key) {
  int buffer[48];
  permute(right,Expansion,buffer,48);   //Expansion from 32 bits to 48 bits
  for(int i=0;i<48;i++) {               //xored with the key
    buffer[i] = xored(buffer[i],key[i]);
  }
  for(int i=0;i<8;i++) {                //for 8 S-boxes
    int d;
    int x = 2*buffer[6*i] + buffer[6*i+5];
    int y = 8*buffer[6*i+1] + 4*buffer[6*i+2] + 2*buffer[6*i+3] + buffer[6*i+4];
    d = Sbox[i][16*x+y];
    for(int j=4*i+3;j>=4*i;j--) {
      right[j] = d%2;
      d /= 2;
    }
  }
  permute(right,PERMUTE,right,32);      //permutation of 32 bits
}

void f_encrypt(int *message,int **keys) {       //fiestel encryption 16 rounds
  int buffer[64];
  int left[32],right[32];
  permute(message,InitialP,buffer,MSG_SIZE);    //Initial Permutation
  for(int i=0;i<ROUNDS;i++) {
    for(int j=0;j<32;j++) {
      left[j] = buffer[j];
      right[j] = buffer[j+32];
      buffer[j] = right[j];
    }
    funcE(right,keys[i]);                       //the transforming function f with key of round i
    for(int j=0;j<32;j++) {
        buffer[j+32] = xored(left[j],right[j]);
    }
  }
  for(int i=0;i<32;i++) {                       //penultimate step of left,right swap
    int temp = buffer[i];
    buffer[i] = buffer[i+32];
    buffer[i+32] = temp;
  }
  permute(buffer,ReverseP,message,64);          //Inverse initial permutation
}
void f_decrypt(int *cipher,int **keys) {      //fiestel decryption 16 rounds
  int buffer[64];
  int left[32],right[32];
  permute(cipher,InitialP,buffer,MSG_SIZE);    //Initial Permutation
  for(int i=0;i<32;i++) {                       //penultimate step of left,right swap
    int temp = buffer[i];
    buffer[i] = buffer[i+32];
    buffer[i+32] = temp;
  }
  for(int i=ROUNDS-1;i>=0;i--) {
    for(int j=0;j<32;j++) {
      left[j] = buffer[j];
      right[j] = buffer[j+32];
      buffer[j+32] = left[j];
    }
    funcE(left,keys[i]);                       //the transforming function f with key of round i
    for(int j=0;j<32;j++) {
        buffer[j] = xored(left[j],right[j]);
    }
  }
  permute(buffer,ReverseP,cipher,64);          //Inverse initial permutation
}

void encrypt(keySystem& key1,keySystem& key2) {
  string msg;
  cout << "Enter a message whose length is " << MSG_SIZE << endl;
  cin >> msg;
  while(msg.size()!= MSG_SIZE) {
    cout << "Enter a valid message of " << MSG_SIZE << " bits" << endl;
    cin >> msg;
  }
  int msge[64];
  for(int i=0;i<64;i++) msge[i] = msg.at(i) - '0';
  f_encrypt(msge,key1.keys);
  f_decrypt(msge,key2.keys);
  f_encrypt(msge,key1.keys);
  for(int i=0;i<64;i++) msg.at(i) = msge[i] + '0';
  cout << endl << "The cipher text is - " << endl;
  cout << msg << endl;
}

void decrypt(keySystem& key1,keySystem& key2) {
  string msg;
  cout << "Enter a cipher whose length is " << MSG_SIZE << endl;
  cin >> msg;
  while(msg.size()!= MSG_SIZE) {
    cout << "Enter a valid cipher of " << MSG_SIZE << " bits" << endl;
    cin >> msg;
  }
  int msge[64];
  for(int i=0;i<64;i++) msge[i] = msg.at(i) - '0';
  f_decrypt(msge,key1.keys);
  f_encrypt(msge,key2.keys);
  f_decrypt(msge,key1.keys);
  for(int i=0;i<64;i++) msg.at(i) = msge[i] + '0';
  cout << endl << "The plain text is - " << endl;
  cout << msg << endl;
}

int main() {
  cout << "                                    Triple DES System" << endl << endl;
  string key;
  do {
  cout << "\nEnter a valid " << 2*KEY_SIZE << "-bit key" << endl;
  cin >> key;
  } while(key.size()!=2*KEY_SIZE);
  string first = key.substr(0,KEY_SIZE);
  string second = key.substr(KEY_SIZE,2*KEY_SIZE);
  keySystem key1(first);
  keySystem key2(second);
  cout << endl << "option = 0,exit" << endl;
  cout << endl << "option = 1,take a plain text(BINARY digits's only) return the cipher text and come to loop again" << endl;
  cout << endl << "option = 2,take a cipher text(BINARY digits's only) return the plain text and come to loop again" << endl;
  int option=1;
  while(option!=0) {
    cout << endl << endl << "Enter a option - ";
    cin >> option;
    if(option==0) break;
    if(option==1) encrypt(key1,key2);
    if(option==2) decrypt(key1,key2);
    if((option<0)||(option>2)) {
        option=1;
        cout << endl << "Enter option = 0 or 1 or 2 only";
    }
  }
  return 0;
}
