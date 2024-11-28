#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VERSION "1.0.0"
#define CONFIG_FILE "~/.rccrc"
#define DEFAULT_GCC_OPTIONS "-Wall"
#define DEFAULT_KEEP_OUTPUT 0

// Function to expand "~" in file paths
char *expand_path(const char *path) {
  if (path[0] == '~') {
    const char *home = getenv("HOME");
    if (home) {
      static char full_path[256];
      snprintf(full_path, sizeof(full_path), "%s%s", home, path + 1);
      return full_path;
    }
  }
  return (char *)path;
}

// Function to read configuration from ~/.runmanrc
void read_config(char *gcc_options, int *keep_output) {
  char *config_path = expand_path(CONFIG_FILE);
  FILE *file = fopen(config_path, "r");
  if (file == NULL) {
    // Use default values if config file is not found
    strcpy(gcc_options, DEFAULT_GCC_OPTIONS);
    *keep_output = DEFAULT_KEEP_OUTPUT;
    return;
  }

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "GCC_OPTIONS=", 12) == 0) {
      sscanf(line + 12, "%255[^\n]", gcc_options);
    } else if (strncmp(line, "KEEP_OUTPUT=", 12) == 0) {
      sscanf(line + 12, "%d", keep_output);
    }
  }
  fclose(file);
}

// Function to clean up the output file
void cleanup(const char *filename, int keep_output) {
  if (!keep_output) {
    if (!remove(filename) == 0) perror("Error removing file");
  }
}

int main(int argc, char *argv[]) {
  char gcc_options[256];
  int keep_output;

  // Read configuration from ~/.rccrc
  read_config(gcc_options, &keep_output);

  if (argc == 2 && strcmp(argv[1], "--version") == 0) {
    // Handle -version argument
    printf("RCC Version: %s\n", VERSION);
    printf("Author: Heptari\n");
    printf("Configured GCC Options: %s\n", gcc_options);
    printf("Keep Output: %s\n", keep_output ? "Yes" : "No");
    return 0;
  }

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <source_file.c> or %s --version\n", argv[0],
            argv[0]);
    return 1;
  }

  const char *source_file = argv[1];
  char output_file[512], exec_output_file[512];
  snprintf(output_file, sizeof(output_file), "%s.out", source_file);
  snprintf(exec_output_file, sizeof(exec_output_file), "./%s", output_file);

  char gcc_command[512];

  // Construct the gcc command with options
  snprintf(gcc_command, sizeof(gcc_command), "gcc %s %s -o %s", gcc_options,
           source_file, output_file);

  // Compile the source file
  printf("Compiling %s with options: %s\n", source_file, gcc_options);
  if (system(gcc_command) != 0) {
    fprintf(stderr, "Compilation failed.\n");
    return 1;
  }

  // Run the output file
  printf("Running %s...\n\n", output_file);
  if (system(exec_output_file) != 0) {
    fprintf(stderr, "Error running the program.\n");
  }

  // Clean up the output file based on configuration
  cleanup(output_file, keep_output);

  return 0;
}
