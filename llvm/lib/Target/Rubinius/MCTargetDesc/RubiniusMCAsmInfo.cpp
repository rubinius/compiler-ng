#include "RubiniusMCAsmInfo.h"
using namespace llvm;

RubiniusMCAsmInfo::RubiniusMCAsmInfo() {
  CommentString = "#";
  MinInstAlignment = 4;
}
