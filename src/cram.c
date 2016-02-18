#include <pebble.h>
#include <cram.h>
#include <config.h>

#define DEVICE_WIDTH        144
#define DEVICE_HEIGHT       168
#define QUADRANT1_TOP       0 // 72 tall
#define QUADRANT1_HEIGHT    84
#define QUADRANT2_TOP       0
#define QUADRANT2_HEIGHT    84
#define hour_layer_TOP       0  // 72 tall
#define hour_layer_HEIGHT   84
#define QUADRANT3_TOP       85  // 72 tall
#define QUADRANT3_HEIGHT    84
#define QUADRANT4_TOP       85  // 72 tall
#define QUADRANT4_HEIGHT    84

//#define LAYOUT_SLOT_BOT      96 // 72 tall, 4px gap above
#define QUADRANT_MIDDLE      72 //tall, 4px gap above
#define AK_MESSAGE_TYPE          99
#define AK_SEND_WATCH_VERSION   104
#define AK_SEND_CONFIG_VERSION  105
#define CONFIG_VERSION "1.0"
#define TIMEZONE_UNINITIALIZED 80

#define dow(y, m, d) \
	((((((m) + 9) % 12 + 1) << 4) % 27 + (d) + 1 + \
	  ((y) % 400 + 400) + ((y) % 400 + 400) / 4 - ((y) % 400 + 400) / 100 +	\
	  (((m) <= 2) ? ( \
		   (((((y) % 4) == 0) && (((y) % 100) != 0)) || (((y) % 400) == 0)) \
		   ? 5 : 6) : 0)) % 7)

static bool initDone; // e.g. for avoiding "no BT" vibration with initial opening of the watchface

// colors
GColor hr_txt_color;
GColor hr_bg_color;
GColor min_bg_color;
GColor min_txt_color;
GColor caltoday_txt_color;
GColor caltoday_bg_color;
GColor datebox_txt_color;
GColor datebox_bg_color;
GColor weather_txt_color;
GColor weather_bg_color;
GColor calhdr_txt_color;
GColor calhdr_bg_color;
GColor cal_txt_color;
GColor cal_bg_color;
GColor caltoday_txt_color;
GColor caltoday_bg_color;
GColor tz1_txt_color;
GColor tz1_bg_color;
GColor tz2_txt_color;
GColor tz2_bg_color;
GColor tz3_txt_color;
GColor tz3_bg_color;
GColor calmonthlabel_txt_color;
GColor calmonthlabel_bg_color;
GColor bluetooth_txt_color;
GColor bluetooth_bg_color;
GColor battery_txt_color;
GColor battery_bg_color;

enum MessageKey {
	WEATHER_ICON_KEY = 0,           // TUPLE_INT
	WEATHER_TEMPERATURE_KEY = 1,    // TUPLE_CSTRING
	WEATHER_CITY_KEY = 2,           // TUPLE_CSTRING
	WEATHER_SUNRISE_KEY = 3,
	WEATHER_SUNSET_KEY = 4,
	WEATHER_CONDITIONS_KEY = 5,
	WEATHER_HUMIDITY_KEY = 7,
	WEATHER_WINDSPEED_KEY = 8,
	WEATHER_WINDDEG_KEY = 9,
	WEATHER_TEMPMIN_KEY = 10,
	WEATHER_TEMPMAX_KEY = 11,
	WEATHER_WINDREPORT_KEY = 12,
	WEATHER_TEMPMINMAX_KEY = 13,
	WEATHER_PRESSURE_KEY = 14,
	WEATHER_TEMP_6H_KEY = 15,
	WEATHER_TEMP_12H_KEY = 16,
	WEATHER_TEMP_18H_KEY = 17,
	WEATHER_TEMP_24H_KEY = 18,
	WEATHER_SUN_RISE_UNIX_KEY = 19,
	WEATHER_SUN_SET_UNIX_KEY = 20,
	WEATHER_DATA_TIME_KEY = 21,
	WEATHER_TIME_UTC_OFFSET_KEY = 22,
	WEATHER_LOCATION_NAME_KEY = 23,
	WEATHER_TIME_6H_KEY = 24,
	WEATHER_TIME_12H_KEY = 25,
	WEATHER_TIME_18H_KEY = 26,
	WEATHER_TIME_24H_KEY = 27,
	BACKGROUND_COLOUR_KEY = 100,
	HOUR_TXT_COLOR_KEY = 102,
	HOUR_BG_COLOR_KEY = 103,
	MIN_TXT_COLOR_KEY = 104,
	MIN_BG_COLOR_KEY = 105,
	CALHDR_TXT_COLOR_KEY = 106,
	CALHDR_BG_COLOR_KEY = 107,
	CAL_TXT_COLOR_KEY = 108,
	CAL_BG_COLOR_KEY = 109,
	CALTODAY_TXT_COLOR_KEY = 110,
	CALTODAY_BG_COLOR_KEY = 111,
	DATEBOX_TXT_COLOR_KEY = 112,
	DATEBOX_BG_COLOR_KEY = 113,
	TZTOP_TXT_COLOR_KEY = 114,
	TZTOP_BG_COLOR_KEY = 115,
	TZMID_TXT_COLOR_KEY = 116,
	TZMID_BG_COLOR_KEY = 117,
	TZBOT_TXT_COLOR_KEY = 118,
	TZBOT_BG_COLOR_KEY = 119,
	CALMONTHLABEL_TXT_COLOR_KEY = 120,
	CALMONTHLABEL_BG_COLOR_KEY = 121,
	WEATHER_TXT_COLOR_KEY = 122,
	WEATHER_BG_COLOR_KEY = 123,
	BLUETOOTH_TXT_COLOR_KEY = 124,
	BLUETOOTH_BG_COLOR_KEY = 125,
	BATTERY_TXT_COLOR_KEY = 126,
	BATTERY_BG_COLOR_KEY = 127,
	TZTOP_NUM_KEY = 203,
	TZTOP_TXT_KEY = 204,
	TZMID_NUM_KEY = 205,
	TZMID_TXT_KEY = 206,
	TZBOT_NUM_KEY = 207,
	TZBOT_TXT_KEY = 208,
	CONFIG_CITY_KEY = 209,
	CONFIG_DEGREE_F_KEY = 210
};

// Timezones
static char tz1_txt[20];
static char tz2_txt[20];
static char tz3_txt[20];
int tz1_num;
int tz2_num;
int tz3_num;
static TextLayer *tz_txt_layer1;
static TextLayer *tz_txt_layer2;
static TextLayer *tz_txt_layer3;

static Window *window;
static TextLayer *bluetooth_layer, *battery_layer;
struct tm *currentTime;
static GFont calendar_font;
static GFont tiny_font8;
static GFont conditions_font;

static TextLayer *hour_layer;
static TextLayer *minute_layer;
static TextLayer *weathercity_layer;
static Layer *calendar_layer;
static Layer *h24weather_layer;
static TextLayer *s_temperature_layer;
static TextLayer *s_conditions_layer;
static TextLayer *s_weather_humidity_layer;
static TextLayer *s_temperature_minmax_layer;
static TextLayer *s_weather_windspeed_layer;
static TextLayer *s_weather_pressure_layer;
static TextLayer *monthtext_layer;
static TextLayer *yeardate_layer;
static TextLayer *s_updatetime_layer;

AppTimer *bottom_toggle = NULL;

