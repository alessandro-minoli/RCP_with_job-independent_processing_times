#!/bin/bash

mkdir -p dataset_out

for infile in dataset/*.txt; do
    filename=$(basename "$infile")
    outfile="dataset_out/$filename"
    echo "... processing '$infile'"
    ./bin/RCP_with_job-independent_processing_times "$infile" > "$outfile"
done