//===- ReluLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ReluLower.h"
#include "../Compute/Relu.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReluLower
//===----------------------------------------------------------------------===//
BM188X::ReluLower::ReluLower()
{
}

BM188X::ReluLower::~ReluLower()
{
}

int BM188X::ReluLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("Relu"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::ReluLower::activate(ComputeGraph& pGraph, xNode &pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size())
    return nullptr;

  for (xValue* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  if (1 != pNode.outputs().size())
    return nullptr;

  for (xValue* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // create operators
  BM188X::Relu* op = pGraph.addOperator<BM188X::Relu>();

  // set input/output
  for (xValue* xv : pNode.inputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addInput(*tensor);
  }

  for (xValue* xv : pNode.outputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addOutput(*tensor);
  }
  return op;
}

