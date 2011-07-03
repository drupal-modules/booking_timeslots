<?php

/**
 * @file
 *   Booking Timeslots Functions.
 *
 * @version
 *
 * @developers:
 *   Rafal Wieczorek <kenorb@gmail.com>
 */

/*
 * Add specified time to datetime
 * @datetime
 *    datetime in 'yyyy-mm-dd hh:mm:ss' or array format
 * @rules
 *    what should be changed
 *   E.g. array('hour' => 10) will add 10 hours to the datetime
 *        array('minute' => -1) will deduct 1 minute from the datetime
 *        array('mday' => 1, 'hour' => 12) will add day and an half
 *
 * @version 11/12/2008
 * @author kenorb@gmail.com
 */
function booking_timeslots_add_time($datetime, $rules = array()) {
  $arr = !is_array($datetime) ? booking_timeslots_convert_date_to_arr($datetime) : $datetime;
  foreach ($rules as $type => $how_much) {
    $arr[$type] += $how_much;
  }
  $new_datetime = date('Y-m-d H:i:s', mktime($arr['hour'], $arr['minute'], $arr['second'], $arr['month'], $arr['mday'], $arr['year']));
  return is_array($datetime) ? booking_timeslots_convert_date_to_arr($new_datetime) : $new_datetime;
}

/*
 * Convert format 'yyyy-mm-dd hh:mm:ss' to array
 *
 * @version 11/12/2008
 * @author kenorb@gmail.com
 */
function booking_timeslots_convert_date_to_arr($datetime, $dt_sep = ' ', $d_sep = '-', $t_sep = ':') {
  $ret = FALSE;
  if (strpos($datetime, $dt_sep)!==FALSE) {
    list($date, $time) = explode($dt_sep, $datetime);
    if (strpos($date, $d_sep)!==FALSE) {
      list($yyyy, $mm, $dd) = explode($d_sep, $date);
    }
    if (strpos($time, $t_sep)!==FALSE) {
      list($hour, $min, $sec) = explode($t_sep, $time);
    }
    $ret = array(
      'year' => $yyyy,
      'month' => $mm,
      'mday' => $dd,
      'hour' => $hour,
      'minute' => $min,
      'second' => $sec,
    );
  }
  return $ret;
}

/**
 * Validate datetime
 *
 * @param $datetime
 *      datetime to check (in `yyyy-mm-dd hh-mm-ss` or `yyyy-mm-dd hh-mmAM/PM` format)
 * @return
 *      TRUE if datetime is in valid format
 */
function booking_timeslots_valid_datetime($datetime) {
  return preg_match("/^([0-9]{2,4})-([0-1]?[0-9])-([0-3]?[0-9]) (?:([0-2]?[0-9]):([0-5][0-9]))?(:[0-9]{2}|[AP]M)?$/", $datetime);
}

/**
 * Validate datetime
 *
 * @param string $date
 *      date to check (format: yyyy-mm-dd hh:mm)
 * @return
 *      TRUE if date is available
 */
function booking_timeslots_check_slot_availability($date, $nodeType, $forms = NULL) {
    try {
      $date = new DateTime($date['year']."-".$date['month']."-".$date['day']." ".$date['hour'].":".$date['minute']);
    } catch (Exception $e) {
        $err = $e->getMessage();
        drupal_set_message('php', $e->getMessage(), 'error');
        watchdog('php', $e->getMessage() . "<br>\n" . print_r($forms, TRUE), 'error');
    }
    $date = date_format_date($date, 'custom', 'Y-m-d H:i');
    $timeslot_fields = variable_get('booking_timeslot_fields', array('field_booking_slot' => 'field_booking_slot'));

    $slots = db_query('SELECT nid FROM {node} WHERE type = "%s"', $nodeType);
    while ($row = db_fetch_array($slots)) {
        $slot = node_load($row['nid']);
        foreach ($timeslot_fields as $func_name) {
            if (!isset($slot->$func_name)) {
              continue;
            }

            $slotDate = $slot->{$func_name}[0]['value'];
            $curTimezone = $slot->{$func_name}[0]['timezone_db'];
            $tarTimezone = $slot->{$func_name}[0]['timezone'];

            // converting time
            $utc_date = date_make_date(date_format_date(new DateTime($slotDate), 'custom', 'Y-m-d H:i'), $curTimezone, DATE_DATETIME);
            date_timezone_set($utc_date, timezone_open($tarTimezone));
            $utc_date_string = date_format_date($utc_date, 'custom', 'Y-m-d H:i');
            $slotDate = $utc_date_string;

            if(!isset($slotMap[$slotDate]))
                $slotMap[$slotDate] = 1;
            else
                $slotMap[$slotDate]++;
        }
    }

    $slots = variable_get('booking_timeslot_available_slots', 1);
    if(isset($slotMap[$date]) || $slotMap[$date] > $slots)
        return false;
    
    return true;
}

