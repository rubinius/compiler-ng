#pragma once

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace Rubinius {
enum FixupKind {
  // Rubinius specific relocations.
  FK_Rubinius_PCRel_4 = FirstTargetFixupKind,

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // end namespace Rubinius
} // end namespace llvm
