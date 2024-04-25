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

#ifdef ENABLE_HDFS
#include "utils/HdfsUtils.h"
#endif

using namespace facebook;

namespace gluten {

namespace {
// Velox configs
} // namespace

SQL2FPGAResultIterator::SQL2FPGAResultIterator(
    const std::vector<std::shared_ptr<ResultIterator>>& inputs,
    const std::unordered_map<std::string, std::string>& confMap)
    : iters_(inputs), confMap_(confMap)
{

}

std::shared_ptr<ColumnarBatch> SQL2FPGAResultIterator::next() {
  
  // FOR SORT, AGG that accepts only one input stream
  if (iters_.length == 1) {
    auto inputIterator = iters_[0].get()
    while(inputIterator->hasNext()) {
        auto inputBatch = inputIterator->next();
        // TODO implement fpgaJniIterator
        auto outputBatch = fpgaJniIterator->next(inputBatch);
        if (outputBatch == nullptr) {
            return nullptr;
        }
        uint64_t batchRows = outputBatch->size();
        if (batchRows == 0) {
            return nullptr;
        }
        return std::make_shared<ArrowColumnarBatch>(outputBatch);
    }
   
  } else {
    // FOR JOIN that accept multiple input streams
    // Todo: pass build side iterator, use all data to build the hash table and then probing side process in batch
  }
 
}

int64_t SQL2FPGAResultIterator::spillFixedSize(int64_t size) {
  return 0;
}

} // namespace gluten