/**
 * Detect current view from URL
 *
 */
function booking_timeslot_detect_view() {
  $view = views_get_current_view();
  return $view->name ? $view->name : FALSE; 
}

/**
 * Get proper content type name or link for slot booking
 *
 * @param string $content_type
 *      name of content type which is related to booking timeslots
 * @param bool $link
 *      if TRUE, result will be as node link, otherwise it will be plain name of content type
 * @return
 *      name of the link
 */
function booking_timeslots_get_ctype_name($content_type = NULL, $link = FALSE) {
  static $content_type_cache = array();
  $my_fields = array_flip(variable_get('booking_timeslot_fields', array('field_booking_slot' => 'field_booking_slot')));
  $arg0 = arg(0);
  if (!isset($content_type_cache[$arg0])) {
    $view_name = booking_timeslot_detect_view(); // get view from URL
    if ($field_name = booking_timeslot_detect_field_from_view($view_name)) { // detect field name via View
      $content_type = booking_timeslot_content_type_via_field($field_name); // detect content type via field name
      $content_type_cache[$view_name] = $content_type;
    }
  } else {
    $content_type = $arg0;
  }

  if (empty($content_type)) {
    drupal_set_message(t("Booking time slots are not properly configured for this page, please contact your site administrator!"), 'warning');
    if (user_access('administer booking timeslots')) {
      drupal_set_message(t("Please <a href='!url'>configure Booking Timeslots</a>.", array('!url' => url('admin/settings/booking_timeslots/form_settings'))), 'error');
    }
  }

  return $link ? "node/add/" . str_replace('_', '-', $content_type) : $content_type;
}

/**
 * Detect configured Datetime field assosiated with specified View
 *
 */
function booking_timeslot_detect_field_from_view($view) {
  $field_name = FALSE;
  if (!is_object($view)) {
    $view_name = $view;
    $view = views_get_view($view);
  } else {
    $view_name = $view->name;
  }
  $my_fields = array_flip(variable_get('booking_timeslot_fields', $view_name == 'booking_calendar' ? array('field_booking_slot' => 'field_booking_slot') : array()));
  $handler = $view->display['default'];
  $field_args = $handler->display_options['arguments']['date_argument']['date_fields'];
  if (!empty($field_args)) {
    foreach ($field_args as $field) {
      if (strpos($field, 'field_') !== FALSE) {
        $view_field_name = next(explode('.', $field));
        foreach ($my_fields as $key => $field_conf) {
          if (strpos($view_field_name, $field_conf) !== FALSE) { // check if our field is in this view
            $field_name = $field_conf;
            break 2;
          }
        }
      }
    }
  } else if (user_access('administer booking timeslots')) {
    drupal_set_message(t('Booking Timeslots: Your calendar view has missing date arguments!'), 'warning');
  }
  return $field_name;
}

/**
 * Detect content type via field
 *
 */
function booking_timeslot_content_type_via_field($field_name) {
  $field = content_fields($field_name);
  return $field['type_name'];
}

/**
 * Get field name used for booking timeslots
 *
 * @param string $content_type
 *      name of content type which is related to booking timeslots
 * @return
 *      name of the field
 */
