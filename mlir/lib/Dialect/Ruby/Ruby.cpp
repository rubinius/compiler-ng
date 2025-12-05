#include "mlir/Dialect/Ruby/Ruby.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/DialectImplementation.h"
using namespace mlir;
using namespace mlir::ticg;

RubyDialect::RubyDialect(MLIRContext *ctx)
    : Dialect(getDialectNamespace(), ctx, TypeID::get<RubyDialect>()) {
  addOperations<
#define GET_OP_LIST
#include "mlir/Dialect/Ruby/Ruby.cpp.inc"
      >();
}

#include "mlir/Dialect/Ruby/Ruby.cpp.inc"
