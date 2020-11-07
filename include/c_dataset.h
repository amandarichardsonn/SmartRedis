#ifndef SMARTSIM_C_DATASET_H
#define SMARTSIM_C_DATASET_H
///@file
///\brief C-wrappers for the C++ DataSet class
#include "dataset.h"
#ifdef __cplusplus
extern "C" {
#endif

//! DataSet c-interface constructor
void* CDataSet(const char* name /*!< The name of the DataSet*/,
               const size_t name_length /*!< The length of the DataSet name c-string, excluding null terminating character*/
              );

//! Add a tensor to the DataSet
void add_tensor(void* dataset /*!< A c_ptr to the dataset object */,
                const char* name /*!< The name of the tensor*/,
                const size_t name_length /*!< The length of the tensor name c-string, excluding null terminating character*/,
                const char* type /*!< The data type of the tensor */,
                const size_t type_length /*!< The length of the data type c-string, excluding null terminating character*/,
                void* data /*!< A c_ptr to the data of the tensor*/,
                const int* dims /*!< Length along each dimension of the tensor*/,
                const int n_dims /*!< The number of dimensions of the tensor*/
                );

//! Add metadata field to the DataSet.  Default behavior is to append existing fields.
void add_meta(void* dataset /*!< A c_ptr to the dataset object */,
              const char* name /*!< The name of the metadata field*/,
              const size_t name_length /*!< The length of the metadata name c-string, excluding null terminating character*/,
              const char* type /*!< The data type of the metadata */,
              const size_t type_length /*!< The length of the data type c-string, excluding null terminating character*/,
              const void* data /*!< A c_ptr to the metadata value*/
              );

//! Get tensor data and return an allocated multi-dimensional array
void get_dataset_tensor(void* dataset /*!< A c_ptr to the dataset object */,
                const char* name /*!< The name used to reference the tensor*/,
                const size_t name_length /*!< The length of the tensor name c-string, excluding null terminating character*/,
                const char* type /*!< The data type of the tensor*/,
                const size_t type_length /*!< The length of the data type c-string, excluding null terminating character*/,
                void*& data /*!< A c_ptr to the tensor data */,
                const int* dims /*!< Length along each dimension of the tensor*/,
                const int n_dims /*!< The number of dimensions of the tensor*/
                );

//! Get tensor data and fill an already allocated array
void unpack_tensor(void* dataset /*!< A c_ptr to the dataset object */,
                   const char* name /*!< The name used to reference the tensor*/,
                   const size_t name_length /*!< The length of the tensor name c-string, excluding null terminating character*/,
                   const char* type /*!< The data type of the tensor*/,
                   const size_t type_length /*!< The length of the data type c-string, excluding null terminating character*/,
                   void* data /*!< A c_ptr to the data of the tensor*/,
                   const int* dims /*!< Length along each dimension of the tensor*/,
                   const int n_dims /*!< The number of dimensions of the tensor*/
                   );

//! Get metadata field from the DataSet
void get_meta(void* dataset /*!< A c_ptr to the dataset object */,
              const char* name /*!< The name used to reference the metadata*/,
              const size_t name_length /*!< The length of the metadata field name c-string, excluding null terminating character*/,
              const char* type /*!< The data type of the metadata*/,
              const size_t type_length /*!< The length of the data type c-string, excluding null terminating character*/,
              void*& data /*!< A c_ptr reference to the metadata*/,
              int* length /*!< The length of the metadata c_ptr*/
              );

#ifdef __cplusplus
}
#endif
#endif // SMARTSIM_C_DATASET_H