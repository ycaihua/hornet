/* teletest.c: TELEMETRY TEST
 *
 * $Id: //info.ravenbrook.com/project/mps/version/1.113/code/teletest.c#1 $
 * Copyright (c) 2001-2013 Ravenbrook Limited.  See end of file for license.
 *
 * .source: The command parser here was taken and adapted from bttest.c.
 */

#include "mpm.h"
#include "mps.h"
#include "mpsavm.h"
#include "testlib.h"
#include "mpslib.h"

#include <stdlib.h>

SRCID(teletest, "$Id: //info.ravenbrook.com/project/mps/version/1.113/code/teletest.c#1 $");


static mps_arena_t arena;


#define MAX_ARGS 3
#define INPUT_BUFFER_SIZE 512

#if (MPS_WORD_WIDTH == 32)
#define WORD_FORMAT "0x%08lx"
#elif (MPS_WORD_WIDTH == 64)
#define WORD_FORMAT "0x%016lx"
#else
#error "Unrecognized word width"
#endif


static mps_word_t args[MAX_ARGS];
static char *stringArg;
static Count argCount;


static void callControl(mps_word_t reset, mps_word_t flip)
{
  mps_word_t old, new;
  old = mps_telemetry_control(reset, flip);
  new = mps_telemetry_control((mps_word_t)0, (mps_word_t)0);

  (void)printf(WORD_FORMAT " -> " WORD_FORMAT "\n", old, new);
}


static void doControl(void)
{
  callControl(args[0], args[1]);
}


static void doRead(void)
{
  mps_word_t old;
  old = mps_telemetry_control((mps_word_t)0, (mps_word_t)0);
      
  (void)printf(WORD_FORMAT "\n", old);
}


static void doSet(void)
{
  callControl(args[0], args[0]);
}


static void doReset(void)
{
  callControl(args[0], (mps_word_t)0);
}


static void doFlip(void)
{
  callControl((mps_word_t)0, args[0]);
}


static void doIntern(void)
{
  mps_word_t id;

  id = mps_telemetry_intern(stringArg);
  (void)printf(WORD_FORMAT "\n", id);
}

static void doLabel(void)
{
  mps_telemetry_label((mps_addr_t)args[0], args[1]);
}

static void doFlush(void)
{
  mps_telemetry_flush();
}

static void doQuit(void)
{
  mps_arena_destroy(arena);
  exit(0);
}


static void doHelp(void)
{
  (void)printf("control <reset> <flip>   -> <old> <new>    Control filter\n"
               "read                     -> <old>          Read filter\n"
               "set <mask>               -> <old> <new>    Set filter\n"
               "reset <mask>             -> <old> <new>    Reset filter\n"
               "flip <mask>              -> <old> <new>    Toggle filter\n");
  (void)printf("intern <string>          -> <id>           Intern string\n"
               "label <address> <id>                       Label address\n"
               "flush                                      Flush buffer\n"
               "help                                       Print this message\n"
               "quit                                       Quit\n");
}


static struct commandShapeStruct {
  const char *name;
  Count int_args;
  mps_bool_t string_arg;
  void (*fun)(void);
} commandShapes[] = {
  {"control", 2, 0, doControl},
  {"read", 0, 0, doRead},
  {"set", 1, 0, doSet},
  {"reset", 1, 0, doReset},
  {"flip", 1, 0, doFlip},
  {"intern", 0, 1, doIntern},
  {"label", 2, 0, doLabel},
  {"flush", 0, 0, doFlush},
  {"help", 0, 0, doHelp},
  {"quit", 0, 0, doQuit},
  {NULL, 0, 0, NULL}
};


typedef struct commandShapeStruct *commandShape;


static void obeyCommand(char *command)
{
  commandShape shape = commandShapes;
  while(shape->name != NULL) {
    const char *csp = shape->name;
    char *p = command;
    while (*csp == *p) {
      csp++;
      p++;
    }
    if ((*csp == 0) && ((*p == '\n') || (*p == ' '))) { /* complete match */
      argCount = 0;
      while ((*p == ' ') && (argCount < shape->int_args)) {
        /* get an argument */
        char *newP;
        mps_word_t word;
        word = (mps_word_t)strtoul(p, &newP, 0);
        args[argCount] = word;
        if (newP == p) { /* strtoul failed */
          printf("couldn't parse an integer argument\n");
          return;
        }
        p = newP;
        ++ argCount;
      }
      if(shape->string_arg) {
        char *end;
        while(*p == ' ')
          ++p;
        for(end = p; *end != '\n'; end++)
          NOOP;
        *end = '\0';
        stringArg = p;
      } else {
        stringArg = NULL;
      }
      if (argCount < shape->int_args) {
        printf("insufficient arguments to command\n");
      } else if (*p != '\n' && stringArg == NULL) {
        printf("too many arguments to command\n");
      } else { /* do the command */
        shape->fun();
      }
      return;
    } else {
      ++ shape; /* try next command */
    }
  }
  printf("command not understood\n> %s\n", command);
  doHelp();
}
     

#define testArenaSIZE (((size_t)64)<<20)

extern int main(int argc, char *argv[])
{
  testlib_unused(argc);
  testlib_unused(argv);

  die(mps_arena_create((mps_arena_t*)&arena, mps_arena_class_vm(),
                       testArenaSIZE),
      "mps_arena_create");
  doHelp();
  while(1) {
    char input[INPUT_BUFFER_SIZE];
    printf("telemetry test> ");
    fflush(stdout);
    if (fgets(input, INPUT_BUFFER_SIZE , stdin)) {
      obeyCommand(input);
    } else {
      doQuit();
    }
  }
  return EXIT_SUCCESS;
}


/* C. COPYRIGHT AND LICENSE
 *
 * Copyright (C) 2001-2013 Ravenbrook Limited <http://www.ravenbrook.com/>.
 * All rights reserved.  This is an open source license.  Contact
 * Ravenbrook for commercial licensing options.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * 3. Redistributions in any form must be accompanied by information on how
 * to obtain complete source code for this software and any accompanying
 * software that uses this software.  The source code must either be
 * included in the distribution or be available for no more than the cost
 * of distribution plus a nominal fee, and must be freely redistributable
 * under reasonable conditions.  For an executable file, complete source
 * code means the source code for all modules it contains. It does not
 * include source code for modules or files that typically accompany the
 * major components of the operating system on which the executable file
 * runs.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
