<?php
/**
 * @file
 * booking_timeslots_example.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function booking_timeslots_example_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_booking_details|booking|booking|form';
  $field_group->group_name = 'group_booking_details';
  $field_group->entity_type = 'booking';
  $field_group->bundle = 'booking';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Booking Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_bt_ct_ref_1',
      1 => 'field_bt_ct_ref_2',
      2 => 'field_bt_ct_ref_3',
      3 => 'field_bt_ct_ref_4',
      4 => 'field_bt_number_of_people',
      5 => 'field_bt_user',
      6 => 'field_select_sport',
      7 => 'field_cost_event',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-booking-details field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_booking_details|booking|booking|form'] = $field_group;

  return $export;
}
