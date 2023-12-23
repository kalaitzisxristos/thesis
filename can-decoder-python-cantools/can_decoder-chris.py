"""
 Function load the can log file from socketcan ros package, DBC file and decode the can message
  to physical messages
  Ashish Roongta
 SAFEAI lab, Carnegie Mellon University

    -----------Documentation------------------------------

[Input]:
dbc_file: the .dbc file name of the DBC file, with the realtive directory
            eg- DBC_dir/chassis_kia_soul_ev.dbc
data_file: the file name and realtive directory of the data file
            eg- data_dir/can0_rostopic.txt
de_file: the file name for the saving the decrypted can data

"""

#----------------Importing Packages-----------------------------------------------------
import cantools
import numpy as np
import can
import re
from binascii import hexlify
import os

import json

def can_decoder(
        dbc_file="panther.dbc",
        data_file='test.txt',
        de_file='decoded_file'
    ):
    decoded_output_jsons_folder = "decoded_output_jsons"
    previous_datetime_str = ""
    message = ""
    allMessages = []
    log_data = {}

    os.makedirs(decoded_output_jsons_folder)
    # ----------------Loading the dbc file ---------------------
    db = cantools.database.load_file(dbc_file)

    ## ----------------Creating a dictionary--------------------
    ref = {'seq': 0, 'time': 1, 'id': 2, 'is_error': 3, 'data': 4}

    # ------Loading the data log file---------------------------
    data = open(data_file, 'r')
    lines = data.readlines()

    # Extracting date from comments
    date_line = [line for line in lines if line.startswith("# Time:")][0]
    date_match = re.search(r'\d{8}T\d{6}', date_line)
    if date_match:
        date_str = date_match.group(0)
        print("Date extracted from comments:", date_str)

    # ------------------Parsing the log file--------------------
    for i, line in enumerate(lines):
        if not line.strip():
            print("This is the last line or an empty line.")
            break
        if line.startswith("Timestamp"):
            print("THIS IS THE HEADER: " + line.strip())
            continue
        if line.startswith("#"):
            print("IGNORE COMMENT: " + line.strip())
            continue

        # Parsing the timestamp
        timestamp_str = line.split(';')[0]
        date_part = date_str.split('T')[0]
        timestamp_part = timestamp_str.split('T')[1]  # Extracting time part after 'T'
        datetime_str = date_part + 'T' + timestamp_part  # Combining date and 
                
        if datetime_str != previous_datetime_str:
            previous_datetime_str = datetime_str
            try:
                log_data[previous_datetime_str] = allMessages
                with open(f"{decoded_output_jsons_folder}/{previous_datetime_str}.json", 'w') as json_content:
                    json.dump(log_data, json_content, indent=4)
                print("file saved")
                log_data = {}
                allMessages = []
            except Exception as e:
                print(f"Error creating and saving file: {e}")

        # Parsing ID and Data using dbc file
        can_id_str = "0x" + line.split(';')[2].strip()
        can_id_int = int(can_id_str, 16)
        can_id_hex = hex(can_id_int)
        can_data = line.split(';')[3].strip()

        try:
            message = db.decode_message(can_id_int, bytearray.fromhex(can_data))
            message_json = json.dumps(message)
            message_dict = json.loads(message_json)
            # allMessages.append(json.dumps(str(message)))
            allMessages.extend(
                json.dumps({"canbus_id": can_id_str,"canbus_id_name": key, "value": value})
                for key, value in message_dict.items()
            )
            # print("Decoded Message:", json.dumps(message))
        except ValueError as ve:
            message = f"Error: {ve}"
            print(message)
        except Exception as e:
            print(f"Error: Unable to decode message for ID {can_id_str}. {e}")

        # print(str(i) + " Use this line: " + line.strip() 
        #       + " | datetime_str: " + datetime_str
        #       + " | can_id_str: " + can_id_str
        #       + " | can_id_int: " + str(can_id_int)
        #       + " | can_id_hex: " + str(can_id_hex)
        #       + " | can_data: " + can_data
        #       + " | message: " + str(message))
        

    # for i,line in enumerate(lines):
    #     if(line.startswith("header:")):
    #         # Extracting data in decimal format
    #         words=lines[i+11].rstrip().split('[')
    #         datastr=words[1].rstrip().split(']')[0]

    #         #extracting seq
    #         words=lines[i+1].rstrip().split()
    #         seq=int(words[1])

    #         #extracting time
    #         words1=lines[i+3].rstrip().split()
    #         words=lines[i+4].rstrip().split()
    #         time=float(words1[1]+"."+words[1])

    #         #extracting id
    #         words=lines[i+6].rstrip().split()
    #         mid=int(words[1])

    #         #extracting if it is error
    #         words=lines[i+9].rstrip().split()
    #         error=(words[1]=='True')

    #         #appending the data to an array
    #         if i==0:
    #             table=np.array([seq,time,mid,error,datastr])
    #         else:
    #             table=np.vstack((table,[seq,time,mid,error,datastr]))

    # print("++++++++++++(++++++++=++++++++++++Raw Data++++++++++=+++++++++++++++++++++++++++++++++++++")
    # print("Seq","\t","Time","\t","Id","\t","is_error","\t","Data")
    # print(table)
    # np.save("can0_rec1_rostopic_raw",table)
    # print("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")

    # #--------------Decoding the can messages to phycial messages-------------------------------------------
    # flag=0
    # for i in range(len(table)):
    #     print ("in table")
    #     m_id=int(table[i,ref['id']])
    #     if (m_id!=688 and m_id!=1200 and m_id!= 544):
    #         print("continue")
    #         continue
    #     msg_data=table[i,ref['data']].split(',')
    #     data_hex=""

    #     for subdata in msg_data:
    #         data_hex+=str(format(int(subdata),'02x'))
    #     if flag==0:
    #         decoded_table=np.array([float(table[i,ref['time']]),db.decode_message(int(table[i,ref['id']]),bytearray.fromhex(data_hex))])
    #         flag=1

    #         print ("in flag 0")
    #         print("Time","\t","Physical Message")
    #         print(decoded_table)
    #     else:
    #         decoded_table=np.vstack((decoded_table,[float(table[i,ref['time']]),db.decode_message(int(table[i,ref['id']]),bytearray.fromhex(data_hex))]))        

    #         print ("in flag 1")
    #         print("Time","\t","Physical Message")
    #         print(decoded_table)
    # #saving the decoded file
    # # np.savetxt("can0_rec1_rostopic_decoded",decoded_table)
    # save_file_path=de_file+".txt"
    # fileh=open(save_file_path,"w")
    # for i in range(len(decoded_table)):
    #     if(i ==0):
    #         fileh.write("Time (seconds)"+"\t\t"+"Phyical Message\n")
    #     fileh.write(str(decoded_table[i,0])+"\t"+str(decoded_table[i,1])+"\n")
    # fileh.close()
    # print("Decoded can messages saved to file: {save_file_path}")

can_decoder()

str=json.dumps(['foo', {'bar': ('baz', None, 1.0, 2)}])
print(str)