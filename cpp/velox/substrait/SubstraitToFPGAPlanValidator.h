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

#include "velox/core/QueryCtx.h"

namespace gluten {

/// This class is used to validate whether the computing of
/// a Substrait plan is supported in Velox.
class SubstraitToFPGAPlanValidator {
 public:
  SubstraitToFPGAPlanValidator();

  /// Used to validate whether the computing of this Write is supported.
  bool validate(const ::substrait::WriteRel& writeRel);

  /// Used to validate whether the computing of this Limit is supported.
  bool validate(const ::substrait::FetchRel& fetchRel);

  /// Used to validate whether the computing of this Expand is supported.
  bool validate(const ::substrait::ExpandRel& expandRel);

  /// Used to validate whether the computing of this Generate is supported.
  bool validate(const ::substrait::GenerateRel& generateRel);

  /// Used to validate whether the computing of this Sort is supported.
  bool validate(const ::substrait::SortRel& sortRel);

  /// Used to validate whether the computing of this Window is supported.
  bool validate(const ::substrait::WindowRel& windowRel);

  /// Used to validate whether the computing of this Aggregation is supported.
  bool validate(const ::substrait::AggregateRel& aggRel);

  /// Used to validate whether the computing of this Project is supported.
  bool validate(const ::substrait::ProjectRel& projectRel);

  /// Used to validate whether the computing of this Filter is supported.
  bool validate(const ::substrait::FilterRel& filterRel);

  /// Used to validate Join.
  bool validate(const ::substrait::JoinRel& joinRel);

  /// Used to validate Cartesian product.
  bool validate(const ::substrait::CrossRel& crossRel);

  /// Used to validate whether the computing of this Read is supported.
  bool validate(const ::substrait::ReadRel& readRel);

  /// Used to validate whether the computing of this Rel is supported.
  bool validate(const ::substrait::Rel& rel);

  /// Used to validate whether the computing of this RelRoot is supported.
  bool validate(const ::substrait::RelRoot& relRoot);

  /// Used to validate whether the computing of this Plan is supported.
  bool validate(const ::substrait::Plan& plan);


};

} // namespace gluten
