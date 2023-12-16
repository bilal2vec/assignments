#!/usr/bin/env bash

set -o pipefail
set -o errexit
set -o nounset
set -o xtrace

if [[ "$#" != 1 ]] ; then
  echo "Usage: $0 FirstName-LastName-StudentNo"
  exit 1
fi

readonly NAME=$1
readonly TAR_FILE_NAME="${NAME}.tar.gz"

readonly TEMP=$(mktemp -d)
trap 'rm -rf ${TEMP}' EXIT
readonly TEMP_TAR="${TEMP}/${TAR_FILE_NAME}"

rm -f "${NAME}.tar.gz"
find * -mindepth 1 -maxdepth 1 ! -name "*.tar.gz" -exec tar zcf "${TEMP_TAR}" \{\} \+ || exit 1
cp --interactive "${TEMP_TAR}" "${TAR_FILE_NAME}"
cp --interactive report/a2_sub.pdf "${NAME}.pdf"