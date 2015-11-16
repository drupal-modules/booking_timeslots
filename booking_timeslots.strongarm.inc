<?php
/**
 * @file
 * booking_timeslots.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function booking_timeslots_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_booking_type_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_booking_type_pattern'] = $strongarm;

  return $export;
}
