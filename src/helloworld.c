#include <pebble.h> 
#include "helloworld.h"
#include <string.h>


static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;

//Angry bird layer/bitmap
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

//Fire Mario layer/bitmap
static BitmapLayer *s_background_layerFM;
static GBitmap *s_background_bitmapFM;

//Kirby layer/bitmap
static BitmapLayer *s_background_layerK;
static GBitmap *s_background_bitmapK;

//Pikachu layer/bitmap
static BitmapLayer *s_background_layerPKMN;
static GBitmap *s_background_bitmapPKMN;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  
  // Create a long-lived buffer
  static char buffer[] = "00:00pm";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00 : 00"), "%H : %M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00pm"), "%l:%M%p", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

 static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  
   //Creating background image layer for Angry Bird
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ABIRD);
  s_background_layer = bitmap_layer_create(GRect(89, 0, 50, 50));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  //Creating background image layer for Fire Mario
  s_background_bitmapFM = gbitmap_create_with_resource(RESOURCE_ID_FIREMARIO);
  s_background_layerFM = bitmap_layer_create(GRect(4, 0, 88, 58));
  bitmap_layer_set_bitmap(s_background_layerFM, s_background_bitmapFM);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layerFM));
  
  //Creating background image layer for Kirby
  s_background_bitmapK = gbitmap_create_with_resource(RESOURCE_ID_KIRBYC);
  s_background_layerK = bitmap_layer_create(GRect(4, 105, 62, 44));
  bitmap_layer_set_bitmap(s_background_layerK, s_background_bitmapK);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layerK));
  
  //Creating background image layer for Pac-man
  s_background_bitmapPKMN = gbitmap_create_with_resource(RESOURCE_ID_PIKACHU);
  s_background_layerPKMN = bitmap_layer_create(GRect(73, 97, 50, 51));
  bitmap_layer_set_bitmap(s_background_layerPKMN, s_background_bitmapPKMN);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layerPKMN));
  
   // Create time TextLayer
  s_time_layer = text_layer_create(GRect(1, 63, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);

  // Improve the layout to be more like a watchface
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MARKERS_28));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  update_time();
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(s_background_bitmapFM);
  bitmap_layer_destroy(s_background_layerFM);
  gbitmap_destroy(s_background_bitmapK);
  bitmap_layer_destroy(s_background_layerK);
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
  fonts_unload_custom_font(s_time_font);
  text_layer_destroy(s_time_layer);
}

static void init() {
  s_main_window = window_create();
  
   window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  }); 
  
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(){
  window_destroy(s_main_window);
}



int main() {
  init();
  app_event_loop();
  deinit();
}
