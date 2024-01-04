import requests
import re

# get location name, return [latitude, longitude, fullname]
def get_coordinates(name:str):
    url = "https://msearch.gsi.go.jp/address-search/AddressSearch?q=" + name
    locations = requests.get(url).json()
    if not locations:
        return[float("nan"), float("nan"), "not found"]
    title_list = [location["properties"]["title"] for location in locations]
    p = re.compile(r'.*'+name+"$")
    index = 0
    for title in title_list:
        if p.match(title):
            break
        index += 1
    if index == len(title_list):
        return[float("nan"), float("nan"), "not found"]
    coordinates = [locations[index]["geometry"]["coordinates"][0],
                   locations[index]["geometry"]["coordinates"][1],
                   locations[index]["properties"]["title"]
                  ]
    return(coordinates)
