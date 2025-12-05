#pragma once
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"

#define GET_OP_CLASSES
#include "mlir/Dialect/Ruby/Ruby.h.inc"

namespace mlir {
namespace ruby {

class RubyDialect : public Dialect {
public:
  explicit RubyDialect(MLIRContext *ctx);
  static StringRef getDialectNamespace() { return "ruby"; }
};

} // namespace ruby
} // namespace mlir
