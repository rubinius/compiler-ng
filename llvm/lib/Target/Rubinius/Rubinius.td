// Describe the Rubinius Target Machine

include "llvm/Target/Target.td"

include "RubiniusRegisterInfo.td"
include "RubiniusCallingConv.td"
include "RubiniusInstrInfo.td"
include "GISel/RubiniusRegisterBanks.td"


defm : RemapAllTargetPseudoPointerOperands<GPR>;

def RubiniusInstrInfo : InstrInfo;

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def DummyFeature : SubtargetFeature<"dummy", "isDummyMode",
                                    "true", "unused feature">;

def ALU32 : SubtargetFeature<"alu32", "HasAlu32", "true",
                             "Enable ALU32 instructions">;

def DwarfRIS: SubtargetFeature<"dwarfris", "UseDwarfRIS", "true",
                               "Disable MCAsmInfo DwarfUsesRelocationsAcrossSections">;

def MisalignedMemAccess : SubtargetFeature<"allows-misaligned-mem-access",
                                           "AllowsMisalignedMemAccess", "true",
                                           "Allows misaligned memory access">;

def AllowBuiltinCall : SubtargetFeature<"allow-builtin-calls",
                                        "AllowBuiltinCalls", "true",
                                        "Allow calls to builtin functions">;

def : Proc<"generic", []>;
def : Proc<"v1", []>;
def : Proc<"v2", []>;
def : Proc<"v3", [ALU32]>;
def : Proc<"v4", [ALU32]>;
def : Proc<"probe", []>;

def RubiniusInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

def RubiniusAsmParser : AsmParser {
  bit HasMnemonicFirst = 0;
}

def RubiniusAsmParserVariant : AsmParserVariant {
  int Variant = 0;
  string Name = "Rubinius";
  string BreakCharacters = ".";
  string TokenizingCharacters = "#()[]=:.<>!+*%/";
}

def Rubinius : Target {
  let InstructionSet = RubiniusInstrInfo;
  let AssemblyWriters = [RubiniusInstPrinter];
  let AssemblyParsers = [RubiniusAsmParser];
  let AssemblyParserVariants = [RubiniusAsmParserVariant];
}
