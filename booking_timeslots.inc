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

/**
 * Validate datetime
 *
 * @param $datetime
 *      datetime to check (in `yyyy-mm-dd hh-mm-ss` format)
 * @return
 *      TRUE if datetime is in valid format
 */
function booking_timeslots_valid_datetime($datetime) {
  return preg_match("/^([0-9]{2,4})-([0-1]?[0-9])-([0-3]?[0-9]) (?:([0-2]?[0-9]):([0-5][0-9]):([0-5][0-9]))?$/", $datetime);
}

