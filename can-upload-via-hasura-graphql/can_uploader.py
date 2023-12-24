import os
import json
import requests
from datetime import datetime

JSONS_DIRECTORY="decoded_output_jsons"
HASURA_URL="http://192.168.123.239:8080/v1/graphql"

files = os.listdir(JSONS_DIRECTORY)
files.sort()

for file in files:
    try:
        print("uploading " + file)
        dt = datetime.fromisoformat(file.split('.')[0])

        with open(JSONS_DIRECTORY + "/" + file, 'r') as content:
            response = requests.post(HASURA_URL, json={
                'query': '''
                    mutation MyMutation ($timestamp: timestamptz, $canData: [canbus_data_insert_input!]!) {
                        insert_canbus_timeline(objects: { timestamp: $timestamp, canbus_data: { data: $canData } }) {
                            returning {
                                id
                                canbus_data {
                                    canbus_id
                                    value
                                }
                            }
                        }
                    }
                ''',
                'variables': {
                    'timestamp': dt.isoformat(),
                    'canData': json.loads(content.read())
                }
            })

            response_json = response.json()
            if len(response_json['errors']) > 0:
                print(response_json)
    except Exception as e:
        print(f"Error reading file: {e}")
