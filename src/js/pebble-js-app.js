// Thaks to https://github.com/cfg1/pebble-mss/tree/master/src
// Thanks to pebble-mms
var CLOUDPEBBLE = 0;
var CLIMACON = {
  'cloud'            : '!',
  'cloud_day'        : '"',
  'cloud_night'      : '#',
  'rain'             : '$',
  'rain_day'         : '%',
  'rain_night'       : '&',
  'showers'          : "'",
  'showers_day'      : '(',
  'showers_night'    : ')',
  'downpour'         : '*',
  'downpour_day'     : '+',
  'downpour_night'   : ',',
  'drizzle'          : '-',
  'drizzle_day'      : '.',
  'drizzle_night'    : '/',
  'sleet'            : '0',
  'sleet_day'        : '1',
  'sleet_night'      : '2',
  'hail'             : '3',
  'hail_day'         : '4',
  'hail_night'       : '5',
  'flurries'         : '6',
  'flurries_day'     : '7',
  'flurries_night'   : '8',
  'snow'             : '9',
  'snow_day'         : ':',
  'snow_night'       : ';',
  'fog'              : '<',
  'fog_day'          : '=',
  'fog_night'        : '>',
  'haze'             : '?',
  'haze_day'         : '@',
  'haze_night'       : 'A',
  'wind'             : 'B',
  'wind_cloud'       : 'C',
  'wind_cloud_day'   : 'D',
  'wind_cloud_night' : 'E',
  'lightning'        : 'F',
  'lightning_day'    : 'G',
  'lightning_night'  : 'H',
// ---
  'sun'              : 'I',
   'set'             : 'J',
   'rise'            : 'K',
   'low'             : 'L',
   'lower'           : 'M',
   'moon'            : 'N',
   'new'             : 'O',
   'wax_cresc'       : 'P',
   'wax_quart'       : 'Q',
   'wax_gib'         : 'R',
   'full'            : 'S',
   'wane_cresc'      : 'T',
   'wane_quart'      : 'U',
   'wane_gib'        : 'V',
  'snowflake'        : 'W',
  'tornado'          : 'X',
  'thermometer'      : 'Y',
   'temp_low'        : 'Z',
   'temp_med-low'    : '[',
   'temp_med-high'   : "\\",
   'temp_high'       : ']',
   'temp_full'       : '^',
  'celsius'          : '`',
  'fahrenheit'       : '_',
  'compass'          : 'a',
   'north'           : 'b',
   'east'            : 'c',
   'south'           : 'd',
   'west'            : 'e',
  'umbrella'         : 'f',
  'sunglasses'       : 'g',
  'cloud_refresh'    : 'h',
  'cloud_up'         : 'i',
  'cloud_down'       : 'j'
};

