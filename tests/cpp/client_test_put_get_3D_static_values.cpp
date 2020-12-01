#include "client.h"
#include "client_test_utils.h"
#include <mpi.h>
#include <vector>
#include <string>

template <typename T_send, typename T_recv>
void put_get_3D_array(
		    std::vector<size_t> dims,
        std::string type,
        std::string key_suffix="")
{
  SmartSimClient client(true);

  //Allocate and fill arrays
  T_send*** array = allocate_3D_array<T_send>(dims[0], dims[1], dims[2]);
  T_recv*** u_result = allocate_3D_array<T_recv>(dims[0], dims[1], dims[2]);

  int a = 0;
  for(int i = 0; i < dims[0]; i++) {
    for(int j = 0; j < dims[1]; j++) {
      for(int k = 0; k < dims[2]; k++) {
        array[i][j][k] = (T_send)a+2;
        a++;
      }
    }
  }

  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string key = "3d_tensor_test_rank_" +
                    std::to_string(rank) + key_suffix;

  /*
  for(int i = 0; i < dims[0]; i++) {
    for(int j = 0; j < dims[1]; j++) {
      for(int k = 0; k < dims[2]; k++) {
        std::cout.precision(17);
        std::cout<<"Sending value "<<i<<","<<j<<","<<k<<": "
                 <<std::fixed<<array[i][j][k]<<std::endl;
      }
    }
  }
  */

  client.put_tensor(key, type, (void*)array, dims, MemoryLayout::nested);
  client.unpack_tensor(key, type, u_result, dims, MemoryLayout::nested);

  /*
  for(int i = 0; i < dims[0]; i++) {
    for(int j = 0; j < dims[1]; j++) {
      for(int k = 0; k < dims[2]; k++) {
        std::cout<< "Value " << i << "," << j << "," << k
                 << " Sent: " << array[i][j][k] <<" Received: "
                 << u_result[i][j][k] << std::endl;
      }
    }
  }
  */

  int b = 0;
  for(int i = 0; i < dims[0]; i++) {
    for(int j = 0; j < dims[1]; j++) {
      for(int k = 0; k < dims[2]; k++) {
        if( (array[i][j][k]!=(T_send)(b+2)) ||
            (u_result[i][j][k]!=(T_recv)(b+2)) ) {
              throw std::runtime_error("The results do not match for "\
				                               "the 3d put and get test!");
            }
        b++;
      }
    }
  }

  std::string g_type;
  std::vector<size_t> g_dims;
  void* g_result;
  client.get_tensor(key, g_type, g_result, g_dims, MemoryLayout::nested);
  T_recv*** g_type_result = (T_recv***)g_result;

  if(type.compare(g_type)!=0)
    throw std::runtime_error("The tensor type " + g_type + " "\
                             "retrieved with client.get_tensor() "\
                             "does not match the known type " +
                             type);

  if(g_dims!=dims)
    throw std::runtime_error("The tensor dimensions retrieved "\
                             "client.get_tensor() do not match "\
                             "the known tensor dimensions.");
  /*
  for(int i = 0; i < dims[0]; i++) {
    for(int j = 0; j < dims[1]; j++) {
      for(int k = 0; k < dims[2]; k++) {
        std::cout<< "Value " << i << "," << j << "," << k
                 << " Sent: " << array[i][j][k] <<" Received: "
                 << g_result[i][j][k] << std::endl;
      }
    }
  }
  */

  int c = 0;
  for(int i = 0; i < dims[0]; i++) {
    for(int j = 0; j < dims[1]; j++) {
      for(int k = 0; k < dims[2]; k++) {
        if( (array[i][j][k]!=(T_send)(c+2)) ||
            (g_type_result[i][j][k]!=(T_recv)(c+2)) ) {
              throw std::runtime_error("The results do not match for "\
				                               "the 3d put and get test!");
            }
        c++;
      }
    }
  }

  free_3D_array(array, dims[0], dims[1]);
  free_3D_array(u_result, dims[0], dims[1]);

  return;
}

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);

  size_t dim1 = 2;
  size_t dim2 = 6;
  size_t dim3 = 3;

  std::vector<size_t> dims = {dim1, dim2, dim3};

  put_get_3D_array<double,double>(dims, "DOUBLE", "_dbl");

  put_get_3D_array<float,float>(dims, "FLOAT", "_flt");

  put_get_3D_array<int64_t,int64_t>(dims, "INT64", "_i64");

  put_get_3D_array<int32_t,int32_t>(dims, "INT32", "_i32");

  put_get_3D_array<int16_t,int16_t>(dims, "INT16", "_i16");

  put_get_3D_array<int8_t,int8_t>(dims, "INT8", "_i8");

  put_get_3D_array<uint16_t,uint16_t>(dims, "UINT16", "_ui16");

  put_get_3D_array<uint8_t,uint8_t>(dims, "UINT8", "_ui8");

  std::cout<<"3D put and get test using "\
             "static values is complete."<<std::endl;

  MPI_Finalize();

  return 0;
}