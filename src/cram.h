/* Timely header file - structs and function prototypes
 * ( to be split out into pieces, for dynamic overlays, at a later date )
 */

// Create a struct to hold our persistent settings...
typedef struct persist { // 18 bytes
  uint8_t version;                // version key
  uint8_t inverted;               // Invert display
  uint8_t day_invert;             // Invert colors on today's date
  uint8_t grid;                   // Show the grid
  uint8_t vibe_hour;              // vibrate at the top of the hour?
  uint8_t dayOfWeekOffset;        // first day of our week
  uint8_t date_format;            // date format
  uint8_t show_am_pm;             // Show AM/PM below time
  uint8_t show_day;               // Show day name below time
  uint8_t show_week;              // Show week number below time
  uint8_t week_format;            // week format (calculation, e.g. ISO 8601)
  uint8_t vibe_pat_disconnect;    // vibration pattern for disconnect
  uint8_t vibe_pat_connect;       // vibration pattern for connect
  char *strftime_format;          // custom date_format string (date_format = 255)
  uint8_t track_battery;          // track battery information
} __attribute__((__packed__)) persist;

typedef struct persist_months_lang { // 252 bytes
  char monthsNames[12][21];       // 252: 10-20 UTF8 characters for each of 12 months
//                                   252 bytes
} __attribute__((__packed__)) persist_months_lang;

typedef struct persist_days_lang { // 238 bytes
  char DaysOfWeek[7][34];         //  238: 16-33 UTF8 characters for each of 7 weekdays
//                                    238 bytes
} __attribute__((__packed__)) persist_days_lang;

typedef struct persist_general_lang { // 253 bytes
  char statuses[2][26];           //  40: 12-25 characters for each of  2 statuses
  char abbrTime[2][12];           //  24:  5-11 characters for each of  2 abbreviations
  char abbrDaysOfWeek[7][6];      //  42:  2- 5 characters for each of  7 weekdays abbreviations
  char abbrMonthsNames[12][11];   // 132:  5-11 characters for each of 12 months abbreviations
  char language[3];               //   3:  2 characters for language (internal, stored as ascii for convenience)
//                                   253 bytes
} __attribute__((__packed__)) persist_general_lang;

typedef struct persist_debug { // 6 bytes
  bool general;              // debugging messages (general)
  bool language;             // debugging messages (language/translation)
  bool reserved_1;           // debugging messages (reserved to spare updates later)
  bool reserved_2;           // debugging messages (reserved to spare updates later)
  bool reserved_3;           // debugging messages (reserved to spare updates later)
  bool reserved_4;           // debugging messages (reserved to spare updates later)
} __attribute__((__packed__)) persist_debug;

typedef struct persist_adv_settings { // 243 bytes
  uint8_t week_pattern;    //  1 byte
  uint8_t invertStatBar;   //  1 byte
  uint8_t invertTopSlot;   //  1 byte
  uint8_t invertBotSlot;   //  1 byte
  uint8_t showStatus;      //  1 byte
  uint8_t showStatusBat;   //  1 byte
  uint8_t showDate;        //  1 byte
  uint8_t DND_start;       //  1 byte
  uint8_t DND_stop;        //  1 byte
  uint8_t DND_accel_off;   //  1 byte
  uint8_t vibe_hour_start; //  1 byte
  uint8_t vibe_hour_stop;  //  1 byte
  uint8_t vibe_hour_days;  //  1 byte
  uint8_t idle_reminder;   //  1 byte
  uint8_t idle_pattern;    //  1 byte
  char idle_message[32];   // 32 bytes
  uint8_t idle_start;      //  1 byte
  uint8_t idle_stop;       //  1 byte
  int8_t clock2_tz;        //  1 byte
  char clock2_desc[32];    // 32 bytes
  uint8_t weather_format;  //  1 byte
  uint8_t weather_update;  //  1 byte
  char weather_lat[8];     //  8 bytes
  char weather_lon[8];     //  8 bytes
  uint8_t clock_font;      //  1 byte
  uint8_t token_type[2];   //  2 bytes
  char token_code[2][65];  //130 bytes
  uint8_t slots[10];       // 10 bytes
} __attribute__((__packed__)) persist_adv_settings;

void setInvColors(GContext* ctx);
void update_time_text();
bool dnd_period_check();
bool hourvibe_period_check();
void set_status_charging_icon();
void handle_vibe_suppression();
//static void request_weather(void *data);
//static void request_timezone(void *data);
void update_datetime_subtext();
void generate_vibe(uint32_t vibe_pattern_number);
void update_date_text();
//static void request_weather(void *data);
//static void request_timezone(void *data);
