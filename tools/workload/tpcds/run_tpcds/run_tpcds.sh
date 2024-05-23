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

#export CXX=$(conda info --root)/envs/velox-build/bin/x86_64-conda-linux-gnu-g++
#export CC=$(conda info --root)/envs/velox-build/bin/x86_64-conda-linux-gnu-gcc
#export LD_LIBRARY_PATH=$(conda info --root)/envs/velox-build/lib:$LD_LIBRARY_PATH
#export CPATH=$(conda info --root)/envs/velox-build/include
export SPARK_LOCAL_DIRS=/localssd/hza214/sparktmp
#export GLUTEN_JAR=/localhdd/hza214/spark/spark-3.2.2-bin-hadoop2.7/jars/gluten-package-1.2.0-SNAPSHOT.jar 
#--conf spark.plugins=org.apache.gluten.GlutenPlugin \
  #--conf spark.driver.extraClassPath=${GLUTEN_JAR} \
  #--conf spark.executor.extraClassPath=${GLUTEN_JAR} \
    #--conf spark.gluten.sql.columnar.forceShuffledHashJoin=true \
  
  

#/localhdd/hza214/spark/spark-3.2.2-bin-hadoop2.7/bin/spark-shell   --conf spark.memory.offHeap.enabled=true   --conf spark.memory.offHeap.size=160g --conf spark.plugins=org.apache.gluten.GlutenPlugin   --conf spark.sql.adaptive.enabled=true    --conf spark.local.dir=/mnt/glusterfs/users/hza214/tmp   --conf spark.shuffle.manager=org.apache.spark.shuffle.sort.ColumnarShuffleManager   --num-executors 48   --executor-cores 48   --driver-memory 128g   --executor-memory 128g   --conf spark.gluten.sql.injectNativePlanStringToExplain=true  --conf spark.gluten.sql.debug=true  --conf spark.executor.memoryOverhead=160g   --conf spark.driver.maxResultSize=32g --conf spark.serializer=org.apache.spark.serializer.KryoSerializer --jars /localhdd/hza214/spark-tera sort/target/spark-terasort-1.2-SNAPSHOT-jar-with-dependencies.jar

#--conf spark.plugins=org.apache.gluten.GlutenPlugin \
  #--conf spark.gluten.sql.injectNativePlanStringToExplain=true\
  #--conf spark.gluten.sql.debug=true\
cat tpcds_orc.scala |  /localhdd/hza214/spark/bin/spark-shell \
  --conf spark.memory.offHeap.enabled=true \
  --conf spark.memory.offHeap.size=30g \
  --conf spark.sql.adaptive.enabled=true \
  --conf spark.local.dir=/localssd/hza214/sparktmp \
  --conf spark.shuffle.manager=org.apache.spark.shuffle.sort.ColumnarShuffleManager \
  --num-executors 48 \
  --executor-cores 48 \
  --driver-memory 140g \
  --executor-memory 140g \
  --conf spark.executor.memoryOverhead=10g &

  # If there are some "*.so" libs dependencies issues on some specific Distros,
  # try to enable spark.gluten.loadLibFromJar and build your own gluten-thirdparty-lib Jar.
  # e.g.
  #   --conf spark.gluten.loadLibFromJar=true \
  #   --jars /PATH_TO_GLUTEN_HOME/package/target/thirdparty-lib/gluten-thirdparty-lib-ubuntu-22.04-x86_64.jar,
  #          /PATH_TO_GLUTEN_HOME/package/target/gluten-velox-bundle-spark3.3_2.12-ubuntu_22.04_x86_64-1.2.0-SNAPSHOT.jar
