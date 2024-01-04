import math
import coordinates_searcher as csrch

class MeteoURLBuilder: # easily created, not completed fully.

    def __init__(self):
        self.url = "https://api.open-meteo.com/v1/forecast"
        self.latitude  = float("nan")   # as a null number
        self.longitude = float("nan")   # as a null number
        self.hourly_parameters  = []    # should create Parameter class?
        self.daily_parameters   = []
        self.current_parameters = []
        self.timezone           = None
        self.forecast_days      = None

    def __init__(self, x:float, y:float):
        self.url = "https://api.open-meteo.com/v1/forecast"
        self.latitude  = y
        self.longitude = x
        self.hourly_parameters  = []
        self.daily_parameters   = []
        self.current_parameters = []
        self.timezone           = None
        self.forecast_days      = None

    def setLongitude(self, x:float):
        self.longitude = x

    def setLatitude(self, y:float):
        self.latitude  = y

    def setTimeZone(self, timezone:str):
        self.timezone = timezone

    def setForecastDays(self, forecast_days:str):
        self.forecast_days = forecast_days

    def addParameter(self, parameter:str): # unused. useful for generalization?
        pass

    def addHourlyParameter(self, parameter:str):
        self.hourly_parameters.append(parameter)

    def addDailyParameter(self, parameter:str):
        self.daily_parameters.append(parameter)

    def addCurrentParameter(self, parameter:str):
        self.current_parameters.append(parameter)

    def buildUrl(self) -> str:
        if math.isnan(self.latitude) or math.isnan(self.longitude):
            return None
        url =  self.url + "?longitude={:.4f}&latitude={:.4f}".format(self.longitude, self.latitude)
        if self.hourly_parameters:
            url += "&hourly="
            i = 1
            for parameter in self.hourly_parameters:
                url += parameter
                if i < len(self.hourly_parameters):
                    url += ","
                i += 1
        if self.daily_parameters:
            url += "&daily="
            i = 1
            for parameter in self.daily_parameters:
                url += parameter
                if i < len(self.daily_parameters):
                    url += ","
                i += 1
        if self.current_parameters:
            url += "&current="
            i = 1
            for parameter in self.current_parameters:
                url += parameter
                if i < len(self.current_parameters):
                    url += ","
                i += 1
        if self.timezone:
            url += "&timezone=" + self.timezone
        if self.forecast_days:
            url += "&forecast_days=" + str(self.forecast_days)
        return url
