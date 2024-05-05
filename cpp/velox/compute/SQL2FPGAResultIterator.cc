/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "WholeStageResultIterator.h"
#include "VeloxBackend.h"
#include "VeloxRuntime.h"
#include "config/GlutenConfig.h"
#include "velox/connectors/hive/HiveConfig.h"
#include "velox/connectors/hive/HiveConnectorSplit.h"
#include "velox/exec/PlanNodeStats.h"

#include "utils/ConfigExtractor.h"


using namespace facebook;

namespace gluten {

using offloadPtr = void(*)(
    const std::vector<std::shared_ptr<ResultIterator>>& inputs);

void* loadLibrary(
    const std::filesystem::path& path,
    void* initArgument = nullptr) {
  // TODO error handling
  void* loadedLib = dlopen(path.string().c_str(), kDefaultDlopenFlags);
  return loadedLib;
}


/*template <typename Func>
Func getFunction(const std::string& functionName, void* libraryPtr) {
  constexpr bool isFunctionPtr = std::is_pointer_v<Func> &&
      std::is_function_v<std::remove_pointer_t<Func>>;
  static_assert(
      isFunctionPtr, "Template argument need to be a function pointer");
  if (loadedLibraries.find(libraryPtr) == loadedLibraries.end()) {
    throw NativeLibraryLoaderException(
        fmt::format("No library loaded at {}", libraryPtr));
  };
  auto func =
      loadedLibraries[libraryPtr].functionTable->exportedFunctions.find(
          functionName);
  if (func ==
      loadedLibraries[libraryPtr].functionTable->exportedFunctions.end()) {
    throw NativeLibraryLoaderException(fmt::format(
        "library loaded at {} doesn't export function {}",
        libraryPtr,
        functionName));
  }
  if (typeid(Func).hash_code() != func->second.typeInfo) {
    throw NativeLibraryLoaderException(fmt::format(
        "library loaded at {} exports function {} with a different type",
        libraryPtr,
        functionName));
  };
  return reinterpret_cast<Func>(func->second.functionAddress);
}*/

void offload(void* handle, const std::vector<std::shared_ptr<ResultIterator>>& inputs) {
  auto offloadPtr = reinterpret_cast<offloadPtr>(dlsym(handle, "offload"));
  //Check if the function was found
  if (!offloadPtr) {
      std::cerr << "Error finding function " << "offload" << ": " << dlerror() << std::endl;
      return;
  }
  (*offloadPtr)(inputs);
}


SQL2FPGAResultIterator::SQL2FPGAResultIterator(
    const std::vector<std::shared_ptr<ResultIterator>>& inputs,
    const std::unordered_map<std::string, std::string>& confMap)
    : inputs(inputs), confMap_(confMap) {
  // TODO: load the .so if generated, skip if already loaded
  nativeFuncHandle_ = loadLibrary("/localhdd/hza215/gluten/SQL2FPGA/libsql2fpga.so");

  offload(nativeFuncHandle_, inputs);
}


std::shared_ptr<ColumnarBatch> SQL2FPGAResultIterator::next() {
  auto startTime = std::chrono::steady_clock::now();
  auto batch = getNextBatch();
  //DLOG(INFO) << "FPGAIterator get a batch, num rows: " << (batch ? batch->num_rows() : 0);
  collectBatchTime_ +=
      std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - startTime).count();
  if (batch == nullptr) {
    return nullptr;
  }
  return std::make_shared<gluten::ArrowColumnarBatch>(batch);
 
}

int64_t SQL2FPGAResultIterator::spillFixedSize(int64_t size) {
  return 0;
}

} // namespace gluten
