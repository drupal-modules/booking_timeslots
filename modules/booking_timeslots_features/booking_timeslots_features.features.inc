<?php
/**
 * @file
 * booking_timeslots_features.features.inc
 */

/**
 * Implements hook_views_api().
 */
function booking_timeslots_features_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function booking_timeslots_features_node_info() {
  $items = array(
    'booking_slot' => array(
      'name' => t('Booking Slot'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'booking_timeslots_xmp_class' => array(
      'name' => t('Booking Timeslots Example Class'),
      'base' => 'node_content',
      'description' => t('Booking Timeslots: Example Tertiary Content Type'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'booking_timeslots_xmp_facility' => array(
      'name' => t('Booking Timeslots Example Facility'),
      'base' => 'node_content',
      'description' => t('Booking Timeslots: Example Secondary Content Type'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'booking_timeslots_xmp_instructor' => array(
      'name' => t('Booking Timeslots Example Instructor'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'booking_timeslots_xmp_venue' => array(
      'name' => t('Booking Timeslots Example Venue'),
      'base' => 'node_content',
      'description' => t('Booking Timeslots: Example Primary Content Type'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
