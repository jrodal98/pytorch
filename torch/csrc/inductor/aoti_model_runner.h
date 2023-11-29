#if !defined(C10_MOBILE) && !defined(ANDROID)
#pragma once

#include <ATen/Tensor.h>
#include <torch/csrc/inductor/aoti_runtime/interface.h>

// Forward declare DynamicLibrary
namespace at {
struct DynamicLibrary;
}

namespace torch::inductor {

class TORCH_API AOTIModelRunner {
 public:
  AOTIModelRunner() = delete;
  AOTIModelRunner(const AOTIModelRunner& other) = delete;
  AOTIModelRunner(AOTIModelRunner&& other) = delete;
  AOTIModelRunner& operator=(const AOTIModelRunner& other) = delete;
  AOTIModelRunner& operator=(AOTIModelRunner&& other) = delete;
  ~AOTIModelRunner();

  std::vector<at::Tensor> run(
      std::vector<at::Tensor> inputs,
      AOTInductorStreamHandle stream_handle = nullptr,
      AOTIProxyExecutorHandle proxy_executor_handle = nullptr);

  AOTIModelRunner(const char* model_path);

 protected:
  std::unique_ptr<at::DynamicLibrary> model_so_;
  decltype(&AOTInductorModelCreate) create_func_{nullptr};
  decltype(&AOTInductorModelDelete) delete_func_{nullptr};
  decltype(&AOTInductorModelGetNumOutputs) get_num_outputs_func_{nullptr};
  decltype(&AOTInductorModelRun) run_func_{nullptr};
  AOTInductorModelHandle model_handle_ = nullptr;
};

class TORCH_API AOTIModelRunnerCpu : public AOTIModelRunner {
 public:
  AOTIModelRunnerCpu(const char* model_path) : AOTIModelRunner(model_path) {}

  std::vector<at::Tensor> run(
      std::vector<at::Tensor> inputs,
      AOTIProxyExecutorHandle proxy_executor_handle = nullptr) {
    return AOTIModelRunner::run(inputs, nullptr, proxy_executor_handle);
  }
};

} // namespace torch::inductor
#endif
