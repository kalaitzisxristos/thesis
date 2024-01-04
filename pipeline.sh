#!/usr/bin/env bash

_CWD="$PWD"

LOGS_DIRECTORY='log-grouper/result'
REF_FILE="$LOGS_DIRECTORY/.gitignore"

JSONS_DIRECTORY='can-decoder-python-cantools/decoded_output_jsons'
UPLOAD_DIRECTORY='can-upload-via-hasura-graphql/decoded_output_jsons'

trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT

process_file () {
    file="$1"

    echo "Processing file '$file' ..."

    (
        echo "Converting file '$file' ..."
        mkdir -p "$JSONS_DIRECTORY"
        cd "$JSONS_DIRECTORY/.."
        rm -rf "$JSONS_DIRECTORY/*"
        python3 can_decoder-chris.py "../$file"
        echo "Finished converting file '$file'"
    )

    (
        echo "Uploading file '$file' ..."
        rm -rf "$UPLOAD_DIRECTORY"
        mv "$JSONS_DIRECTORY" "$UPLOAD_DIRECTORY"
        cd "$UPLOAD_DIRECTORY/.."
        python3 can_uploader.py
        echo "Finished uploading file '$file'"
    )

    echo "Finished processing file '$file'"
}

cd log-grouper
bash run-with-can.sh &
cd $_CWD

sleep 1
while :; do
    for f in $(find "$LOGS_DIRECTORY" -type f -newer "$REF_FILE"); do
        process_file $f
        REF_FILE="$f"
    done
    sleep 1
done
