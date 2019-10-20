checks='*,

-android-*
-openmp-*

-clang-analyzer-optin.osx.cocoa.localizability.NonLocalizedStringChecker,
-clang-analyzer-nullability.NullabilityBase,
-clang-analyzer-osx.cocoa.RetainCountBase,

-llvm-include-order,
-llvm-namespace-comment,
-llvm-header-guard,

-hicpp-avoid-c-arrays,
-hicpp-signed-bitwise,
-hicpp-no-malloc,
-hicpp-uppercase-literal-suffix,
-hicpp-member-init,
-hicpp-explicit-conversions,
-hicpp-use-auto,
-hicpp-no-array-decay,

-google-readability-namespace-comments,
-google-runtime-references,
-google-explicit-constructor,
-google-objc-global-variable-declaration,
-google-objc-function-naming,
-google-build-using-namespace,

-cppcoreguidelines-avoid-c-arrays,
-cppcoreguidelines-non-private-member-variables-in-classes,
-cppcoreguidelines-avoid-magic-numbers,
-cppcoreguidelines-no-malloc,
-cppcoreguidelines-owning-memory,
-cppcoreguidelines-macro-usage,
-cppcoreguidelines-pro-type-reinterpret-cast,
-cppcoreguidelines-pro-bounds-pointer-arithmetic,
-cppcoreguidelines-pro-bounds-constant-array-index,
-cppcoreguidelines-pro-type-member-init,
-cppcoreguidelines-pro-type-static-cast-downcast,
-cppcoreguidelines-pro-bounds-array-to-pointer-decay,

-modernize-concat-nested-namespaces,
-modernize-use-trailing-return-type,
-modernize-avoid-c-arrays,
-modernize-use-nodiscard,
-modernize-use-auto,
-modernize-loop-convert,

-portability-simd-intrinsics,

-fuchsia-overloaded-operator,
-fuchsia-statically-constructed-objects,
-fuchsia-default-arguments-calls,

-readability-magic-numbers,
-readability-uppercase-literal-suffix,
-readability-else-after-return,
-readability-implicit-bool-conversion,

-misc-non-private-member-variables-in-classes,

-cert-oop54-cpp
'

checks_no_whitespace="$(echo "${checks}" | tr -d '[:space:]')"

clang-tidy -dump-config -checks=$checks_no_whitespace -header-filter='[a-zA-Z0-9\s/]*src/.*' > .clang-tidy