<?php
/**
 * @file
 *   Theme include file
 *
 * @version
 *
 * @developers
 *   Rafal Wieczorek <kenorb@gmail.com>
 */

/**
 * Theme preprocess callback
 *
 * Format an calendar day node for display.
 *  See: template_preprocess_calendar($vars);
 *
 */
function template_preprocess_booking_timeslots_day(&$vars) {
  // initialize 
  module_load_include('inc', 'booking_timeslots'); // load additional functions
  extract($vars);
  /*
   * Variables available:
   * $view: The view.
   * $columns: an array of column names.
   * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
   * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
   */

  /** calculate event lengths **/
  $hours_per_slot = variable_get('booking_timeslot_length_hours', 1);
  $minutes_per_slot = variable_get('booking_timeslot_length_minutes', 0);
  $hours_per_slot = $hours_per_slot + $minutes_per_slot/60; // calculate how many hours have one event

  /** define tpl variables **/

  // for HOW MANY SLOTS each event should be booked
  $vars['bt_groupby_times'] = $view->style_options['groupby_times']; 

  // for HOW MANY SLOTS each event should be booked
  $vars['bt_half_hour_mode'] = ($view->style_options['groupby_times'] === 'half');

  // name of current day of the week
  $vars['bt_day_of_week'] = $today = date("l", strtotime($rows['date'])); 

  // Check if current weekday is available for booking
  $week_availability = variable_get('booking_timeslot_not_avaliable', 1);
  $vars['bt_weekday_avail'] = $week_availability[strtolower($today)] != '0';

  // for HOW MANY SLOTS each event should be booked
  $vars['bt_groupby_times_custom'] = ($view->style_options['groupby_times'] == 'custom' ? $view->style_options['groupby_times_custom'] : FALSE); 

  // for HOW MANY SLOTS each event should be booked
  $vars['bt_slots_per_event'] = ($view->style_options['groupby_times'] == 'custom' ? $hours_per_slot : ((bool)$vars['bt_half_hour_mode'] ? $hours_per_slot/0.5 : $hours_per_slot));

  // Number of events booked for the all day
  $vars['bt_booked_all_day'] = $bt_booked_all_day = count($rows['all_day']['Items']);

  // Maximum number of available slots per defined timeframe (hour, half-hour)
  $vars['bt_max_avail_slots'] = $bt_max_avail_slots = variable_get('booking_timeslot_available_slots', 1);

  // Check how many slots are available that day
  if ($bt_max_avail_slots >= 2) $bt_max_avail_slots++; // FIXME: I'm not sure what is the purpose of this
  $vars['bt_no_of_slots_available'] = ($bt_max_avail_slots == 0 ? 1 : max(0,max(1,max(1,$bt_max_avail_slots)-1)-$bt_booked_all_day));

  // Set starting and ending hours
  $vars['bt_hour']['start'] = variable_get('booking_timeslot_hour_from', 8); // from what hour day is starting (8 by default)
  $vars['bt_hour']['end'] = variable_get('booking_timeslot_hour_to', 18); // what hour day is finished (18 by default)

  // Check if user has permission to see booking event details.
  $vars['bt_show_events'] = (user_access('show booking dates'));

  // link to booking node creation page
  $vars['bt_node_add_link'] = $ctype_link = booking_timeslots_get_ctype_name(NULL, TRUE);

  // name of content type which is used for non-available dates
  $vars['bt_ctype_non_avail'] = booking_timeslots_non_avail_ctype();

  // Contains text for slots, which are booked, not available or already booked
  $vars['bt_text']['book_now'] = t('Book now'); // Text for slot, which is available to book
  $vars['bt_text']['slot_booked'] = t('Already booked'); // Text for slot, which is already booked
  $vars['bt_text']['slot_unavailable'] = t('Not Available'); // Text for slot, which is unavailable
}

/**
 * Theme preprocess callback
 *
 * Format an calendar week node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_week(&$vars) {
  extract($vars);

  // name of current day of the week
  $vars['bt_day_of_week'] = $today = date("l", strtotime($rows['date'])); 

  // Contains text for slots, which are booked, not available or already booked
  $vars['bt_text']['book_now'] = t('Book now'); // Text for slot, which is available to book
  $vars['bt_text']['slot_booked'] = t('Already booked'); // Text for slot, which is already booked
  $vars['bt_text']['slot_unavailable'] = t('Not Available'); // Text for slot, which is unavailable
}


/**
 * Theme preprocess callback
 *
 * Format an calendar month node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_month(&$vars) {
  extract($vars);

  // name of current day of the week
  $vars['bt_day_of_week'] = $today = date("l", strtotime($rows['date'])); 

  // Contains text for slots, which are booked, not available or already booked
  $vars['bt_text']['book_now'] = t('Book now'); // Text for slot, which is available to book
  $vars['bt_text']['slot_booked'] = t('Already booked'); // Text for slot, which is already booked
  $vars['bt_text']['slot_unavailable'] = t('Not Available'); // Text for slot, which is unavailable
}