var OWMclimacon= {
// Thunderstorm
  200 : CLIMACON['lightning'], // thunderstorm with light rain
  201 : CLIMACON['lightning'], // thunderstorm with rain
  202 : CLIMACON['lightning'], // thunderstorm with heavy rain
  210 : CLIMACON['lightning'], // light thunderstorm
  211 : CLIMACON['lightning'], // thunderstorm
  212 : CLIMACON['lightning'], // heavy thunderstorm
  221 : CLIMACON['lightning'], // ragged thunderstorm
  230 : CLIMACON['lightning'], // thunderstorm with light drizzle
  231 : CLIMACON['lightning'], // thunderstorm with drizzle
  232 : CLIMACON['lightning'], // thunderstorm with heavy drizzle
// Drizzle
  300 : CLIMACON['drizzle'], // light intensity drizzle
  301 : CLIMACON['drizzle'], // drizzle
  302 : CLIMACON['drizzle'], // heavy intensity drizzle
  310 : CLIMACON['drizzle'], // light intensity drizzle rain
  311 : CLIMACON['drizzle'], // drizzle rain
  312 : CLIMACON['drizzle'], // heavy intensity drizzle rain
  313 : CLIMACON['showers'], // shower rain and drizzle
  314 : CLIMACON['showers'], // heavy shower rain and drizzle
  321 : CLIMACON['showers'], // shower drizzle
// Rain
  500 : CLIMACON['rain'], // light rain
  501 : CLIMACON['rain'], // moderate rain
  502 : CLIMACON['downpour'], // heavy intensity rain
  503 : CLIMACON['downpour'], // very heavy rain
  504 : CLIMACON['downpour'], // extreme rain
  511 : CLIMACON['downpour'], // freezing rain
  520 : CLIMACON['showers'], // light intensity shower rain
  521 : CLIMACON['showers'], // shower rain
  522 : CLIMACON['showers'], // heavy intensity shower rain
  531 : CLIMACON['showers'], // ragged shower rain
// Snow
  600 : CLIMACON['snow'], // light snow
  601 : CLIMACON['snow'], // snow
  602 : CLIMACON['snow'], // heavy snow
  611 : CLIMACON['sleet'], // sleet
  612 : CLIMACON['sleet'], // shower sleet
  615 : CLIMACON['snow'], // light rain and snow
  616 : CLIMACON['snow'], // rain and snow
  620 : CLIMACON['snow'], // light shower snow
  621 : CLIMACON['snow'], // shower snow
  622 : CLIMACON['snow'], // heavy shower snow
// Atmosphere
  701 : CLIMACON['haze'], // mist
  711 : CLIMACON['haze'], // smoke
  721 : CLIMACON['haze'], // haze
  731 : CLIMACON['haze'], // Sand/Dust Whirls
  741 : CLIMACON['fog'], // Fog
  751 : CLIMACON['haze'], // sand
  761 : CLIMACON['haze'], // dust
  762 : CLIMACON['haze'], // VOLCANIC ASH
  771 : CLIMACON['wind'], // SQUALLS
  781 : CLIMACON['tornado'], // TORNADO
// Clouds
  800 : CLIMACON['sun'], // sky is clear
  801 : CLIMACON['cloud_day'], // few clouds
  802 : CLIMACON['cloud_day'], // scattered clouds
  803 : CLIMACON['cloud_day'], // broken clouds
  804 : CLIMACON['cloud'], // overcast clouds
// Extreme
  900 : CLIMACON['tornado'], // tornado
  901 : CLIMACON['tornado'], // tropical storm
  902 : CLIMACON['tornado'], // hurricane
  903 : CLIMACON['temp_low'], // cold
  904 : CLIMACON['temp_high'], // hot
  905 : CLIMACON['wind'], // windy
  906 : CLIMACON['hail'], // hail
// Additional
  950 : CLIMACON['set'], // Setting
  951 : CLIMACON['sun'], // Calm
  952 : CLIMACON['sun'], // Light breeze
  953 : CLIMACON['sun'], // Gentle Breeze
  954 : CLIMACON['sun'], // Moderate breeze
  955 : CLIMACON['sun'], // Fresh Breeze
  956 : CLIMACON['wind'], // Strong breeze
  957 : CLIMACON['wind'], // High wind, near gale
  958 : CLIMACON['wind'], // Gale
  959 : CLIMACON['wind'], // Severe Gale
  960 : CLIMACON['lightning'], // Storm
  961 : CLIMACON['lightning'], // Violent Storm
  962 : CLIMACON['tornado'], // Hurricane
};

var OWM_DEFAULT_API_KEY = "1b5b37a3117bb6acd583d662fdbb24c7";

var configuration = {
  invert: 0,
  light: 1,
  display_sec: 1,
  date_format: "%a, %d.%m.%Y",
  date_format_index: 2,
  time_zone_info: 2,

  vibe_disconnect: 1,
  vibe_full: 0,
  vibe_hour: 0,

  default_loc: "London",
  autodetect_loc: 1,
  lang_id: "en",
  show_update_time: 0,
  moon_phase: 0,
  weatherLine1: 5,
  weatherLine2: 2,
  weatherLine3: 3,
  weatherLine4: 4,
  weatherUpdateInt: 20,

  degree_f: 0,
  speed_unit: 0,
  pressure_unit: 0,

  OWM_API_KEY: OWM_DEFAULT_API_KEY
};

