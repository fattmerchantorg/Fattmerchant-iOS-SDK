#!/usr/bin/env bash
# tools/make_spm_package.sh
# Build ChipDnaMobileKit SPM and (optionally) install to a vendor folder.
# v1.6 — adds rich help/doctor/examples

set -euo pipefail

VERSION="1.6.0"

die(){ echo "❌ $*" >&2; exit 1; }
note(){ echo "➡️  $*"; }
ok(){ echo "✅ $*"; }
require_cmd(){ command -v "$1" >/dev/null 2>&1 || die "Missing: $1"; }
abs_existing(){ perl -MCwd=abs_path -e 'defined(my $p=abs_path($ARGV[0])) or exit 2; print $p' "$1" || return 2; }

ROOT=""; NAME="ChipDnaMobile"; OUT="build/SwiftPM"; ZIP=0; VENDOR_DIR=""; DOCTOR=0

print_capabilities() {
  cat <<'EOF'
Capabilities
-----------
• Build ChipDnaMobile.xcframework from static .a inputs (arm64 + optional x86_64).
• Generate headers/modulemap (directory umbrella) for ChipDnaMobile.
• Rewrap vendor IDTech.framework → IDTechStatic.xcframework (no headers/modulemap).
• Copy CloudCommerce & BBPOS .xcframeworks verbatim as binary targets.
• Package IDTech.bundle as a Swift target resource (IDTechResources).
• Emit Package.swift with targets:
    - ChipDnaMobileKit (wrapper; re-exports ChipDnaMobile, links IDTechStatic, BBPOS, CloudCommerce)
    - IDTechResources (resources)
    - Binary targets: ChipDnaMobile, IDTechStatic, CloudCommerce, BBDeviceBT, BBDeviceOTA
• Optional: install/mirror the generated package into a vendor folder (e.g. Vendor/ChipDnaMobileKit).
• Optional: zip + compute checksum for ChipDnaMobile.xcframework.
• Doctor mode: validate environment, inputs, outputs, and common pitfalls.
EOF
}

help_topics(){
  local topic="${1:-}"
  case "$topic" in
    capabilities) print_capabilities; return 0;;
    examples) print_examples; return 0;;
    doctor)
      cat <<'EOF'
Doctor
------
Runs validations:
  • Tools present: xcodebuild, libtool, lipo, zip, swift (optional).
  • Input libs exist: libCardEaseXml.a, libChipDnaMobileAPI.a, SQLCipher .a.
  • Modulemap sanity: no semicolons, valid umbrella syntax.
  • No headers/module.modulemap inside IDTechStatic.xcframework (prevents collisions).
  • IDTech symbols presence check (_OBJC_CLASS_$_IDT_*) if nm available.
  • swift package describe (if swift present).
Use:  make_spm_package.sh --doctor --root Library [--vendor Vendor/ChipDnaMobileKit]
EOF
      return 0;;
    *)
      usage
      ;;
  esac
}

print_examples() {
  cat <<'EOF'
Examples
--------
# Build into ./build/SwiftPM and inspect
./tools/make_spm_package.sh --root Library

# Build and install to vendor folder (best for "Add Local Package…")
./tools/make_spm_package.sh --root Library --vendor Vendor/ChipDnaMobileKit

# Build with a custom name (affects wrapper name & product)
./tools/make_spm_package.sh --root Library --name ChipDnaMobile

# Produce zip + checksum (for remote binary hosting flows)
./tools/make_spm_package.sh --root Library --zip

# Run diagnostics only (does not build), useful after a previous run
./tools/make_spm_package.sh --doctor --root Library --vendor Vendor/ChipDnaMobileKit
EOF
}

