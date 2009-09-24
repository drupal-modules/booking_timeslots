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

/**
 *  Get title names from content type used for grouping
 */
function custom_timeslots_get_grouping_names() {
  $rows = array();
  if ($grouping_ct = variable_get('timeslot_schedule_grouping_ct', array())) {
    $result = db_query("SELECT nid, title FROM {node} WHERE type = '%s'", $grouping_ct);
    while ($row = db_fetch_object($result)) {
      if (node_access('view', node_load($row->nid)) == TRUE) {
        $rows[$row->nid] = $row->title;
      }
    }
  }
  return $rows;
}

/**
 *  Get available time slots from custom_timeslot content type
 *
 * @param integer $gnid
 *      nid of group to filter, if empty - show all slots
 * @param bool $show_nid
 *      if TRUE, show additional information about nid
 * @return
 *      return list of the slots
 */
function custom_timeslots_get_timeslots($gnid = NULL, $show_nid = FALSE, $sort = TRUE) {
  $rows = array();
  $slot_settings = array_flip(variable_get('custom_timeslot_slots_' . $gnid, array()));
  $result = db_query("SELECT nid FROM {node} WHERE type = '%s'", 'custom_timeslot');
  while ($row = db_fetch_object($result)) {
    if (empty($gnid) || empty($slot_settings) || array_key_exists($row->nid, $slot_settings)) {
      $node = node_load($row->nid);
      $rows[$row->nid] = $node->field_time_from[0]['value'] . ' - ' . $node->field_time_to[0]['value'] . ($show_nid == TRUE ? t(" (nid: %nid)", array('%nid' => $row->nid)) : '');
    }
  }
  if ($sort) {
    uasort($rows, 'custom_timeslots_sort_timeslots');
  }
  return $rows;
}

/**
 *  Implementation of sort callback
 *
 */
function custom_timeslots_sort_timeslots($a, $b) {
  return (strtotime(current(explode(' ', $a))) < strtotime(current(explode(' ', $b)))) ? -1 : 1;
}

/**
 *  Check datetime conflict between input values and nodes
 *
 * @param string $date
 *      date (format: yyyy-mm-dd)
 * @param string $time
 *      time (format: hh:mm)
 * @param string $ctype
 *      name of the content type
 * @return
 *      return node object if there is a conflict, otherwise FALSE
 */
function custom_timeslots_check_conflict($date, $time, $ctype) {
  $res = FALSE;
  $datetime_check = strtotime($date . ' ' . $time); /* convert input date to unixtime */
  $result = db_query("SELECT nid FROM node WHERE type= '%s' AND status = %d", $ctype, 1); 
  while ($rows = db_fetch_object($result)){
    foreach ($rows as $key => $nid){
      $node = node_load($nid);
      $field_name = booking_timeslots_get_field_name($ctype); /* get field name */
      $date_start = $node->{$field_name}[0]['value']; /* before timezone conversion */ // FIXME: hardcoded fields
      $date_end = $node->{$field_name}[0]['value']; /* before timezone conversion */ // FIXME: hardcoded fields
      $date_timezone = $node->{$field_name}[0]['timezone_db']; // FIXME: hardcoded fields
      $datetime_node = strtotime($date_start . ' ' . $date_timezone);
      if ($datetime_check == $datetime_node) {
        $res = $node; 
        break 2;
      }
    } // end: foreach
  } // end: while
  return $res;
}

