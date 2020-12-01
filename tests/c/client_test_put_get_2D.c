#include "c_client.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include "stdint.h"

void put_get_2D_tensor(void* tensor, int* dims, int n_dims,
                       void* result, char* type,
                       int type_length,
                       char* key_suffix,
                       int key_suffix_length)
{
  /* This function is a data type agnostic put and
  get for 3D tensors.  The result vector
  is filled with the fetched tensor.  Accuracy
  checking is done outside of this function because
  the type is not known.
  */

  void* client = SmartSimCClient(true);

  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank > 9)
    printf("%s", "C test does not support MPI ranks "\
                 "greater than 9.\n");

  char* prefix_str = "2D_tensor_test_rank_";
  char* rank_str = malloc(2*sizeof(char));
  rank_str[0] = rank + (int)'0';
  rank_str[1] = 0;

  size_t prefix_str_length = strlen(prefix_str);
  size_t rank_str_length = strlen(rank_str);

  int key_length = prefix_str_length + rank_str_length +
                   key_suffix_length;
  char* key = (char*)malloc((key_length+1)*sizeof(char));

  int pos;
  pos = 0;
  memcpy(&key[pos], prefix_str, prefix_str_length);
  pos += prefix_str_length;
  memcpy(&key[pos], rank_str, rank_str_length);
  pos += rank_str_length;
  memcpy(&key[pos], key_suffix, key_suffix_length);
  pos += key_suffix_length;
  key[pos] = 0;

  CMemoryLayout layout = c_nested;
  put_tensor(client, key, key_length, type, type_length,
             (void*)tensor, dims, n_dims, layout);
  get_tensor(client, key, key_length, type, type_length,
             result, dims, n_dims, layout);

  free(rank_str);
  free(key);
  DeleteCClient(client);
}

int put_get_2D_tensor_double(int* dims, int n_dims,
                  char* key_suffix, int key_suffix_length)
{
  /* This function puts and gets a 2D tensor of double
  values.  If the sent and received tensors do not match,
  a non-zero value is returned.
  */
  double** tensor = (double**)malloc(dims[0]*sizeof(double*));
  double** result = (double**)malloc(dims[0]*sizeof(double*));

  for(int i=0; i<dims[0]; i++) {
    tensor[i] = (double*)malloc(dims[1]*sizeof(double));
    result[i] = (double*)malloc(dims[1]*sizeof(double));
  }

  for(int i=0; i<dims[0]; i++)
    for(int j=0; j<dims[1]; j++)
      tensor[i][j] = ((double)rand())/RAND_MAX;

  char* type = "DOUBLE";
  size_t type_length = strlen(type);

  put_get_2D_tensor((void*)tensor, dims, n_dims, (void*)result,
                    type, type_length, key_suffix,
                    key_suffix_length);

  int r_value = 0;
  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++) {
      if(tensor[i][j]!=result[i][j]) {
        printf("%s", "The tensors do not match!");
        r_value = -1;
      }
    }
  }

  for(int i=0; i<dims[0]; i++) {
    free(tensor[i]);
    free(result[i]);
  }
  free(tensor);
  free(result);
  return r_value;
}

int put_get_2D_tensor_float(int* dims, int n_dims,
                           char* key_suffix,
                           int key_suffix_length)
{
  /* This function puts and gets a 2D tensor of float
  values.  If the sent and received tensors do not match,
  a non-zero value is returned.
  */
  float** tensor = (float**)malloc(dims[0]*sizeof(float*));
  float** result = (float**)malloc(dims[0]*sizeof(float*));

  for(int i=0; i<dims[0]; i++) {
    tensor[i] = (float*)malloc(dims[1]*sizeof(float));
    result[i] = (float*)malloc(dims[1]*sizeof(float));
  }

  for(int i=0; i<dims[0]; i++)
    for(int j=0; j<dims[1]; j++)
      tensor[i][j] = ((float)rand())/RAND_MAX;

  char* type = "FLOAT";
  size_t type_length = strlen(type);

  put_get_2D_tensor((void*)tensor, dims, n_dims, (void*)result,
                    type, type_length, key_suffix,
                    key_suffix_length);

  int r_value = 0;
  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++) {
      if(tensor[i][j]!=result[i][j]) {
        printf("%s", "The tensors do not match!");
        r_value = -1;
      }
    }
  }

  for(int i=0; i<dims[0]; i++) {
    free(tensor[i]);
    free(result[i]);
  }
  free(tensor);
  free(result);
  return r_value;
}

