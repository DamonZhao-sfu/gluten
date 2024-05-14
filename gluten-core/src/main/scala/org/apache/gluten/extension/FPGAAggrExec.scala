package org.apache.gluten.extension

import org.apache.arrow.vector.VectorSchemaRoot
import org.apache.spark.TaskContext
import org.apache.spark.rdd.RDD
import org.apache.spark.sql.SparkArrowUtils
import org.apache.spark.sql.catalyst.InternalRow
import org.apache.spark.sql.catalyst.expressions.{Attribute, AttributeReference}
import org.apache.spark.sql.execution.{SparkPlan, UnaryExecNode}
import org.apache.spark.sql.vectorized.ColumnarBatch

case class FPGAAggrExec(child: SparkPlan, outputs: Seq[Attribute])
  extends UnaryExecNode {

  override def supportsColumnar: Boolean = true

  override protected def doExecute(): RDD[InternalRow] = {
    throw new IllegalAccessException(s"${getClass.getSimpleName} does only support columnar data processing.")
  }

  override protected def doExecuteColumnar(): RDD[ColumnarBatch] = {

    child.executeColumnar().mapPartitions(

        batchIter => {
            // Call Kernel function to process batches
            // need to pass the Iterator[ColumnarBatch] to the Kernel
            val wrapper = new SQL2FPGAJNIWrapper();
            val kernel = wrapper.createKernel("gqeAggr", "", batchIter)
            val resultRecordBatchIterator = kernel.getResultIterator();
            resultRecordBatchIterator.map(arrowBatch => {
                // Convert Arrow batch back to ColumnarBatch
                val columnarBatch = arrowBatch.toColumnarBatch
                // Optionally, close Arrow batch
                arrowBatch.close()
                columnarBatch
            })
        }
    )
  }

  override def output: Seq[Attribute] = child.output
}
