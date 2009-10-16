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
function booking_timeslots_check_slot_availability($date = NULL) {
  if (empty($date)) {
    $date = end(arg());
  }

  list($date, $time) = split(' ', $date);
  $rows = array(
    'date' => $date,
    'datebox' => '',
    'empty' => '',
    'link' => '',
    'all_day' => array(),
    'items' => array(),
  );

  $page_name = arg(2);

  $timeslot_fields = variable_get('booking_timeslot_fields', array());

  $slots = db_query('SELECT nid FROM {node} WHERE type = "'.$page_name.'"');
  while ($row = db_fetch_array($slots)) {
    $slot = node_load($row['nid']);
    foreach ($timeslot_fields as $func_name) {
      if (!isset($slot->$func_name)) continue;
      if (strtotime($slot->{$func_name}[0]['value'] . ' ' . $slot->{$func_name}[0]['timezone_db']) == strtotime($date . ' ' . $time)) {
        $rows['items'][$time]['values']['Items'][] = '';
        $rows['items'][$time]['hour'] = substr($time,0,-3);
        $rows['items'][$time]['ampm'] = '';
      }
    }
  }

  $q = db_query("SELECT * FROM {variable} WHERE name LIKE 'booking_timeslot_view_%%'");
  while ($row = db_fetch_array($q)) {
    if (unserialize($row['value']) == $page_name) {
      $view_name = substr($row['name'],strlen('booking_timeslot_view_'));
      break;
    }
  }

  $view = views_get_view($view_name);
  $columns = array(0 => 'Items');

  $view = (object)$view->display['calendar_period_3']->display_options;

  if (
    ($theme_file = drupal_get_path('theme', variable_get('theme_default','none')) . '/calendar-day.tpl.php')
    || ($theme_file = drupal_get_path('module', 'booking_timeslots' . '/themes/calendar-day.tpl.php'))
  ) {
    ob_start();
    require_once($theme_file);
    ob_end_clean();
  }

  return $available_slots ? $available_slots[$time] : TRUE;
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
  $my_forms = variable_get('booking_timeslot_forms', array());
  $form_name = key(array_flip($my_forms));
  $my_fields = array_flip(variable_get('booking_timeslot_fields', array()));
  unset($my_fields[0]);
  $my_fields = key($my_fields);
  /* detect content type name */
  $content_types = content_types();
  if ($my_form_id = $_SESSION['booking_timeslot_ct_' . arg(0)] !== FALSE) {
    foreach ($my_forms as $my_form_key => $my_form_id) {  // find associated content type with field
        if (isset($content_types[$my_form_key]['fields'][$my_fields]) && !empty($my_form_key)) { // if field exist in this content type...
          $_SESSION['booking_timeslot_ct_' . arg(0)] = $my_form_id; /// associate this content type with base path for futher use
          break;
        }
    }
  }
  return $link ? "node/add/" . $my_form_id : $my_form_id;
}

/**
 * Get field name used for booking timeslots
 *
 * TODO:
 *   name support for multi-fields and filter only those who are related to specified content type
 *
 * @param string $content_type
 *      name of content type which is related to booking timeslots
 * @return
 *      name of the field
 */
function booking_timeslots_get_field_name($content_type = NULL) {
  // empty($content_type) ? $content_type = booking_timeslots_get_ctype_name() : NULL; /* not used for now */
  //       $fields = content_fields(); /* not used for now */
  $my_fields = array_flip(variable_get('booking_timeslot_fields', array()));
  unset($my_fields[0]);
  return key($my_fields);
}

