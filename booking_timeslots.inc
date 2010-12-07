<?php

/**
 * @file
 *   Booking Timeslots Functions.
 *
 * @version
 *   $Id$
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
    $timeslot_fields = variable_get('booking_timeslot_fields', array());

    $slots = db_query('SELECT nid FROM {node} WHERE type = "%s"', $nodeType);
    while ($row = db_fetch_array($slots))
    {
        $slot = node_load($row['nid']);
        foreach ($timeslot_fields as $func_name)
        {
            if (!isset($slot->$func_name))
                    continue;

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
  $view_name = arg(0);
  // $configured = db_result(db_query("SELECT COUNT(*) FROM {variable} WHERE name LIKE 'booking_timeslot_view_%s'", $view_name)) !== FALSE;
  return views_get_view($view_name) ? $view_name : FALSE; // FIXME: any view_exists instead of views_get_view?
}

/**
 * Detect configured Datetime field assosiated with specified View
 *
 */
function booking_timeslot_detect_field_from_view($view) {
  $field_name = FALSE;
  if (!is_object($view)) {
    $view = views_get_view($view);
  }
  $my_fields = array_flip(variable_get('booking_timeslot_fields', array()));
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
    drupal_set_message(t('Booking Timeslots: Your View has missing date arguments!'));
  }
  return $field_name;
}

/**
 * Get proper content type name or link for party booking
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
  $my_fields = array_flip(variable_get('booking_timeslot_fields', array()));

  $arg0 = arg(0);
  if (!isset($content_type_cache[$arg0])) {
    $view_name = booking_timeslot_detect_view(); // get view from URL
    if ($field_name = booking_timeslot_detect_field_from_view($view_name)) { // detect field name via View
      $content_type = booking_timeslot_content_type_via_field($field_name); // detect content type via field name
      $content_type_cache[$view_name] = $content_type;
    } else {
      drupal_set_message(t("Booking time slots are not properly configured for this page, please contact your site administrator!"), 'error');
    }
  } else {
    $content_type = $arg0;
  }

  return $link ? "node/add/" . str_replace('_', '-', $content_type) : $content_type;
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

