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
package org.apache.spark.sql.catalyst

import io.glutenproject.GlutenConfig

import org.apache.spark.sql.SparkSession
import org.apache.spark.sql.catalyst.plans.logical.Subquery
import org.apache.spark.sql.catalyst.plans.logical.LogicalPlan
import org.apache.spark.sql.catalyst.rules.Rule
import org.apache.spark.sql.catalyst.optimizer.Optimizer
import org.apache.spark.sql.catalyst.optimizer._
import org.apache.spark.sql.connector.catalog.CatalogManager
import org.apache.spark.sql.catalyst.plans.logical.{GlobalLimit, LocalLimit, Project}

import sfu.ca.hiaccel.{SQL2FPGA_Codegen, SQL2FPGA_QConfig, SQL2FPGA_QParser}

class DefaultCatalystOptimizer(catalogManager: CatalogManager) extends Optimizer(catalogManager) {
  def myBatches: Seq[Batch] = super.batches
}

case class SQL2FPGARewriteRule(session: SparkSession)
  extends Rule[LogicalPlan] {

  @transient val codegen = new SQL2FPGA_Codegen
  @transient val qParser = new SQL2FPGA_QParser
  @transient val qConfig = new SQL2FPGA_QConfig
  qConfig.pure_sw_mode = 0
  qConfig.scale_factor = 1
  // Define a function to determine whether a LogicalPlan node is a GlobalLimit, LocalLimit, or Project
  def isSpecialNode(plan: LogicalPlan): Boolean = plan match {
    case _: GlobalLimit | _: LocalLimit | _: Project => true
    case _ => false
  }

  override def apply(plan: LogicalPlan): LogicalPlan = {
    if (!plan.isInstanceOf[Subquery] && GlutenConfig.getConf.offloadToFPGA) {
      print("logicalplan before optimizer:" + plan)
      val catalogManager = session.sessionState.catalogManager
      val defaultOptimizer = new DefaultCatalystOptimizer(catalogManager)
      val optimizedPlan = defaultOptimizer.myBatches.foldLeft(plan) { (currentPlan, batch) =>
        batch.rules.foldLeft(currentPlan) { (plan, rule) =>
          rule.apply(plan)
        }
      }
      print("logicalplan Plan after optimizer" + optimizedPlan)

      // Drop the root occurrences of GlobalLimit, LocalLimit, and Project nodes from the logical plan
      val finalPlan = optimizedPlan match {
        case GlobalLimit(_, _) =>
          optimizedPlan.children.headOption match {
            case Some(LocalLimit(_, _)) =>
              optimizedPlan.children.headOption.flatMap(_.children.headOption) match {
                case Some(_: Project) => optimizedPlan.children.head.children.head.children.head
                case _ => optimizedPlan
              }
            case _ => optimizedPlan
          }
        case _ => optimizedPlan
      }

      print("final plan" + finalPlan)
      qParser.parse_SparkQPlan_to_SQL2FPGAQPlan(finalPlan, qConfig)
      qParser.qPlan.allocateOperators(0, 0)
      qParser.qPlan.genCode(null, qConfig)
      codegen.genHostCode(
        qParser.qPlan,
        qConfig.pure_sw_mode,
        qConfig.num_fpga_device,
        0,
        qConfig.scale_factor,
        0,
        0,
        1)
      codegen.genFPGAConfigCode(qParser.qPlan, 0, qConfig.scale_factor)
      codegen.genSWConfigCode(qParser.qPlan, 0, qConfig.scale_factor)
    }
    plan
  }
}