int put_get_2D_tensor_i8(int* dims, int n_dims,
                         char* key_suffix,
                         int key_suffix_length)
{
  /* This function puts and gets a 2D tensor of int8_t
  values.  If the sent and received tensors do not match,
  a non-zero value is returned.
  */
  int8_t** tensor = (int8_t**)malloc(dims[0]*sizeof(int8_t*));
  int8_t** result = (int8_t**)malloc(dims[0]*sizeof(int8_t*));

  for(int i=0; i<dims[0]; i++) {
    tensor[i] = (int8_t*)malloc(dims[1]*sizeof(int8_t));
    result[i] = (int8_t*)malloc(dims[1]*sizeof(int8_t));
  }

  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++) {
      tensor[i][j] = rand()%INT8_MAX;
      if(rand()%2)
        tensor[i][j] *= -1;
    }
  }

  char* type = "INT8";
  size_t type_length = strlen(type);

  put_get_2D_tensor((void*)tensor, dims, n_dims, (void*)result,
                    type, type_length, key_suffix,
                    key_suffix_length);

  int r_value = 0;
  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++){
      if(tensor[i][j]!=result[i][j]) {
        printf("%s", "The tensors do not match!");
        r_value = -1;
      }
    }
  }

  for(int i=0; i<dims[0]; i++) {
    free(tensor[i]);
    free(result[i]);
  }
  free(tensor);
  free(result);
  return r_value;
}

int put_get_2D_tensor_i16(int* dims, int n_dims,
                         char* key_suffix,
                         int key_suffix_length)
{
  /* This function puts and gets a 2D tensor of int16_t
  values.  If the sent and received tensors do not match,
  a non-zero value is returned.
  */
  int16_t** tensor = (int16_t**)malloc(dims[0]*sizeof(int16_t*));
  int16_t** result = (int16_t**)malloc(dims[0]*sizeof(int16_t*));

  for(int i=0; i<dims[0]; i++) {
    tensor[i] = (int16_t*)malloc(dims[1]*sizeof(int16_t));
    result[i] = (int16_t*)malloc(dims[1]*sizeof(int16_t));
  }

  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++) {
      tensor[i][j] = rand()%INT16_MAX;
      if(rand()%2)
        tensor[i][j] *= -1;
    }
  }

  char* type = "INT16";
  size_t type_length = strlen(type);

  put_get_2D_tensor((void*)tensor, dims, n_dims, (void*)result,
                    type, type_length, key_suffix,
                    key_suffix_length);

  int r_value = 0;
  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++){
      if(tensor[i][j]!=result[i][j]) {
        printf("%s", "The tensors do not match!");
        r_value = -1;
      }
    }
  }

  for(int i=0; i<dims[0]; i++) {
    free(tensor[i]);
    free(result[i]);
  }
  free(tensor);
  free(result);
  return r_value;
}

int put_get_2D_tensor_i32(int* dims, int n_dims,
                         char* key_suffix,
                         int key_suffix_length)
{
  /* This function puts and gets a 2D tensor of int32_t
  values.  If the sent and received tensors do not match,
  a non-zero value is returned.
  */
  int32_t** tensor = (int32_t**)malloc(dims[0]*sizeof(int32_t*));
  int32_t** result = (int32_t**)malloc(dims[0]*sizeof(int32_t*));

  for(int i=0; i<dims[0]; i++) {
    tensor[i] = (int32_t*)malloc(dims[1]*sizeof(int32_t));
    result[i] = (int32_t*)malloc(dims[1]*sizeof(int32_t));
  }

  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++) {
      tensor[i][j] = rand()%INT32_MAX;
      if(rand()%2)
        tensor[i][j] *= -1;
    }
  }

  char* type = "INT32";
  size_t type_length = strlen(type);

  put_get_2D_tensor((void*)tensor, dims, n_dims, (void*)result,
                    type, type_length, key_suffix,
                    key_suffix_length);

  int r_value = 0;
  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++){
      if(tensor[i][j]!=result[i][j]) {
        printf("%s", "The tensors do not match!");
        r_value = -1;
      }
    }
  }

  for(int i=0; i<dims[0]; i++) {
    free(tensor[i]);
    free(result[i]);
  }
  free(tensor);
  free(result);
  return r_value;
}

int put_get_2D_tensor_i64(int* dims, int n_dims,
                         char* key_suffix,
                         int key_suffix_length)
{
  /* This function puts and gets a 2D tensor of int64_t
  values.  If the sent and received tensors do not match,
  a non-zero value is returned.
  */
  int64_t** tensor = (int64_t**)malloc(dims[0]*sizeof(int64_t*));
  int64_t** result = (int64_t**)malloc(dims[0]*sizeof(int64_t*));

  for(int i=0; i<dims[0]; i++) {
    tensor[i] = (int64_t*)malloc(dims[1]*sizeof(int64_t));
    result[i] = (int64_t*)malloc(dims[1]*sizeof(int64_t));
  }

  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++) {
      tensor[i][j] = rand()%INT64_MAX;
      if(rand()%2)
        tensor[i][j] *= -1;
    }
  }

  char* type = "INT64";
  size_t type_length = strlen(type);

  put_get_2D_tensor((void*)tensor, dims, n_dims, (void*)result,
                    type, type_length, key_suffix,
                    key_suffix_length);

  int r_value = 0;
  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++){
      if(tensor[i][j]!=result[i][j]) {
        printf("%s", "The tensors do not match!");
        r_value = -1;
      }
    }
  }

  for(int i=0; i<dims[0]; i++) {
    free(tensor[i]);
    free(result[i]);
  }
  free(tensor);
  free(result);
  return r_value;
}

