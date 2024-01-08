# Import necessary libraries
import os
import json
import requests
from datetime import datetime

# Define the directory where JSON files are located
JSONS_DIRECTORY="decoded_output_jsons"

# Define the URL for the Hasura GraphQL endpoint
HASURA_URL="http://192.168.123.239:8080/v1/graphql"

# List all files in the JSONS_DIRECTORY and sort them
files = os.listdir(JSONS_DIRECTORY)
files.sort()

# Iterate through each JSON file in the directory
for file in files:
    try:
        # Print a message indicating the file being uploaded
        print("uploading " + file)

        # Convert the filename (timestamp) to a datetime object
        dt = datetime.fromtimestamp(float(file.split('.json')[0]))

        # Open the JSON file for reading
        with open(JSONS_DIRECTORY + "/" + file, 'r') as content:
            # Send a POST request to the Hasura GraphQL endpoint with a mutation
            # It inserts the following fields the canbus_data table for each timestamp:
            #   - canbus_id
            #   - canbus_id_name
            #   - value
            #   - value_text
            #   - unit
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
                    # Convert datetime to ISO format
                    'timestamp': dt.isoformat(),
                    # Load JSON data from the file
                    'canData': json.loads(content.read())
                }
            })

            # Parse the response as JSON
            response_json = response.json()

            # Check for errors in the response
            if 'errors' in response_json and len(response_json['errors']) > 0:
                # Print any errors that occurred
                print(response_json)
    except Exception as e:
        print(f"Error reading file: {e}")  # Handle any exceptions that may occur during file processing
