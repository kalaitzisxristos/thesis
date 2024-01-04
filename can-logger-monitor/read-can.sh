#!/usr/bin/env bash

DEVICE="${1}"
BAUD="${2}"

CONFIG_DEBUG="${DEBUG:-}"

debug_print() {
  [ -z $CONFIG_DEBUG ] || echo "${@}"
}

debug_print "Device: ${DEVICE}"
debug_print "BAUD Rate: ${BAUD}"

stty -F "${DEVICE}" raw speed "${BAUD}" >/dev/null

echo "CANEN0=1" > "${DEVICE}"
echo "CANSPEED0=1000000" > "${DEVICE}"
echo "CANEN1=0" > "${DEVICE}"

echo "BTMODE=0" > "${DEVICE}"
echo "WIFIMODE=0" > "${DEVICE}"

echo "LAWICEL=1" > "${DEVICE}"

# Discard current incoming line to prevent parsing errors
cat "${DEVICE}" | ( head -n1 >/dev/null ; while read line; do (
  line="$(echo "${line}" | tr -d "\r")"

  _idx=0
  _payloadIdx=0
  debug_print "Got line: ${line}" # | hexdump -C

  for s in ${line}; do
    case $_idx in
      0)
        _time="$(echo "${s}" | grep -E '^[0-9]+$')"
        ;;
      1)
        [ ${s} = '-' ] || exit
        ;;
      2)
        _address="$(echo "${s}" | grep -E '^[0-9a-f]+$')"
        ;;
      3)
        [ ${s} = 'S' ] || [ ${s} = 'X' ] || exit
        ;;
      4)
        [ ${s} = '0' ] || exit
        ;;
      5)
        _length="$(echo "${s}" | grep -E '^[0-9]+$')"
        ;;
      6)
        _byteTxt="0${s}"
        _byteTxt="${_byteTxt: -2}"
        _payload=("${_byteTxt}")
        _payloadIdx=$(( $_payloadIdx + 1 ))
        ;;
      *)
        _byteTxt="0${s}"
        _byteTxt="${_byteTxt: -2}"
        _payload[$_payloadIdx]="${_byteTxt}"
        _payloadIdx=$(( $_payloadIdx + 1 ))
        ;;
    esac 
    _idx=$(( $_idx + 1 ))
  done

  # _time="$(echo "${_time}" | rev | sed 's/^\(......\)/\1,' | rev)"}
  _time="$(date +%s%3N | sed "s/\(...\)$/.\1/")"

  _address="00000000${_address}"
  _address="${_address: -8}"

  _payload="$(echo "${_payload[@]}" | tr -d " ")"
  _payload="0000000000000000${_payload}"
  _payload="${_payload: -$(( $_length * 2 ))}"

  # echo "Time: ${_time}"
  # echo "ID: ${_address}"
  # echo "Payload: ${_payload}"
  echo "(${_time}) vcan0 ${_address}#${_payload}"
) done )

