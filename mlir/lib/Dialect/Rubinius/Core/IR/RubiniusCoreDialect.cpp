#include "mlir/Dialect/Rubinius/Core/IR/RubiniusCore.h"

#include "mlir/IR/Builders.h"
#include "mlir/IR/DialectImplementation.h"

using namespace mlir;
using namespace mlir::rubinius::core;

#include "mlir/Dialect/Rubinius/Core/IR/RubiniusCoreOpsDialect.cpp.inc"
#include "mlir/Dialect/Rubinius/Core/IR/RubiniusCoreOps.cpp.inc"
