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
#pragma once

#include "compute/Runtime.h"
#include "iceberg/IcebergPlanConverter.h"
#include "memory/ColumnarBatchIterator.h"
#include "memory/VeloxColumnarBatch.h"
#include "substrait/SubstraitToVeloxPlan.h"
#include "substrait/plan.pb.h"
#include "utils/metrics.h"
#include "velox/connectors/hive/iceberg/IcebergSplit.h"
#include "velox/core/Config.h"
#include "velox/core/PlanNode.h"
#include "velox/exec/Task.h"

#include <dlfcn.h>

namespace gluten {

class SQL2FPGAResultIterator : public ColumnarBatchIterator {
 public:
  SQL2FPGAResultIterator(
    const std::vector<std::shared_ptr<ResultIterator>>& inputs,
    const std::unordered_map<std::string, std::string>& confMap);

  virtual ~SQL2FPGAResultIterator() {
    dlclose(nativeFuncHandle_);
  }

  std::shared_ptr<ColumnarBatch> next() override;

  int64_t spillFixedSize(int64_t size) override;


 private:
  std::unordered_map<std::string, std::string> getQueryContextConf();
  std::vector<std::shared_ptr<ResultIterator>> inputs;
  std::unordered_map<std::string, std::string>& confMap_;
  void* nativeFuncHandle_;

  SQL2FPGA::Kernel kernel_;

  static const int kDefaultDlopenFlags = RTLD_LOCAL | RTLD_LAZY;
};

} // namespace gluten
