# Use "pip install requests" in the terminal before importing this
import requests
import json

# Using OpenWeather API: https://openweathermap.org/api

api_key = "_______YOUR_API_KEY_______"

# Setup the base URL of the geolocation API
geo_api_url = "http://api.openweathermap.org/geo/1.0"
# Form the endpoint for geolocation based on zip code and country code based on the documentation
geo_endpoint = "/zip"
geo_endpoint += "?zip=YOUR_ZIP_CODE,US"
geo_endpoint += f"&appid={api_key}"

# Make the request then save the response object
geo_response = requests.get(geo_api_url+geo_endpoint)

# Get the latitude and longitude from the returned JSON object
latitude = geo_response.json()['lat']
longitude = geo_response.json()['lon']


# Setup the base URL of the weather API
weather_api_url = "https://api.openweathermap.org/data/2.5"
# Form the endpoint of the current weather data API call using latitude and longitude based on the documentation
weather_endpoint = "/weather"
weather_endpoint += f"?lat={latitude}"
weather_endpoint += f"&lon={longitude}"
weather_endpoint += f"&appid={api_key}"
weather_endpoint += f"&units=imperial"

# Make the request then save the response object
weather_response = requests.get(weather_api_url+weather_endpoint)

# Print the response JSON in a pretty format
print(json.dumps(weather_response.json(), indent=4))
# Print some of the data from the response
print(f"Current weather: {weather_response.json()['main']['temp']}")
print(weather_response.status_code)
print(weather_response.url)
