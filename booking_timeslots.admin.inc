<?php
// $Id

/**
 * Build the booking timeslots settings form.
 */
function booking_timeslots_settings_form(&$form_state) {

  $form['timeslot_length'] = array(
    '#type' => 'fieldset',
    '#title' => t('Timeslot Length'),
    '#description' => t('The default length of time of a timeslot.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  for ($i = 0; $i < 24; $i++) {
    $hour_options[] = $i;
  }

  $form['timeslot_length']['hours'] = array(
    '#type' => 'select',
    '#title' => t('Hours'),
    '#default_value' => variable_get('booking_timeslot_length_hours', 0),
    '#options' => $hour_options,
  );

  for ($i = 0; $i < 60; $i++) {
    $minute_options[] = $i;
  }

  $form['timeslot_length']['minutes'] = array(
    '#type' => 'select',
    '#title' => t('Minutes'),
    '#default_value' => variable_get('booking_timeslot_length_minutes', 0),
    '#options' => $minute_options,
  );
  
  $form['form_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form Settings'),
    '#description' => t('Form Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  

    $node_types = node_get_types('names'); // get content types
    $content_types = array_merge(array('' => t('<None>')), $node_types);

  // add node_form
  $form['form_settings']['ct_form_id'] = array(
    '#type' => 'select',
    '#title' => t('Content Type'),
    '#default_value' => variable_get('booking_timeslot_form_id', ''),
    '#options' => $content_types,
  );

  $form['form_settings']['ct_field_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Field Name'),
    '#default_value' => variable_get('booking_timeslot_field_name', ''),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
  );

  return $form;
}

/**
* General submit handler for Booking Timeslots configuration form.
*
* Assign values to db variables.
*
*/
function booking_timeslots_settings_form_submit($form, &$form_state) {
  variable_set('booking_timeslot_length_hours', $form_state['values']['hours']);
  variable_set('booking_timeslot_length_minutes', $form_state['values']['minutes']);
  variable_set('booking_timeslot_form_id', $form_state['values']['ct_form_id']);
  variable_set('booking_timeslot_field_name', $form_state['values']['ct_field_name']);

  drupal_set_message('The configuration has been saved.');
  $form_state['redirect'] = 'admin/settings/booking_timeslots';
}
