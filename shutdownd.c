#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <stdbool.h>
#include <mraa.h>

#define GPIO_PIN_12 12

#define MRAA_CHECK_RESULT(X, Y, Z, Q)		\
  if (X == MRAA_SUCCESS) {			\
    syslog(LOG_DAEMON | LOG_NOTICE, Y);		\
  } else {					\
    syslog(LOG_DAEMON | LOG_ERR, Z);		\
    if (Q) {					\
      clean_up();				\
      exit(EXIT_FAILURE);			\
    }						\
  }

void clean_up() {
  syslog(LOG_DAEMON | LOG_WARNING, "Cleaning up.");
  closelog();
}

void sig_handler(int signal) {
  syslog(LOG_DAEMON | LOG_WARNING, "Shutdown daemon interrupted.");
  clean_up();
  exit(EXIT_FAILURE);
}

int main(int argc, char ** argv) {
  int               rv;
  bool              done = false;
  sighandler_t      sh;
  mraa_result_t     mr;
  mraa_gpio_context pin12;

  // Turn the process into a daemon.
  rv = daemon(0, 1);

  if (rv != 0) {
    perror("daemon(0, 1)");
    return EXIT_FAILURE;
  }

  // Set SIGINT signal handler
  sh = signal(SIGINT, sig_handler);

  if (sh == SIG_ERR) {
    perror("sh = signal(SIGINT, sig_handler)");
    return EXIT_FAILURE;
  }

  // Create PID file
  FILE * pid = fopen("/var/run/shutdown.pid", "w");

  if (pid == NULL) {
    perror("FILE * pid = fopen(\"/var/run/shutdown.pid\", \"w\")");
    syslog(LOG_DAEMON | LOG_ERR, "Failed to create PID file.");
    return EXIT_FAILURE;
  }

  fprintf(pid, "%d", getpid());
  fclose(pid);
  
  // Open a syslog connection
  openlog(argv[0], LOG_NDELAY | LOG_PID, LOG_DAEMON);
  syslog(LOG_DAEMON | LOG_NOTICE, "Started shutdown daemon.");

  // Open PIN 12 as a PULLUP input pin for the push button
  pin12 = mraa_gpio_init(GPIO_PIN_12);
  mr = mraa_gpio_dir(pin12, MRAA_GPIO_IN);
  MRAA_CHECK_RESULT(mr, "Opened push button pin.", "Failed to open push button pin.", true);
  mr = mraa_gpio_mode(pin12, MRAA_GPIO_PULLUP);
  MRAA_CHECK_RESULT(mr, "Push button ready.", "Failed to set push button mode.", true);

  // Start pin processing loop
  while (!done) {    
    rv = mraa_gpio_read(pin12);

    if (rv == 0) {
      // If the button is pressed then stop reading and go on with the program
      syslog(LOG_DAEMON | LOG_NOTICE, "Button push detected.");
      done = true;
    }
  }

  syslog(LOG_DAEMON | LOG_NOTICE, "Finishing.");
  closelog();
  execlp("shutdown", "shutdown", "-Ph", "now", (char *)NULL);

  return EXIT_SUCCESS;
}
