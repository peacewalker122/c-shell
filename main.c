#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_COMMANDS 11
#define MAX_ARGUMENTS 11
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define LINE_BUFFER 1024

struct simple_command {
  int numberofAvaibleArguments;
  int numberOfArguments;
  char ***arguments;

  // void insertArgument(char *argument);
};

void parse_argv(struct simple_command *command, char *arg) {
  char *token = strtok(arg, ";");
  int counter = 0;
  char **temp_token = (char **)malloc(sizeof(char *) * 1);

  command->arguments = (char ***)malloc(sizeof(char **) * MAX_COMMANDS);
  command->numberOfArguments = 0;

  while (token != NULL) {
    printf("Token[temp_token]: %s\n", token);
    temp_token[command->numberOfArguments] = token;

    if (command->numberOfArguments != 0) {
      temp_token = (char **)realloc(
          temp_token, sizeof(char *) * (command->numberOfArguments + 1));
    }

    command->numberOfArguments++;
    token = strtok(NULL, ";");
  }

  for (int i = 0; i < command->numberOfArguments; i++) {
    printf("Token[assignee]: %s\n", temp_token[i]);
    command->arguments[i] = (char **)malloc(sizeof(char *) * MAX_ARGUMENTS);

    char *arg_token = strtok(temp_token[i], " ");

    while (arg_token != NULL) {
      printf("Token[arg_token]: %s\n", arg_token);
      command->arguments[i][counter] = arg_token;
      counter++;
      arg_token = strtok(NULL, " ");
    }

    command->arguments[i][++counter] = NULL;

    counter = 0;
  }

  free(temp_token);
}

char *get_line(void) {
  int buf_size = LINE_BUFFER;
  int pointer = 0;
  int c; // character
  char *line = (char *)malloc(buf_size * sizeof(char));

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      line[pointer] = '\0';
      return line;
    } else {
      line[pointer] = c;
    }
    pointer++;

    if (pointer >= buf_size) {
      buf_size += LINE_BUFFER;
      line = (char *)realloc(line, buf_size);
      if (!line) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

int launch(char **args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int main(int argc, char *argv[]) {
  if (isatty(STDIN_FILENO)) {
    while (1) {
      printf("> ");

      struct simple_command *command =
          (struct simple_command *)malloc(sizeof(struct simple_command));

      char *user_input = get_line();

      printf("User input: %s\n", user_input);

      parse_argv(command, user_input);

      printf("Number of arguments: %d\n", command->numberOfArguments);

      for (int i = 0; i < command->numberOfArguments; i++) {
        printf("Argument %d: %s\n", i, command->arguments[i][0]);
        launch(command->arguments[i]);
      }

      free(command->arguments);
      free(user_input);
    };
  } else {
    // TODO: create a deserializer for the binary
  }

  return EXIT_SUCCESS;
}
