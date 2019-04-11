/* Example from Head First C.

Downloaded from https://github.com/twcamper/head-first-c

Modified by Allen Downey.

Further edited by Cassandra Overney

1) The program only prints articles from the first RSS feed, not the others,
    because the original/ parent program gets terminated immediately after
    a child process is created.

*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <wait.h>

/* Print error message
*/
void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/* Search RSS feeds for certain keyword
*/
int main(int argc, char *argv[])
{
  pid_t pid;
  int status;
  if (argc < 2) {
      fprintf(stderr, "Usage: %s <search phrase>\n", argv[0]);
      return 1;
  }
  const char *PYTHON = "/usr/bin/python2";
  const char *SCRIPT = "rssgossip.py";
  char *feeds[] = {
      "http://www.nytimes.com/services/xml/rss/nyt/Africa.xml",
      "http://www.nytimes.com/services/xml/rss/nyt/Americas.xml",
      "http://www.nytimes.com/services/xml/rss/nyt/MiddleEast.xml",
      "http://www.nytimes.com/services/xml/rss/nyt/Europe.xml",
      "http://www.nytimes.com/services/xml/rss/nyt/AsiaPacific.xml"
  };
  int num_feeds = 5;
  char *search_phrase = argv[1];
  char var[255];

  // For each feed, create a child process and call the python script
  // with the inputted search phrase
  for (int i=0; i<num_feeds; i++) {
      sprintf(var, "RSS_FEED=%s", feeds[i]);
      char *vars[] = {var, NULL};
      pid = fork();

      // A return value of -1 means there was a problem cloning the process
      if (pid==-1) {
        error("Can't fork process.");
        return 1;
      }
      // Code is running in the child process if pid == 0 or !pid
      if (!pid) {
        // Throw error if python script didn't execute
        if (execle(PYTHON, PYTHON, SCRIPT, search_phrase, NULL, vars) == -1) {
          error("Can't run script.");
          return 1;
        }
        // With no error, then exit out of child process
        else{
          return 0;
        }
      }

      // Old stuff:
      // int res = execle(PYTHON, PYTHON, SCRIPT, search_phrase, NULL, vars);
      // if (res == -1) {
      //     error("Can't run script.");
      // }

  }
  // Only the parent would get here
  // Parent wait for child process to complete, check for errors and exits
  for (int i=0; i<num_feeds; i++) {
    pid = wait(&status);
    // Throw error if return -1
    if (pid == -1) {
        error("Wait failed");
        perror(argv[0]);
        return 1;
    }

    // Check the exit status of the child, gives the error code
    status = WEXITSTATUS(status);
  }
  return 0;
}
