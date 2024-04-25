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

#include "SubstraitToFPGAPlanValidator.h"
#include <string>
#include "TypeUtils.h"
#include "utils/Common.h"

namespace gluten {

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::WriteRel& writeRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::FetchRel& fetchRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::GenerateRel& generateRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::ExpandRel& expandRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::WindowRel& windowRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::SortRel& sortRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::ProjectRel& projectRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::FilterRel& filterRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::JoinRel& joinRel) {
  
  switch (joinRel.type()) {
    case ::substrait::JoinRel_JoinType_JOIN_TYPE_INNER:
    case ::substrait::JoinRel_JoinType_JOIN_TYPE_OUTER:
    case ::substrait::JoinRel_JoinType_JOIN_TYPE_LEFT:
    case ::substrait::JoinRel_JoinType_JOIN_TYPE_RIGHT:
    case ::substrait::JoinRel_JoinType_JOIN_TYPE_LEFT_SEMI:
    case ::substrait::JoinRel_JoinType_JOIN_TYPE_RIGHT_SEMI:
    case ::substrait::JoinRel_JoinType_JOIN_TYPE_ANTI:
      break;
    default:
      LOG_VALIDATION_MSG("Sort merge join only support inner and left join.");
      return false;
  }
  return true;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::CrossRel& crossRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::AggregateRel& aggRel) {
  return true;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::ReadRel& readRel) {
  return false;
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::Rel& rel) {
  if (rel.has_aggregate()) {
    return validate(rel.aggregate());
  } else if (rel.has_project()) {
    return validate(rel.project());
  } else if (rel.has_filter()) {
    return validate(rel.filter());
  } else if (rel.has_join()) {
    return validate(rel.join());
  } else if (rel.has_cross()) {
    return validate(rel.cross());
  } else if (rel.has_read()) {
    return validate(rel.read());
  } else if (rel.has_sort()) {
    return validate(rel.sort());
  } else if (rel.has_expand()) {
    return validate(rel.expand());
  } else if (rel.has_generate()) {
    return validate(rel.generate());
  } else if (rel.has_fetch()) {
    return validate(rel.fetch());
  } else if (rel.has_window()) {
    return validate(rel.window());
  } else if (rel.has_write()) {
    return validate(rel.write());
  } else {
    return false;
  }
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::RelRoot& relRoot) {
  if (relRoot.has_input()) {
    return validate(relRoot.input());
  } else {
    return false;
  }
}

bool SubstraitToFPGAPlanValidator::validate(const ::substrait::Plan& plan) {
  for (const auto& rel : plan.relations()) {
    if (rel.has_root()) {
      return validate(rel.root());
    } else if (rel.has_rel()) {
      return validate(rel.rel());
    }
  }

  return false;
}

} // namespace gluten
