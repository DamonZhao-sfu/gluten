package org.apache.gluten.extension

import org.apache.arrow.vector.VectorSchemaRoot
import org.apache.spark.TaskContext
import org.apache.spark.rdd.RDD
import org.apache.spark.sql.SparkArrowUtils
import org.apache.spark.sql.catalyst.InternalRow
import org.apache.spark.sql.catalyst.expressions.{Attribute, AttributeReference}
import org.apache.spark.sql.execution.{SparkPlan, UnaryExecNode}
import org.apache.spark.sql.vectorized.ColumnarBatch

case class FPGAJoinExec(left: SparkPlan,
  right: SparkPlan,
  joinType: JoinType,
  condition: Option[Expression],
  outputs: Seq[Attribute])
  extends BinaryExecNode {

  override def supportsColumnar: Boolean = true

  override protected def doExecute(): RDD[InternalRow] = {
    throw new IllegalAccessException(s"${getClass.getSimpleName} does only support columnar data processing.")
  }

  override protected def doExecuteColumnar(): RDD[ColumnarBatch] = {
    // Execute left and right child plans to get columnar batches
    val leftBatches = left.executeColumnar()
    val rightBatches = right.executeColumnar()

    // Iterator[ColumnarBatch]
    val joinedBatches = leftBatches.zipPartitions(rightBatches) { 
        (leftIter, rightIter) =>
            val leftIterVector = leftIter.toVector
            val rightIterVector = rightIter.toVector

            // Call Kernel function to process batches
            val resultRecordBatchIterator = Kernel(leftIterVector, rightIterVector)
            resultRecordBatchIterator.map(arrowBatch => {
                // Convert Arrow batch back to ColumnarBatch
                val columnarBatch = arrowBatch.toColumnarBatch
                // Optionally, close Arrow batch
                arrowBatch.close()
                columnarBatch
            })
    }

    joinedBatches
  }

  override def output: Seq[Attribute] = outputs
}
