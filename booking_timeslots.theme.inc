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
  $week_availability = variable_get('booking_timeslot_not_avaliable', array());
  $vars['bt_weekday_avail'] = $bt_weekday_avail = !in_array(strtolower($today), $week_availability, TRUE);

  // for HOW MANY SLOTS each event should be booked
  $vars['bt_groupby_times_custom'] = ($view->style_options['groupby_times'] == 'custom' ? $view->style_options['groupby_times_custom'] : FALSE); 

  // for HOW MANY SLOTS each event should be booked
  $vars['bt_slots_per_event'] = ($view->style_options['groupby_times'] == 'custom' ? $hours_per_slot : ((bool)$vars['bt_half_hour_mode'] ? $hours_per_slot/0.5 : $hours_per_slot));

  // Number of events booked for the all day
  $vars['bt_booked_all_day'] = $bt_booked_all_day = count($rows['all_day']['Items']);

  // Maximum number of available slots per defined timeframe (hour, half-hour)
  $vars['bt_max_avail_slots'] = $bt_max_avail_slots = variable_get('booking_timeslot_available_slots', 1);

  // Check how many slots are available that day per timeframe
  $vars['bt_no_of_slots_available'] = $bt_no_of_slots_available = $bt_weekday_avail ? ($bt_max_avail_slots == 0 ? 1 : max(0, $bt_max_avail_slots - $bt_booked_all_day)) : 0;

  // Set starting and ending hours
  $vars['bt_hour']['start'] = variable_get('booking_timeslot_hour_from', 8); // from what hour day is starting (8 by default)
  $vars['bt_hour']['end'] = variable_get('booking_timeslot_hour_to', 18); // what hour day is finished (18 by default)

  // Set array of available times of current day (i.e. 08:00:00 - 18:00:00)
  $vars['bt_day_times'] = booking_timeslots_get_day_availability($view->date_info->style_groupby_times, $rows, $bt_no_of_slots_available);

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

  // Get all holidays
  $vars['bt_holidays'] = booking_timeslots_get_holidays();

  // TODO: render timeslots, see: calendar/theme/theme.inc
  $date = $rows['date'];
  // var_dump($times, $rows); exit;
  // do we need this?
  // $show_empty_times = $view->date_info->style_show_empty_times;
  // $grouping_field = $view->date_info->style_groupby_field;
  // $end_start_time = '23:59:59';
  // $start_time = array_shift($start_times);
  // $next_start_time = count($start_times) ? array_shift($start_times) : $end_start_time;
}

/**
 * Theme preprocess callback
 *
 * Format an calendar week node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_week(&$vars) {
  
  extract($vars);
  /*
   * Variables available:
   * $view: The view.
   * $columns: an array of column names.
   * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
   * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
   */

  // name of current day of the week
  $vars['bt_day_of_week'] = $today = date("l", strtotime($rows['date'])); 

  // Contains text for slots, which are booked, not available or already booked
  $vars['bt_text']['book_now'] = t('Book now'); // Text for slot, which is available to book
  $vars['bt_text']['slot_booked'] = t('Already booked'); // Text for slot, which is already booked
  $vars['bt_text']['slot_unavailable'] = t('Not Available'); // Text for slot, which is unavailable

  // TODO: #863550 http://drupal.org/node/863550 - check weekday availability
}


/**
 * Theme preprocess callback
 *
 * Format an calendar month node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_month(&$vars) {
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

  // name of current day of the week
  $vars['bt_day_of_week'] = $today = date("l", strtotime($rows['date'])); 

  // link to booking node creation page
  $vars['bt_node_add_link'] = $bt_node_add_link = booking_timeslots_get_ctype_name(NULL, TRUE);

  // Contains text for slots, which are booked, not available or already booked
  $vars['bt_text']['book_now'] = t('Book now'); // Text for slot, which is available to book
  $vars['bt_text']['slot_booked'] = t('Already booked'); // Text for slot, which is already booked
  $vars['bt_text']['slot_unavailable'] = t('Not Available'); // Text for slot, which is unavailable

  echo '@TODO: Here we have rows as html code but we would like to operate on cells so we need to get the cell structure from somewhere'; 

  $cid = arg(0);
  foreach ($vars['rows'] as $weekno => $row) {
    foreach ($row as $day => $data) {
      $cell = $data['data'];
      $date = str_replace($cid . '-', $cid . '/', $data['id']);
      if (!is_array($cell) && $data['class'] != 'week' && strpos($data['class'], 'empty') === FALSE) {
        // FIXME: && booking_timeslots_day_not_available($date)) { // If this cell is already rendered...
        $link = l($vars['bt_text']['book_now'], $date);
        
        $vars['rows'][$weekno][$day] .= "<div class='slot_free'>$link</div>"; // ...and which is free
      }
    }
  }
  // var_dump($rows); exit;
  // $vars['bt_day_times'] = booking_timeslots_get_day_availability($view->date_info->style_groupby_times, $rows, $bt_no_of_slots_available);

  // TODO: #863550 http://drupal.org/node/863550 - check weekday availability - booking_timeslots_day_not_available - booking_timeslots_get_day_availability
}