function booking_timeslots_get_field_name($view_name = NULL) {
  return booking_timeslot_detect_field_from_view($view_name ? $view_name : booking_timeslot_detect_view());
}

/**
 * Return name of content type which is used for non-available dates
 *
 */
function booking_timeslots_non_avail_ctype() {
  $non_available_type = FALSE;
  $my_forms = variable_get('booking_timeslot_forms', array()); // FIXME
  $form_name = key(array_flip($my_forms));
  $my_field = booking_timeslots_get_field_name();

  $non_available = array_flip(variable_get('booking_timeslot_fields', array('field_booking_slot' => 'field_booking_slot')));
  foreach ($non_available as $key => $value) {
      if ($key != '0') unset($non_available[$key]);
  }
  $non_available = key(array_flip($non_available));

  foreach (content_fields() as $key => $field) {
     if ($field['field_name'] == $non_available) {
       $non_available_type = $field['type_name'];
     }
   }
  return $non_available_type;
}

/**
 * Generate timeslot availability for specified day
 *
 * @param array $times
 *      array of available times in view
 * @param array $rows
 *      array of preprocess rows
 * @param integer $bt_no_of_slots_available (optional)
 *      default number of available slots
 * @return
 *      name of the field
 */
function booking_timeslots_get_day_availability($times, $rows = array(), $bt_no_of_slots_available = 1) {
  $date = $rows['date'];
  $times = array_flip(array_filter($times, '_bt_filter_times')); // filter and format to those which we configured
  if (!booking_timeslots_day_not_available($date)) {
    $bt_no_of_slots_available = -1;
  }
  _bt_set_slot_avail(NULL, $bt_no_of_slots_available);
  $times = array_map('_bt_set_slot_avail', $times); // fill with numbers of available slots
  // check if time slots are limited by configured times
  if ($available_times = booking_timeslots_generate_limited_dates()) {
    foreach ($times as $time => $avail) { // for each time slot...
      $unixtime = strtotime("$date $time"); // convert current slot to unixtime
      $avail_slots = booking_timeslots_unixtime_within_ranges($unixtime, $available_times['all']); // check if current slot is within available dates
      $times[$time] = min($bt_no_of_slots_available, $avail_slots == 0 ? -1 : $avail_slots); // set the number of slots (otherwise set to -1 as unavailable)
    }
  }
  // parse booked slots and deduct available slots
  if ($bt_no_of_slots_available > 0 && isset($rows['items'])) {
    foreach ($rows['items'] as $time => $slot_record) {
      $times[$time]--;
    }
  }
  // var_dump($date, $times); exit; // use it for debugging
  return $times;
}

/**
 * Count how many times specified unixtime is within array of unixtime ranges
 *
 * @param integer $unixtime
 *      unixtime to check
 * @param array $unixtime_ranges
 *      array of unixtime ranges
 * @return
 *      return how many times specified unixtime is within array of unixtime ranges, otherwise 0
 */
function booking_timeslots_unixtime_within_ranges($unixtime, $unixtime_ranges) {
  $within_range = 0;
  foreach ($unixtime_ranges as $key => $unixtime_range) {
    list($unixstart, $unixend) = $unixtime_range;
    if ($unixtime >= $unixstart && $unixtime < $unixend) {
      $within_range++;
    }
  }
  return $within_range;
}

/**
 * Check if specified day is available
 *  Not available days are:
 *    - in the past (configurable)
 *    - holidays (configurable)
 *
 * @param string $date
 *      date to check (in format: yyyy-mm-dd)
 * @return
 *      return TRUE if day is available, otherwise FALSE
 */
