#include <pebble.h>
#include "messaging.h"
#include "settings.h"
#include "solarUtils.h"

#define SECONDARY_TEXT_LEN 25

static char secondaryText0[SECONDARY_TEXT_LEN];
static char secondaryText1[SECONDARY_TEXT_LEN];
static char secondaryText2[SECONDARY_TEXT_LEN];
static char secondaryText3[SECONDARY_TEXT_LEN];

char *secondaryText0T() {
    return secondaryText0;
}
char *secondaryText1T() {
    return secondaryText1;
}
char *secondaryText2T() {
    return secondaryText2;
}
char *secondaryText3T() {
    return secondaryText3;
}

void (*message_processed_callback)(void);

void messaging_init(void (*processed_callback)(void)) {
  // Custom callback
  message_processed_callback = processed_callback;

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  app_message_open(512, 8);

  app_message_register_inbox_received(inbox_received_callback);
}

void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Does this message contain new location data?
  Tuple *lat_tuple = dict_find(iterator, MESSAGE_KEY_LOCATION_LAT);
  Tuple *lng_tuple = dict_find(iterator, MESSAGE_KEY_LOCATION_LNG);
  Tuple *tzOffset_tuple = dict_find(iterator, MESSAGE_KEY_LOCATION_GMT_OFFSET);

  if (lat_tuple != NULL && lng_tuple != NULL && tzOffset_tuple != NULL) {
    LocationInfo loc;

    // set the coordinates
    float lat = (int)lat_tuple->value->int32;
    lat /= 1000000;

    float lng = (int)lng_tuple->value->int32;
    lng /= 1000000;

    float tzOffset = (int)tzOffset_tuple->value->int32;
    tzOffset /= 60;

    loc.lat = lat;
    loc.lng = lng;
    loc.tzOffset = tzOffset;
    loc.lastUpdatedTime = time(NULL);

    solarUtils_updateLocation(loc);
  }
  
  // Or perhaps it contains new configuration data?
  Tuple *timeColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_TIME_COLOR);
  Tuple *subTextPrimaryColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_SUBTEXT_PRIMARY_COLOR);
  Tuple *subTextSecondaryColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_SUBTEXT_SECONDARY_COLOR);
  Tuple *bgColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_BG_COLOR);
  Tuple *pipColorPrimary_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_PIP_COLOR_PRIMARY);
  Tuple *pipColorSecondary_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_PIP_COLOR_SECONDARY);
  Tuple *ringStrokeColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_RING_STROKE_COLOR);
  Tuple *ringNightColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_RING_NIGHT_COLOR);
  Tuple *ringDayColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_RING_DAY_COLOR);
  Tuple *ringSunriseColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_RING_SUNRISE_COLOR);
  Tuple *ringSunsetColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_RING_SUNSET_COLOR);
  Tuple *sunStrokeColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_SUN_STROKE_COLOR);
  Tuple *sunFillColor_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_SUN_FILL_COLOR);
  Tuple *useLargeFonts_tuple = dict_find(iterator, MESSAGE_KEY_SETTING_USE_LARGE_FONTS);

  if(timeColor_tuple != NULL) {
    globalSettings.timeColor = GColorFromHEX(timeColor_tuple->value->int32);
  }

  if(subTextPrimaryColor_tuple != NULL) {
    globalSettings.subtextPrimaryColor = GColorFromHEX(subTextPrimaryColor_tuple->value->int32);
  }

  if(subTextSecondaryColor_tuple != NULL) {
    globalSettings.subtextSecondaryColor = GColorFromHEX(subTextSecondaryColor_tuple->value->int32);
  }

  if(bgColor_tuple != NULL) {
    globalSettings.bgColor = GColorFromHEX(bgColor_tuple->value->int32);
  }

  if(pipColorPrimary_tuple != NULL) {
    globalSettings.pipColorPrimary = GColorFromHEX(pipColorPrimary_tuple->value->int32);
  }

  if(pipColorSecondary_tuple != NULL) {
    globalSettings.pipColorSecondary = GColorFromHEX(pipColorSecondary_tuple->value->int32);
  }

  if(ringStrokeColor_tuple != NULL) {
    globalSettings.ringStrokeColor = GColorFromHEX(ringStrokeColor_tuple->value->int32);
  }

  if(ringNightColor_tuple != NULL) {
    globalSettings.ringNightColor = GColorFromHEX(ringNightColor_tuple->value->int32);
  }

  if(ringDayColor_tuple != NULL) {
    globalSettings.ringDayColor = GColorFromHEX(ringDayColor_tuple->value->int32);
  }

  if(ringSunriseColor_tuple != NULL) {
    globalSettings.ringSunriseColor = GColorFromHEX(ringSunriseColor_tuple->value->int32);
  }

  if(ringSunsetColor_tuple != NULL) {
    globalSettings.ringSunsetColor = GColorFromHEX(ringSunsetColor_tuple->value->int32);
  }

  if(sunStrokeColor_tuple != NULL) {
    globalSettings.sunStrokeColor = GColorFromHEX(sunStrokeColor_tuple->value->int32);
  }

  if(sunFillColor_tuple != NULL) {
    globalSettings.sunFillColor = GColorFromHEX(sunFillColor_tuple->value->int32);
  }

  if(useLargeFonts_tuple != NULL) {
    globalSettings.useLargeFonts = (bool)useLargeFonts_tuple->value->int8;
  }

  // Or perhaps it contains secondary text?
  Tuple *secondaryText0_tuple = dict_find(iterator, MESSAGE_KEY_SECONDARY_TEXT_0);
  if(secondaryText0_tuple != NULL) {
      APP_LOG(APP_LOG_LEVEL_INFO, "indeed its text!");
//    char *secondaryText0 = secondaryText0_tuple->value->cstring;
    APP_LOG(APP_LOG_LEVEL_INFO, secondaryText0_tuple->value->cstring);
    snprintf(secondaryText0, sizeof(secondaryText0), "%s", secondaryText0_tuple->value->cstring);
  }
  Tuple *secondaryText1_tuple = dict_find(iterator, MESSAGE_KEY_SECONDARY_TEXT_1);
  if(secondaryText1_tuple != NULL) {
//    char *secondaryText1 = secondaryText1_tuple->value->cstring;
    APP_LOG(APP_LOG_LEVEL_INFO, secondaryText1_tuple->value->cstring);
    snprintf(secondaryText1, sizeof(secondaryText1), "%s", secondaryText1_tuple->value->cstring);
  }
  Tuple *secondaryText2_tuple = dict_find(iterator, MESSAGE_KEY_SECONDARY_TEXT_2);
  if(secondaryText2_tuple != NULL) {
//    char *secondaryText2 = secondaryText2_tuple->value->cstring;
    APP_LOG(APP_LOG_LEVEL_INFO, secondaryText2_tuple->value->cstring);
    snprintf(secondaryText2, sizeof(secondaryText2), "%s", secondaryText2_tuple->value->cstring);
  }
  Tuple *secondaryText3_tuple = dict_find(iterator, MESSAGE_KEY_SECONDARY_TEXT_3);
  if(secondaryText3_tuple != NULL) {
//    char *secondaryText3 = secondaryText3_tuple->value->cstring;
    APP_LOG(APP_LOG_LEVEL_INFO, secondaryText3_tuple->value->cstring);
    snprintf(secondaryText3, sizeof(secondaryText3), "%s", secondaryText3_tuple->value->cstring);
  }

  Settings_saveToStorage();

  message_processed_callback();
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  // APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator,
                            AppMessageResult reason, void *context) {
  // APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  // APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}