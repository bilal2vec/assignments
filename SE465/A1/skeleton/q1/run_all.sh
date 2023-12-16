#!/usr/bin/env bash

set -o pipefail
set -o nounset
set -o errexit

if [[ "$#" != 1 ]] ; then
  echo "Usage: $0 <a number>" 1>&2
  exit 1
fi

readonly INPUT=$1

readonly CC_FILE="mod.cc"
readonly CC_EXE="mod.out"

readonly PYTHON_FILE="mod.py"

readonly PERL_FILE="mod.pl"

readonly JAVA_FILE="Mod.java"
readonly JAVA_CLASS="Mod.class"
readonly JAVA_JAR="mod.jar"

# Compile CC and Java
rm "${CC_EXE}" &> /dev/null || true
g++ "${CC_FILE}" -o "${CC_EXE}"

rm "${JAVA_CLASS}" &> /dev/null || true
javac "${JAVA_FILE}"
jar cvf "${JAVA_JAR}" *.class &> /dev/null


# Run each one.
readonly CC_OUTPUT=$("./${CC_EXE}" "${INPUT}")
readonly JAVA_OUTPUT=$(java -cp "${JAVA_JAR}" "Mod" "${INPUT}")

chmod +x "${PYTHON_FILE}"
readonly PYTHON_OUTPUT=$("./${PYTHON_FILE}" "${INPUT}")

chmod +x "${PERL_FILE}"
readonly PERL_OUTPUT=$("./${PERL_FILE}" "${INPUT}")

echo "CC=${CC_OUTPUT}"
echo "JAVA=${JAVA_OUTPUT}"
echo "PYTHON=${PYTHON_OUTPUT}"
echo "PERL=${PERL_OUTPUT}"
