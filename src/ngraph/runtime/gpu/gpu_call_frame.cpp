// ----------------------------------------------------------------------------
// Copyright 2017 Nervana Systems Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// ----------------------------------------------------------------------------

#include <cstdlib>
#include <fstream>
#include <stdio.h>

#include <cuda_runtime.h>
#include "cublas.h"

#include "ngraph/runtime/gpu/gpu_call_frame.hpp"
#include "ngraph/runtime/gpu/gpu_external_function.hpp"
#include "ngraph/runtime/gpu/gpu_tensor_view.hpp"

using namespace std;
using namespace ngraph;

runtime::gpu::GPU_CallFrame::GPU_CallFrame(std::shared_ptr<GPU_ExternalFunction> external_function,
                                           EntryPoint compiled_function)
    : m_external_function(external_function)
    , m_compiled_function(compiled_function)
{
    cublasStatus_t stat = cublasCreate(&m_cublas_handle);
    if (stat != CUBLAS_STATUS_SUCCESS)
    {
        throw runtime_error("cuBLAS create failed");
    }
    cublasSetPointerMode(m_cublas_handle, CUBLAS_POINTER_MODE_HOST);
    // Pass scalars as reference on the device
    cublasSetPointerMode(m_cublas_handle, CUBLAS_POINTER_MODE_DEVICE);
}

runtime::gpu::GPU_CallFrame::~GPU_CallFrame()
{
    cublasDestroy(m_cublas_handle);
}

void runtime::gpu::GPU_CallFrame::tensor_call(
    const std::vector<std::shared_ptr<ngraph::runtime::TensorView>>& input_tvs,
    const std::vector<std::shared_ptr<ngraph::runtime::TensorView>>& output_tvs)
{
    // Host tensors
    vector<void*> inputs;
    vector<void*> outputs;

    for (size_t i = 0; i < input_tvs.size(); i++)
    {
        shared_ptr<runtime::gpu::GPU_TensorView> tv =
            static_pointer_cast<runtime::gpu::GPU_TensorView>(input_tvs[i]);
        inputs.push_back(tv->m_allocated_buffer_pool);
    }
    for (size_t i = 0; i < output_tvs.size(); i++)
    {
        shared_ptr<runtime::gpu::GPU_TensorView> tv =
            static_pointer_cast<runtime::gpu::GPU_TensorView>(output_tvs[i]);
        outputs.push_back(tv->m_allocated_buffer_pool);
    }

    m_compiled_function(inputs.data(), outputs.data(), m_cublas_handle);
}

void runtime::gpu::GPU_CallFrame::call(
    const std::vector<std::shared_ptr<runtime::TensorView>>& arguments,
    const std::vector<std::shared_ptr<runtime::TensorView>>& results)
{
    // TODO: Check types of args and result
    vector<shared_ptr<runtime::TensorView>> inputs;
    for (shared_ptr<runtime::TensorView> argument : arguments)
    {
        argument->collect_tensor_views(inputs, argument);
    }

    vector<shared_ptr<runtime::TensorView>> outputs;
    for (shared_ptr<runtime::TensorView> result : results)
    {
        result->collect_tensor_views(outputs, result);
    }

    tensor_call(inputs, outputs);
}
