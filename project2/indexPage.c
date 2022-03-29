/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* TODO: structure definitions */

struct triNode
{
  int isLeaf; // a leaf checker, contains no children if  == 0.
  int count; //keeps track of word occurance. 
  char letter; //letter it contains
  struct trieNode *children[26]; // up to 26. one for each letter.
};

/* NOTE: int return values can be used to indicate errors 
(typically non-zero) or success (typically zero return value) */

/* TODO: change this return type */
struct triNode indexPage(const char* url);
int addWordOccurrence(const char* word, const int wordLength/* TODO: other parameters you need */);
void printTrieContents(const struct triNode *node, char word[], int index);
int freeTrieMemory(const struct triNode *node);

//should not have to change.
int getText(const char* srcAddr, char* buffer, const int bufSize);


//starts
int main(int argc, char** argv)
{
  /* argv[1] will be the URL to index, if argc > 1 */
  if(argc < 2)
  {
    fprintf(stderr, "ERROR: argc is not less than 2\n");
    return 0;
  }
  //First step. Getting the triNode from indexpage().
  struct triNode *root = indexPage(argv[1]);

  //Second to last step. Print the contents. 
  int index = 0;
  char word[100];
  printTrieContents(root, word, index);

  //Last Step. Free memory.
  freeTrieMemory(root);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */

/* TODO: change this return type */
struct triNode indexPage(const char* url)
{
  //Get the rest from LK
  //Just to remove error, do not use below!
  struct triNode *root;
  root = malloc(sizeof(struct triNode));
  root->letter = "\0";
  root->isLeaf = 0;
  root->count = 0;
  for(int x = 0; x < 26; x++)
  {
    root->children[x] = NULL;
  }
  return *root;
}

int addWordOccurrence(const char* word, const int wordLength/* TODO: other parameters you need */)
{
  //Get from LK
}

//Prints contents starting at root.
void printTrieContents(const struct triNode *node, char word[], int index)
{
  if(node->isLeaf == 0)
  {
    for (int x = 0; x < 26; x++)
    {
      if(node->children != NULL)
      {
        word[index] = node->letter;
        printTrieContents(node->children[x], word, index +1);
      }
    }
  }
  if(node->isLeaf == 1)
  {
    word[index] = '\0';
    printf("%s %d\n", word, node->count);
  }
}

//Frees the memory starting at root.
int freeTrieMemory(const struct triNode *node)
{
    if(node != NULL)
  {
    for (int x = 0; x < 26; x++)
    {
      if(node->children[x] != NULL)
      {
        freeTrieMemory(node->children[x]);
      }
    }
    free(node);
    node = NULL;
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
