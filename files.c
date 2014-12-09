#include <stdio.h>
#include <stdlib.h>

#define verify(S)         if(!(S)) { goto error; }
#define fx_alloc(S)       calloc(1, sizeof(S))

typedef struct FxError {
  const char *message;
} FxError;

typedef struct FxFile {
  const char *path;
  char *mode;
  FILE *data;
  FxError *error;
} FxFile;

#define fx_error_message(E)       ((E)->message)

#define fx_file_path(F)           ((F)->path)
#define fx_file_mode(F)           ((F)->mode)
#define fx_file_data(F)           ((F)->data)
#define fx_file_error(F)          ((F)->error)
#define fx_file_error_message(F)  ((F)->error->message)

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
}

int main(int argc, char *argv[]) {
  const char *path = argv[1];
  FxFile *file = FxFile_new(path, "r");
  fx_file_open(file);

  if (fx_file_error(file)) {
    printf("Error: %s, %s\n", fx_file_error_message(file), path);
  } else {
    printf("got a file '%s'\n", path);
    fx_file_close(file);
  }

  return 0;
}
