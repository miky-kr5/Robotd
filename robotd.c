#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <stdbool.h>

#include <mraa.h>

#define GPIO_PIN_4 4
#define GPIO_PIN_13 13

int main(int argc, char ** argv) {
  int               rv;
  bool              done = false;
  mraa_gpio_context pin13, pin4;
  mraa_result_t     mr;

  // Turn the process into a daemon.
  rv = daemon(0, 1);

  if (rv != 0) {
    perror("daemon(0, 1)");
    return EXIT_FAILURE;
  }

  // Open a syslog connection
  openlog(argv[0], LOG_NDELAY | LOG_PID, LOG_DAEMON);
  syslog(LOG_DAEMON | LOG_NOTICE, "Started robot daemon.");

  // Open PIN 13 as a PULLUP input pin for the push button
  pin13 = mraa_gpio_init(GPIO_PIN_13);
  mr = mraa_gpio_dir(pin13, MRAA_GPIO_IN);
  mr = mraa_gpio_mode(pin13, MRAA_GPIO_PULLUP);
  syslog(LOG_DAEMON | LOG_NOTICE, "Push button ready.");

  // TODO: MRAA error handling.

  // Open PIN 4 as an output pin for the red indicator LED
  pin4 = mraa_gpio_init(GPIO_PIN_4);
  mr = mraa_gpio_dir(pin4, MRAA_GPIO_OUT);
  syslog(LOG_DAEMON | LOG_NOTICE, "Red LED ready.");

  // TODO: MRAA error handling.

  // Star pin processing loop
  while (!done) {
    // Keep the red LED on and read from the push button pin
    mr = mraa_gpio_write(pin4, 1);
    rv = mraa_gpio_read(pin13);

    // TODO: MRAA error handling.

    if (rv == 0) {
      // If the button is pressed then stop reading and go on with the program
      syslog(LOG_DAEMON | LOG_NOTICE, "Button push detected.");
      done = true;
    }
  }

  // Turn off the red LED
  mr = mraa_gpio_write(pin4, 0);

  // TODO: MRAA error handling.

  // Open the source code file for the Python script
  syslog(LOG_DAEMON | LOG_NOTICE, "Opening Python source code.");
  FILE * code = fopen("/home/root/robot.py", "r");

  if (code == NULL) {
    syslog(LOG_DAEMON | LOG_NOTICE, "Failed to load Python program.");
    return EXIT_FAILURE;
  }

  // Execute the Python code
  syslog(LOG_DAEMON | LOG_NOTICE, "Performing Python call.");  
  Py_SetProgramName(argv[0]);
  Py_Initialize();
  PyRun_SimpleFile(code, "/home/root/robot.py");
  Py_Finalize();

  // TODO: Python error handling.

  // Clean up and leave
  syslog(LOG_DAEMON | LOG_NOTICE, "Finishing.");
  closelog();
  //execlp("shutdown", "shutdown", "-Ph", "now", (char *)NULL);

  return EXIT_SUCCESS;
}
