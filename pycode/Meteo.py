import open_meteo_url_builder as omubld
import open_meteo_utility as omutil
import coordinates_searcher as csrch
import datetime

def getWeather(name:str, year:int, month:int, day:int):
    FORECAST_DAYS = 16
    today = datetime.datetime.now()
    todayzero = datetime.datetime(year=today.year, month=today.month, day=today.day)
    tgtdate = datetime.datetime(year=year, month=month, day=day)
    datediff = tgtdate - todayzero
    if   datediff.days < 0: # for using open meteo archive
        return ("error", "error", -1, -300.0, -300.0)
    elif datediff.days > 15:
        return ("error", "error", -1, -300.0, -300.0)
    else:
        coordinates = csrch.get_coordinates(name)
        murlbld = omubld.MeteoURLBuilder(coordinates[0], coordinates[1])
        fullname = coordinates[2]
        murlbld.addDailyParameter("weather_code")
        murlbld.addDailyParameter("temperature_2m_min")
        murlbld.addDailyParameter("temperature_2m_max")
        murlbld.setTimeZone("Asia%2FTokyo")
        murlbld.setForecastDays(FORECAST_DAYS)
        url = murlbld.buildUrl()
        pass
    weather = omutil.get_weather(url)
    weather_code    = weather["daily"]["weather_code"][datediff.days]
    weather_msg     = omutil.code2WeatherJP(weather_code)
    #weather_msg     = omutil.code2Weather(weather_code)
    max_temperature = weather["daily"]["temperature_2m_max"][datediff.days]
    min_temperature = weather["daily"]["temperature_2m_min"][datediff.days]
    return (fullname, weather_msg, weather_code, max_temperature, min_temperature)
