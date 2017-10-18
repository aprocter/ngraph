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

#include <vector>

#include "ngraph/node.hpp"
#include "ngraph/runtime/tensor_view_info.hpp"
#include "ngraph/runtime/cpu/external_function.hpp"
#include "ngraph/runtime/cpu/emitter.hpp"

using namespace std;
using namespace ngraph::runtime::cpu;

void Emitter::EmitAdd(const ngraph::Node*,
                      ExternalFunction*,
                      FunctionMap&,
                      const std::vector<TensorViewInfo>& inputs,
                      const std::vector<TensorViewInfo>& outputs) const
{

}
         
void Emitter::EmitDot(const ngraph::Node*,
                      ExternalFunction*,
                      FunctionMap&,
                      const std::vector<TensorViewInfo>& inputs,
                      const std::vector<TensorViewInfo>& outputs) const
{

}
         
