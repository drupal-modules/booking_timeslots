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
      $rows[$row->nid] = $row->title;
    }
  }
  return $rows;
}

/**
 *  Get available time slots from custom_timeslot content type
 */
function custom_timeslots_get_timeslots() {
  $rows = array();
  $result = db_query("SELECT nid FROM {node} WHERE type = '%s'", 'custom_timeslot');
  while ($row = db_fetch_object($result)) {
    $node = node_load($row->nid);
    $rows[$row->nid] = $node->field_time_from[0]['value'] . ' - ' . $node->field_time_to[0]['value'];
  }
  return $rows;
}

/**
 *  Check datetime conflict between input values and nodes
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

