#include "sighandler.h"
#include <ctype.h>
#include <stdio.h>

/*
 * Because the parser is implemented here, and doesn't take care of
 * strings, character constants, or comments, we hide the { } with
 * compile time constants.
 */
constexpr char left = '{';
constexpr char right = '}';

/**
 ** @brief Initial string to indent a line
 **/
constexpr char head[] = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>| ";

/**
 ** @brief Skips any number of spaces, tabs, or newlines in @a s
 **/
static
char const* skipspace(char const s[static 1]) {
  while (isspace(s[0])) {
    ++s;
  }
  return s;
}

/**
 ** @brief Exceptional states of the parse algorithm
 **/
enum state {
  execution = 0,    //!< Normal execution
  plusL,            //!< Too many left braces
  plusR,            //!< Too many right braces
  tooDeep,          //!< Nesting too deep to handle
  eofOut,           //!< End of output
  interrupted,      //!< Interrupted by a signal
};

/**
 ** @brief Outputs end of line and eats all subsequent spaces on input
 **/
static
char const* end_line(char const s[static 1], jmp_buf jmpTarget) {
  if (putchar('\n') == EOF) longjmp(jmpTarget, eofOut);
  return skipspace(s);
}



/**
 ** @brief Keeps track of the last signal that interrupted us
 **/
static sig_atomic_t volatile interrupt = 0;

static
char const* descend(char const* act,
                    unsigned dp[restrict static 1], // Bad
                    size_t len, char buffer[static len],
                    jmp_buf jmpTarget) {
  if (dp[0]+3 > sizeof head) longjmp(jmpTarget, tooDeep);
  ++dp[0];/*@\label{lab:incr-rec}*/
 NEW_LINE:                             // Loops on output
  while (!act || !act[0]) {            // Loops for input
    if (interrupt) longjmp(jmpTarget, interrupted);
    act = skipspace(fgets(buffer, len, stdin));
    if (!act) {                        // End of stream
      if (dp[0] != 1) longjmp(jmpTarget, plusL);
      else goto ASCEND;
    }
  }
  fputs(&head[sizeof head - (dp[0] + 2)], stdout); // Header

  for (; act && act[0]; ++act) { // Remainder of the line
    switch (act[0]) {            /*@\label{lab:switch-char}*/
    case left:                   // Descends on left brace
      act = end_line(act+1, jmpTarget);
      act = descend(act, dp, len, buffer, jmpTarget);/*@\label{lab:descend}*/
      act = end_line(act+1, jmpTarget);
      goto NEW_LINE;
    case right:                  // Returns on right brace
      if (dp[0] == 1) longjmp(jmpTarget, plusR);
      else goto ASCEND;
    default:                     // Prints char and goes on
      putchar(act[0]);
    }
  }
  goto NEW_LINE;
 ASCEND:
  --dp[0];/*@\label{lab:decr-rec}*/
  return act;
}

constexpr unsigned maxline = 256;

void basic_blocks(void) {
  char buffer[maxline];
  unsigned depth = 0;
  char const* format =
    "All matching %0.0d'%c' '%c' pairs have been closed correctly\n";
  jmp_buf jmpTarget;
  switch (setjmp(jmpTarget)) {
  case 0:
    descend(nullptr, &depth, maxline, buffer, jmpTarget);
    break;
  case plusL:
    format =
      "Warning: %d '%c' have not been closed properly (expected '%c')\n";
    break;
  case plusR:
    format =
      "Error: closing too many (%d) '%c' parenthesis with additional '%c'\n";
    break;
  case tooDeep:
    format =
      "Error: nesting (%d) of '%c' '%c' constructs is too deep\n";
    break;
  case eofOut:
    format =
      "Error: EOF for stdout at %d open '%c', expecting same amount of '%c'\n";
    break;
  case interrupted:
    format =
      "Interrupted at level %d of '%c' '%c' nesting\n";
    break;
  default:;
    format =
      "Error: unknown error within (%d) '%c' '%c' constructs\n";
  }
  fflush(stdout);
  fprintf(stderr, format, depth, left, right);
  if (interrupt) {
    SH_PRINT(stderr, interrupt,
             "is somebody trying to kill us?");
    raise(interrupt);
  }
}

/**
 ** @brief A minimal signal handler
 **
 ** After updating the signal count, for most signals this
 ** simply stores the signal value in "interrupt" and returns.
 **/
static void signal_handler(int sig) {
  sh_count(sig);
  switch (sig) {
  case SIGTERM: quick_exit(EXIT_FAILURE);
  case SIGABRT: _Exit(EXIT_FAILURE);
#ifdef SIGCONT
    // continue normal operation
  case SIGCONT: return;
#endif
  default:
    /* reset the handling to its default */
    signal(sig, SIG_DFL);
    interrupt = sig;
    return;
  }
}

// Will point to the command-line arguments
static char** lastOpen = nullptr;

// Checks if we were in the middle of an operation
void doAtExit(void) {
  if (lastOpen && lastOpen[0]) {
    fprintf(stderr, "\n***********\nabnormal exit, last open file was %s\n", lastOpen[0]);
  }
}

int main(int argc, char* argv[argc+1]) {
  // Ensures that stdout is line buffered
  if (setvbuf(stdout, nullptr, _IOLBF, maxline + sizeof head + 2)) {
    fputs("we could not establish line buffering for stdout, terminating.", stderr);
    return EXIT_FAILURE;
  }

  // Establishes exit handlers
  atexit(doAtExit);
  at_quick_exit(doAtExit);

  // Establishes signal handlers
  for (unsigned i = 1; i < sh_known; ++i)
    sh_enable(i, signal_handler);

  // If there are no command-line arguments, reads from stdin
  lastOpen = argv;
  if (argc < 2) goto RUN;

  // Runs basic_blocks for each command-line argument
  for (++lastOpen; lastOpen[0]; ++lastOpen) {
    if (!freopen(lastOpen[0], "r", stdin)) {
      perror(lastOpen[0]);
      return EXIT_FAILURE;
    }
    printf("++++++++++ %s +++++++++++\n", lastOpen[0]);
  RUN:
    basic_blocks();
  }
  return EXIT_SUCCESS;
}