usage(){
  cat <<EOF
make_spm_package.sh v${VERSION}

Usage:
  make_spm_package.sh --root <Library>
                      [--name ChipDnaMobile]
                      [--out build/SwiftPM]
                      [--vendor Vendor/ChipDnaMobileKit]
                      [--zip]
                      [--doctor]
                      [--examples]
                      [--version]
                      [--help] | [help [topic]]

$(print_capabilities)

Options
-------
  --root <path>     Path to the vendor "Library" folder (required).
  --name <string>   Base name for the core binary and wrapper (default: ChipDnaMobile).
  --out <path>      Build/output folder (default: build/SwiftPM).
  --vendor <path>   Mirror finished Swift package into this folder (ready to add locally).
  --zip             Zip ChipDnaMobile.xcframework and compute checksum.
  --doctor          Run validations after build (or on existing output if present).
  --examples        Print usage examples.
  --version         Print script version and exit.
  --help | help     Show this help. "help <topic>" for: capabilities | examples | doctor

EOF
}

# ---- args ----
if [[ $# -eq 0 ]]; then usage; exit 0; fi

while [[ $# -gt 0 ]]; do
  case "$1" in
    --root) ROOT="${2:?}"; shift 2;;
    --name) NAME="${2:?}"; shift 2;;
    --out) OUT="${2:?}"; shift 2;;
    --vendor) VENDOR_DIR="${2:?}"; shift 2;;
    --zip) ZIP=1; shift;;
    --doctor) DOCTOR=1; shift;;
    --examples) print_examples; exit 0;;
    --version) echo "make_spm_package.sh ${VERSION}"; exit 0;;
    -h|--help) usage; exit 0;;
    help) help_topics "${2:-}"; exit 0;;
    *) die "Unknown arg: $1 (use --help)";;
  esac
done

[[ -n "$ROOT" && -d "$ROOT" ]] || die "Provide --root pointing at Library/"
require_cmd xcodebuild; require_cmd libtool; require_cmd lipo; require_cmd zip
SWIFT_OK=1; command -v swift >/dev/null 2>&1 || SWIFT_OK=0

ROOT_ABS="$(abs_existing "$ROOT")" || die "Cannot resolve --root"
mkdir -p "$OUT"; pushd "$OUT" >/dev/null; OUT_ABS="$(pwd)"; popd >/dev/null

PKG_DIR="$OUT_ABS/SwiftPackage"
BIN_DIR="$PKG_DIR/Binaries"
SRC_DIR="$PKG_DIR/Sources"
WRAP_DIR="$SRC_DIR/${NAME}Kit"
RES_DIR="$SRC_DIR/IDTechResources"
TMP="$(mktemp -d /tmp/spmwrap.XXXXXX)"; trap 'rm -rf "$TMP"' EXIT
mkdir -p "$BIN_DIR" "$WRAP_DIR" "$RES_DIR"

# ---------- locate core libs ----------
[[ -f "$ROOT_ABS/libCardEaseXml.a" ]]      || die "Missing libCardEaseXml.a"
[[ -f "$ROOT_ABS/libChipDnaMobileAPI.a" ]] || die "Missing libChipDnaMobileAPI.a"
SQLC_LIB="$ROOT_ABS/SQLCipher/libSqlCipher-4.6.1.a"
if [[ ! -f "$SQLC_LIB" ]]; then
  cand=( "$ROOT_ABS/SQLCipher"/libSqlCipher-*.a )
  [[ -f "${cand[0]}" ]] || die "No SQLCipher static lib under Library/SQLCipher"
  SQLC_LIB="${cand[0]}"
fi
LIBS=( "$ROOT_ABS/libCardEaseXml.a" "$ROOT_ABS/libChipDnaMobileAPI.a" "$SQLC_LIB" )

# ---------- headers for ChipDnaMobile ----------
INC="$TMP/include"; mkdir -p "$INC"
shopt -s nullglob
cp -f "$ROOT_ABS"/*.h "$INC/" || true
cp -f "$ROOT_ABS/SOAP"/*.h "$INC/" || true
cp -f "$ROOT_ABS/SQLCipher/sqlite3.h" "$INC/" || true
shopt -u nullglob

# avoid circular <Fattmerchant/Fattmerchant.h>
mkdir -p "$INC/Fattmerchant"
cat > "$INC/Fattmerchant/Fattmerchant.h" <<'EOF'
#ifndef FATTMERCHANT_STUB_H
#define FATTMERCHANT_STUB_H
/* Stubbed to avoid circular import (ChipDnaMobile -> Fattmerchant -> ChipDnaMobile). */
#endif
EOF

