/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 300000
#define ALPHA_LEN 26

/* TODO: structure definitions */

struct triNode
{
  int isLeaf;                          // a leaf checker, contains no children if  == 0.
  int count;                           // keeps track of word occurance.
  int node_count;                      // count the number of node
  char letter;                         // letter it been pointed
  struct triNode *children[ALPHA_LEN]; // up to 26. one for each letter.
};

/* NOTE: int return values can be used to indicate errors
(typically non-zero) or success (typically zero return value) */

/* TODO: change this return type */
struct triNode *indexPage(const char *url);
struct triNode *node_create(const char c);
int addWordOccurrence(const char *word, const int wordLength, struct triNode *root /* TODO: other parameters you need */);
void printTrieContents(const struct triNode *node, char word[], int index);
void freeTrieMemory(struct triNode *node);

// should not have to change.
int getText(const char *srcAddr, char *buffer, const int bufSize);

// starts
int main(int argc, char **argv)
{
  /* argv[1] will be the URL to index, if argc > 1 */
  if (argc < 2)
  {
    fprintf(stderr, "ERROR: argc is not less than 2\n");
    return 0;
  }
  // First step. Getting the triNode from indexpage().
  // Structure pointer need to be malloc before use.
  struct triNode *root = indexPage(argv[1]);

  // Second step. Print the contents.
  int index = 0;
  char word[MAX_STR_LEN];
  printTrieContents(root, word, index);

  // Last Step. Free memory.
  freeTrieMemory(root);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */

/* TODO: change this return type */
struct triNode *indexPage(const char *url)
{
  // Get text from URL
  char buffer[MAX_STR_LEN];
  char word[MAX_STR_LEN];
  getText(url, buffer, MAX_STR_LEN);

  // Create root node
  struct triNode *root = node_create(' ');

  // If buffer is ture
  for (int i = 0, j = 0; i < strlen(buffer); i++)
    if (isalpha(buffer[i]))
      word[j++] = buffer[i];
    else
    {
      if (!j)
      {
        word[j] = '\0';
        addWordOccurrence(word, j, root);
      }
      j = 0;
    }

  return root;
}

// Standard procedure to create empty node.
struct triNode *node_create(const char c)
{

  struct triNode *root = malloc(sizeof(struct triNode));
  root->count = 0;
  root->node_count = 0;
  root->isLeaf = 1;
  root->letter = c;
  for (int i = 0; i < ALPHA_LEN; i++)
    root->children[i] = NULL;

  return root;
}

int addWordOccurrence(const char *word, const int wordLength, struct triNode *root)
{
  struct triNode *pointer = root;
  int index = 0;

  while (index < wordLength)
  {
    // Check if the node is contained
    struct triNode *child;
    int contains = 0;
    for (int i = 0; i < pointer->node_count && !contains; i++)
    {
      struct triNode *child = pointer->children[i];
      if (child != NULL && child->letter == word[index]) // If all node created using indexPage::node_create, children[] elements been iterated should not be NULL.
        contains = 1;
    }

    if (!contains)
    {
      // create a new node
      pointer->children[pointer->node_count] = node_create(word[index]);
      pointer->node_count++;
      pointer->isLeaf = 0;

      child = pointer->children[pointer->node_count];
    }

    pointer = child;
    index++;
  }

  pointer->count++;

  return 0;
}

// Prints contents starting at root.
void printTrieContents(const struct triNode *node, char word[], int index)
{
  if(node->isLeaf == 1)
  {
    word[index] = '\0';
    printf("%s: %d\n", word, node->count);
  }
  else if(node->node_count < 1 && node->isLeaf == 0)
  {
    word[index] = '\0';
    printf("%s: %d\n", word, node->count);
  }
  if(node->isLeaf == 0)
  {
    for (int i = 0; i < ALPHA_LEN; i++)
    {
      for (int j = 0; j < node->node_count; j++)
      {
        if(node->children[j]->letter == 'a' + i 
           && node->children != NULL)
        {
          word[index] = node->children[j]->letter;
          printTrieContents(node->children[j], word, index + 1);
        }
      }
    }
  }
}


// Frees the memory starting at root.
void freeTrieMemory(struct triNode *node)
{
  if (node != NULL)
  {
    for (int x = 0; x < 26; x++)
      if (node->children[x] != NULL)
        freeTrieMemory(node->children[x]);
    free(node);
  }
}

/* You should not need to modify this function */
int getText(const char *srcAddr, char *buffer, const int bufSize)
{
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if (pipe == NULL)
  {
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
            buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize - 1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);
  

  return bytesRead;
}
