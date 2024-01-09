"""
Function to load the CAN log file from socketcan ROS package, DBC file, and decode the CAN messages
to physical messages.
Ashish Roongta
SAFEAI lab, Carnegie Mellon University

[Input]:
dbc_file: the .dbc file name of the DBC file, with the relative directory
          eg- DBC_dir/chassis_kia_soul_ev.dbc
data_file: the file name and relative directory of the data file
           eg- data_dir/can0_rostopic.txt
"""

# Importing required packages
import cantools  # for handling DBC files
import re  # for regular expressions
import os  # for file and directory operations
import json  # for JSON operations
import sys  # for accessing system-specific parameters
from numbers import Number  # for number type checking

# Directory where decoded output JSONs will be stored
DECODED_OUTPUT_JSONS_FOLDER = "decoded_output_jsons"
os.makedirs(DECODED_OUTPUT_JSONS_FOLDER, exist_ok=True)  # Create the directory if it doesn't exist

def save_data(messages, previous_datetime_str):
    """
    Saves decoded CAN messages to a JSON file.

    Args:
    messages: The decoded CAN messages.
    previous_datetime_str: Timestamp used as the filename.
    """
    try:
        with open(f"{DECODED_OUTPUT_JSONS_FOLDER}/{previous_datetime_str}.json", 'w') as json_content:
            json.dump(messages, json_content, indent=4)
    except Exception as e:
        print(f"Error creating and saving file: {e}")

def can_decoder(
        dbc_file="panther.dbc",
        data_file='test.txt'
    ):
    """
    Decodes CAN messages using a DBC file.

    Args:
    dbc_file: Filename of the DBC file.
    data_file: Filename of the data file containing CAN logs.
    """
    previous_datetime_str = ""
    message = ""
    allMessages = []
    log_data = {}

    # Loading the dbc file
    db = cantools.database.load_file(dbc_file)

    # Creating a dictionary to map signal names to their units
    signal_dict = {y.name: y.unit for x in db.messages for y in x.signals}

    # Loading the data log file
    data = open(data_file, 'r')
    lines = data.readlines()

    # Parsing the log file
    for i, line in enumerate(lines):
        if not line.strip():
            print("This is the last line or an empty line.")
            break

        # Parsing the timestamp
        datetime_str = line.split(' ')[0].split('(')[1].split(')')[0]

        if previous_datetime_str == "":
            previous_datetime_str = datetime_str

        if datetime_str != previous_datetime_str:
            previous_datetime_str = datetime_str
            log_data[previous_datetime_str] = allMessages
            save_data(allMessages, previous_datetime_str)
            log_data = {}
            allMessages = []

        payload = line.split(' ')[2]

        # Parsing ID and Data using dbc file
        can_id_str = "0x" + payload.split('#')[0].strip()
        can_id_int = int(can_id_str, 16)
        can_data = payload.split('#')[1].strip()

        try:
            if can_id_int not in db._frame_id_to_message:
                continue

            message = db.decode_message(can_id_int, bytearray.fromhex(can_data))
            allMessages.extend(
                {
                    "canbus_id": can_id_str,
                    "canbus_id_name": key,
                    "value": round(value, 6) if isinstance(value, Number) else 0,
                    "value_text": str(value) if not isinstance(value, (int, float)) else "",
                    "unit": signal_dict.get(key)
                }
                for key, value in message.items()
            )
        except ValueError as ve:
            message = f"Error: {ve}"
            print(message)
        except Exception as e:
            print(f"Error: Unable to decode message for ID {can_id_str}. {e}")

    save_data(allMessages, datetime_str)

# Executing the can_decoder function with the data file as an argument
can_decoder(data_file=sys.argv[1])
