#include "pyclient.h"

using namespace SILC;

namespace py = pybind11;

PyClient::PyClient(bool cluster)
{
  Client* client = new Client(cluster);
  this->_client = client;
}

PyClient::~PyClient() {
  delete this->_client;
}

void PyClient::put_tensor(std::string& key, std::string& type, py::array data) {

  auto buffer = data.request();
  void* ptr = buffer.ptr;

  // get dims
  std::vector<size_t> dims(buffer.ndim);
  for (int i=0; i < buffer.shape.size(); i++) {
      dims[i] = (size_t) buffer.shape[i];
  }

  TensorType ttype = TENSOR_TYPE_MAP.at(type);
  this->_client->put_tensor(key, ptr, dims, ttype,
                            MemoryLayout::contiguous);
  return;
}

py::array PyClient::get_tensor(std::string& key) {

  // placeholder variables to be filled
  TensorType type;
  std::vector<size_t> dims;
  void* ptr;

  // call C++ client
  this->_client->get_tensor(key, ptr, dims, type,
                            MemoryLayout::contiguous);

  // detect data type
  switch(type) {
    case TensorType::dbl : {
      double* data;
      data = (double*) ptr;
      return py::array(dims, data, py::none());
      break;
    }
    case TensorType::flt : {
      float* data;
      data = (float*) ptr;
      return py::array(dims, data, py::none());
      break;
    }
    case TensorType::int64 : {
      int64_t* data;
      data = (int64_t*) ptr;
      return py::array(dims, data, py::none());
      break;
    }
    case TensorType::int32 : {
      int32_t* data;
      data = (int32_t*) ptr;
      return py::array(dims, data, py::none());
      break;
    }
    case TensorType::int16 : {
      int16_t* data;
      data = (int16_t*) ptr;
      return py::array(dims, data, py::none());
      break;
    }
    case TensorType::int8 : {
      int8_t* data;
      data = (int8_t*) ptr;
      return py::array(dims, data, py::none());
      break;
    }
    case TensorType::uint16 : {
      uint16_t* data;
      data = (uint16_t*) ptr;
      return py::array(dims, data, py::none());
      break;
    }
    case TensorType::uint8 : {
      uint8_t* data;
      data = (uint8_t*) ptr;
      return py::array(dims, data, py::none());
      break;
    }
    default :
      // TODO throw python expection here
      throw std::runtime_error("Could not infer type");
      break;
  }
}

void PyClient::put_dataset(PyDataset& dataset) {

  this->_client->put_dataset(*dataset.get());
  return;
}

PyDataset* PyClient::get_dataset(const std::string& name) {
  DataSet* data = new DataSet(this->_client->get_dataset(name));
  PyDataset* dataset = new PyDataset(*data);
  return dataset;
}

void PyClient::set_script_from_file(const std::string& key,
                                    const std::string& device,
                                    const std::string& script_file)
{
  this->_client->set_script_from_file(key, device, script_file);
  return;
}

void PyClient::set_script(const std::string& key,
                          const std::string& device,
                          const std::string_view& script)
{
  this->_client->set_script(key, device, script);
  return;
}

std::string_view PyClient::get_script(const std::string& key) {
  return this->_client->get_script(key);
}

void PyClient::run_script(const std::string& key,
                const std::string& function,
                std::vector<std::string>& inputs,
                std::vector<std::string>& outputs)
{
  this->_client->run_script(key, function, inputs, outputs);
  return;
}

py::bytes PyClient::get_model(const std::string& key) {
  std::string model = std::string(this->_client->get_model(key));
  model = py::bytes(model);
  return model;
}

void PyClient::set_model(const std::string& key,
                 const std::string_view& model,
                 const std::string& backend,
                 const std::string& device,
                 int batch_size,
                 int min_batch_size,
                 const std::string& tag,
                 const std::vector<std::string>& inputs,
                 const std::vector<std::string>& outputs)
{
  this->_client->set_model(key, model, backend, device,
                           batch_size, min_batch_size, tag,
                           inputs, outputs);
  return;
}

void PyClient::set_model_from_file(const std::string& key,
                                   const std::string& model_file,
                                   const std::string& backend,
                                   const std::string& device,
                                   int batch_size,
                                   int min_batch_size,
                                   const std::string& tag,
                                   const std::vector<std::string>& inputs,
                                   const std::vector<std::string>& outputs)
{
  this->_client->set_model_from_file(key, model_file, backend, device,
                                     batch_size, min_batch_size, tag,
                                     inputs, outputs);
  return;
}

void PyClient::run_model(const std::string& key,
                         std::vector<std::string> inputs,
                         std::vector<std::string> outputs)
{
  this->_client->run_model(key, inputs, outputs);
  return;
}

void PyClient::set_data_source(const std::string& source_id)
{
  this->_client->set_data_source(source_id);
  return;
}

bool PyClient::key_exists(const std::string& key, bool use_prefix)
{
  return this->_client->key_exists(key, use_prefix);
}

bool PyClient::poll_key(const std::string& key,
                        bool use_prefix,
                        int poll_frequency_ms,
                        int num_tries)
{
  return this->_client->poll_key(key, use_prefix, poll_frequency_ms, num_tries);
}
