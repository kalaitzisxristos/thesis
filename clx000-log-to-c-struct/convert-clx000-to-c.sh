#!/usr/bin/env bash

# Skip headers
# head -n17 >/dev/null
# head -n17
# exit

grep --line-buffered  -E '^[0-3]' | while IFS=";" read canTimestamp canType canId canData; do
  # "%DDT%hh%mm%ss%kkk" -> "1970-01-01 %hh:%mm:%ss,%kkk"
  _formattedTime="$(echo "${canTimestamp}" | sed -e "s/..T\(..\)\(..\)\(..\)\(...\)/1970-01-01 \1:\2:\3,\4/")"
  _timeInMillis="$(date -u -d "${_formattedTime}" +%s%3N)"

  _sizeInChars="$(printf "${canData}" | wc -c)"
  _sizeInBytes="$(( $_sizeInChars / 2 ))"

  echo ", { .time = ${_timeInMillis}, .isExtended = ${canType}, .id = 0x${canId}, .size = ${_sizeInBytes}, .data = 0x${canData} }"
done

