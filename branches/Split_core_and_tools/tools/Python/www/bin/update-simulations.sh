#!/bin/bash

## This script checks the sim/src for new or modified instrument source files
## and update sim/bin accordingly.


# Scan src/sim for new instrument files and generate binaries
./sim/compile.sh

echo ""

# Generate documentation
python generate_docs.py

echo ""

# Add new instruments to the database
python populate_db.py
