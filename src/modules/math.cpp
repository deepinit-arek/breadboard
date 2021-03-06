// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "breadboard/modules/math.h"

#include <algorithm>

#include "breadboard/base_node.h"
#include "breadboard/module_registry.h"

namespace breadboard {

// clang-format off
#define COMPARISON_NODE(name, op)                     \
  template <typename T>                               \
  class name : public BaseNode {                      \
   public:                                            \
    virtual ~name() {}                                \
                                                      \
    static void OnRegister(NodeSignature* node_sig) { \
      node_sig->AddInput<T>();                        \
      node_sig->AddInput<T>();                        \
      node_sig->AddOutput<bool>();                    \
    }                                                 \
                                                      \
    virtual void Initialize(NodeArguments* args) {    \
      auto a = args->GetInput<T>(0);                  \
      auto b = args->GetInput<T>(1);                  \
      bool result = *a op *b;                         \
      args->SetOutput(0, result);                     \
    }                                                 \
                                                      \
    virtual void Execute(NodeArguments* args) {       \
      Initialize(args);                               \
    }                                                 \
  }

#define ARITHMETIC_NODE(name, op)                     \
  template <typename T>                               \
  class name : public BaseNode {                      \
   public:                                            \
    virtual ~name() {}                                \
                                                      \
    static void OnRegister(NodeSignature* node_sig) { \
      node_sig->AddInput<T>();                        \
      node_sig->AddInput<T>();                        \
      node_sig->AddOutput<T>();                       \
    }                                                 \
                                                      \
    virtual void Initialize(NodeArguments* args) {    \
      auto a = args->GetInput<T>(0);                  \
      auto b = args->GetInput<T>(1);                  \
      T result = *a op *b;                            \
      args->SetOutput(0, result);                     \
    }                                                 \
                                                      \
    virtual void Execute(NodeArguments* args) {       \
      Initialize(args);                               \
    }                                                 \
  }

// Returns true if both input values are equal.
COMPARISON_NODE(EqualsNode, ==);

// Returns true if both input values are not equal.
COMPARISON_NODE(NotEqualsNode, !=);

// Returns true if the first input is greater than the second input.
COMPARISON_NODE(GreaterThanNode, >);

// Returns true if the first input is greater than or equal to the second input.
COMPARISON_NODE(GreaterThanOrEqualsNode, >=);

// Returns true if the first input is less than the second input.
COMPARISON_NODE(LessThanNode, <);

// Returns true if the first input is less than or equal to the second input.
COMPARISON_NODE(LessThanOrEqualsNode, <=);

// Retuns the sum of the arguments.
ARITHMETIC_NODE(AddNode, +);

// Retuns the difference of the arguments.
ARITHMETIC_NODE(SubtractNode, -);

// Retuns the product of the arguments.
ARITHMETIC_NODE(MultiplyNode, *);

// Retuns the quotient of the arguments.
ARITHMETIC_NODE(DivideNode, /);
// clang-format on

template <typename T>
class MaxNode : public BaseNode {
 public:
  virtual ~MaxNode() {}

  static void OnRegister(NodeSignature* node_sig) {
    node_sig->AddInput<T>();
    node_sig->AddInput<T>();
    node_sig->AddOutput<T>();
  }

  virtual void Execute(NodeArguments* args) {
    auto a = args->GetInput<T>(0);
    auto b = args->GetInput<T>(1);
    args->SetOutput(0, std::max(*a, *b));
  }
};

template <typename T>
class MinNode : public BaseNode {
 public:
  virtual ~MinNode() {}

  static void OnRegister(NodeSignature* node_sig) {
    node_sig->AddInput<T>();
    node_sig->AddInput<T>();
    node_sig->AddOutput<T>();
  }

  virtual void Execute(NodeArguments* args) {
    auto a = args->GetInput<T>(0);
    auto b = args->GetInput<T>(1);
    args->SetOutput(0, std::min(*a, *b));
  }
};

template <typename T>
void InitializeMathModuleType(ModuleRegistry* module_registry,
                              const char* name) {
  Module* module = module_registry->RegisterModule(name);
  module->RegisterNode<EqualsNode<T>>("equals");
  module->RegisterNode<NotEqualsNode<T>>("not_equals");
  module->RegisterNode<GreaterThanNode<T>>("greater_than");
  module->RegisterNode<GreaterThanOrEqualsNode<T>>("greater_than_or_equals");
  module->RegisterNode<LessThanNode<T>>("less_than");
  module->RegisterNode<LessThanOrEqualsNode<T>>("less_than_or_equals");
  module->RegisterNode<AddNode<T>>("add");
  module->RegisterNode<SubtractNode<T>>("subtract");
  module->RegisterNode<MultiplyNode<T>>("multiply");
  module->RegisterNode<DivideNode<T>>("divide");
  module->RegisterNode<MaxNode<T>>("max");
  module->RegisterNode<MinNode<T>>("min");
}

void InitializeIntegerMathModule(ModuleRegistry* module_registry) {
  InitializeMathModuleType<int>(module_registry, "integer_math");
}

void InitializeFloatMathModule(ModuleRegistry* module_registry) {
  InitializeMathModuleType<float>(module_registry, "float_math");
}

}  // namespace breadboard
