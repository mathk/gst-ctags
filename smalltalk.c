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

static vString 
nextToken(vString line)
{
  
}


static void
smalltalkKeywordMethod (const char *const line, const regexMatch * const matches,
				const unsigned int count)
{
  bool copy = true;
  if (count > 2) /* should always be true per regex */
	{
	  vString *const matchString = vStringNew ();
	  vString *const name = vStringNew ();
	  vStringNCopyS (matchString, line + matches [0].start, matches [0].length);
	  vStringStripLeading (matchString);
	  vStringStripTrailing (matchString);
	  for(int i = 0; i < matchString.length; i++)
		{
		  if (copy)
			{
			  vStringPut (name, vStringChar (matchString, i));
			}
		  if (vStringItem (matchString, i) == ':')
			{
			  
			}
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

