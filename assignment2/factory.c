#include <stdio.h>
#include <stdlib.h>
#include "factory.h"
#include "packers.c"
#include "assemblers.c"
#include <pthread.h>

/**
   CMPUT379 Assignment 2
   Author: Connor Resler
   Comments: Was not able to implement main functionality of the program. It will compile with
   no errors but not actually do anything. I managed to make a lot of the setup with comments, however
   unfortunately ran out of time for figuring out how to implement the main functionality.
**/
//Colors
const char *COLORS[] = {"AliceBlue","AntiqueWhite","Aqua","Aquamarine","Azure","Beige","Bisque","Black","BlanchedAlmond","Blue","BlueViolet","Brown","BurlyWood","CadetBlue","Chartreuse","Chocolate","Coral","CornflowerBlue","Cornsilk","Crimson","Cyan","DarkBlue","DarkCyan","DarkGodenRod","DarkGray","DarkGrey","DarkGreen","DarkKhaki","DarkMagenta","DarkOliveGreen","Darkorange","DarkOrchid","DarkRed","DarkSalmon","DarkSeaGreen","DarkSlateBlue","DarkSlateGray","DarkSlateGrey","DarkTurquoise","DarkViolet","DeepPink","DeepSkyBlue","DimGray","DimGrey","DodgerBlue","FireBrick","FloralWhite","ForestGreen","Fuchsia","Gainsboro","GhostWhite","God","GodenRod","Gray","Grey","Green","GreenYellow","HoneyDew","HotPink","IndianRed","Indigo","Ivory","Khaki","Lavender","LavenderBlush","LawnGreen","LemonChiffon","LightBlue","LightCoral","LightCyan","LightGodenRodYellow","LightGray","LightGrey","LightGreen","LightPink","LightSalmon","LightSeaGreen","LightSkyBlue","LightSlateGray","LightSlateGrey","LightSteelBlue","LightYellow","Lime","LimeGreen","Linen","Magenta","Maroon","MediumAquaMarine","MediumBlue","MediumOrchid","MediumPurple","MediumSeaGreen","MediumSlateBlue","MediumSpringGreen","MediumTurquoise","MediumVioletRed","MidnightBlue","MintCream","MistyRose","Moccasin","NavajoWhite","Navy","OdLace","Olive","OliveDrab","Orange","OrangeRed","Orchid","PaleGodenRod","PaleGreen","PaleTurquoise","PaleVioletRed","PapayaWhip","PeachPuff","Peru","Pink","Plum","PowderBlue","Purple","Red","RosyBrown","RoyalBlue","SaddleBrown","Salmon","SandyBrown","SeaGreen","SeaShell","Sienna","Silver","SkyBlue","SlateBlue","SlateGray","SlateGrey","Snow","SpringGreen","SteelBlue","Tan","Teal","Thistle","Tomato","Turquoise","Violet","Wheat","White","WhiteSmoke","Yellow","YellowGreen"};
#define MAXCOLORS 147
//int colorIndex = 0;
//int indexModifier = 0;
packer *packerList;
assembler *assemblerList;
pthread_mutex_t lock;
pthread_t *assemblerThreadID;
pthread_t *packerThreadID;
node *head;

int productID = 0;

/**
   @brief Place product onto assembly line
   @param *arg The assembler to place a product
   @return The assembler
**/
void *placeProduct(void *arg){
  pthread_mutex_lock(&lock);
  //Do stuff
  pthread_mutex_unlock(&lock);
  return arg;
}

/**
   @brief Take product from assembly line
   @param *arg The packer to take product
   @return The packer
**/
void *takeProduct(void *arg){
  pthread_mutex_lock(&lock);
  //Do stuff
  pthread_mutex_unlock(&lock);
  return arg;
}

/**
   @brief Print contents of a given packer's box
   @param *p The packer whose box contents should be printed
**/
void printBoxContents(packer *p)
{
  printf("Box includes product numbers: \n");
  for(int i = 0; i < p->n; i++)
  {
    printf("%d, ", p->boxContents[i].id);
  }
  printf("\n");
}

/**
   @brief Return true if given packers box is full
   @param *p packer whose box should be checked
   @return 0 if box is not full, 1 if box is full
**/
int boxFull(packer *p){
  if(p->boxCount == p->n)
  {
    printBoxContents(p);
    return 1;
  }
  return 0;
}

//Return true if assembler is done creating
int finishedCreating(assembler *a)
{
  if(a->productsCreated == a->c)
  {
    return 1;
  }
  return 0;
}

/**
   @breif Return true if assembly line is full
   @param length Length of assembly line
   @return 0 if assembly line is not full, 1 if full
**/
int lineFull(int length)
{
  node *current = head;
  int initialID = current->data->id;
  for(int i = 0; i < length; i++){
    if(current->next){ 
      current = current->next;
    }
    //If the initial ID is reached again, whole list has been searched
    if(current->data->id == initialID)
    {
      return 1;
    }
    //If an id < 0 is found, this means there is an empty spot
    if(current->data->id < 0)
    {
      return 0;
    }
  }
  return 0;
}

/**
   @brief run main program to manage factory
   @param a Number of assmembler threads
   @param p Number of packer threads
   @param l Size of assembly line
   @param n Number of products per box
   @param c Number of products each assembler will produce
**/
int main(int argc, char *argv[])
{
  if(argc != 6)
  {
    printf("Please enter ./factory a p l n c\n");
    exit(-1);
  }
  int a = atoi(argv[1]);
  int p = atoi(argv[2]);
  int l = atoi(argv[3]);
  int n = atoi(argv[4]);
  int c = atoi(argv[5]);

  //Create assembler list
  assemblerList = (assembler*)malloc(sizeof(assembler) * a);
  for(int i = 0; i < a; i++)
  {
    //if(colorIndex == MAXCOLORS)
      //{
      //colorIndex = 0;
      //}
    //Set color of assembler
    //assemblerList[i].color += COLORS[colorIndex];
    assemblerList[i].c = c;
    assemblerList[i].productsCreated = 0;
  }

  //Create packer list
  packerList = (packer*)malloc(sizeof(packer) * p);
  for(int i = 0; i < p; i++)
  {
    packerList[i].n = n;
    //Create packer's box of items
    packerList[i].boxContents = (product*)malloc(sizeof(product) * n);
    packerList[i].boxCount = 0;
    //Initialize all items in packer's box
    for(int j = 0; j < n; j++)
    {
      packerList[i].boxContents[j].id = -1;
      //packerList[i].boxContents[j].color = "";
    }
  }

  //Create linked list for assembly line
  node *current;
  head = NULL;
  //Create linked list of products, intialize values
  for(int i = 0; i < l; i++){
    current = (node*)malloc(sizeof(node));
    current->data = (product*)malloc(sizeof(product));
    current->data->id = -1;
    current->next = head;
    head = current;
  }
  current = head;

  //Find last node, link to first one to create circular list
  for(int i = 0; i < l; i++){
    if(current->next){ 
      current = current->next;
    }
  }
  current->next = head;

  current = head;

  node *prev = head;
  //Cleanup
  for(int i = 0; i < l - 1; i++)
  {
    free(current->data);
    prev = current;
    current = current->next;
    free(prev);
  }
  free(current);

  for(int i = 0; i < p; i++)
  {
    free(packerList[i].boxContents);
  }
  free(packerList);
  
  return 0;
}
