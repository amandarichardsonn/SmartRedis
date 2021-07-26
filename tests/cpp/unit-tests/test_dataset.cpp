#include "../../../third-party/catch/catch.hpp"
#include "dataset.h"

SCENARIO("Testing DataSet object", "[DataSet]")
{

    GIVEN("A DataSet object")
    {
        std::string dataset_name;
        dataset_name = "dataset_name";
        SmartRedis::DataSet dataset(dataset_name);

        WHEN("A tensor is added to the DataSet")
        {
            std::string tensor_name = "test_tensor";
            std::vector<size_t> dims = {1, 2, 3};
            TensorType type = TensorType::flt;
            size_t tensor_size = dims.at(0) * dims.at(1) * dims.at(2);
            std::vector<float> tensor(tensor_size, 2.0);
            void* data = tensor.data();
            MemoryLayout mem_layout = MemoryLayout::contiguous;
            dataset.add_tensor(tensor_name, data, dims, type, mem_layout);

            THEN("The tensor name can be retrieved")
            {
                std::vector<std::string> tensor_names =
                    dataset.get_tensor_names();
                CHECK(tensor_names[0] == tensor_name);
            }

            THEN("The tensor can be retrieved")
            {
                void* retrieved_data;
                std::vector<size_t> retrieved_dims;
                TensorType retrieved_type;
                dataset.get_tensor(tensor_name, retrieved_data, retrieved_dims,
                                   retrieved_type, mem_layout);

                // Ensure the retrieved tensor is correct
                CHECK(retrieved_dims == dims);
                CHECK(retrieved_type == type);
                float* data_ptr = (float*)data;
                float* ret_data_ptr = (float*)retrieved_data;
                for (int i=0; i<tensor_size; i++) {
                    CHECK(*data_ptr == *ret_data_ptr);
                    data_ptr++;
                    ret_data_ptr++;
                }
            }

            THEN("The tensor can be retrieved with the c-style interface")
            {
                void* retrieved_data;
                size_t* retrieved_dims;
                size_t retrieved_n_dims;
                TensorType retrieved_type;
                dataset.get_tensor(tensor_name, retrieved_data,
                                   retrieved_dims, retrieved_n_dims,
                                   retrieved_type, mem_layout);

                // Ensure the retrieved tensor is correct
                CHECK(retrieved_type == type);
                size_t* curr_retrieved_dim = retrieved_dims;
                for (size_t i=0; i<retrieved_n_dims; i++) {
                    CHECK(*curr_retrieved_dim == dims.at(i));
                    curr_retrieved_dim++;
                }
                float* data_ptr = (float*)data;
                float* ret_data_ptr = (float*)retrieved_data;
                for (int i=0; i<tensor_size; i++)
                    CHECK(*data_ptr++ == *ret_data_ptr++);
            }

            THEN("The tensor can be retrieved by unpacking")
            {
                void* retrieved_data =
                    malloc(dims[0] * dims[1] * dims[2] * sizeof(float));
                dataset.unpack_tensor(tensor_name, retrieved_data,
                                      dims, type, mem_layout);

                // Ensure the unpacked tensor is correct
                float* data_ptr = (float*)data;
                float* ret_data_ptr = (float*)retrieved_data;
                for (int i=0; i<tensor_size; i++) {
                    CHECK(*data_ptr == *ret_data_ptr);
                    data_ptr++;
                    ret_data_ptr++;
                }
            }

            THEN("The tensor cannot be retrieved if the "
                 "specified name does not exist")
            {
                void* retrieved_data;
                std::vector<size_t> retrieved_dims;
                TensorType retrieved_type;

                CHECK_THROWS_AS(
                    dataset.get_tensor("does_not_exist", retrieved_data,
                                       retrieved_dims, retrieved_type,
                                       mem_layout),
                    std::runtime_error);
            }
        }

        AND_WHEN("A meta scalar is added to the DataSet")
        {
            std::string meta_scalar_name = "flt_meta_scalars";
            float meta_scalar = 10.0;
            MetaDataType type = MetaDataType::flt;

            CHECK(dataset.has_field(meta_scalar_name) == false);
            dataset.add_meta_scalar(meta_scalar_name, &meta_scalar, type);
            CHECK(dataset.has_field(meta_scalar_name) == true);

            THEN("The meta scalar can be retrieved")
            {
                float* retrieved_data;
                size_t retrieved_length;
                dataset.get_meta_scalars(meta_scalar_name,
                                        (void*&)retrieved_data,
                                         retrieved_length, type);

                CHECK(retrieved_length == 1);
                CHECK(*retrieved_data == meta_scalar);
            }

            AND_THEN("The meta scalar can be cleared")
            {
                float* retrieved_data;
                size_t retrieved_length;
                dataset.clear_field(meta_scalar_name);

                // The meta scalar no longer exists
                CHECK_THROWS_AS(
                    dataset.get_meta_scalars(meta_scalar_name,
                                            (void*&)retrieved_data,
                                             retrieved_length, type),
                    std::runtime_error);
            }
        }

        AND_WHEN("A meta string is added to the DataSet")
        {
            std::string meta_str_name = "meta_string_name";
            std::string meta_str_val = "100";
            dataset.add_meta_string(meta_str_name, meta_str_val);

            THEN("The meta string can be retrieved")
            {
                char** meta_str_data;
                size_t n_strings;
                size_t* lengths;
                dataset.get_meta_strings(meta_str_name, meta_str_data,
                                         n_strings, lengths);
                CHECK(*meta_str_data == meta_str_val);
            }

            THEN("The meta string can be retrieved by name")
            {
                std::vector<std::string> meta_str_data =
                    dataset.get_meta_strings(meta_str_name);
                CHECK(meta_str_data[0] == meta_str_val);
            }
        }
    }
}