#include "RubiniusLegalizerInfo.h"

#define DEBUG_TYPE "rbx-legalinfo"

using namespace llvm;
using namespace LegalizeActions;

RubiniusLegalizerInfo::RubiniusLegalizerInfo(const RubiniusSubtarget &ST) {
  getLegacyLegalizerInfo().computeTables();
}
