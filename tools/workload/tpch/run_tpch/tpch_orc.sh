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
#please choose right os system jar
GLUTEN_JAR=/localhdd/hza215/gluten/backends-velox/target/backends-velox-1.2.0-SNAPSHOT-3.2.jar
SPARK_HOME=/localhdd/hza215/spark/spark-3.2.1-bin-hadoop3.2/
cat tpch_orc.scala | ${SPARK_HOME}/bin/spark-shell \
  --master yarn --deploy-mode client \
  --conf spark.plugins=io.glutenproject.GlutenPlugin \
  --conf spark.driver.extraClassPath=${GLUTEN_JAR} \
  --conf spark.executor.extraClassPath=${GLUTEN_JAR} \
  --conf spark.memory.offHeap.enabled=true \
  --conf spark.memory.offHeap.size=2g \
  --conf spark.gluten.sql.columnar.forceShuffledHashJoin=true \
  --conf spark.shuffle.manager=org.apache.spark.shuffle.sort.ColumnarShuffleManager \
  --num-executors 3 \
  --executor-cores 3 \
  --driver-memory 2g \
  --executor-memory 2g \
  --conf spark.executor.memoryOverhead=2g \
  --conf spark.driver.maxResultSize=2g