// Weather and other global variables:
static int weather_TEMP        = 0;                             //in degree C
static char weather6H[6];
static char weather12H[6];
static char weather18H[6];
static char weather24H[6];
static char time6H[6];
static char time12H[6];
static char time18H[6];
static char time24H[6];
static char weathercity[32];
static char weatherminmax_TEMP[32];                             // temp min max
static int weatherupdatetime = 0;                               // temp min max
static char weatherhumidity[32];                                // temp min max
static char weatherpressure[32];                                // temp min max
static char weatherwindspeed[32];                               // temp min max
static int WeatherIcon         = (int)'I';                      //sun
//static char sun_rise[10] = "--:--";
//static char sun_set[10] = "--:--";
//static time_t sun_rise_unix_loc = 0;
//static time_t sun_set_unix_loc  = 0;

static void get_timezones()
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: get_timezones");
	static char buffer1[20];
	static char buffer2[20];
	static char buffer3[20];

	static char utc_time_text1[] = "00:00";
	static char utc_time_text2[] = "00:00";
	static char utc_time_text3[] = "00:00";

	time_t utctime1 = (time(NULL));
	time_t utctime2 = (time(NULL));
	time_t utctime3 = (time(NULL));
	utctime1 += tz1_num;
	utctime2 += tz2_num;
	utctime3 += tz3_num;

	struct tm *T1 = gmtime(&utctime1);
	strftime(utc_time_text1, sizeof(utc_time_text1), "%R:%S", T1);
	snprintf(buffer1, sizeof(buffer1), "%s\n%s", tz1_txt, utc_time_text1);

	struct tm *T2 = gmtime(&utctime2);
	strftime(utc_time_text2, sizeof(utc_time_text2), "%R:%S", T2);
	snprintf(buffer2, sizeof(buffer2), "%s\n%s", tz2_txt, utc_time_text2);

	struct tm *T3 = gmtime(&utctime3);
	strftime(utc_time_text3, sizeof(utc_time_text3), "%R:%S", T3);
	snprintf(buffer3, sizeof(buffer3), "%s\n%s", tz3_txt, utc_time_text3);

	text_layer_set_text(tz_txt_layer1, buffer1);
	text_layer_set_text(tz_txt_layer2, buffer2);
	text_layer_set_text(tz_txt_layer3, buffer3);
}

void setCalDayColors(GContext* ctx)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: setCalDayColors");
	graphics_context_set_stroke_color(ctx, calhdr_bg_color);
	graphics_context_set_fill_color(ctx, calhdr_bg_color);
	graphics_context_set_text_color(ctx, calhdr_txt_color);
}

void setCalColors(GContext* ctx)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: setCalColors");
	graphics_context_set_stroke_color(ctx, cal_bg_color);
	graphics_context_set_fill_color(ctx, cal_bg_color);
	graphics_context_set_text_color(ctx, cal_txt_color);
}

void setCalCurColors(GContext* ctx)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: setCalCurColors");
	graphics_context_set_stroke_color(ctx, caltoday_bg_color);
	graphics_context_set_fill_color(ctx, caltoday_bg_color);
	graphics_context_set_text_color(ctx, caltoday_txt_color);
}

void bluetooth_handler(bool connected)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: bluetooth_handler");
	if (connected)
		//text_layer_set_text(bluetooth_layer, "OK");
		text_layer_set_text(bluetooth_layer, "\U0001F603");
	else
		text_layer_set_text(bluetooth_layer, "\U0001F625");
}

void battery_handler(BatteryChargeState state)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: battery_handler");
	static char battery_format[] = "100% and charging";
	static char *battery_bools[] = {
		"", "+"
	};
	int percent = state.charge_percent;
	snprintf(battery_format, sizeof(battery_format), "[%d%%%s]", percent, battery_bools[state.is_charging]);
	text_layer_set_text(battery_layer, battery_format);
}

///////////////////////////////////////////////////////////////
// How many days are/were in the month
int daysInMonth(int mon, int year)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: daysInMonth");
	//mon++; // dec = 0|12, lazily optimized
	//jan = 1, dec = 12
	// April, June, September and November have 30 Days
	if (mon == 4 || mon == 6 || mon == 9 || mon == 11)
		return 30;
	else if (mon == 2) {
		// Deal with Feburary & Leap years
		if (year % 400 == 0)
			return 29;
		else if (year % 100 == 0)
			return 28;
		else if (year % 4 == 0)
			return 29;
		else
			return 28;
	} else
		// Most months have 31 days
		return 31;
}