function booking_timeslots_day_not_available($date) {
  // TODO: var_dump(booking_timeslots_get_holidays()); exit;
  $avail = TRUE;
  $dayofweek = date("l", strtotime($date));
  if (!variable_get('booking_timeslot_past_booking', FALSE) && strtotime($date) < strtotime("now")) {
    if (user_access('administer booking timeslots') && variable_get('booking_timeslot_show_debug_messages', FALSE)) {
      drupal_set_message(__FUNCTION__ . t("(): Date %date is not available, because it's in the past.", array('%date' => $date)), 'status');
    }
    $avail = FALSE; // not available if it's in the past
    // TODO: http://drupal.org/node/869418 - close bookings xxx hours in advance
  } else if (in_array(strtolower($dayofweek), variable_get('booking_timeslot_not_avaliable', array()), TRUE)) {
    if (user_access('administer booking timeslots') && variable_get('booking_timeslot_show_debug_messages', FALSE)) {
      drupal_set_message(__FUNCTION__ . t("(): Date %date is not available, because %dayofweek is on non-available list.", array('%date' => $date, '%dayofweek' => $dayofweek)), 'status');
    }
    $avail = FALSE; // current weekday is not available for booking (configurable)
  }
  $limited_dates = booking_timeslots_generate_limited_dates();
  return $avail;
}

/**
 * Implementation of array_filter callback
 *
 *   Filter times array only to those which are configured
 */
function _bt_filter_times($var) {
  list($hour, $minute, $second) = explode(':', $var);
  return ($hour >= variable_get('booking_timeslot_hour_from', 8) && $hour <= variable_get('booking_timeslot_hour_to', 18));
}

/**
 * Implementation of array_map callback
 *
 *   Fill array with number of available slots
 *
 * @param element $var
 *      load element of the array (NULL if you need to init number of available slots)
 * @param integer $set_no_slots
 *      if set, change default number of available slots
 * @return
 *      return number of available slots per timeframe
 */
function _bt_set_slot_avail($var, $set_no_slots = NULL) {
  static $no_slots = 1;
  if ($set_no_slots) {
    $no_slots = $set_no_slots;
  }
  return $no_slots;
}

/**
 * Get the list of holidays
 *
 * @param ?
 *      ?
 * @return
 *      array of holidays
 */
function booking_timeslots_get_holidays() {
  $matches = array();

  $my_forms = variable_get('booking_timeslot_forms', array()); // FIXME
  $form_name = key(array_flip($my_forms));
  $my_field = booking_timeslots_get_field_name();

  $q = db_query('SELECT * FROM {node} WHERE type = "%s"', $form_name);
  while ($row = db_fetch_array($q)) {
    $temp_node = node_load(array('type' => $form_name, 'nid' => $row['nid']));
    if (node_access('view', $temp_node)) $matches[] = $temp_node;
  }

  $q = db_query('SELECT * FROM {node} WHERE type = "%s"', $bt_ctype_non_avail);
  while ($row = db_fetch_array($q)) {
    $matches[] = node_load(array('type' => $bt_ctype_non_avail, 'nid' => $row['nid']));
  }

  $holidays = array();
  foreach ($matches as $node) {
      $date_from = !empty($node->$my_field) ? $node->$my_field : (!empty($node->$non_available) ? $node->$non_available : NULL);
      $date_from_value = $date_from[0]['value'];
      $date_to = !empty($node->$my_field) ? $node->$my_field : (!empty($node->$non_available) ? $node->$non_available : NULL);
      $date_to_value = $date_to[0]['value2'];

      if (empty($date_from) || empty($date_to))
        continue;

      $date_from_unix = strtotime($date_from_value . ' ' . $date_from[0]['timezone_db']);
      $date_to_unix = strtotime($date_to_value . ' ' . $date_to[0]['timezone_db']);

      if ($date_to_unix < $date_from_unix) {
        list($date_from_unix,$date_to_unix) = array($date_to_unix,$date_from_unix);
      }

      if ((($date_from_unix == $date_to_unix) && ($date_from == $date_to)) || ($node->type == $bt_ctype_non_avail)) {
        $date_to_unix += 60*60*24; // add 24 hour
      }

      $holidays[] = array($date_from_unix, $date_to_unix, $node, $date_from);
  }

  return $holidays;
}


