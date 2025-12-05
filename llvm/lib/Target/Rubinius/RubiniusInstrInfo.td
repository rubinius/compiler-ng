include "RubiniusRegisterInfo.td"

def RubiniusInstrInfo : InstrInfo;

let hasSideEffects = 0 in {
  def ADDrr : Instruction {
    let OutOperandList = (outs GPR:$dst);
    let InOperandList  = (ins GPR:$lhs, GPR:$rhs);
    let AsmString      = "add $dst, $lhs, $rhs";
  }
}

def RET : PseudoInst<(outs), (ins GPR:$src),
                     "ret $src", []>;
