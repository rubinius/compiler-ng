#pragma once
#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/OpImplementation.h"
#include "mlir/Interfaces/CastInterfaces.h"
#include "mlir/Interfaces/ControlFlowInterfaces.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"

#include "mlir/Dialect/Rubinius/Python/IR/RubiniusPythonOpsDialect.h.inc"

#define GET_OP_CLASSES
#include "mlir/Dialect/Rubinius/Python/IR/RubiniusPythonOps.h.inc"
