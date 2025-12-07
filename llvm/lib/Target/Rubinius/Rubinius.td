include "llvm/Target/Target.td"

// --- Subtarget / Instruction Set -----------------------------------------

def RubiniusInstrInfo : InstrInfo;

class RubiniusProc<string Name>
  : Processor<Name, NoItineraries, []>;
def : RubiniusProc<"generic">;

// --- Target ---------------------------------------------------------------

def Rubinius : Target {
  let InstructionSet = RubiniusInstrInfo;
}

// --- Registers ------------------------------------------------------------

class RReg<string N, int Enc> : Register<N> {
  let HWEncoding = Enc;
  let Namespace = "Rubinius";
}

def R0 : RReg<"r0", 0>;
def R1 : RReg<"r1", 1>;
def R2 : RReg<"r2", 2>;
def R3 : RReg<"r3", 3>;

def GPR : RegisterClass<"Rubinius", [i64], 32,
                        (add R0, R1, R2, R3)>;

// --- Instructions ---------------------------------------------------------

class RInst<string Asm> : Instruction {
  let Namespace      = "Rubinius";
  let OutOperandList = (outs);
  let InOperandList  = (ins);
  let AsmString      = Asm;
  field bits<32> Inst;
  let Size = 4;
}

def NOP : RInst<"nop"> { let Inst = 0; }
