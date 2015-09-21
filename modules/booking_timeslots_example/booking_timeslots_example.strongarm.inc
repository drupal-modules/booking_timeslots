<?php
/**
 * @file
 * booking_timeslots_example.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function booking_timeslots_example_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'additional_settings__active_tab_booking_timeslots_xmp_class';
  $strongarm->value = 'edit-opening-hours';
  $export['additional_settings__active_tab_booking_timeslots_xmp_class'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'additional_settings__active_tab_booking_timeslots_xmp_facility';
  $strongarm->value = 'edit-opening-hours';
  $export['additional_settings__active_tab_booking_timeslots_xmp_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'admin_theme_opening_hours_opening_hours';
  $strongarm->value = 1;
  $export['admin_theme_opening_hours_opening_hours'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_1st_content_type';
  $strongarm->value = 'booking_timeslots_xmp_venue';
  $export['booking_timeslots_1st_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_1st_title';
  $strongarm->value = 'Primary';
  $export['booking_timeslots_1st_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_2nd_content_type';
  $strongarm->value = 'booking_timeslots_xmp_facility';
  $export['booking_timeslots_2nd_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_2nd_title';
  $strongarm->value = 'Secondary';
  $export['booking_timeslots_2nd_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_2nd_to_1st_ref_field';
  $strongarm->value = 'field_bt_xmp_ref_2nd_to_1st';
  $export['booking_timeslots_2nd_to_1st_ref_field'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_3rd_capacity_field';
  $strongarm->value = 'field_bt_xmp_capacity';
  $export['booking_timeslots_3rd_capacity_field'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_3rd_content_type';
  $strongarm->value = 'booking_timeslots_xmp_class';
  $export['booking_timeslots_3rd_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_3rd_title';
  $strongarm->value = 'Tertiary';
  $export['booking_timeslots_3rd_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_3rd_to_2nd_ref_field';
  $strongarm->value = 'field_bt_xmp_ref_3rd_to_2nd';
  $export['booking_timeslots_3rd_to_2nd_ref_field'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_3rd_to_4th_ref_field';
  $strongarm->value = 'field_bt_xmp_ref_3rd_to_4th';
  $export['booking_timeslots_3rd_to_4th_ref_field'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_4th_content_type';
  $strongarm->value = 'booking_timeslots_xmp_instructor';
  $export['booking_timeslots_4th_content_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_4th_title';
  $strongarm->value = 'Quaternary';
  $export['booking_timeslots_4th_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_calendar_granularity';
  $strongarm->value = '15';
  $export['booking_timeslots_calendar_granularity'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_category_ct';
  $strongarm->value = '2';
  $export['booking_timeslots_category_ct'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_category_term_ref_field';
  $strongarm->value = '0';
  $export['booking_timeslots_category_term_ref_field'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_durations';
  $strongarm->value = '15|15 min
30|30 min
45|45 min
60|60 min
120|120 min';
  $export['booking_timeslots_durations'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_enable_booking_for_past_dates';
  $strongarm->value = 1;
  $export['booking_timeslots_enable_booking_for_past_dates'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_filter_title_category';
  $strongarm->value = 'Category';
  $export['booking_timeslots_filter_title_category'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_view_booking_timeslots';
  $strongarm->value = '';
  $export['booking_timeslots_view_booking_timeslots'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_view_booking_timeslots_xmp_schedule';
  $strongarm->value = 'booking_timeslots_xmp_venue';
  $export['booking_timeslots_view_booking_timeslots_xmp_schedule'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'booking_timeslots_view_bt_schedule';
  $strongarm->value = 'booking_timeslots_xmp_venue';
  $export['booking_timeslots_view_bt_schedule'] = $strongarm;

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

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__booking_timeslots_xmp_class';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
        'workflow' => array(
          'weight' => '10',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__booking_timeslots_xmp_class'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__booking_timeslots_xmp_facility';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
        'workflow' => array(
          'weight' => '10',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__booking_timeslots_xmp_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__booking_timeslots_xmp_venue';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
        'workflow' => array(
          'weight' => '10',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__booking_timeslots_xmp_venue'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_booking_timeslots_xmp_class';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_booking_timeslots_xmp_class'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_booking_timeslots_xmp_facility';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_booking_timeslots_xmp_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_booking_timeslots_xmp_venue';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_booking_timeslots_xmp_venue'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_booking_timeslots_xmp_class';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_booking_timeslots_xmp_class'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_booking_timeslots_xmp_facility';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_booking_timeslots_xmp_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_booking_timeslots_xmp_venue';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_booking_timeslots_xmp_venue'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_booking_timeslots_xmp_class';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_booking_timeslots_xmp_class'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_booking_timeslots_xmp_facility';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_booking_timeslots_xmp_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_booking_timeslots_xmp_venue';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_booking_timeslots_xmp_venue'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_booking_timeslots_xmp_class';
  $strongarm->value = '1';
  $export['node_preview_booking_timeslots_xmp_class'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_booking_timeslots_xmp_facility';
  $strongarm->value = '1';
  $export['node_preview_booking_timeslots_xmp_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_booking_timeslots_xmp_venue';
  $strongarm->value = '1';
  $export['node_preview_booking_timeslots_xmp_venue'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_booking_timeslots_xmp_class';
  $strongarm->value = 1;
  $export['node_submitted_booking_timeslots_xmp_class'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_booking_timeslots_xmp_facility';
  $strongarm->value = 1;
  $export['node_submitted_booking_timeslots_xmp_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_booking_timeslots_xmp_venue';
  $strongarm->value = 1;
  $export['node_submitted_booking_timeslots_xmp_venue'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'opening_hours_enabled_booking_timeslots_xmp_class';
  $strongarm->value = 1;
  $export['opening_hours_enabled_booking_timeslots_xmp_class'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'opening_hours_enabled_booking_timeslots_xmp_facility';
  $strongarm->value = 1;
  $export['opening_hours_enabled_booking_timeslots_xmp_facility'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'opening_hours_enabled_booking_timeslots_xmp_venue';
  $strongarm->value = 1;
  $export['opening_hours_enabled_booking_timeslots_xmp_venue'] = $strongarm;

  return $export;
}