static void inbox_dropped_callback(AppMessageResult reason, void *context)
{
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context)
{
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context)
{
	APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void h24weather_layer_update_callback(Layer *me, GContext* ctx)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: h24weather_layer_update_callback");

    #define WEATHER_WIDTH  27           // width of columns
    #define WEATHER_GAP    0.5          // gap around calendar
    #define WEATHER_LEFT   0            // left side of calendar
    #define WEATHER_HEIGHT 12           // How tall rows should be depends on how many weeks there are
    #define WEATHER_LABEL_WIDTH  15
	int column  = 0;
	int label_vert_offset = -4;
	int label_horiz_offset = 0;
	int data_vert_offset = -4;
	int data_horiz_offset = 0;

	char *day_lables[] =
	{
		time6H,
		time12H,
		time18H,
		time24H
	};
	char *weather_lables[] =
	{
		weather6H,
		weather12H,
		weather18H,
		weather24H
	};

	setCalColors(ctx);
	char label_text[6];
	char cell_text[6];

	for ( int cell = 0; cell <= 3; cell++) {
		snprintf(label_text, sizeof(label_text), "%s", day_lables[cell]);
		snprintf(cell_text, sizeof(cell_text), "%s", weather_lables[cell]);
		// Header
		setCalDayColors(ctx);
		graphics_fill_rect(ctx, GRect( WEATHER_LEFT + WEATHER_GAP, WEATHER_HEIGHT * column, WEATHER_LABEL_WIDTH - WEATHER_GAP, WEATHER_HEIGHT - WEATHER_GAP), 0, GCornerNone);
		graphics_draw_text(ctx, label_text, fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(label_horiz_offset + WEATHER_LEFT + WEATHER_GAP, WEATHER_HEIGHT * column + label_vert_offset, WEATHER_LABEL_WIDTH, WEATHER_HEIGHT), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		// Data
		setCalColors(ctx);
		graphics_fill_rect(ctx, GRect( WEATHER_LABEL_WIDTH + WEATHER_LEFT + WEATHER_GAP, WEATHER_HEIGHT * column, WEATHER_WIDTH, WEATHER_HEIGHT - WEATHER_GAP), 0, GCornerNone);
		graphics_draw_text(ctx, cell_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(WEATHER_LABEL_WIDTH + data_horiz_offset + WEATHER_LEFT + WEATHER_GAP, WEATHER_HEIGHT * column + data_vert_offset, WEATHER_WIDTH, WEATHER_HEIGHT), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		column++;
	}
}

void DisplayWeather(void)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: DisplayWeather");
	static char weather_icon[] = "I";
	//int WeatherIcon=36;
	static char buffer_1[12];
	static char buffer_2[12];
	time_t timestamp = (time(NULL));
	static char weatherupdatetime_text[] = "00-00:00";

	//update 24h time
	layer_set_update_proc(h24weather_layer, h24weather_layer_update_callback);

	// work out weather timestamp
	weatherupdatetime = persist_read_int(WEATHER_DATA_TIME_KEY);
	timestamp = weatherupdatetime;
	struct tm *T = gmtime(&timestamp);
	strftime(weatherupdatetime_text, sizeof(weatherupdatetime_text), "%d-%R", T);
	snprintf(buffer_2, sizeof(buffer_2), "%s", weatherupdatetime_text);

	// TODO check if these exist
	weather_TEMP = persist_read_int(WEATHER_TEMPERATURE_KEY);
	persist_read_string(WEATHER_TEMPMINMAX_KEY, weatherminmax_TEMP, sizeof(weatherminmax_TEMP));
	persist_read_string(WEATHER_HUMIDITY_KEY, weatherhumidity, sizeof(weatherhumidity));
	persist_read_string(WEATHER_WINDSPEED_KEY, weatherwindspeed, sizeof(weatherwindspeed));
	persist_read_string(WEATHER_PRESSURE_KEY, weatherpressure, sizeof(weatherpressure));
	persist_read_string(WEATHER_LOCATION_NAME_KEY, weathercity, sizeof(weathercity));

	bool degree_f = false;
	if (degree_f)
		snprintf(buffer_1, sizeof(buffer_1), "%d°F", (int)(weather_TEMP * 1.8 + 32));
	else
		snprintf(buffer_1, sizeof(buffer_1), "%d°C", weather_TEMP);
	text_layer_set_text(s_temperature_layer, buffer_1);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Trying to set text %s", weatherminmax_TEMP );
	text_layer_set_text(s_temperature_minmax_layer, weatherminmax_TEMP );
	text_layer_set_text(s_updatetime_layer, buffer_2 );
	text_layer_set_text(weathercity_layer, weathercity );
	//text_layer_set_text(s_weather_humidity_layer, weatherhumidity );
	//text_layer_set_text(s_weather_pressure_layer, weatherpressure );
	//text_layer_set_text(s_weather_windspeed_layer, weatherwindspeed );

	weather_icon[0] = (unsigned char)WeatherIcon; //wi_counter;
	text_layer_set_text(s_conditions_layer, weather_icon);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: inbox_received_callback");
	int temperature_buffer;
	int conditions_buffer;
	static char city_buffer[32];
	static char windspeed_buffer[6];
	static char winddeg_buffer[6];
	static char tempmin_buffer[6];
	static char tempmax_buffer[6];
	static char tempminmax_buffer[15];
	static char windreport_buffer[15];
	static char humidity_buffer[15];
	static char pressure_buffer[15];
	static char weather6H_buffer[6];
	static char weather12H_buffer[6];
	static char weather18H_buffer[6];
	static char weather24H_buffer[6];
	static char weathercity_buffer[32];
	static char time6H_buffer[6];
	static char time12H_buffer[6];
	static char time18H_buffer[6];
	static char time24H_buffer[6];

	static char config_city_txt_buffer[32];

	int hr_txt_color;
	int hr_bg_color;
	int min_bg_color;
	int min_txt_color;
	int calhdr_txt_color;
	int calhdr_bg_color;
	int cal_txt_color;
	int cal_bg_color;
	int caltoday_txt_color;
	int caltoday_bg_color;
	int datebox_txt_color;
	int datebox_bg_color;
	int tz1_txt_color;
	int tz1_bg_color;
	int tz2_txt_color;
	int tz2_bg_color;
	int tz3_txt_color;
	int tz3_bg_color;
	int calmonthlabel_txt_color;
	int calmonthlabel_bg_color;
	int weather_txt_color;
	int weather_bg_color;
	int bluetooth_txt_color;
	int bluetooth_bg_color;
	int battery_txt_color;
	int battery_bg_color;
	int weatherupdatetime_buffer;

	int tzleft_num_buffer;
	int tzmid_num_buffer;
	int tzright_num_buffer;
	static char tzleft_txt_buffer[64];
	static char tzmid_txt_buffer[64];
	static char tzright_txt_buffer[64];

	Tuple *t = dict_read_first(iterator);
	bool weatherchanged = false;
	// For all items
	while (t != NULL) {
		// Which key was received?
		switch (t->key) {
		case WEATHER_TEMPERATURE_KEY:
			temperature_buffer = (int)t->value->int32;
			persist_write_int(WEATHER_TEMPERATURE_KEY, temperature_buffer);
			weatherchanged = true;
			break;
		case WEATHER_CONDITIONS_KEY:
			conditions_buffer = (int)t->value->int32;
			persist_write_int(WEATHER_CONDITIONS_KEY, conditions_buffer);
			break;
		case WEATHER_CITY_KEY:
			snprintf(city_buffer, sizeof(city_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_CITY_KEY, city_buffer);
			break;
		case WEATHER_HUMIDITY_KEY:
			snprintf(humidity_buffer, sizeof(humidity_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_HUMIDITY_KEY, humidity_buffer);
			break;
		case WEATHER_PRESSURE_KEY:
			snprintf(pressure_buffer, sizeof(pressure_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_PRESSURE_KEY, pressure_buffer);
			break;
		case WEATHER_WINDDEG_KEY:
			snprintf(winddeg_buffer, sizeof(winddeg_buffer), "%d", (int)t->value->int32);
			persist_write_string(WEATHER_WINDDEG_KEY, winddeg_buffer);
			break;
		case WEATHER_DATA_TIME_KEY:
			weatherupdatetime_buffer = (int)t->value->int32;
			persist_write_int(WEATHER_DATA_TIME_KEY, weatherupdatetime_buffer);
			break;
		case WEATHER_WINDREPORT_KEY:
			snprintf(windreport_buffer, sizeof(windreport_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_WINDDEG_KEY, windreport_buffer);
			break;
		case WEATHER_WINDSPEED_KEY:
			snprintf(windspeed_buffer, sizeof(windspeed_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_WINDSPEED_KEY, windspeed_buffer);
			break;
		case WEATHER_TEMPMIN_KEY:
			snprintf(tempmin_buffer, sizeof(tempmin_buffer), "%d", (int)t->value->int32);
			persist_write_string(WEATHER_TEMPMIN_KEY, tempmin_buffer);
			break;
		case WEATHER_TEMPMAX_KEY:
			snprintf(tempmax_buffer, sizeof(tempmax_buffer), "%d", (int)t->value->int32);
			persist_write_string(WEATHER_TEMPMAX_KEY, tempmax_buffer);
			break;
		case WEATHER_TEMPMINMAX_KEY:
			snprintf(tempminmax_buffer, sizeof(tempminmax_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_TEMPMINMAX_KEY, tempminmax_buffer);
			break;
		case HOUR_TXT_COLOR_KEY:
			hr_txt_color = t->value->int32;
			persist_write_int(HOUR_TXT_COLOR_KEY, hr_txt_color);
			break;
		case WEATHER_ICON_KEY:
			WeatherIcon = t->value->int32;
			persist_write_int(WEATHER_ICON_KEY, WeatherIcon);
			break;
		case WEATHER_TEMP_6H_KEY:
			snprintf(weather6H_buffer, sizeof(weather6H_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_TEMP_6H_KEY, weather6H_buffer );
			break;
		case WEATHER_TEMP_12H_KEY:
			snprintf(weather12H_buffer, sizeof(weather12H_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_TEMP_12H_KEY, weather12H_buffer);
			break;
		case WEATHER_TEMP_18H_KEY:
			snprintf(weather18H_buffer, sizeof(weather18H_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_TEMP_18H_KEY, weather18H_buffer);
			break;
		case WEATHER_TEMP_24H_KEY:
			snprintf(weather24H_buffer, sizeof(weather24H_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_TEMP_24H_KEY, weather24H_buffer);
			break;
		case WEATHER_TIME_6H_KEY:
			snprintf(time6H_buffer, sizeof(time6H_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_TIME_6H_KEY, time6H_buffer );
			break;
		case WEATHER_TIME_12H_KEY:
			snprintf(time12H_buffer, sizeof(time12H_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_TIME_12H_KEY, time12H_buffer );
			break;
		case WEATHER_TIME_18H_KEY:
			snprintf(time18H_buffer, sizeof(time18H_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_TIME_18H_KEY, time18H_buffer );
			break;
		case WEATHER_TIME_24H_KEY:
			snprintf(time24H_buffer, sizeof(time24H_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_TIME_24H_KEY, time24H_buffer );
			break;
		case WEATHER_LOCATION_NAME_KEY:
			snprintf(weathercity_buffer, sizeof(weathercity_buffer), "%s", t->value->cstring);
			persist_write_string(WEATHER_LOCATION_NAME_KEY, weathercity_buffer);
			break;
		case HOUR_BG_COLOR_KEY:
			hr_bg_color = t->value->int32;
			persist_write_int(HOUR_BG_COLOR_KEY, hr_bg_color);
			break;
		case MIN_TXT_COLOR_KEY:
			min_txt_color = t->value->int32;
			persist_write_int(MIN_TXT_COLOR_KEY, min_txt_color);
			break;
		case MIN_BG_COLOR_KEY:
			min_bg_color = t->value->int32;
			persist_write_int(MIN_BG_COLOR_KEY, min_bg_color);
			break;
		case CALHDR_TXT_COLOR_KEY:
			calhdr_txt_color = t->value->int32;
			persist_write_int(CALHDR_TXT_COLOR_KEY, calhdr_txt_color);
			break;
		case CALHDR_BG_COLOR_KEY:
			calhdr_bg_color = t->value->int32;
			persist_write_int(CALHDR_BG_COLOR_KEY, calhdr_bg_color);
			break;
		case CALMONTHLABEL_TXT_COLOR_KEY:
			calmonthlabel_txt_color = t->value->int32;
			persist_write_int(CALMONTHLABEL_TXT_COLOR_KEY, calmonthlabel_txt_color);
			break;
		case CALMONTHLABEL_BG_COLOR_KEY:
			calmonthlabel_bg_color = t->value->int32;
			persist_write_int(CALMONTHLABEL_BG_COLOR_KEY, calmonthlabel_bg_color);
			break;
		case CAL_TXT_COLOR_KEY:
			cal_txt_color = t->value->int32;
			persist_write_int(CAL_TXT_COLOR_KEY, cal_txt_color);
			break;
		case CAL_BG_COLOR_KEY:
			cal_bg_color = t->value->int32;
			persist_write_int(CAL_BG_COLOR_KEY, cal_bg_color);
			break;
		case CALTODAY_TXT_COLOR_KEY:
			caltoday_txt_color = t->value->int32;
			persist_write_int(CALTODAY_TXT_COLOR_KEY, caltoday_txt_color);
			break;
		case CALTODAY_BG_COLOR_KEY:
			caltoday_bg_color = t->value->int32;
			persist_write_int(CALTODAY_BG_COLOR_KEY, caltoday_bg_color);
			break;
		case DATEBOX_TXT_COLOR_KEY:
			datebox_txt_color = t->value->int32;
			persist_write_int(DATEBOX_TXT_COLOR_KEY, datebox_txt_color);
			break;
		case DATEBOX_BG_COLOR_KEY:
			datebox_bg_color = t->value->int32;
			persist_write_int(DATEBOX_BG_COLOR_KEY, datebox_bg_color);
			break;
		case TZTOP_TXT_COLOR_KEY:
			tz1_txt_color = t->value->int32;
			persist_write_int(TZTOP_TXT_COLOR_KEY, tz1_txt_color);
			break;
		case TZTOP_BG_COLOR_KEY:
			tz1_bg_color = t->value->int32;
			persist_write_int(TZTOP_BG_COLOR_KEY, tz1_bg_color);
			break;
		case TZMID_TXT_COLOR_KEY:
			tz2_txt_color = t->value->int32;
			persist_write_int(TZMID_TXT_COLOR_KEY, tz2_txt_color);
			break;
		case TZMID_BG_COLOR_KEY:
			tz2_bg_color = t->value->int32;
			persist_write_int(TZMID_BG_COLOR_KEY, tz2_bg_color);
			break;
		case TZBOT_TXT_COLOR_KEY:
			tz3_txt_color = t->value->int32;
			persist_write_int(TZBOT_TXT_COLOR_KEY, tz3_txt_color);
			break;
		case TZBOT_BG_COLOR_KEY:
			tz3_bg_color = t->value->int32;
			persist_write_int(TZBOT_BG_COLOR_KEY, tz3_bg_color);
			break;
		case TZTOP_NUM_KEY:
			tzleft_num_buffer = t->value->int32;
			persist_write_int(TZTOP_NUM_KEY, tzleft_num_buffer);
			APP_LOG(APP_LOG_LEVEL_INFO, "tz left num: %d ", (int)t->value->int32);
			break;
		case TZTOP_TXT_KEY:
			snprintf(tzleft_txt_buffer, sizeof(tzleft_txt_buffer), "%s", t->value->cstring);
			persist_write_string(TZTOP_TXT_KEY, tzleft_txt_buffer);
			APP_LOG(APP_LOG_LEVEL_INFO, "tz left txt: %s ", t->value->cstring);
			break;
		case TZMID_NUM_KEY:
			tzmid_num_buffer = t->value->int32;
			persist_write_int(TZMID_NUM_KEY, tzmid_num_buffer);
			APP_LOG(APP_LOG_LEVEL_INFO, "tz mid num: %d ", (int)t->value->int32);
			break;
		case TZMID_TXT_KEY:
			snprintf(tzmid_txt_buffer, sizeof(tzmid_txt_buffer), "%s", t->value->cstring);
			persist_write_string(TZMID_TXT_KEY, tzmid_txt_buffer);
			APP_LOG(APP_LOG_LEVEL_INFO, "tz mid txt: %s ", t->value->cstring);
			break;
		case TZBOT_NUM_KEY:
			tzright_num_buffer = t->value->int32;
			persist_write_int(TZBOT_NUM_KEY, tzright_num_buffer);
			APP_LOG(APP_LOG_LEVEL_INFO, "tz right num: %d ", (int)t->value->int32);
			break;
		case TZBOT_TXT_KEY:
			snprintf(tzright_txt_buffer, sizeof(tzright_txt_buffer), "%s", t->value->cstring);
			persist_write_string(TZBOT_TXT_KEY, tzright_txt_buffer);
			APP_LOG(APP_LOG_LEVEL_INFO, "tz right txt: %s ", t->value->cstring);
			break;
		case WEATHER_TXT_COLOR_KEY:
			weather_txt_color = t->value->int32;
			persist_write_int(WEATHER_TXT_COLOR_KEY, weather_txt_color);
			break;
		case WEATHER_BG_COLOR_KEY:
			weather_bg_color = t->value->int32;
			persist_write_int(WEATHER_BG_COLOR_KEY, weather_bg_color);
			break;
		case BLUETOOTH_TXT_COLOR_KEY:
			bluetooth_txt_color = t->value->int32;
			persist_write_int(BLUETOOTH_TXT_COLOR_KEY, bluetooth_txt_color);
			break;
		case BLUETOOTH_BG_COLOR_KEY:
			bluetooth_bg_color = t->value->int32;
			persist_write_int(BLUETOOTH_BG_COLOR_KEY, bluetooth_bg_color);
			break;
		case BATTERY_TXT_COLOR_KEY:
			battery_txt_color = t->value->int32;
			persist_write_int(BATTERY_TXT_COLOR_KEY, battery_txt_color);
			break;
		case BATTERY_BG_COLOR_KEY:
			battery_bg_color = t->value->int32;
			persist_write_int(BATTERY_BG_COLOR_KEY, battery_bg_color);
			break;
		case CONFIG_CITY_KEY:
			snprintf(config_city_txt_buffer, sizeof(config_city_txt_buffer), "%s", t->value->cstring);
			persist_write_string(CONFIG_CITY_KEY, config_city_txt_buffer);
			APP_LOG(APP_LOG_LEVEL_INFO, "config city txt: %s ", t->value->cstring);
			break;

		default:
			APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
			break;
		}
		// Look for next item
		t = dict_read_next(iterator);
	}

	if (weatherchanged) DisplayWeather();
}

void calendar_layer_update_callback(Layer *me, GContext* ctx)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: calendar_layer_update_callback");

    #define CAL_DAYS   7        // number of columns (days of the week)
    #define CAL_WIDTH  15       // width of columns
    #define CAL_GAP    1        // gap around calendar
    #define CAL_LEFT   0        // left side of calendar
    #define CAL_HEIGHT 12       // How tall rows should be depends on how many weeks there are
    #define LABEL_HEIGHT  10    // How high is the day label

	int week   =  0;
	int column  = 0;
	int font_vert_offset = 1;
	int font_horiz_offset = 1;
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	int month = tick_time->tm_mon + 1;
	int year = tick_time->tm_year + 1900;
	int day = tick_time->tm_mday;
	int daycode = dow(year, month, 1);

	char *day_lables[] =
	{
		"Su",
		"Mo",
		"Tu",
		"We",
		"Th",
		"Fr",
		"Sa"
	};

	APP_LOG(APP_LOG_LEVEL_DEBUG, "daycode: %d, year: %d, month: %d, day: %d", daycode, year, month, day);

	setCalDayColors(ctx);
	//Draw day labels
	for ( int label = 0; label <= 6; label++ ) {
		graphics_fill_rect(ctx, GRect(CAL_WIDTH * label + CAL_LEFT + CAL_GAP, 0, CAL_WIDTH - CAL_GAP, LABEL_HEIGHT - CAL_GAP), 0, GCornerNone);
		char label_text[3];
		snprintf(label_text, sizeof(label_text), "%s", day_lables[label]);
		graphics_draw_text(ctx, label_text, tiny_font8, GRect(CAL_WIDTH * label + CAL_LEFT, 0 - CAL_GAP + font_vert_offset, CAL_WIDTH, LABEL_HEIGHT), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	}

	setCalColors(ctx);
	//Draw blank labels
	for ( int d = 0; d < daycode; d++ ) {
		graphics_fill_rect(ctx, GRect(CAL_WIDTH * d + CAL_LEFT + CAL_GAP, CAL_HEIGHT * week + LABEL_HEIGHT, CAL_WIDTH - CAL_GAP, CAL_HEIGHT - CAL_GAP), 0, GCornerNone);
		char blank[3];
		snprintf(blank, sizeof(blank), "%s", " ");
		graphics_draw_text(ctx, blank, calendar_font, GRect(CAL_WIDTH * d + CAL_LEFT, CAL_HEIGHT * week - CAL_GAP + font_vert_offset + LABEL_HEIGHT, CAL_WIDTH, CAL_HEIGHT), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	}
	//Draw days
	int row = daycode;
	int dim = daysInMonth(month, year );
	for ( int d = 1; d <= dim; d++) {
		if (d == day)
			setCalCurColors(ctx);
		else
			setCalColors(ctx);
		graphics_fill_rect(ctx, GRect(CAL_WIDTH * row + CAL_LEFT + CAL_GAP, CAL_HEIGHT * column + LABEL_HEIGHT, CAL_WIDTH - CAL_GAP, CAL_HEIGHT - CAL_GAP), 0, GCornerNone);
		char day[3];
		snprintf(day, sizeof(day), "%d", d);
		graphics_draw_text(ctx, day, calendar_font, GRect(CAL_WIDTH * row + CAL_LEFT + font_horiz_offset, CAL_HEIGHT * column - CAL_GAP + font_vert_offset + LABEL_HEIGHT, CAL_WIDTH, CAL_HEIGHT), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

		if ( ( row / 6 ) == 1 ) {
			column++;
			row = 0;
		}else
			row++;
	}
}

static void day_handler(struct tm *t, TimeUnits units_changed)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: day_handler");
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char buffer_monthtext[] = "xxxxxxxxxx";
	static char buffer_yeardate[] = "00/00/0000  ";
	strftime(buffer_yeardate, sizeof("00/00/0000  "), "%d/%m/%Y  ", tick_time);
	strftime(buffer_monthtext, sizeof("xxxxxxxxxx"), "%B", tick_time);
	text_layer_set_text(monthtext_layer, buffer_monthtext);
	text_layer_set_text(yeardate_layer, buffer_yeardate);
	layer_set_update_proc(calendar_layer, calendar_layer_update_callback);
}

static void doWeather()
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: doWeather");
	//TODO Request weather data at custome time
	//if (((int)time_since_last_update >= (WeatherUpdateInterval*60-60))) {
	if (initDone && bluetooth_connection_service_peek()) {
		// Begin dictionary
		DictionaryIterator *iter;
		app_message_outbox_begin(&iter);
		// Add a key-value pair
		dict_write_uint8(iter, 0, 0);
		// Send the message!
		app_message_outbox_send();
		APP_LOG(APP_LOG_LEVEL_INFO, "Weather Update requested");
	}
	//}
}

static void hour_handler(struct tm *t, TimeUnits units_changed)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: hour_handler");
	doWeather();
}

static void minute_handler(struct tm *t, TimeUnits units_changed)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: minute_handler");
	get_timezones();
	// Get a tm structure
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char buffer[] = "00";
	static char buffer_minute[] = "00";

	// Write the current hours and minutes into the buffer
	if (clock_is_24h_style() == true)
		//Use 24h hour format
		strftime(buffer, sizeof("00"), "%H", tick_time);
	else
		//Use 12 hour format
		strftime(buffer, sizeof("00"), "%I", tick_time);
	strftime(buffer_minute, sizeof("00"), "%M", tick_time);
	// Display this time on the TextLayer
	text_layer_set_text(hour_layer, buffer);
	text_layer_set_text(minute_layer, buffer_minute);
	//doWeather();
}

void handle_tick(struct tm *tick_time, TimeUnits units_changed)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Function: handle_tick");
	if ((units_changed & MINUTE_UNIT) != 0 )
		minute_handler(tick_time, units_changed);
	if ((units_changed & HOUR_UNIT) != 0 )
		hour_handler(tick_time, units_changed);
	if ((units_changed & DAY_UNIT) != 0 )
		day_handler(tick_time, units_changed);
}

static void window_load(Window *window)
{
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	APP_LOG(APP_LOG_LEVEL_DEBUG, "starting the window_load");
	initDone = false;

	//define our image resource
	calendar_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CAL_8));
	tiny_font8 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TINY_8));
	conditions_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CLIMACONS_30));

   #ifdef PBL_COLOR
	hr_txt_color = persist_exists(HOUR_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(HOUR_TXT_COLOR_KEY)) : GColorWhite;
	hr_bg_color = persist_exists(HOUR_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(HOUR_BG_COLOR_KEY)) : GColorDukeBlue;
	min_txt_color = persist_exists(MIN_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(MIN_TXT_COLOR_KEY)) : GColorWhite;
	min_bg_color = persist_exists(MIN_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(MIN_BG_COLOR_KEY)) : GColorDukeBlue;
	calhdr_txt_color = persist_exists(CALHDR_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(CALHDR_TXT_COLOR_KEY)) : GColorWhite;
	calhdr_bg_color = persist_exists(CALHDR_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(CALHDR_BG_COLOR_KEY)) : GColorDukeBlue;
	cal_txt_color = persist_exists(CAL_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(CAL_TXT_COLOR_KEY)) : GColorWhite;
	cal_bg_color = persist_exists(CAL_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(CAL_BG_COLOR_KEY)) : GColorBlue;
	caltoday_txt_color = persist_exists(CALTODAY_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(CALTODAY_TXT_COLOR_KEY)) : GColorWhite;
	caltoday_bg_color = persist_exists(CALTODAY_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(CALTODAY_BG_COLOR_KEY)) : GColorRed;
	datebox_txt_color = persist_exists(DATEBOX_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(DATEBOX_TXT_COLOR_KEY)) : GColorWhite;
	datebox_bg_color = persist_exists(DATEBOX_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(DATEBOX_BG_COLOR_KEY)) : GColorDukeBlue;
	tz1_txt_color = persist_exists(TZTOP_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(TZTOP_TXT_COLOR_KEY)) : GColorWhite;
	tz1_bg_color = persist_exists(TZTOP_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(TZTOP_BG_COLOR_KEY)) : GColorDukeBlue;
	tz2_txt_color = persist_exists(TZMID_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(TZMID_TXT_COLOR_KEY)) : GColorWhite;
	tz2_bg_color = persist_exists(TZMID_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(TZMID_BG_COLOR_KEY)) : GColorDukeBlue;
	tz3_txt_color = persist_exists(TZBOT_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(TZBOT_TXT_COLOR_KEY)) : GColorWhite;
	tz3_bg_color = persist_exists(TZBOT_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(TZBOT_BG_COLOR_KEY)) : GColorDukeBlue;
	calmonthlabel_txt_color = persist_exists(CALMONTHLABEL_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(CALMONTHLABEL_TXT_COLOR_KEY)) : GColorWhite;
	calmonthlabel_bg_color = persist_exists(CALMONTHLABEL_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(CALMONTHLABEL_BG_COLOR_KEY)) : GColorDukeBlue;
	weather_txt_color = persist_exists(WEATHER_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(WEATHER_TXT_COLOR_KEY)) : GColorWhite;
	weather_bg_color = persist_exists(WEATHER_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(WEATHER_BG_COLOR_KEY)) : GColorDukeBlue;
	bluetooth_txt_color = persist_exists(BLUETOOTH_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(BLUETOOTH_TXT_COLOR_KEY)) : GColorWhite;
	bluetooth_bg_color = persist_exists(BLUETOOTH_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(BLUETOOTH_BG_COLOR_KEY)) : GColorDukeBlue;
	battery_txt_color = persist_exists(BATTERY_TXT_COLOR_KEY) ? GColorFromHEX(persist_read_int(BATTERY_TXT_COLOR_KEY)) : GColorWhite;
	battery_bg_color = persist_exists(BATTERY_BG_COLOR_KEY) ? GColorFromHEX(persist_read_int(BATTERY_BG_COLOR_KEY)) : GColorDukeBlue;

   #else
	min_txt_color = GColorBlack;
	min_bg_color = GColorWhite;
	datebox_txt_color = GColorBlack;
	datebox_bg_color = GColorWhite;
	hr_txt_color = GColorBlack;
	hr_bg_color = GColorWhite;
	weather_txt_color = GColorBlack;
	weather_bg_color = GColorWhite;
	calmonthlabel_bg_color = GColorWhite;
	calmonthlabel_txt_color = GColorBlack;
	tz3_bg_color = GColorBlack;
	tz3_txt_color = GColorWhite;
	tz2_bg_color = GColorBlack;
	tz2_txt_color = GColorWhite;
	tz1_bg_color = GColorBlack;
	tz1_txt_color = GColorWhite;
	caltoday_bg_color = GColorBlack;
	caltoday_txt_color = GColorWhite;
	cal_bg_color = GColorBlack;
	cal_txt_color = GColorWhite;
	calhdr_bg_color = GColorBlack;
	calhdr_txt_color = GColorWhite;
	weather_txt_color = GColorWhite;
	weather_bg_color = GColorBlack;
	bluetooth_txt_color = GColorWhite;
	bluetooth_bg_color = GColorBlack;
	battery_txt_color = GColorWhite;
	battery_bg_color = GColorBlack;
   #endif

	if (persist_exists(TZTOP_TXT_KEY))
		persist_read_string(TZTOP_TXT_KEY, tz1_txt, sizeof(tz1_txt));
	else {
		strcpy(tz1_txt, "NYC");
		persist_write_string(TZTOP_TXT_KEY, tz1_txt);
	}
	if (persist_exists(TZMID_TXT_KEY))
		persist_read_string(TZMID_TXT_KEY, tz2_txt, sizeof(tz2_txt));
	else {
		strcpy(tz2_txt, "LON");
		persist_write_string(TZMID_TXT_KEY, tz2_txt);
	}
	if (persist_exists(TZBOT_TXT_KEY))
		persist_read_string(TZBOT_TXT_KEY, tz3_txt, sizeof(tz3_txt));
	else {
		strcpy(tz3_txt, "SYD");
		persist_write_string(TZBOT_TXT_KEY, tz3_txt);
	}
	if (persist_exists(TZTOP_NUM_KEY))
		tz1_num = persist_read_int(TZTOP_NUM_KEY);
	else {
		tz1_num = -5;
		persist_write_int(TZTOP_NUM_KEY, tz1_num);
	}
	if (persist_exists(TZMID_NUM_KEY))
		tz2_num = persist_read_int(TZMID_NUM_KEY);
	else {
		tz2_num = 0;
		persist_write_int(TZMID_NUM_KEY, tz2_num);
	}
	if (persist_exists(TZBOT_NUM_KEY))
		tz3_num = persist_read_int(TZBOT_NUM_KEY);
	else {
		tz3_num = 11;
		persist_write_int(TZBOT_NUM_KEY, tz3_num);
	}
	if (persist_exists(WEATHER_ICON_KEY))
		WeatherIcon = persist_read_int(WEATHER_ICON_KEY);
	else {
		WeatherIcon = 33;
		persist_write_int(WEATHER_ICON_KEY, WeatherIcon);
	}
	if (persist_exists(WEATHER_TEMP_6H_KEY))
		persist_read_string(WEATHER_TEMP_6H_KEY, weather6H, sizeof(weather6H));
	else {
		strcpy(weather6H, "0");
		persist_write_string(WEATHER_TEMP_6H_KEY, weather6H);
	}
	if (persist_exists(WEATHER_TEMP_12H_KEY))
		persist_read_string(WEATHER_TEMP_12H_KEY, weather12H, sizeof(weather12H));
	else {
		strcpy(weather12H, "0");
		persist_write_string(WEATHER_TEMP_12H_KEY, weather12H);
	}
	if (persist_exists(WEATHER_TEMP_18H_KEY))
		persist_read_string(WEATHER_TEMP_18H_KEY, weather18H, sizeof(weather18H));
	else {
		strcpy(weather18H, "0");
		persist_write_string(WEATHER_TEMP_18H_KEY, weather18H);
	}
	if (persist_exists(WEATHER_TEMP_24H_KEY))
		persist_read_string(WEATHER_TEMP_24H_KEY, weather24H, sizeof(weather24H));
	else {
		strcpy(weather24H, "0");
		persist_write_string(WEATHER_TEMP_24H_KEY, weather24H);
	}
	if (persist_exists(WEATHER_TIME_6H_KEY))
		persist_read_string(WEATHER_TIME_6H_KEY, time6H, sizeof(time6H));
	else {
		strcpy(time6H, "00");
		persist_write_string(WEATHER_TIME_6H_KEY, time6H);
	}
	if (persist_exists(WEATHER_TIME_12H_KEY))
		persist_read_string(WEATHER_TIME_12H_KEY, time12H, sizeof(time12H));
	else {
		strcpy(time12H, "00");
		persist_write_string(WEATHER_TIME_12H_KEY, time12H);
	}
	if (persist_exists(WEATHER_TIME_18H_KEY))
		persist_read_string(WEATHER_TIME_18H_KEY, time18H, sizeof(time18H));
	else {
		strcpy(time18H, "00");
		persist_write_string(WEATHER_TIME_18H_KEY, time18H);
	}
	if (persist_exists(WEATHER_TIME_24H_KEY))
		persist_read_string(WEATHER_TIME_24H_KEY, time24H, sizeof(time24H));
	else {
		strcpy(time24H, "00");
		persist_write_string(WEATHER_TIME_24H_KEY, time24H);
	}

	if (persist_exists(WEATHER_LOCATION_NAME_KEY))
		persist_read_string(WEATHER_LOCATION_NAME_KEY, weathercity, sizeof(weathercity));
	else {
		strcpy(weathercity, "Unknown City");
		persist_write_string(WEATHER_LOCATION_NAME_KEY, weathercity);
	}

	//define the services
	bluetooth_connection_service_subscribe(bluetooth_handler);
	battery_state_service_subscribe(battery_handler);

	yeardate_layer = text_layer_create(GRect(0, DEVICE_HEIGHT - 14, 105, 14));
	text_layer_set_font(yeardate_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text(yeardate_layer, "dd/mm/yyyy");
	text_layer_set_text_alignment(yeardate_layer, GTextAlignmentRight);
	text_layer_set_text_color(yeardate_layer, datebox_txt_color);
	text_layer_set_background_color(yeardate_layer, datebox_bg_color);
	layer_add_child(window_layer, text_layer_get_layer(yeardate_layer));
	calendar_layer = layer_create(GRect(0, QUADRANT3_TOP, (DEVICE_WIDTH / 4) * 3, QUADRANT3_HEIGHT + 5));
	layer_add_child(window_layer, calendar_layer);
	layer_set_update_proc(calendar_layer, calendar_layer_update_callback);

	monthtext_layer = text_layer_create((GRect) {.origin = { 0, QUADRANT3_TOP - 18 }, .size = { ((DEVICE_WIDTH / 2)), 18 } });
	text_layer_set_font(monthtext_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text(monthtext_layer, "Month");
	text_layer_set_text_alignment(monthtext_layer, GTextAlignmentCenter);
	text_layer_set_text_color(monthtext_layer, calmonthlabel_txt_color);
	text_layer_set_background_color(monthtext_layer, calmonthlabel_bg_color);
	layer_add_child(window_layer, text_layer_get_layer(monthtext_layer));

	weathercity_layer = text_layer_create((GRect) {.origin = { 0, QUADRANT1_HEIGHT - 28 }, .size = { ((DEVICE_WIDTH / 7) * 4) + 9, 11 } });
	text_layer_set_font(weathercity_layer, calendar_font);
	text_layer_set_text(weathercity_layer, "City");
	text_layer_set_text_alignment(weathercity_layer, GTextAlignmentCenter);
	text_layer_set_text_color(weathercity_layer, weather_txt_color);
	text_layer_set_background_color(weathercity_layer, weather_bg_color);
	layer_add_child(window_layer, text_layer_get_layer(weathercity_layer));

	minute_layer = text_layer_create(GRect(DEVICE_WIDTH - ((DEVICE_WIDTH / 7) * 3), 23, (DEVICE_WIDTH / 7) * 3, 44 ));
	text_layer_set_font(minute_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text(minute_layer, "MM");
	text_layer_set_text_alignment(minute_layer, GTextAlignmentCenter);
	text_layer_set_text_color(minute_layer, min_txt_color);
	text_layer_set_background_color(minute_layer, min_bg_color);
	layer_add_child(window_layer, text_layer_get_layer(minute_layer));

	hour_layer = text_layer_create(GRect(DEVICE_WIDTH - ((DEVICE_WIDTH / 7) * 3), QUADRANT2_TOP - 9, (DEVICE_WIDTH / 7) * 3, 44));
	text_layer_set_font(hour_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text(hour_layer, "HH");
	text_layer_set_text_alignment(hour_layer, GTextAlignmentCenter);
	text_layer_set_text_color(hour_layer, hr_txt_color);
	text_layer_set_background_color(hour_layer, hr_bg_color);
	layer_add_child(window_layer, text_layer_get_layer(hour_layer));

	s_temperature_layer = text_layer_create((GRect) {.origin = { ((DEVICE_WIDTH / 7) * 2) + 3, 30 }, .size = { ((DEVICE_WIDTH / 7) * 2) + 1, 18 } });
	text_layer_set_font(s_temperature_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_text(s_temperature_layer, "----");
	text_layer_set_text_color(s_temperature_layer, weather_txt_color);
	text_layer_set_background_color(s_temperature_layer, weather_bg_color);
	text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(s_temperature_layer));

	s_conditions_layer = text_layer_create((GRect) {.origin = { ((DEVICE_WIDTH / 7) * 2) + 3, 0 }, .size = { ((DEVICE_WIDTH / 7) * 2) + 1, 35 } });
	text_layer_set_font(s_conditions_layer, conditions_font);
	text_layer_set_text_color(s_conditions_layer, weather_txt_color);
	text_layer_set_background_color(s_conditions_layer, weather_bg_color);
	text_layer_set_text_alignment(s_conditions_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(s_conditions_layer));

	s_updatetime_layer = text_layer_create((GRect) {.origin = { 0, 48 }, .size = { ((DEVICE_WIDTH / 7) * 2), 10 } });
	text_layer_set_font(s_updatetime_layer, tiny_font8);
	text_layer_set_text(s_updatetime_layer, "00-00:00");
	text_layer_set_text_color(s_updatetime_layer, weather_txt_color);
	text_layer_set_background_color(s_updatetime_layer, weather_bg_color);
	text_layer_set_text_alignment(s_updatetime_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(s_updatetime_layer));

	s_temperature_minmax_layer = text_layer_create((GRect) {.origin = { ((DEVICE_WIDTH / 7) * 2), 48 }, .size = { ((DEVICE_WIDTH / 7) * 2) + 4, 10 } });
	text_layer_set_font(s_temperature_minmax_layer, tiny_font8);
	text_layer_set_text(s_temperature_minmax_layer, "--/--");
	text_layer_set_text_color(s_temperature_minmax_layer, weather_txt_color);
	text_layer_set_background_color(s_temperature_minmax_layer, weather_bg_color);
	text_layer_set_text_alignment(s_temperature_minmax_layer, GTextAlignmentRight);
	layer_add_child(window_layer, text_layer_get_layer(s_temperature_minmax_layer));
/*
        s_weather_windspeed_layer = text_layer_create((GRect) {.origin = { 0, 37 }, .size = { DEVICE_WIDTH / 4, 14 } });
        text_layer_set_font(s_weather_windspeed_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
        text_layer_set_text(s_weather_windspeed_layer, "----");
        text_layer_set_text_color(s_weather_windspeed_layer, weather_txt_color);
        text_layer_set_background_color(s_weather_windspeed_layer, weather_bg_color);
        text_layer_set_text_alignment(s_weather_windspeed_layer, GTextAlignmentCenter);
        layer_add_child(window_layer, text_layer_get_layer(s_weather_windspeed_layer));

        s_weather_humidity_layer = text_layer_create((GRect) {.origin = { DEVICE_WIDTH / 4, 37 }, .size = { DEVICE_WIDTH / 4, 14 } });
        text_layer_set_font(s_weather_humidity_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
        text_layer_set_text(s_weather_humidity_layer, "----");
        text_layer_set_text_color(s_weather_humidity_layer, weather_txt_color);
        text_layer_set_background_color(s_weather_humidity_layer, weather_bg_color);
        text_layer_set_text_alignment(s_weather_humidity_layer, GTextAlignmentCenter);
        layer_add_child(window_layer, text_layer_get_layer(s_weather_humidity_layer));

        s_weather_pressure_layer = text_layer_create((GRect) {.origin = { 0, 51 }, .size = { DEVICE_WIDTH / 2, 16 } });
        text_layer_set_font(s_weather_pressure_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
        text_layer_set_text(s_weather_pressure_layer, "----");
        text_layer_set_text_color(s_weather_pressure_layer, weather_txt_color);
        text_layer_set_background_color(s_weather_pressure_layer, weather_bg_color);
        text_layer_set_text_alignment(s_weather_pressure_layer, GTextAlignmentCenter);
        layer_add_child(window_layer, text_layer_get_layer(s_weather_pressure_layer));
 */

	h24weather_layer = layer_create(GRect(0, 0, ((DEVICE_WIDTH / 7) * 4), 60));
	layer_add_child(window_layer, h24weather_layer);
	layer_set_update_proc(h24weather_layer, h24weather_layer_update_callback);

	battery_layer = text_layer_create((GRect) {.origin = { (DEVICE_WIDTH / 3 * 2), QUADRANT4_TOP - 18 }, .size = { DEVICE_WIDTH / 3, 18 } });
	text_layer_set_font(battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text(battery_layer, "battery");
	text_layer_set_text_color(battery_layer, battery_txt_color);
	text_layer_set_background_color(battery_layer, battery_bg_color);
	text_layer_set_text_alignment(battery_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(battery_layer));

	tz_txt_layer1 = text_layer_create(GRect( (bounds.size.w / 4) * 3 - 2, QUADRANT4_TOP,  bounds.size.w / 4 + 2, 28 ));
	text_layer_set_font(tz_txt_layer1, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text(tz_txt_layer1, "XXX\n00:00");
	text_layer_set_text_color(tz_txt_layer1, tz1_txt_color);
	text_layer_set_background_color(tz_txt_layer1, tz1_bg_color);
	text_layer_set_text_alignment(tz_txt_layer1, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(tz_txt_layer1));

	tz_txt_layer2 = text_layer_create((GRect) {.origin = { ((bounds.size.w / 4) * 3 - 2), QUADRANT4_TOP + 28 }, .size = { bounds.size.w / 4 + 2, 28 } });
	text_layer_set_font(tz_txt_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text(tz_txt_layer2, "YYY\n00:00");
	text_layer_set_text_color(tz_txt_layer2, tz2_txt_color);
	text_layer_set_background_color(tz_txt_layer2, tz2_bg_color);
	text_layer_set_text_alignment(tz_txt_layer2, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(tz_txt_layer2));

	tz_txt_layer3 = text_layer_create((GRect) {.origin = {  ((bounds.size.w / 4) * 3 - 2), QUADRANT4_TOP + 56 }, .size = { bounds.size.w / 4 + 2, 28 } });
	text_layer_set_font(tz_txt_layer3, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text(tz_txt_layer3, "XXX");
	text_layer_set_text_color(tz_txt_layer3, tz3_txt_color);
	text_layer_set_background_color(tz_txt_layer3, tz3_bg_color);
	text_layer_set_text_alignment(tz_txt_layer3, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(tz_txt_layer3));

	bluetooth_layer = text_layer_create((GRect) {.origin = { QUADRANT_MIDDLE, QUADRANT3_TOP - 18 }, .size = { ((DEVICE_WIDTH / 2) - (DEVICE_WIDTH / 3)), 18 } });
	fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
	text_layer_set_text(bluetooth_layer, "bluetooth");
	text_layer_set_text_color(bluetooth_layer, bluetooth_txt_color);
	text_layer_set_background_color(bluetooth_layer, bluetooth_bg_color);
	text_layer_set_text_alignment(bluetooth_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(bluetooth_layer));

	bool connected = bluetooth_connection_service_peek();
	bluetooth_handler(connected);
	BatteryChargeState state = battery_state_service_peek();
	battery_handler(state);

	time_t now = time(NULL);
	struct tm *tick_time = localtime(&now);
	minute_handler(tick_time, MINUTE_UNIT);
	hour_handler(tick_time, HOUR_UNIT);
	day_handler(tick_time, DAY_UNIT);
	tick_timer_service_subscribe(MINUTE_UNIT | HOUR_UNIT | DAY_UNIT, &handle_tick);

	initDone = true;
	// TODO only do weather if timeout reached
	doWeather();
}

static void window_unload(Window *window)
{
	//:: grep -r text_layer_create\(  src/cram.c |egrep -v '(\/\/|\\\*|\/\*)' |sed 's/_create(.*/_destroy(/g' |awk '{print $3$1");"}'|uniq
	text_layer_destroy(yeardate_layer);
	text_layer_destroy(monthtext_layer);
	text_layer_destroy(weathercity_layer);
	text_layer_destroy(minute_layer);
	text_layer_destroy(hour_layer);
	text_layer_destroy(s_temperature_layer);
	text_layer_destroy(s_conditions_layer);
	text_layer_destroy(s_updatetime_layer);
	text_layer_destroy(s_temperature_minmax_layer);
	text_layer_destroy(s_weather_windspeed_layer);
	text_layer_destroy(s_weather_humidity_layer);
	text_layer_destroy(s_weather_pressure_layer);
	text_layer_destroy(battery_layer);
	text_layer_destroy(tz_txt_layer1);
	text_layer_destroy(tz_txt_layer2);
	text_layer_destroy(tz_txt_layer3);
	text_layer_destroy(bluetooth_layer);

	//:: grep -r gbitmap_create src/cram.c |egrep -v '(\/\/|\\\*|\/\*)' |sed 's/_create.*/_destroy(/g' |awk '{print $3$1");"}'|uniq
	//nothing 

	//:: grep -r layer_create  src/cram.c |egrep -v '(text_layer|bitmap_layer|\/\/|\\\*|\/\*)' |awk '{print "layer_destroy("$1");"}'|uniq
	layer_destroy(calendar_layer);
	layer_destroy(h24weather_layer);

}

static void init(void)
{
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
					   .load = window_load,
					   .unload = window_unload,
				   });
	const bool animated = true;
	window_stack_push(window, animated);

	// Register callbacks
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	// Open AppMessage
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit(void)
{
	window_destroy(window);
	//:: grep -r service_subscribe\( src/cram.c |egrep -v '(//|\*)' | sed  's/subscribe.*/unsubscribe()\;/g'
	bluetooth_connection_service_unsubscribe();
	battery_state_service_unsubscribe();
	tick_timer_service_unsubscribe();
}

int main(void)
{
	init();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
	app_event_loop();
	deinit();
}