int put_get_2D_tensor_ui8(int* dims, int n_dims,
                          char* key_suffix,
                          int key_suffix_length)
{
  /* This function puts and gets a 2D tensor of uint8_t
  values.  If the sent and received tensors do not match,
  a non-zero value is returned.
  */
  uint8_t** tensor = (uint8_t**)malloc(dims[0]*sizeof(uint8_t*));
  uint8_t** result = (uint8_t**)malloc(dims[0]*sizeof(uint8_t*));

  for(int i=0; i<dims[0]; i++) {
    tensor[i] = (uint8_t*)malloc(dims[1]*sizeof(uint8_t));
    result[i] = (uint8_t*)malloc(dims[1]*sizeof(uint8_t));
  }

  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++) {
      tensor[i][j] = rand()%UINT8_MAX;
    }
  }

  char* type = "UINT8";
  size_t type_length = strlen(type);

  put_get_2D_tensor((void*)tensor, dims, n_dims, (void*)result,
                    type, type_length, key_suffix,
                    key_suffix_length);

  int r_value = 0;
  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++){
      if(tensor[i][j]!=result[i][j]) {
        printf("%s", "The tensors do not match!");
        r_value = -1;
      }
    }
  }

  for(int i=0; i<dims[0]; i++) {
    free(tensor[i]);
    free(result[i]);
  }
  free(tensor);
  free(result);
  return r_value;
}

int put_get_2D_tensor_ui16(int* dims, int n_dims,
                          char* key_suffix,
                          int key_suffix_length)
{
  /* This function puts and gets a 2D tensor of uint16_t
  values.  If the sent and received tensors do not match,
  a non-zero value is returned.
  */
  uint16_t** tensor = (uint16_t**)malloc(dims[0]*sizeof(uint16_t*));
  uint16_t** result = (uint16_t**)malloc(dims[0]*sizeof(uint16_t*));

  for(int i=0; i<dims[0]; i++) {
    tensor[i] = (uint16_t*)malloc(dims[1]*sizeof(uint16_t));
    result[i] = (uint16_t*)malloc(dims[1]*sizeof(uint16_t));
  }

  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++) {
      tensor[i][j] = rand()%UINT16_MAX;
    }
  }

  char* type = "UINT16";
  size_t type_length = strlen(type);

  put_get_2D_tensor((void*)tensor, dims, n_dims, (void*)result,
                    type, type_length, key_suffix,
                    key_suffix_length);

  int r_value = 0;
  for(int i=0; i<dims[0]; i++) {
    for(int j=0; j<dims[1]; j++){
      if(tensor[i][j]!=result[i][j]) {
        printf("%s", "The tensors do not match!");
        r_value = -1;
      }
    }
  }

  for(int i=0; i<dims[0]; i++) {
    free(tensor[i]);
    free(result[i]);
  }
  free(tensor);
  free(result);
  return r_value;
}

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  int* dims = malloc(2*sizeof(int));
  dims[0] = 10;
  dims[1] = 26;
  int n_dims = 2;

  int result = 0;
  //2D double tensor
  char* dbl_suffix = "_dbl_c";
  result += put_get_2D_tensor_double(dims, n_dims,
                      dbl_suffix, strlen(dbl_suffix));

  //2D float tensor
  char* flt_suffix = "_flt_c";
  result += put_get_2D_tensor_float(dims, n_dims,
                      flt_suffix, strlen(flt_suffix));

  //2D int8 tensor
  char* i8_suffix = "_i8_c";
  result += put_get_2D_tensor_i8(dims, n_dims,
                      i8_suffix, strlen(i8_suffix));

  //2D int16 tensor
  char* i16_suffix = "_i16_c";
  result += put_get_2D_tensor_i16(dims, n_dims,
                      i16_suffix, strlen(i16_suffix));

  //2D int32 tensor
  char* i32_suffix = "_i32_c";
  result += put_get_2D_tensor_i32(dims, n_dims,
                      i32_suffix, strlen(i32_suffix));

  //2D int64 tensor
  char* i64_suffix = "_i64_c";
  result += put_get_2D_tensor_i64(dims, n_dims,
                      i64_suffix, strlen(i64_suffix));

  //2D uint8 tensor
  char* ui8_suffix = "_ui8_c";
  result += put_get_2D_tensor_ui8(dims, n_dims,
                      ui8_suffix, strlen(ui8_suffix));

  //2D uint16 tensor
  char* ui16_suffix = "_ui16_c";
  result += put_get_2D_tensor_ui16(dims, n_dims,
                      ui16_suffix, strlen(ui16_suffix));

  free(dims);
  MPI_Finalize();
  printf("%s","Test passed: ");
  if(result==0)
    printf("%s", "YES\n");
  else
    printf("%s", "NO\n");

  return result;
}