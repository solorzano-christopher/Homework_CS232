#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ADDR_LENGTH 1000

/*
 * a node in our linked-list of web addresses
 */
struct listNode{
  char addr[MAX_ADDR_LENGTH];

  struct listNode *next; //*link
};

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr);

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr);

/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *pNode);

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode);
  
/*
 * srcAddr should be a web address (e.g., http://www.yahoo.com).
 * link should point to an array of maxLinkLength characters.
 * getLink returns 1 if a link was found and 0 otherwise.
 * If a link was found, "link" will be filled in with the web address.
 */
int getLink(const char* srcAddr, char* link, const int maxLinkLength);

int main(int argc, char** argv){
  
  long seed; // seed value 

  char startAddr[MAX_ADDR_LENGTH]; //starting address
  char destAddr[MAX_ADDR_LENGTH]; //the destination address
  
  int hopNum, numHops; //initializing hopping int
  
  struct listNode *pListStart; //head

  if(argc < 3 || argc > 4){
    fprintf(stderr, "USAGE: %s startAddr numHops [rand seed]", argv[0]);
    return -1;
  }
  
  /* initialization  of the seed value*/
  if(argc >= 4){
    seed = atol(argv[3]);
  }
  else{
    seed = time(NULL);
  }

  printf("seed = %ld\n", seed); //printing seed to screen
  srand(seed); //" "

  strncpy(startAddr, argv[1], MAX_ADDR_LENGTH);
  startAddr[MAX_ADDR_LENGTH - 1] = '\0';

  numHops = atoi(argv[2]); //gets the number of hops 

  pListStart = malloc(sizeof(struct listNode)); //helping create a node
  if(pListStart == NULL){
    fprintf(stderr, "ERROR: could not allocate memory\n");
    return -2;
  }
  strncpy(pListStart->addr, startAddr, MAX_ADDR_LENGTH);
  pListStart->next = NULL; // next == NULL

  /* start the crawling */
  for(hopNum=1; hopNum <= numHops; hopNum++){
    int res = getLink(startAddr, destAddr, MAX_ADDR_LENGTH);

    if(!res){
      printf("Dead end on hop %d: no outgoing links\n", hopNum);
      break;
    }

    /*first statment */ 
    if(contains(pListStart, destAddr)){
      printf("Cycle detected on hop %d: address %s\n", hopNum, destAddr);
      hopNum--; // try again for this hop in the next iteration
    }
    else{
      /*Second statment */ 
      insertBack(pListStart, destAddr);
      strncpy(startAddr, destAddr, MAX_ADDR_LENGTH);
    }
  }
  /*Third statement*/
  printAddresses(pListStart);

  /*Last statement*/
  destroyList(pListStart);

  return 0;
}


/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr){
  //creating a temporary node to be used as a checker
  struct listNode *tempNode = pNode;

  while (tempNode -> next != NULL)
  {
    if(strcmp(tempNode ->addr, addr) ==0)
    {
      return 1;
    }
    tempNode = tempNode -> next;
  }
  return 0;
}
    

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr){
  //creating a temporary node to be added
  struct listNode *temp = pNode;
  //checking to see if the current node of the list is NULL. 
  if( temp -> next == NULL )
  {
    struct listNode *pListEnd = malloc(sizeof(struct listNode));
    strncpy(pListEnd -> addr, addr , strlen(addr) +1);
    pListEnd -> next = pListEnd; //setting the next node to the pListEnd node;
    pListEnd -> next = NULL; //setting the next node value to NULL for future use.
  }
  else
  {
    //checking next node. Cycling through the list. To find the next NULL node.
    insertBack(temp -> next, addr);
  }
}

/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *pNode){
  //creating a temporary node to be printed to screen. 
  struct listNode *tempNode = pNode; 
  if(tempNode != NULL )
  {
    printf("%s\n", tempNode -> addr);
    printAddresses(tempNode -> next);
  }
}

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode){
  struct listNode *tempNode = pNode;
  struct listNode *freeNode;
  while( tempNode != NULL)
  {
    freeNode = tempNode;
    tempNode = tempNode -> next;
    free(freeNode);
  }
}
  


int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
		    break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    
    /* get rid of the newline */
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		    *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}
