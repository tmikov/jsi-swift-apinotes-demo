# JSI + Swift via API Notes

A minimal demonstration that Swift can import JSI's `IRuntime` (and other
`I*` interfaces) **without modifying the JSI headers** — using Swift's
[API Notes](https://clang.llvm.org/docs/APINotes.html) mechanism.

## Why this exists

JSI's `I*` interface classes (`ICast`, `IRuntime`, `ISerialization`, ...)
intentionally have **`protected:` non-virtual destructors**. The contract is:
*interfaces are not destructible by their consumers; only the concrete object
that produced them owns its lifetime.* The destructor visibility is what
enforces that invariant at compile time.

Swift's C++ interop, by default, refuses to import a C++ class with an
inaccessible destructor:

```
note: record 'IRuntime' is not automatically available: it must have a
      copy/move constructor and a destructor; does this type have
      reference semantics?
```

The compiler hint is the right answer: JSI interfaces *are* reference types.
Swift's solution for that is the `import_reference` annotation — and Swift
ships **API Notes** so that consumers can apply such annotations to a C++
module *without touching the C++ source*.

## What's in this repo

| File | Role |
|---|---|
| `jsi.h` | Pristine mirror of today's JSI interface design — protected, non-virtual destructors. No Swift attributes. No `#ifdef`. No macros. |
| `jsi.cpp` | A free function `runScript(IRuntime*, ...)` representing real JSI consumer code. |
| `module.modulemap` | Standard Clang module map exposing `jsi.h` as module `JSI`. |
| `JSI.apinotes` | **The entire Swift fix.** A YAML file marking the three `I*` interfaces as `SwiftImportAs: reference` with immortal retain/release. Auto-discovered by `swiftc` when its name matches the module. |
| `main.swift` | Swift code that takes `IRuntime` by reference, calls inherited `castInterface()`, and calls `runScript(rt, ...)` idiomatically (no `&`, no `inout`). |
| `build.sh` | One `swiftc` invocation. |

## Build

```
./build.sh
```

Requires Swift with C++ interop (Swift 5.9+). Tested on Apple Swift 6.3
(Xcode 26, macOS 26).

Expected output:
```
compiled OK
```

## Why this matters for facebook/react-native#56770

That PR moves `~IRuntime` from `protected` to `public` to satisfy Swift's
importer. The change:

1. **Breaks the JSI contract.** Consumers can now `delete` an interface they
   don't own; Swift will run the destructor when an `IRuntime` value goes
   out of scope.
2. **Forces every Swift caller into value-type ergonomics** (`inout` + `&`)
   instead of treating interfaces as the references they are.
3. **Is unnecessary.** This repo proves the same goal can be achieved with
   a single ~15-line YAML file shipped by the *consumer* — no React Native
   change required at all. ExpoModulesJSI can drop its own `JSI.apinotes`
   alongside its build inputs and the problem is solved without affecting
   any other JSI consumer.
