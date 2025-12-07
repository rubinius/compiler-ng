#ifndef MLIR_DIALECT_RUBINIUS_CORE_IR_RUBINIUSCOREBASE_TD
#define MLIR_DIALECT_RUBINIUS_CORE_IR_RUBINIUSCOREBASE_TD

include "mlir/IR/OpBase.td"

def RubiniusCore_Dialect : Dialect {
  let name = "rubinius_core";
  let cppNamespace = "::mlir::rubinius::core";
}

class RubiniusCore_Op<string mnemonic, list<Trait> traits = []> :
    Op<RubiniusCore_Dialect, mnemonic, traits>;

#endif // MLIR_DIALECT_RUBINIUS_CORE_IR_RUBINIUSCOREBASE_TD