/**
 * Wrapper function to generate array of limited dates
 *
 * @param ?
 *      ?
 * @return
 *      array of dates
 */
function booking_timeslots_generate_limited_dates() {
  // load fields used for limited dates
  $fields_limited = variable_get('booking_timeslot_limited_dates', array());
  $fields_limited = array_flip($fields_limited);
  unset($fields_limited[0]);

  return !empty($fields_limited) ? booking_timeslots_generate_field_dates($fields_limited) : array();
}

/**
 * Generate array of dates from field within specified content type
 *   TODO: static caching
 *
 * @param ?
 *      ?
 * @return
 *      array of dates
 */
function booking_timeslots_generate_field_dates($field_names = array(), $perm_req = 'view') {
  $res = array('all' => array());
  foreach ($field_names as $field_name => $value) {
    $ctype = booking_timeslot_content_type_via_field($field_name);
    $res[$ctype][$field_name] = array();
  }

  foreach ($res as $ctype => $field_data) {
    $q = db_query('SELECT nid FROM {node} WHERE type = "%s"', $ctype); // get all nodes from specified type
    while ($row = db_fetch_object($q)) {
      $node = node_load($row->nid);
      if (node_access($perm_req, $node)) { // add date if user has specified permission ('view' by default)
        foreach ($field_data as $field_name => $value) {
          if ($node->$field_name) {
            // convert dates from the field to unixtime and insert in specified format: [content_type_name][field_name][nid][key] => 'unixtimestart-unixtimeend'
            $unixtime_field = booking_timeslots_load_date_field($node, $field_name); // get array of unixtime ranges separated by '-'
            $res[$ctype][$field_name][$node->nid] = $unixtime_field; // add to the array

            // merge as well with 'all' section
            foreach ($unixtime_field as $key => $unixtime_range) {
              $res['all'][] = explode('-', $unixtime_range);
            }
          }
        }
      }
    }
  }
  return $res;
}

/**
 * Convert unixtime into array of time slots
 *
 * @param integer $unixstart
 *      Start unixtime
 * @param integer $unixend
 *      End unixtime
 * @return
 *      array of time slots
 */
function booking_timeslots_unixtime2timeslots($unixstart, $unixend) {
  $DateTime_start = date("Y-m-d H:i:s", $unixstart); 
  $DateTime_end = date("Y-m-d H:i:s", $unixend); 
  var_dump($DateTime_start, $DateTime_end); 
}

/**
 * Generate array of dates from field within specified content type
 *
 * @param ?
 *      ?
 * @return
 *      array of dates
 */
function booking_timeslots_load_date_field($node, $field_name) {
  $field_conf = content_fields($field_name); // load configuration of fields
  $res = array();
  $endtime = '23:59:59';
  // TODO: multiple
  // TODO: todate: optional, required
  switch ($field_conf['type']) {
    case 'datetime':
      foreach ($node->{$field_name} as $key => $value) {
        if ($node->{$field_name}[$key]['value']) {
          $start = $node->{$field_name}[$key]['value'] ? strtotime($node->{$field_name}[$key]['value']) : 0;
          $end = $node->{$field_name}[$key]['value2'] ? strtotime($node->{$field_name}[$key]['value2']) : $start;
          if ($start == $end) {
            $hours = variable_get('booking_timeslot_length_hours', 1);
            $mins = variable_get('booking_timeslot_length_minutes', 0);
            $unix_slot = $hours * 3600 + $mins * 60;
            $end+= $unix_slot;
          }
          $res[] = implode('-', array($start, $end));
        }
      }
      break;
    case 'date':
      foreach ($node->{$field_name} as $key => $value) {
        if ($node->{$field_name}[$key]['value']) {
          $start = $node->{$field_name}[$key]['value'] ? strtotime($node->{$field_name}[$key]['value']) : 0;
          $end = $node->{$field_name}[$key]['value2'] ? strtotime($node->{$field_name}[$key]['value2'])+86400-60 : $start;
          $res[] = implode('-', array($start, $end));
        }
      }
      break;
  }
  return $res;
}

