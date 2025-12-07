#ifndef MLIR_DIALECT_RUBINIUS_RUBY_IR_RUBINIUSRUBYBASE_TD
#define MLIR_DIALECT_RUBINIUS_RUBY_IR_RUBINIUSRUBYBASE_TD

include "mlir/IR/OpBase.td"

def RubiniusRuby_Dialect : Dialect {
  let name = "rubinius_ruby";
  let cppNamespace = "::mlir::rubinius::ruby";
}

class RubiniusRuby_Op<string mnemonic, list<Trait> traits = []> :
    Op<RubiniusRuby_Dialect, mnemonic, traits>;

#endif // MLIR_DIALECT_RUBINIUS_RUBY_IR_RUBINIUSRUBYBASE_TD