var ForecastDataJSON;
var WeatherDataJSON;

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  console.log("locationSuccess Begin");
  SendToPebble(pos, 0);
}

function locationError(err) {
  console.log("locationError Begin");
  console.log('location error (' + err.code + '): ' + err.message);
  console.log("Error requesting location! Using Default.");
  var pos = {
    coords: {latitude: 0, longitude: 0}
  };
  SendToPebble(pos, 1);
}


// this is to pad the hour for 24hr eg 3 = 03
//function pad(num, size) {
//    var s = num+"";
//    while (s.length < size) s = "0" + s;
//    return s;
//}


function SendToPebble(pos, use_default) {
  var url;
  var url_forecast;

  var multiplier = 10000;
  var pos_lat = Math.round(multiplier*pos.coords.latitude)/multiplier;
  var pos_lon = Math.round(multiplier*pos.coords.longitude)/multiplier;
  console.log("pos_lat = " + pos_lat);
  console.log("pos_lon = " + pos_lon);

  // Construct URL
  console.log("conf.auto_loc = " + configuration.autodetect_loc);
  //configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY; //TODO
  if (typeof configuration.OWM_API_KEY === 'string' || configuration.OWM_API_KEY instanceof String){
    if (configuration.OWM_API_KEY == "default"){
      configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY;
    }
    if (String(configuration.OWM_API_KEY).length < 20){
      configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY;
    }
  } else {
    configuration.OWM_API_KEY = OWM_DEFAULT_API_KEY;
  }
  console.log("Using OWM API KEY: "+configuration.OWM_API_KEY);
  if ((use_default === 0) && (configuration.autodetect_loc)){
    url = "http://api.openweathermap.org/data/2.5/weather?APPID=" + configuration.OWM_API_KEY + "&lat=" +
        pos_lat + "&lon=" + pos_lon + "&lang=" + configuration.lang_id;
    url_forecast = "http://api.openweathermap.org/data/2.5/forecast?APPID=" + configuration.OWM_API_KEY + "&lat=" +
        pos_lat + "&lon=" + pos_lon + "&lang=" + configuration.lang_id;
  } else {
    console.log("conf.def_loc = " + configuration.default_loc);
    var city_name_req = configuration.default_loc;
    // Construct URL
    url = "http://api.openweathermap.org/data/2.5/weather?APPID=" + configuration.OWM_API_KEY + "&q=" + city_name_req + "&lang=" + configuration.lang_id;
    url_forecast = "http://api.openweathermap.org/data/2.5/forecast?APPID=" + configuration.OWM_API_KEY + "&q=" + city_name_req + "&lang=" + configuration.lang_id;
  }

  console.log("Weather URL = " + url);
  console.log("Weather Forecast URL = " + url_forecast);

  var utc_offset = new Date().getTimezoneOffset() * 60;

  xhrRequest(url_forecast, 'GET',
    function(responseText) {
      var ForecastDataJSON_error = 0;
      try {
        ForecastDataJSON = JSON.parse(responseText);
        console.log("successfully parsed returned text of weather forecast.");
      } catch (e){
        ForecastDataJSON_error = 1;
        console.log("could not parse returned text of weather forecast: " + e);
      }

      xhrRequest(url, 'GET',
        function(responseText) {
          var WeatherDataJSON_error = 0;
          try {
            WeatherDataJSON = JSON.parse(responseText);
            console.log("successfully parsed returned text of weather data.");
          } catch (e){
            WeatherDataJSON_error = 1;
            console.log("could not parse returned text of weather data: " + e);
          }


          //---------------------------------------------------------------------------------------------------

          if (!WeatherDataJSON_error){

            // Temperature in Kelvin requires adjustment
            var temperature = Math.round((WeatherDataJSON.main.temp - 273.15));
            console.log("Temperature is " + temperature);
            var temp_min = Math.round((WeatherDataJSON.main.temp_min - 273.15));
            console.log("Temp. MIN is " + temp_min);
            var temp_max = Math.round((WeatherDataJSON.main.temp_max - 273.15));
            console.log("Temp. MAX is " + temp_max);

            // Conditions
            var conditions = WeatherDataJSON.weather[0].description;
            console.log("Conditions are " + conditions);

            var conditions_icon = OWMclimacon[WeatherDataJSON.weather[0].id].charCodeAt(0);
            console.log("Conditions icon is " + conditions_icon);

            // Pressure
            var pressure = Math.round(WeatherDataJSON.main.pressure);
            var pressure_unit = "hPa";
            switch (configuration.pressure_unit){
              case 1:
                pressure = Math.round(pressure/1.333);
                pressure_unit = "mmHg";
                break;
              case 2:
                pressure = Math.round(pressure/33.86389*100)/100;
                pressure_unit = "inHg";
                break;
            }
            console.log("Pressure is " + pressure + " " + pressure_unit);

            // Humidity
            var humidity = Math.round(WeatherDataJSON.main.humidity);
            console.log("Humidity is " + humidity);

            // Speed
            var speed_unit_conversion_factor = 1;
            if (configuration.speed_unit === 0){
              speed_unit_conversion_factor = 3.6; //m/s -> km/h
            } else if (configuration.speed_unit == 1){
              speed_unit_conversion_factor = 2.236; //m/s -> mph
            }
            var wind_speed = WeatherDataJSON.wind.speed*speed_unit_conversion_factor;
            if (wind_speed < 10){
              wind_speed = Math.round(wind_speed*10)/10;
            } else {
              wind_speed = Math.round(wind_speed);
            }
            var wind_speed_unit = "m/s";
            if (configuration.speed_unit === 0) wind_speed_unit = "km/h";
            if (configuration.speed_unit == 1) wind_speed_unit = "mph";
            console.log("Wind Speed is " + wind_speed + " " + wind_speed_unit);

            // Sunrise / Sunset
            var sunrise_unix = WeatherDataJSON.sys.sunrise;
            var sunset_unix  = WeatherDataJSON.sys.sunset;
            console.log("sunrise unix = "+sunrise_unix);
            console.log("sunset  unix = "+sunset_unix);
            var sunrise = timeConverter(Math.round(sunrise_unix));
            var sunset  = timeConverter(Math.round(sunset_unix));
            console.log("sunrise = " + sunrise);
            console.log("sunset =  " + sunset);
            //sunrise_unix = sunrise_unix - utc_offset;
            //sunset_unix  = sunset_unix  - utc_offset;

            // Update time
            var time_of_last_data = Number(WeatherDataJSON.dt);
            console.log("time_of_last_data = "+time_of_last_data);

            // Location:
            var location_name = WeatherDataJSON.name;
            var warn_location = 0;
            if ((configuration.autodetect_loc) && (use_default)){ //tried autodection of location, but could not get the lat long values from phone, so used default location set by the user.
              warn_location = 1;
              console.log("Tried autodection of location, but could not get the lat long values from phone.");
            }
            if ((configuration.autodetect_loc === 0) && (use_default === 0)){ //detected location, but used user input
              console.log("Could autodect location, but used user input instead.");
              console.log(" |lat_autodetect - user_lat| = " + Math.abs(WeatherDataJSON.coord.lat - pos_lat));
              console.log(" |lon_autodetect - user_lon| = " + Math.abs(WeatherDataJSON.coord.lon - pos_lon));
              console.log("pos_lat                   = " + pos_lat);
              console.log("WeatherDataJSON.coord.lat = " + WeatherDataJSON.coord.lat);
              console.log("pos_lon                   = " + pos_lon);
              console.log("WeatherDataJSON.coord.lon = " + WeatherDataJSON.coord.lon);
              if ((Math.abs(WeatherDataJSON.coord.lat - pos_lat) > 0.3) && (Math.abs(WeatherDataJSON.coord.lon - pos_lon) > 0.5)){
                console.log("User input location differs from autodeteced location.");
                warn_location = 1;
              }
            }
            console.log("City name is " + location_name);
            console.log("LATITUDE  is " + pos.coords.latitude);
            console.log("LONGITUDE is " + pos.coords.longitude);
            console.log("warn_location = " + warn_location);

            // TIME:
            console.log("UTC Offset is " + utc_offset);

            // Get Min/Max temp. from forecast:
            var Forecast = {
              TempMin: 10000, // in Kelvin
              TempMax:     0  // in Kelvin
            };
            if (!ForecastDataJSON_error){
              console.log("forecast_list has " + ForecastDataJSON.cnt + " elements");
              var i;
              for (i = 0; i < Math.min(ForecastDataJSON.cnt, 8); i++) { // 8 entries means 24 hours for 3 hour forecast
                console.log("forecast_list[" + i + "].dt_text = "+ForecastDataJSON.list[i].dt_txt+"; T = " + (ForecastDataJSON.list[i].main.temp - 273.15) + " C");
                Forecast.TempMin = Math.min(ForecastDataJSON.list[i].main.temp, Forecast.TempMin);
                Forecast.TempMax = Math.max(ForecastDataJSON.list[i].main.temp, Forecast.TempMax);
              }
            }
            console.log("ForecastTempMin = "+Forecast.TempMin);
            console.log("ForecastTempMax = "+Forecast.TempMax);

	    //24H Times - yeah this sux, I can't js
	    //6h
	    a1=ForecastDataJSON.list[1].dt_txt.split(' ');
            a2=a1[1].split(':');
	    var time_6h = String(a2[0]); 
	    //12h
            a1=ForecastDataJSON.list[3].dt_txt.split(' ');
	    a2=a1[1].split(':');
            var time_12h = String(a2[0]);		
            //18h
            a1=ForecastDataJSON.list[5].dt_txt.split(' ');
            a2=a1[1].split(':');
            var time_18h = String(a2[0]);
            //24h
            a1=ForecastDataJSON.list[7].dt_txt.split(' ');
            a2=a1[1].split(':');
            var time_24h = String(a2[0]);
	

	    console.log("time_6h = " +time_6h);
	    console.log("time_12h = " +time_12h);
	    console.log("time_18h = " +time_18h);
	    console.log("time_24h = " +time_24h);

      	    //24H Temps	
            if (configuration.degree_f){
              var temp_6h = Math.round((ForecastDataJSON.list[1].main.temp - 273.15)*1.8+32)+"°";
              var temp_12h = Math.round((ForecastDataJSON.list[3].main.temp - 273.15)*1.8+32)+"°";
              var temp_18h = Math.round((ForecastDataJSON.list[5].main.temp - 273.15)*1.8+32)+"°";
              var temp_24h = Math.round((ForecastDataJSON.list[7].main.temp - 273.15)*1.8+32)+"°";
            } else {
              var temp_6h = Math.round(ForecastDataJSON.list[1].main.temp - 273.15)+"°";
              var temp_12h = Math.round(ForecastDataJSON.list[3].main.temp - 273.15)+"°";
              var temp_18h = Math.round(ForecastDataJSON.list[5].main.temp - 273.15)+"°";
              var temp_24h = Math.round(ForecastDataJSON.list[7].main.temp - 273.15)+"°";
            }
            console.log("temp_6h = " +temp_6h);
            console.log("temp_12h = " +temp_12h);
            console.log("temp_18h = " +temp_18h);
            console.log("temp_24h = " +temp_24h);

            var weather_Line_1 = "";
            var weather_Line_2 = "";
            var weather_Line_3 = "";
            var weather_Line_4 = "";

            // min / max
            if ((Forecast.TempMin == 10000) || (Forecast.TempMax === 0)){
                  weather_Line_1 = " --/-- ";
                } else {
                  if (configuration.degree_f){
                    weather_Line_1 = Math.round((Forecast.TempMix-273.15)*1.8+32) + "°/" + Math.round((Forecast.TempMax-273.15)*1.8+32) + "°F";
                  } else {
                    weather_Line_1 = Math.round((Forecast.TempMin-273.15)) + "/" + Math.round((Forecast.TempMax-273.15)) + " °C";
                    //weather_Line_1 = Math.round((Forecast.TempMax-273.15)) + "/" + Math.round((Forecast.TempMin-273.15)) + "C";
                  }
                }
            console.log("weather_Line_1 = " + (weather_Line_1.replace('°', ' ')).replace('°', ' '));

            // windspeed
            weather_Line_2 = wind_speed + " " + wind_speed_unit;
            console.log("weather_Line_2 = " + (weather_Line_2.replace('°', ' ')).replace('°', ' '));

            // humidity
            weather_Line_3 = humidity + "%";
            console.log("weather_Line_3 = " + (weather_Line_3.replace('°', ' ')).replace('°', ' '));

            // pressure
            weather_Line_4 = pressure + " " + pressure_unit;
            console.log("weather_Line_4 = " + (weather_Line_4.replace('°', ' ')).replace('°', ' '));

            if (CLOUDPEBBLE) {
              weather_string_1 = (weather_string_1.replace('°', '__')).replace('°', '__');
              weather_string_2 = (weather_string_2.replace('°', '__')).replace('°', '__');
            }

            // Assemble dictionary using our keys
            var dictionary = {
              "WEATHER_LOCATION_NAME_KEY": location_name,
              //"KEY_LOCATION_LAT": Math.round(pos.coords.latitude*1000000),
              //"KEY_LOCATION_LON": Math.round(pos.coords.longitude*1000000),
              "WEATHER_TEMPERATURE_KEY": temperature,
              "WEATHER_PRESSURE_KEY": weather_Line_4,
              "WEATHER_HUMIDITY_KEY": weather_Line_3,
              "WEATHER_WINDSPEED_KEY": weather_Line_2,
              "WEATHER_ICON_KEY": conditions_icon,
              "WEATHER_TEMPMINMAX_KEY": weather_Line_1,
              "WEATHER_TEMP_6H_KEY": temp_6h,
              "WEATHER_TEMP_12H_KEY": temp_12h,
              "WEATHER_TEMP_18H_KEY": temp_18h,
              "WEATHER_TEMP_24H_KEY": temp_24h,
              "WEATHER_TIME_6H_KEY": time_6h,
              "WEATHER_TIME_12H_KEY": time_12h,
              "WEATHER_TIME_18H_KEY": time_18h,
              "WEATHER_TIME_24H_KEY": time_24h,

              //"KEY_WEATHER_STRING_1": weather_string_1,
              //"KEY_WEATHER_STRING_2": weather_string_2,
              //"KEY_WEATHER_ICON": conditions_icon,
              "WEATHER_TIME_UTC_OFFSET_KEY": utc_offset,
              //"KEY_TIME_ZONE_NAME": getTimeZone(),
              //"KEY_SUN_RISE": sunrise,
              //"KEY_SUN_SET": sunset,
              "WEATHER_SUN_RISE_UNIX_KEY": sunrise_unix,
              "WEATHER_SUN_SET_UNIX_KEY": sunset_unix, //both converted to local time zone
              "WEATHER_DATA_TIME_KEY": time_of_last_data
              //"KEY_WARN_LOCATION": warn_location
            };

            // Send to Pebble

            console.log("Sending Weather Info to Pebble ...");
            Pebble.sendAppMessage(dictionary,
                                  function(e) {
                                    console.log("Weather info sent to Pebble successfully!");
                                  },
                                  function(e) {
                                    console.log("Error sending weather info to Pebble!");
                                  }
                                 );

          } //end: if (!WeatherDataJSON_error)
          var date = new Date();
          console.log("Time is " + date);

          ForecastDataJSON = {};
          WeatherDataJSON  = {};


          //---------------------------------------------------------------------------------------------------
        }
      );
    }
  );
}

