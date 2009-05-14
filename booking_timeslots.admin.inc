<?php
// $Id

/**
 *  Menu callback for the settings form.
 */
function booking_timeslots_settings_form(&$form_state) {

  $form['timeslot_length'] = array(
    '#type' => 'fieldset',
    '#title' => t('Timeslot settings'),
    '#description' => t('The default length of time of a timeslot.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  for ($i = 0; $i < 24; $i++) {
    $hour_options[] = $i;
  }

  $form['timeslot_length']['booking_timeslot_length_hours'] = array(
    '#type' => 'select',
    '#title' => t('Hours'),
    '#default_value' => variable_get('booking_timeslot_length_hours', 1),
    '#options' => $hour_options,
  );

  for ($i = 0; $i < 60; $i++) {
    $minute_options[] = $i;
  }

  $form['timeslot_length']['booking_timeslot_length_minutes'] = array(
    '#type' => 'select',
    '#title' => t('Minutes'),
    '#default_value' => variable_get('booking_timeslot_length_minutes', 0),
    '#options' => $minute_options,
  );

  for ($i = 0; $i < 10; $i++) {
    $avaliable_slots[] = $i;
  }

  $form['timeslot_length']['booking_timeslot_avaliable_slots'] = array(
    '#type' => 'select',
    '#title' => t('Maximum avaliable slots'),
    '#default_value' => variable_get('booking_timeslot_avaliable_slots', 1),
    '#options' => $avaliable_slots,
    '#description' => t('Choose 0 to disable checking for slot availability'),
  );
  
  
  $form['form_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form Settings'),
    '#description' => t('Form Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['form_settings']['booking_timeslot_forms'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content Types'),
    '#default_value' => variable_get('booking_timeslot_forms', array()),
    '#options' => node_get_types('names'),
  );
  
  if (module_exists('date')) {
    $field_list = array();
    foreach( content_fields() as $field_name => $data ) {
      if( $data['type'] == 'datetime' ) {
        $field_list[$field_name] = $field_name;
      }
    }

    $form['form_settings']['booking_timeslot_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Field Names'),
      '#default_value' => variable_get('booking_timeslot_fields', ''),
      '#options' => $field_list,
    );
  } else {
    drupal_set_message(t('Please activate %module module for fields settings.', array('%module' => 'Date')));
  }

  return system_settings_form($form);
}

