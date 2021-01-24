#include <stdio.h>
#include <stdlib.h>

// Unix standard, need windows alternative
#include <unistd.h> // access() if file exist
// getline also unix standard, not windows

#define verify(S)           if(!(S)) { goto error; }
#define verify_no_error(S)  if((S)->error) { goto error; }
#define fx_alloc(S)         calloc(1, sizeof(S))

typedef enum { false, true } Boolean;

typedef struct FxError {
  const char *message;
} FxError;

typedef struct FxFile {
  const char *path;
  char *mode;
  int cursor;
  FILE *data;
  FxError *error;
} FxFile;

#define fx_error_message(E)       ((E)->message)

#define fx_file_path(F)           ((F)->path)
#define fx_file_mode(F)           ((F)->mode)
#define fx_file_data(F)           ((F)->data)
#define fx_file_error(F)          ((F)->error)
#define fx_file_cursor(F)         ((F)->cursor)
#define fx_file_error_message(F)  ((F)->error->message)
#define fx_file_eof(F)            (fx_file_cursor(F) < 0 ? true : false)

FxError *FxError_new(const char *message) {
  FxError *error = fx_alloc(FxError);
  verify(error);

  fx_error_message(error) = message;

  return error;
error:
  printf("Memory error\n");
  return NULL;
}

FxFile *FxFile_new(const char *path, char *mode) {
  FxFile *file = fx_alloc(FxFile);
  verify(file);

  fx_file_path(file) = path;
  fx_file_mode(file) = mode;

  return file;
error:
  printf("Memory error\n");
  return NULL;
}

void fx_file_open(FxFile *file) {
  fx_file_data(file) = fopen(fx_file_path(file), fx_file_mode(file));
  verify(fx_file_data(file));
  return;
error:
  fx_file_error(file) = FxError_new("Unable to open file");
}

void fx_file_close(FxFile *file) {
  fclose(fx_file_data(file));
  fx_file_cursor(file) = 0;
}

Boolean fx_file_exist(FxFile *file) {
  return (access( fx_file_path(file), F_OK ) != -1) ? true : false;
}

char *fx_file_readline(FxFile *file) {
  // fx_file_open(file);
  verify_no_error(file);

  char *line = NULL;
  size_t length = 0;
  ssize_t read_length = 0;

  if ((read_length = getline(&line, &length, fx_file_data(file))) != -1) {
    fx_file_cursor(file) += read_length;
  } else {
    fx_file_cursor(file) = -1;
  }

  verify(line);
  return line;
error:
  return NULL;
}

int main(int argc, char *argv[]) {
  const char *path = argv[1];
  FxFile *file = FxFile_new(path, "r");
  char *contents;

  fx_file_open(file);
  verify_no_error(file);

  printf("Reading contents, end of file? %d\n", fx_file_eof(file));

  int line_count = 0;
  while( !fx_file_eof(file) ) {
    line_count ++;
    contents = fx_file_readline(file);
    printf("%d line: %s", line_count, contents);
    free(contents);
  }
  puts("");
  fx_file_close(file);

  return 0;
error:
  printf("Error: %s, %s\n", fx_file_error_message(file), path);
  return 1;
}
