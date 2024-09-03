# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# Find the TinyXML library
# Specify the path to OpenCL headers
# Specify the path to OpenCL libraries

find_package(OpenCL REQUIRED)
if(OpenCL_FOUND)
  message(STATUS "OpenCL found!")
  message(STATUS "OpenCL include directory: ${OpenCL_INCLUDE_DIRS}")
  message(STATUS "OpenCL libraries: ${OpenCL_LIBRARIES}")
else()
  message(FATAL_ERROR "OpenCL not found.")
endif()

message(STATUS "OpenCL_INCLUDE_DIRS: ${OpenCL_INCLUDE_DIRS}")
message(STATUS "OpenCL_LIBRARIES: ${OpenCL_LIBRARIES}")

# to include CL
include_directories(/localhdd/hza214/CL-lib) 
include_directories(/localhdd/hza214/arrow-lib2/include)
include_directories(/localhdd/hza214/orc-lib/orc/build-3.21.3/ORC-2.1.0-SNAPSHOT-Linux/include)
include_directories(/localhdd/hza214/2020.2/include)
include_directories(/opt/xilinx/xrt/include)
include_directories(/localhdd/hza214/protobuf/include)
include_directories(/localhdd/hza214/tapa)

# Link directory where Arrow and Parquet libraries are located
link_directories(/localhdd/hza214/arrow-lib2/lib)
link_directories(/localhdd/hza214/2020.2/lib)
link_directories(/opt/xilinx/xrt/lib)
link_directories(/localhdd/hza214/protobuf/lib)
link_directories(/localhdd/hza214/tapa)
link_directories(/localhdd/hza214/orc-lib/orc/build-3.21.3/ORC-2.1.0-SNAPSHOT-Linux/lib)

#find_arrow_lib(${PARQUET_LIB_NAME})
set(ORC_LIBRARIES orc protobuf zstd snappy lz4 pthread z)

set(VELOX_BENCHMARK_COMMON_SRCS common/FileReaderIterator.cc common/BenchmarkUtils.cc)
add_library(velox_benchmark_common STATIC ${VELOX_BENCHMARK_COMMON_SRCS})
target_include_directories(velox_benchmark_common PUBLIC 
${CMAKE_SOURCE_DIR}/velox ${CMAKE_SOURCE_DIR}/core /localhdd/hza214/arrow-lib2/include 
/localhdd/hza214/orc-lib/orc/build-3.21.3/ORC-2.1.0-SNAPSHOT-Linux/include /local-scratch/Xilinx/Vitis_HLS/2020.2/include
 /opt/xilinx/xrt/include /localhdd/hza214/CL-lib  /localhdd/hza214/tapa /localhdd/hza214/protobuf/include)
target_link_libraries(velox_benchmark_common PUBLIC tinyxml rt tapa gmp mpfr gflags velox ${OpenCL_LIBRARIES} /localhdd/hza214/arrow-lib2/lib/libarrow.a 
/localhdd/hza214/orc-lib/orc/build-3.21.3/ORC-2.1.0-SNAPSHOT-Linux/lib/liborc.a 
/localhdd/hza214/orc-lib/orc/build-3.21.3/ORC-2.1.0-SNAPSHOT-Linux/lib/libzstd.a
/localhdd/hza214/orc-lib/orc/build-3.21.3/ORC-2.1.0-SNAPSHOT-Linux/lib/libz.a
/localhdd/hza214/orc-lib/orc/build-3.21.3/ORC-2.1.0-SNAPSHOT-Linux/lib/liblz4.a
/localhdd/hza214/orc-lib/orc/build-3.21.3/ORC-2.1.0-SNAPSHOT-Linux/lib/libsnappy.a
/localhdd/hza214/orc-lib/orc/build-3.21.3/ORC-2.1.0-SNAPSHOT-Linux/lib/libzstd.a

 /localhdd/hza214/protobuf/lib/libprotobuf.a benchmark::benchmark google::glog /localhdd/hza214/arrow-build/arrow/cpp/build-orc/jemalloc_ep-prefix/src/jemalloc_ep/lib/libjemalloc.a)

function(add_velox_benchmark BM_EXEC BM_FILE)
  add_executable(${BM_EXEC} ${BM_FILE})
  target_link_libraries(${BM_EXEC} PRIVATE velox_benchmark_common OpenCL::OpenCL /usr/lib/x86_64-linux-gnu/libOpenCL.so)
endfunction()

# Generic benchmark
add_velox_benchmark(generic_benchmark GenericBenchmark.cc)

add_velox_benchmark(columnar_to_row_benchmark ColumnarToRowBenchmark.cc)

add_velox_benchmark(parqupet_write_benchmark ParquetWriteBenchmark.cc)

add_velox_benchmark(plan_validator_util PlanValidatorUtil.cc)

add_velox_benchmark(shuffle_split_benchmark ShuffleSplitBenchmark.cc)

if(ENABLE_ORC)
  add_velox_benchmark(orc_converter exec/OrcConverter.cc)
endif()
