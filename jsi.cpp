#include "jsi.h"

int runScript(IRuntime* rt, const char* source) {
  return rt->evaluate(source);
}
