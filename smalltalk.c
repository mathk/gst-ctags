/*
*   Copyright (c) 2009, Mathieu Suen
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   This module contains functions for generating tags for Objective Caml
*   language files.
*/
/*
*   INCLUDE FILES
*/
#include "general.h"	/* must always come first */

#include <string.h>
#include <stdbool.h>

#include "parse.h"      /* always include */
#include "read.h"       /* to define file fileReadLine() */

/*
  Main function that walk throw the file
 */
static void 
findSmalltalkTags (void);

/*
  Install regexp callback
 */
static void
installSmalltalk(const langType);

/*
  Callback function for class definition
 */
static void
smalltalkClass (const char *const, const regexMatch *const,
				const unsigned int);

/*
  Callback function for keyword method definition
 */
static void
smalltalkKeywordMethod (const char *const, const regexMatch *const,
				const unsigned int);

/*
  Callback method for unary method declaration
 */
static void
smalltalkUnaryMethod (const char *const, const regexMatch *const,
				const unsigned int);


/* DATA DEFINITIONS */
typedef enum eSmalltalkKinds {
  K_CLASS,
  K_METHOD,
} smalltalkKind;

static kindOption SmalltalkKinds [] = {
  {TRUE, 'c', "class", "Classes"},
  {TRUE, 'm', "method", "Object's methods"}
};


/* Create parser definition stucture */
extern parserDefinition* 
SmalltalkParser (void)
{
    static const char *const extensions [] = { "st", NULL };
    parserDefinition* def = parserNew ("Smalltalk");
    def->kinds      = SmalltalkKinds;
    def->kindCount  = KIND_COUNT (SmalltalkKinds);
    def->extensions = extensions;
    def->parser     = findSmalltalkTags;
	def->initialize = installSmalltalk;
    return def;
}

static void
installSmalltalk (const langType language)
{
  addCallbackRegex (language, "^[ \t]*[A-Z][A-Za-z0-9_]+[ \t]subclass:[ \t]([A-Z][A-Za-z0-9_]+)[[ \t]", NULL, smalltalkClass);
  addCallbackRegex (language, "^[ \t]*([A-Za-z_][A-Za-z0-9_]*)[ \t]*[[]", NULL, smalltalkUnaryMethod);
  addCallbackRegex (language, "^[ \t]*([A-Za-z_][A-Za-z0-9_]*:[ \t]?[A-Za-z_][A-Za-z0-9_]*[ \t]?)+[[]", NULL, smalltalkKeywordMethod);
}

static void
smalltalkClass (const char *const line, const regexMatch *const matches,
				const unsigned int count)
{
  if (count > 1) /* should always be true per regex */
	{
	  vString *const name = vStringNew ();
	  vStringNCopyS (name, line + matches [1].start, matches [1].length);
	  makeSimpleTag (name, SmalltalkKinds, K_CLASS);
	}
}

static void
smalltalkUnaryMethod (const char *const line, const regexMatch *const matches,
				const unsigned int count)
{
  if (count > 1) /* should always be true per regex */
	{
	  vString *const name = vStringNew ();
	  vStringNCopyS (name, line + matches [1].start, matches [1].length);
	  makeSimpleTag (name, SmalltalkKinds, K_METHOD);
	}
}

static vString *const 
nextToken(const char ** pcurrentChar)
{
  vString *const token = vStringNew ();
  int next = (int)*pcurrentChar[0];
  *pcurrentChar = *pcurrentChar + 1;
  while (next == ' ' || next == '\t')
	{
	  next = (int)*pcurrentChar[0];
	  *pcurrentChar = *pcurrentChar + 1;
	}

  if (next == '\0')
	return token;

  while (next != ' ' && next != '\t' && next != '\0')
	{
	  vStringPut(token, next);
	  next = (int)*pcurrentChar[0];
	  *pcurrentChar = *pcurrentChar + 1;
	}
  return token;
}


static void
smalltalkKeywordMethod (const char *const line, const regexMatch * const matches,
				const unsigned int count)
{
  const char* currentChar = line;
  if (count > 1) /* should always be true per regex */
	{
	  vString *const name = vStringNew ();
	  vString * token = nextToken (&currentChar);
	  while (vStringLength (token) != 0)
		{
		  if (vStringLast (token) == ':')
			vStringCatS(name, vStringValue(token));

		  token = nextToken (&currentChar);
		}
	  makeSimpleTag (name, SmalltalkKinds, K_METHOD);
	}
}

static void
findSmalltalkTags (void)
{
  while (fileReadLine () != NULL)
	; /* All is done in a callback */
}

