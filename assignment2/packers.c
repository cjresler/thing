#include <stdio.h>
#include <stdlib.h>
#include "factory.h"

struct packer{
  int n; //max number of products per box
  int boxCount; // number of things currently in box
  struct product *boxContents;
};
