# Use "pip install requests" in the terminal before importing this
import requests

# Using Fun Facts API: https://uselessfacts.jsph.pl/

# Setup the API url and endpoint
api_url = "https://uselessfacts.jsph.pl/"
api_endpoint = "api/v2/facts/random"

# Make the request and accept JSON then save the response object
header = {"Accept":"application/json"}
response = requests.get(api_url+api_endpoint, headers=header)

# Print the 'text' element from the JSON returned in the response
print(response.json()['text'])
# Print the response status code
print(response.status_code)
# Print the URL used for the today request
print(response.url)


# Make the request and accept plain text then save the response object
header = {"Accept":"text/plain"}
response = requests.get(api_url+api_endpoint, headers=header)

# Print the content from the response -> This is the entire fun fact because we accept plain text in the response
print(response.content)
# Print the response status code
print(response.status_code)
# Print the URL used for the today request
print(response.url)
