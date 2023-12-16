#!/usr/bin/env bash

readonly FILE="t.html"

grep --quiet "<SELECT" "${FILE}" && grep --quiet ">" "${FILE}"