function getTimeZone() {
    return /\((.*)\)/.exec(new Date().toString())[1];
}



function timeConverter(UNIX_timestamp){

  var a = new Date(UNIX_timestamp*1000);
  var year = a.getFullYear();
  var month = pad(a.getMonth()+1);
  var date = pad(a.getDate());
  var hour = pad(a.getHours());
  var min = pad(a.getMinutes());
  var sec = pad(a.getSeconds());
  var time = hour + ':' + min;
  console.log("timeConverter: "+year+" "+month+" "+date+" "+hour+" "+min+" "+sec);

  /*
  var t = new Date(UNIX_timestamp*1000);
  var log = t.format("dd.mm.yyyy hh:MM:ss");
  console.log("timeConverter: "+log);
  var time = t.format("hh:MM");
  */
  return time;
}

function pad(input) {
    //var BASE = "00";
    //return input ? BASE.substr(0, 2 - Math.ceil(input / 10)) + input : BASE;
  if (input < 10){
    return ('0' + input);
  } else return input;
}



function getWeather() {

/*
///// debuggggg
		console.log("debug!!!!!!!!!!!!!!!!!!!!!");
  for(var i = 0, x = window.localStorage.length; i < x; i++) {
		var key = window.localStorage.key(i);
		var val = window.localStorage.getItem(key);
		
		if(val != null) {
			url += "&" + encodeURIComponent(key) + "=" + encodeURIComponent(val);
		}
		console.log(url);
	}
		m=localStorage.getItem("tzmidTXTColorPicker");	
		moo=localStorage.getItem(5);	
		console.log(m,moo);
		console.log("debug!!!!!!!!!!!!!!!!!!!!!");

///// debuggggg
*/


  if (window.localStorage.getItem("configuration")){
    console.log("read config start");
    configuration = JSON.parse(window.localStorage.configuration);
    //var test = JSON.parse(window.localStorage.configuration);
    console.log("loaded config = " + JSON.stringify(configuration, null, 2));
    console.log("read config finished");
  } else {
    console.log("error reading config from localStorage");
  }

  console.log("getWeather Begin");
  var options = {
    enableHighAccuracy: false,
    timeout: 5000,
    maximumAge: 0
  };
  navigator.geolocation.getCurrentPosition(    //could also use navigator.geolocation.watchPosition() ?
    locationSuccess,
    locationError,
    options
  );
  console.log("getWeather End");
}
//^thanks to pebble-mms

