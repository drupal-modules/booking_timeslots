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
  $strongarm->name = 'field_bundle_settings_booking__booking';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'created' => array(
          'weight' => '0',
        ),
        'slot_time' => array(
          'weight' => '2',
        ),
        'view_link' => array(
          'weight' => '7',
        ),
        'edit_link' => array(
          'weight' => '8',
        ),
        'remove_link' => array(
          'weight' => '9',
        ),
        'btb_user' => array(
          'weight' => '1',
        ),
        'btb_ref_1st' => array(
          'weight' => '3',
        ),
        'btb_ref_2nd' => array(
          'weight' => '4',
        ),
        'btb_ref_3rd' => array(
          'weight' => '5',
        ),
        'btb_ref_4th' => array(
          'weight' => '6',
        ),
        'uid' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_booking__booking'] = $strongarm;

  return $export;
}
