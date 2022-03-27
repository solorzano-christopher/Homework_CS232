/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* TODO: structure definitions */

typedef struct triNode
{
  int isLeaf; // a leaf checker, contains no children if  == 0.
  int count; //keeps track of word occurance. 
  struct trieNode *children[26]; // up to 26. one for each letter.
}node;

node* newNode()
{
  node* head = (node*)malloc(sizeof(node));
  head->isLeaf = 0;
  head->count = 0;
  for(int x = 0; x < 26; x++)
  {
    head->children[x] = NULL;
  }
  return head;
}

/* NOTE: int return values can be used to indicate errors 
(typically non-zero) or success (typically zero return value) */

/* TODO: change this return type */
void indexPage(const char* url);
int addWordOccurrence(const char* word, const int wordLength/* TODO: other parameters you need */);
void printTrieContents(/* TODO: any parameters you need */);
int freeTrieMemory(/* TODO: any parameters you need */);

//should not have to change.
int getText(const char* srcAddr, char* buffer, const int bufSize);


//starts
int main(int argc, char** argv){
  /* TODO: write the (simple) main function

  /* argv[1] will be the URL to index, if argc > 1 */
  node* head = newNode();



  //Second to last step. Print the contents. 
  printTrieContents(head);

  //Last Step. Free memory.
  freeTrieMemory(head);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */

/* TODO: change this return type */
void indexPage(const char* url)
{
  //todo
}

int addWordOccurrence(const char* word, const int wordLength/* TODO: other parameters you need */)
{
  //todo
}

void printTrieContents(node* thisNode)
{
  
}

int freeTrieMemory(node* thisNode)
{
    if(thisNode != NULL)
  {
    for (int x = 0; x < 26; x++)
    {
      if(thisNode->children[x] != NULL)
      {
        freeTrieMemory(thisNode->children[x]);
      }
    }
    free(thisNode);
    thisNode = NULL;
  }
}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
