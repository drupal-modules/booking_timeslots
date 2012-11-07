<?php
/**
 * @file
 * booking_timeslots_features.features.inc
 */

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
  );
  return $items;
}
