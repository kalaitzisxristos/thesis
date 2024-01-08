#!/usr/bin/env bash

# Read command-line arguments: Device and BAUD Rate
DEVICE="${1}"
BAUD="${2}"

# Configure serial port settings
stty -F "${DEVICE}" raw speed "${BAUD}" >/dev/null

# Configure ESP32RET
echo "CANEN0=1" > "${DEVICE}"                             # Enable CAN0
echo "CANSPEED0=1000000" > "${DEVICE}"                    # Set CAN Speed to 1,000,000
echo "CANEN1=0" > "${DEVICE}"                             # Disable CAN1

echo "BTMODE=0" > "${DEVICE}"                             # Disable Bluetooth
echo "WIFIMODE=0" > "${DEVICE}"                           # Disable Wifi

echo "LAWICEL=1" > "${DEVICE}"                            # Enable LAWICEL compatibility

# Discard current incoming line to prevent parsing errors
cat "${DEVICE}" | ( head -n1 >/dev/null ; while read line; do (
  line="$(echo "${line}" | tr -d "\r")"

  _idx=1            # Initialize index for parsing

  # Initialize variables to store parsed values
  _payloadIdx=0
  _can_id=""
  _length=""
  _payload=()

  # Parse each component of the incoming line
  for s in ${line}; do
    case $_idx in
      1)
        [ ${s} = '-' ] || exit                            # Check for '-' separator, exit if not found
        ;;
      2)
        _can_id="$(echo "${s}" | grep -E '^[0-9a-f]+$')" # Parse and validate CAN ID
        ;;
      3)
        [ ${s} = 'S' ] || [ ${s} = 'X' ] || exit          # Check for 'S' (Standard) or 'X' (Extended), exit if not found
        ;;
      4)
        [ ${s} = '0' ] || exit                            # Check for '0', exit if not found
        ;;
      5)
        _length="$(echo "${s}" | grep -E '^[0-9]+$')"     # Parse and validate payload length
        ;;
      6)
        _byteTxt="0${s}"                                  # Ensure byte format
        _byteTxt="${_byteTxt: -2}"                        # Pad single-digit bytes with '0'
        _payload=("${_byteTxt}")                          # Instantiate the payload array with the byte
        _payloadIdx=$(( $_payloadIdx + 1 ))               # Increment the index in the payload array
        ;;
      *)
        _byteTxt="0${s}"                                  # Ensure byte format
        _byteTxt="${_byteTxt: -2}"                        # Pad single-digit bytes with '0'
        _payload[$_payloadIdx]="${_byteTxt}"              # Append the byte in the payload array
        _payloadIdx=$(( $_payloadIdx + 1 ))               # Increment the index in the payload array
        ;;
    esac 
    _idx=$(( $_idx + 1 ))                                 # Increment the parsing step
  done

  # Generate a timestamp
  _time="$(date +%s%3N | sed "s/\(...\)$/.\1/")"

  # Ensure the CAN ID is 8 characters long
  _can_id="00000000${_can_id}"
  _can_id="${_can_id: -8}"

  # Prepare the payload in the desired format
  _payload="$(echo "${_payload[@]}" | tr -d " ")"         # Concatenate payload bytes
  _payload="0000000000000000${_payload}"                  # Ensure payload is 16 characters long
  _payload="${_payload: -$(( $_length * 2 ))}"            # Trim excess payload bytes

  # Output the formatted data
  echo "(${_time}) vcan0 ${_can_id}#${_payload}"
) done )