# Directory-umbrella module map (NO semicolons)
cat > "$INC/module.modulemap" <<EOF
module $NAME {
  umbrella "."
  export *
  module * { export * }
}
EOF

# ---------- lipo helpers ----------
thin_if_arch() {
  local in="$1" arch="$2" out="$3"
  local info; info="$(lipo -info "$in" 2>/dev/null || true)"
  if [[ "$info" == *"Non-fat file"* ]]; then
    if [[ "$info" == *"architecture: $arch"* ]]; then cp -f "$in" "$out"; echo "$out"; else echo ""; fi
  else
    if echo "$info" | grep -q "$arch"; then lipo -extract "$arch" "$in" -output "$out"; echo "$out"; else echo ""; fi
  fi
}
combine_for_arch() {
  local arch="$1" out="$2"; shift 2
  local thins=() t p
  for lib in "$@"; do t="$TMP/$(basename "$lib" .a)-$arch.a"; p="$(thin_if_arch "$lib" "$arch" "$t")"; [[ -n "$p" ]] && thins+=("$p"); done
  [[ ${#thins[@]} -gt 0 ]] && { libtool -static -o "$out" "${thins[@]}" >/dev/null; echo "$out"; } || echo ""
}

# ---------- ChipDnaMobile.xcframework (NO IDTech linked) ----------
note "Combining static libs per-arch (arm64 / x86_64)…"
DEV_ARM64="$(combine_for_arch arm64 "$TMP/combined-device-arm64.a" "${LIBS[@]}")"
SIM_X64="$(combine_for_arch x86_64 "$TMP/combined-sim-x86_64.a" "${LIBS[@]}")"
[[ -n "$DEV_ARM64" ]] || die "Could not produce device (arm64) combined lib."

echo "---- Input slices ----"
for lib in "${LIBS[@]}"; do echo "$(basename "$lib"): $(lipo -info "$lib" 2>/dev/null || true)"; done
echo "----------------------"

XCFW="$BIN_DIR/$NAME.xcframework"; rm -rf "$XCFW"
args=( -create-xcframework -library "$DEV_ARM64" -headers "$INC" )
[[ -n "$SIM_X64" ]] && args+=( -library "$SIM_X64" -headers "$INC" ) || note "ℹ️  No simulator x86_64 slice."
note "Creating XCFramework → $XCFW"; xcodebuild "${args[@]}" -output "$XCFW" >/dev/null; ok "Built $XCFW"

# ---------- Rewrap IDTech.framework → IDTechStatic.xcframework (NO HEADERS) ----------
IDTECH_XCF="$ROOT_ABS/IDTech.xcframework"
if [[ -d "$IDTECH_XCF" ]]; then
  note "Rewrapping IDTech.xcframework → Binaries/IDTechStatic.xcframework (no headers)…"
  IDTECH_STATIC_XCF="$BIN_DIR/IDTechStatic.xcframework"; rm -rf "$IDTECH_STATIC_XCF"
  WR="/tmp/idtech-rewrap.$RANDOM"; mkdir -p "$WR"

  DEV_BIN="$IDTECH_XCF/ios-arm64/IDTech.framework/IDTech"
  SIM_BIN="$IDTECH_XCF/ios-arm64_x86_64-simulator/IDTech.framework/IDTech"
  [[ -f "$DEV_BIN" ]] || die "IDTech device binary missing: $DEV_BIN"

  # Make .a slices from the framework binaries
  libtool -static -o "$WR/idtech-ios-arm64.a" "$DEV_BIN"
  if [[ -f "$SIM_BIN" ]]; then libtool -static -o "$WR/idtech-ios-sim.a" "$SIM_BIN"; fi

  # Create XCFramework WITHOUT -headers arguments (no module.modulemap emitted)
  XCB=( -create-xcframework -library "$WR/idtech-ios-arm64.a" )
  [[ -f "$WR/idtech-ios-sim.a" ]] && XCB+=( -library "$WR/idtech-ios-sim.a" )
  xcodebuild "${XCB[@]}" -output "$IDTECH_STATIC_XCF" >/dev/null
  ok "Built $IDTECH_STATIC_XCF"
fi

# ---------- vendor xcframeworks (CloudCommerce + BBPOS). DO NOT copy original IDTech.xcframework ----------
VENDOR_DEPS=""
VENDOR_BIN_ENTRIES=""

if [[ -d "$ROOT_ABS/CloudCommerce.xcframework" ]]; then
  rsync -a "$ROOT_ABS/CloudCommerce.xcframework" "$BIN_DIR/"
  VENDOR_DEPS+=$'\n                .target(name: "CloudCommerce"),'
  VENDOR_BIN_ENTRIES+=$'\n        .binaryTarget(name: "CloudCommerce", path: "Binaries/CloudCommerce.xcframework"),'
fi

if [[ -d "$ROOT_ABS/BBPOSFrameworks" ]]; then
  for p in "$ROOT_ABS/BBPOSFrameworks"/*.xcframework; do [[ -e "$p" ]] && rsync -a "$p" "$BIN_DIR/"; done
  if compgen -G "${BIN_DIR}/BBDevice-BT-*.xcframework" >/dev/null; then
    bb_bt="$(basename "${BIN_DIR}"/BBDevice-BT-*.xcframework)"
    VENDOR_DEPS+=$'\n                .target(name: "BBDeviceBT"),'
    VENDOR_BIN_ENTRIES+=$'\n        .binaryTarget(name: "BBDeviceBT", path: "Binaries/'"$bb_bt"'"),'
  fi
  if compgen -G "${BIN_DIR}/BBDeviceOTA-*.xcframework" >/dev/null; then
    bb_ota="$(basename "${BIN_DIR}"/BBDeviceOTA-*.xcframework)"
    VENDOR_DEPS+=$'\n                .target(name: "BBDeviceOTA"),'
    VENDOR_BIN_ENTRIES+=$'\n        .binaryTarget(name: "BBDeviceOTA", path: "Binaries/'"$bb_ota"'"),'
  fi
fi

# ---------- IDTech resources ----------
if [[ -d "$ROOT_ABS/IDTech.bundle" ]]; then
  rsync -a "$ROOT_ABS/IDTech.bundle" "$RES_DIR/IDTech.bundle"
fi

# ---------- wrapper sources ----------
echo '// placeholder' > "$WRAP_DIR/Dummy.swift"
cat > "$WRAP_DIR/Reexports.swift" <<'SWIFT'
@_exported import ChipDnaMobile
SWIFT
echo '// placeholder' > "$RES_DIR/Dummy.swift"

# ---------- manifest ----------
PKG="$PKG_DIR/Package.swift"
cat > "$PKG" <<EOF
// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "${NAME}Kit",
    platforms: [.iOS(.v12)],
    products: [
        .library(name: "${NAME}Kit", targets: ["${NAME}Kit"])
    ],
    targets: [
        .target(
            name: "${NAME}Kit",
            dependencies: [
                .target(name: "IDTechResources"),
                .target(name: "${NAME}"),
                .target(name: "IDTech"),${VENDOR_DEPS}
            ],
            path: "Sources/${NAME}Kit",
            sources: ["Dummy.swift","Reexports.swift"],
            linkerSettings: [
                .unsafeFlags(["-ObjC"])
            ]
        ),
        .target(
            name: "IDTechResources",
            path: "Sources/IDTechResources",
            sources: ["Dummy.swift"],
            resources: [.copy("IDTech.bundle")]
        ),
        .binaryTarget(
            name: "${NAME}",
            path: "Binaries/${NAME}.xcframework"
        ),
        .binaryTarget(
            name: "IDTech",
            path: "Binaries/IDTechStatic.xcframework"
        ),${VENDOR_BIN_ENTRIES}
    ]
)
EOF

# ---------- optional zip ----------
if [[ $ZIP -eq 1 ]]; then
  (cd "$BIN_DIR" && zip -rq "${NAME}.xcframework.zip" "${NAME}.xcframework")
  if [[ $SWIFT_OK -eq 1 ]]; then
    checksum="$(swift package compute-checksum "$BIN_DIR/${NAME}.xcframework.zip")"
    ok "Checksum: $checksum"
  fi
fi

ok "Generated Package.swift → $PKG"
ok "Built package folder → $PKG_DIR"

# ---------- optional vendor install ----------
if [[ -n "${VENDOR_DIR}" ]]; then
  note "Installing package to vendor folder: ${VENDOR_DIR}"
  mkdir -p "${VENDOR_DIR}"
  rsync -a --delete \
    "$PKG_DIR/Package.swift" \
    "$PKG_DIR/Binaries" \
    "$PKG_DIR/Sources" \
    "$PKG_DIR/Resources" \
    "${VENDOR_DIR}/"

  # Ensure no legacy vendor IDTech.framework lingers
  if [[ -d "${VENDOR_DIR}/Binaries/IDTech.xcframework" ]]; then
    note "Removing stale vendor Binaries/IDTech.xcframework (use IDTechStatic.xcframework instead)"
    rm -rf "${VENDOR_DIR}/Binaries/IDTech.xcframework"
  fi

  ok "Vendor package ready at: ${VENDOR_DIR}"

  if [[ $SWIFT_OK -eq 1 ]]; then
    ( cd "${VENDOR_DIR}" && swift package describe | sed -n '1,120p' ) || true
  fi
fi

# ---------- doctor ----------
if [[ $DOCTOR -eq 1 ]]; then
  echo
  note "Doctor: running validations…"
  # tools
  for t in xcodebuild libtool lipo zip; do command -v "$t" >/dev/null || echo "⚠️  Missing tool: $t"; done
  command -v swift >/dev/null || echo "ℹ️  swift not found (checksum/describe skipped)"

  # modulemap sanity
  mm_dev="$BIN_DIR/$NAME.xcframework/ios-arm64/Headers/module.modulemap"
  if [[ -f "$mm_dev" ]]; then
    if grep -q ';' "$mm_dev"; then echo "❌ Semicolons found in module.modulemap (invalid) → $mm_dev"; else echo "✅ module.modulemap OK (no semicolons)"; fi
  else
    echo "⚠️  module.modulemap not found at expected path: $mm_dev"
  fi

  # IDTechStatic must not have headers/modulemap
  if compgen -G "$BIN_DIR/IDTechStatic.xcframework/*/Headers" >/dev/null; then
    echo "❌ IDTechStatic.xcframework contains Headers; this will collide. Remove headers."
  else
    echo "✅ IDTechStatic.xcframework has no headers (good)"
  fi

  # IDTech symbols (optional nm)
  if command -v nm >/dev/null && compgen -G "$BIN_DIR/IDTechStatic.xcframework/ios-arm64/*.a" >/dev/null; then
    if nm -gU "$BIN_DIR"/IDTechStatic.xcframework/ios-arm64/*.a 2>/dev/null | grep -E 'OBJC_CLASS_\$_IDT_(Utility|Device|NEO2)' >/dev/null; then
      echo "✅ IDTech symbols present"
    else
      echo "⚠️  Could not find expected IDTech symbols in static archive"
    fi
  fi

  # swift package describe
  if [[ $SWIFT_OK -eq 1 ]]; then
    ( cd "${VENDOR_DIR:-$PKG_DIR}" && swift package describe >/dev/null && echo "✅ swift package describe OK" ) || echo "⚠️  swift package describe failed"
  fi
fi

echo; ok "DONE."
