#pragma once

#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"

namespace llvm {

class RubiniusSubtarget;

/// This class provides the information for the Rubinius target legalizer for
/// GlobalISel.
class RubiniusLegalizerInfo : public LegalizerInfo {
public:
  RubiniusLegalizerInfo(const RubiniusSubtarget &ST);
};
} // namespace llvm
