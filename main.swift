// Pristine JSI header + apinotes overlay. IRuntime imports as a reference
// type; callers pass it like any other Swift class instance.
import JSI

let kRuntimeUUID = UUID(hi: 1, lo: 2)

func tryRun(_ rt: IRuntime) -> Int32 {
  return runScript(rt, "1+1")  // no &, no inout
}

func tryCast(_ rt: IRuntime) -> Bool {
  return rt.castInterface(kRuntimeUUID) != nil  // inherited method visible
}

print("compiled OK")
