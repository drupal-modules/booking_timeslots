<?php
// $Id$

// author: kenorb@gmail.com

/**
 * @file
 * Booking Timeslots Functions.
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
  $new_datetime = gmdate('Y-m-d H:i:s', mktime($arr['hour'], $arr['minute'], $arr['second'], $arr['month'], $arr['mday'], $arr['year']));
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

/*
 * Get the last argument from the path
 *
 * @version 11/12/2008
 * @author kenorb@gmail.com
 */
function booking_timeslots_get_last_path_arg() {
  $arg = arg(); // Default to standard argument

  if (module_exists('path')) { // if path module exist, translate url from alias
    $path = drupal_get_path_alias($_GET['q']);
    $arg = explode('/', drupal_get_path_alias($path));
  }

  return $arg[count($arg)-1]; // get last argument of URI path
}

/*
 * Disable selected field in the form
 * You can use this function using form_alter hook.
 * i.e. _change_field_attr('field_somename', $form);
 *
 * @param $field_name (string)
 *   Name of the field in the form
 * @param $form (array)
 *   Form to change
 *
 * @author kenorb@gmail.com
 */
function _change_field_attr($field_name, &$form, $option = array('disabled' => 'disabled')) {
  $keyname = '#attributes';

  if (!isset($form[$field_name])) { // case: if field doesn't exists, put keyname in the main array
    $form[$keyname] = $option;
  }
  elseif (!isset($form[$field_name]['#type']) && is_array($form[$field_name])) { // case: if type not exist, find type from inside of array
    foreach ($form[$field_name] as $key => $value) {
      if (is_array($form[$field_name][$key]) && isset($form[$field_name][$key]['#type'])) {
        $curr_arr = &$form[$field_name][$key]; // set array as current
        $type = $form[$field_name][$key]['#type'];
        $curr_arr[$keyname] = $option;
      }
      elseif (is_array($form[$field_name][$key])) {
        foreach ($form[$field_name][$key] as $key_in => $value_in) {
          if (isset($form[$field_name][$key][$key_in]['#type'])) {
            $curr_arr = &$form[$field_name][$key][$key_in]; // set array as current
            $type = $form[$field_name][$key][$key_in]['#type'];
            $curr_arr[$keyname] = $option;
          }
        } // end: foreach
      } // end: else if
    } // end: foreach
  }
  else {
    $curr_arr = &$form[$field_name]; // set field array as current
    $curr_arr[$keyname] = $option;
  }
}

