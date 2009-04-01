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

  drupal_set_message('The configuration has been saved.');
  $form_state['redirect'] = 'admin/settings/booking_timeslots';
}