if (window.localStorage.getItem("HOUR_TXT_COLOR_KEY")){
  console.log('HOUR_TXT_COLOR_KEY' +HOUR_TXT_COLOR_KEY);
}


// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getWeather();
  }
);


// show Config screen
Pebble.addEventListener('showConfiguration', function() {
  var url = 'http://4ndyb.github.io/pebble-cram/';
  console.log('Showing configuration page: ' + url);
  Pebble.openURL(url);
});

// Close config screen
Pebble.addEventListener('webviewclosed', function(e) {
  // check it's big enough to parse
  if (e.response.length > 1) {
    var configData = JSON.parse(decodeURIComponent(e.response));
    var configuration_str = JSON.stringify(configData);
    console.log('Configuration page returned: ' + JSON.stringify(configData, null, 2));

    console.log("Saving config data to local storage...");
    if (configuration_str.charAt(0) == "{" && configuration_str.slice(-1) == "}" && configuration_str.length > 5) {
        window.localStorage.configuration = JSON.stringify(configuration);
    }

    var configdictionary = {
        "CONFIG_CITY_KEY": configData.cityweatherTXTtxtbox,
        "HOUR_TXT_COLOR_KEY": parseInt(configData.hourTXTColorPicker, 16),
        "HOUR_BG_COLOR_KEY": parseInt(configData.hourBGColorPicker, 16),
        "MIN_TXT_COLOR_KEY": parseInt(configData.minTXTColorPicker, 16),
        "MIN_BG_COLOR_KEY": parseInt(configData.minBGColorPicker, 16),
        "CALHDR_TXT_COLOR_KEY": parseInt(configData.calHDRTXTColorPicker, 16),
        "CALHDR_BG_COLOR_KEY": parseInt(configData.calHDRBGColorPicker, 16),
        "CAL_TXT_COLOR_KEY": parseInt(configData.calTXTColorPicker, 16),
        "CAL_BG_COLOR_KEY": parseInt(configData.calBGColorPicker, 16),
        "CALTODAY_TXT_COLOR_KEY": parseInt(configData.calTODAYTXTColorPicker, 16),
        "CALTODAY_BG_COLOR_KEY": parseInt(configData.calTODAYBGColorPicker, 16),
        "DATEBOX_TXT_COLOR_KEY": parseInt(configData.caldateboxTXTColorPicker, 16),
        "DATEBOX_BG_COLOR_KEY": parseInt(configData.caldateboxBGColorPicker, 16),
        "CALMONTHLABEL_TXT_COLOR_KEY": parseInt(configData.calmonthlabelTXTColorPicker, 16),
        "CALMONTHLABEL_BG_COLOR_KEY": parseInt(configData.calmonthlabelBGColorPicker, 16),
        "WEATHER_TXT_COLOR_KEY": parseInt(configData.weatherTXTColorPicker, 16),
        "WEATHER_BG_COLOR_KEY": parseInt(configData.weatherBGColorPicker, 16),
        "BLUETOOTH_TXT_COLOR_KEY": parseInt(configData.bluetoothTXTColorPicker, 16),
        "BLUETOOTH_BG_COLOR_KEY": parseInt(configData.bluetoothBGColorPicker, 16),
        "BATTERY_TXT_COLOR_KEY": parseInt(configData.batteryTXTColorPicker, 16),
        "BATTERY_BG_COLOR_KEY": parseInt(configData.batteryBGColorPicker, 16),
        "TZTOP_TXT_COLOR_KEY": parseInt(configData.tztopTXTColorPicker, 16),
        "TZTOP_BG_COLOR_KEY": parseInt(configData.tztopBGColorPicker, 16),
        "TZMID_TXT_COLOR_KEY": parseInt(configData.tzmidTXTColorPicker, 16),
        "TZMID_BG_COLOR_KEY": parseInt(configData.tzmidBGColorPicker, 16),
        "TZBOT_TXT_COLOR_KEY": parseInt(configData.tzbottomTXTColorPicker, 16),
        "TZBOT_BG_COLOR_KEY": parseInt(configData.tzbottomBGColorPicker, 16),
        "TZTOP_NUM_KEY": (parseInt(configData.tztopNUMtxtbox)*60*60),
        "TZTOP_TXT_KEY": configData.tztopTXTtxtbox,
        "TZMID_NUM_KEY": (parseInt(configData.tzmidNUMtxtbox)*60*60),
        "TZMID_TXT_KEY": configData.tzmidTXTtxtbox,
        "TZBOT_NUM_KEY": (parseInt(configData.tzbottomNUMtxtbox)*60*60),
        "TZBOT_TXT_KEY": configData.tzbottomTXTtxtbox
    }

    console.log("Sending Webpage Info to Pebble ...");
    Pebble.sendAppMessage(configdictionary,
         function(e) {
            console.log("Webpage info sent to Pebble successfully!");
                      },
          function(e) {
             console.log("Error sending webpage info to Pebble!");
                      }
    );
  }// end size check
});
