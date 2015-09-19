<?php
/**
 * @file
 * booking_timeslots_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function booking_timeslots_example_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function booking_timeslots_example_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function booking_timeslots_example_node_info() {
  $items = array(
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
  drupal_alter('node_info', $items);
  return $items;
}
