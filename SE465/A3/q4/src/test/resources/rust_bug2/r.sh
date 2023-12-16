#!/usr/bin/env bash
set -o nounset
set -o pipefail
set -o xtrace

readonly OUTPUT="temp_compilation_output.tmp.txt"
readonly CRASH_EXIT_CODE=101

readonly RUSTC_AT_ECEUBUNTU="/opt-src/rust-1.58.1/bin/rustc"
if [[ -f "${RUSTC_AT_ECEUBUNTU}" ]] ; then
  readonly RUSTC="${RUSTC_AT_ECEUBUNTU}"
else
  readonly RUSTC="rustc"
fi

readonly RUSTC_VERSION=$(${RUSTC} --version)
if [[ "${RUSTC_VERSION}" != *"1.58.1"* ]] ; then
  echo "the version of rustc is not 1.58.1"
  exit 1
fi
timeout -s 9 30 "${RUSTC}" t.rs &> "${OUTPUT}"

[[ "$?" == "${CRASH_EXIT_CODE}" ]] || exit 1

if ! grep --quiet --fixed-strings "error: internal compiler error: compiler/rustc_typeck/src/variance/mod.rs" "${OUTPUT}" ; then
  cat "${OUTPUT}"
  exit 1
fi

if ! grep --quiet --fixed-strings "thread 'rustc' panicked at 'Box<dyn Any>'" "${OUTPUT}" ; then
  cat "${OUTPUT}"
  exit 1
fi

if ! grep --quiet --fixed-strings "note: the compiler unexpectedly panicked. this is a bug." "${OUTPUT}" ; then
  cat "${OUTPUT}"
  exit 1
fi
exit 0
