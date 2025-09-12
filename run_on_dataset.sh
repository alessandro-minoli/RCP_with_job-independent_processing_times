#!/bin/bash

mkdir -p dataset_10_out

for infile in dataset_10/*.txt; do
    filename=$(basename "$infile")
    outfile="dataset_10_out/$filename"
    echo "... processing '$infile'"
    ./bin/RCP_with_fixed_job_sequence "$infile" > "$outfile"
done