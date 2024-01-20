########################################
# Auther : Yuta Watanabe
# Name   : open_meteo_utility
# Date   : 2024-01-07
# About  : open meteo utilities
########################################
import requests

def code2WeatherJP(code:int) -> str:
    if   code == 0:
        return "快晴"
    elif code == 1:
        return "晴れ"
    elif code == 2:
        return "時々曇り"
    elif code == 3:
        return "曇り"
    elif code == 45:
        return "霧"
    elif code == 48:
        return "霧氷"
    elif code == 51:
        return "薄い霧雨"
    elif code == 53:
        return "霧雨"
    elif code == 55:
        return "濃い霧雨"
    elif code == 56:
        return "氷点下の薄い霧雨"
    elif code == 57:
        return "氷点下の濃い霧雨"
    elif code == 61:
        return "弱い雨"
    elif code == 63:
        return "雨"
    elif code == 65:
        return "激しい雨"
    elif code == 66:
        return "氷点下の弱い雨"
    elif code == 67:
        return "氷点下の激しい雨"
    elif code == 71:
        return "弱い雪"
    elif code == 73:
        return "雪"
    elif code == 75:
        return "激しい雪"
    elif code == 77:
        return "霧雪"
    elif code == 80:
        return "弱いにわか雨"
    elif code == 81:
        return "にわか雨"
    elif code == 82:
        return "激しいにわか雨"
    elif code == 85:
        return "弱いにわか雪"
    elif code == 86:
        return "激しいにわか雪"
    # weather codes under this line are available in central europe
    elif code == 95:
        return "雷雨"
    elif code == 96:
        return "雹を伴う雷雨"
    elif code == 99:
        return "雹を伴う激しい雷雨"
    else:
        return None

def code2Weather(code:int) -> str:
    if   code == 0:
        return "clear"
    elif code == 1:
        return "sunny"
    elif code == 2:
        return "partly cloudy"
    elif code == 3:
        return "cloudy"
    elif code == 45:
        return "fog"
    elif code == 48:
        return "freezing fog"
    elif code == 51:
        return "light drizzle"
    elif code == 53:
        return "drizzle"
    elif code == 55:
        return "dense drizzle"
    elif code == 56:
        return "light freezing drizzle"
    elif code == 57:
        return "dense freezing drizzle"
    elif code == 61:
        return "slight rain"
    elif code == 63:
        return "rain"
    elif code == 65:
        return "heavy rain"
    elif code == 66:
        return "light freezing rain"
    elif code == 67:
        return "heavy freezing rain"
    elif code == 71:
        return "slight snow falls"
    elif code == 73:
        return "snow falls"
    elif code == 75:
        return "heavy snow falls"
    elif code == 77:
        return "snow grains"
    elif code == 80:
        return "slight rain showers"
    elif code == 81:
        return "rain showers"
    elif code == 82:
        return "heavy rain showers"
    elif code == 85:
        return "slight snow showers"
    elif code == 86:
        return "heavy snow showers"
    # weather codes under this line are available in central europe
    elif code == 95:
        return "thunderstorm"
    elif code == 96:
        return "thunderstorm with slight hail"
    elif code == 99:
        return "thunderstorm with heavy hail"
    else:
        return None

def get_weather(url:str):
    weather_today = requests.get(url).json()

    return(weather_today)
