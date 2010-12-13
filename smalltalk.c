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
  Callback Method call when a class definition is founded
 */
static void
smalltalkClass (const char *const, const regexMatch *const,
				const unsigned int);



/* DATA DEFINITIONS */
typedef enum eSmalltalkKinds {
  K_CLASS,
  K_METHOD,
} smalltalkKind;

static kindOption SmalltalkKinds [] = {
  {TRUE, 'c', "class", "Classes"},
  {TRUE, 'm', "method", "Object's methods"}
}


/* Create parser definition stucture */
extern parserDefinition* 
SmalltalkParser (void)
{
    static const char *const extensions [] = { "st", NULL };
    parserDefinition* def = parserNew ("Smalltalk");
    def->kinds      = smalltalkKinds;
    def->kindCount  = KIND_COUNT (smalltalkKinds);
    def->extensions = extensions;
    def->parser     = findSmalltalkTags;
	def->initialize = installSmalltalk;
    return def;
}

static void
installSmalltalk (const langType language)
{
  addCallbackRegex (language, "^[A-Z][A-Za-z0-9_]+[ \t]subclass:[ \t]([A-Z][A-Za-z0-9_]+)[[ \t]", NULL, smalltalkClass);
}

static void
smalltalkClass (const char *const line, const regexMatch *const matches,
				const unsigned int cound)
{
  if (count > 1) /* should always be true per regex */
	{
	  vString *const name = vStringNew ();
	  vStringNCopyS (name, line + matches [1].start, matches [1].length);
	  makeSimpleTag (name, SmalltalkKinds, K_CLASS);
	}
}

static void
findSmalltalkTags (void)
{
  while (fileReadLine () != NULL)
	; /* All is done in a callback */

