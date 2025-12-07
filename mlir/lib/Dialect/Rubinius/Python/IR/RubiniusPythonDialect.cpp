#include "mlir/Dialect/Rubinius/Python/IR/RubiniusPython.h"

#include "mlir/IR/Builders.h"
#include "mlir/IR/DialectImplementation.h"

using namespace mlir;
using namespace mlir::rubinius::python;

#include "mlir/Dialect/Rubinius/Python/IR/RubiniusPythonOpsDialect.cpp.inc"
#include "mlir/Dialect/Rubinius/Python/IR/RubiniusPythonOps.cpp.inc"
