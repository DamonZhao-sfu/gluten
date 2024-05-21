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

/*#include <arrow/ipc/api.h>

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

void offload(void* handle, const std::vector<std::shared_ptr<ResultIterator>>& inputs) {
  auto offloadPtr = reinterpret_cast<offloadPtr>(dlsym(handle, "offload"));
  char *err = dlerror();
  //Check if the function was found
  if (err != nullptr) {
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

  auto context = std::make_unique<SQL2FPGA::Context>();

  for (auto iter : inputs) {
    std::vector<std::shared_ptr<arrow::RecordBatch>> batches;
    while(iter->hasNext()) {
      auto batch = iter->next();
      std::shared_ptr<ArrowSchema> schema = batch->exportArrowSchema();
      std::shared_ptr<ArrowArray> array = batch->exportArrowArray();
      auto rb = gluten::arrowGetOrThrow(arrow::ImportRecordBatch(array.get(), schema.get()));
      batches.push_back(rb);
    }
    context->enqueueBatches(batches);
  }

  kernel_ = std::make_unique<SQL2FPGA::Kernel>(context, "gqeJoin");
  // Reset the kernel.
  kernel_->Reset();

  // Start the kernel.
  kernel_->Start();

  // Wait for the kernel to finish.
  kernel_->WaitForFinish();

  readIterator_ = kernel_->getResultIterator();

}


std::shared_ptr<ColumnarBatch> SQL2FPGAResultIterator::next() {

  if (readIterator_->HasNext()) {
    auto startTime = std::chrono::steady_clock::now();
    auto batch = readIterator_->Next();
    collectBatchTime_ += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - startTime).count();
    if (batch == nullptr) {
        return nullptr;
    }
    return std::make_shared<gluten::ArrowColumnarBatch>(batch);
  }
  return nullptr;
}

int64_t SQL2FPGAResultIterator::spillFixedSize(int64_t size) {
  return 0;
}

} // namespace gluten*/
