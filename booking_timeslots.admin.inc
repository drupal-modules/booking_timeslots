<?php

/**
 * @file
 *   Form settings include file
 *
 * @version
 *   $Id$
 *
 * @developers
 *   Rafal Wieczorek <kenorb@gmail.com>
 */


/**
 *  Menu callback for the settings form.
 */
function booking_timeslots_settings_timeslots_form(&$form_state) {
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

  $form['timeslot_length']['booking_timeslot_available_slots'] = array(
    '#type' => 'select',
    '#title' => t('Maximum avaliable slots'),
    '#default_value' => variable_get('booking_timeslot_available_slots', 1),
    '#options' => $avaliable_slots,
    '#description' => t('Choose 0 to disable checking for slot availability'),
  );

   $form['timeslot_length']['booking_timeslot_hour_from'] = array(
    '#type' => 'select',
    '#title' => t('Hour from'),
    '#default_value' => variable_get('booking_timeslot_hour_from', 8),
    '#options' => $hour_options,
  );

  $form['timeslot_length']['booking_timeslot_hour_to'] = array(
    '#type' => 'select',
    '#title' => t('Hour to'),
    '#default_value' => variable_get('booking_timeslot_hour_to', 18),
    '#options' => $hour_options,
  );
  
  $form['timeslot_length']['booking_timeslot_not_avaliable'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Weekdays which are not available for booking'),    
    '#default_value' => variable_get('booking_timeslot_not_avaliable', array()),
    '#options' => array(
        'monday' => t('Monday'),
        'tuesday'=> t('Tuesday'),
        'wednesday' => t('Wednesday'),
        'thursday' => t('Thursday'),
        'friday' => t('Friday'),
        'saturday' => t('Saturday'),
        'Sun' => t('Sunday')
    )
  );
   
   $form['#validate'][] = 'booking_timeslots_form_validate';

  return system_settings_form($form);
}

/**
 *  Menu callback for the settings form.
 */
function booking_timeslots_settings_form(&$form_state){

   $form['form_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form Settings'),
    '#description' => t('Form Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (module_exists('date')) {
    $field_list = array();
    foreach( content_fields() as $field_name => $data ) {
      if( $data['type'] == 'datetime' || $data['type'] == 'date') { // TODO: 'date' field types are not tested yet
        $field_list[$field_name] = $data['type_name'] . ' => ' . $field_name;
        $content_types[$data['type_name']] = $data['type_name'];
      }
    }
    $form['form_settings']['booking_timeslot_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Field Names'),
      '#description' => t('Select field names which are handled automatically by the module.'),
      '#default_value' => $fields = variable_get('booking_timeslot_fields', array()),
      '#options' => $field_list,
      '#required' => TRUE,
    );
    if (empty($field_list)) {
      drupal_set_message(t('Please add at least one CCK %type field into your content type.', array('%type' => 'datetime')), 'error');
    } else if (empty($fields)) {
      drupal_set_message(t('Please select at least one date field.'), 'warning');
    }
  
  } else {
    $content_types = node_get_types('names');
    drupal_set_message(t('Please install %module module for fields settings.', array('%module' => 'Date')), 'error');
  }

  $form['form_settings']['booking_timeslot_excluded_dates'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Fields with non-available dates'),
    '#description' => t('Select fields which contain non-available dates.'),
    '#default_value' => variable_get('booking_timeslot_excluded_dates', array()),
    '#options' => $field_list,
  );


  $options = array();

  $views = views_get_all_views();

  $list_views = array();
  foreach ($views as $view) {
    if (!empty($view->disabled)) continue;
    if (strpos($view->display['default']->display_options['style_plugin'], 'calendar') !== FALSE) {
      $list_views[$view->name] = $view->name; 
    }
  }

  foreach ($list_views as $key => $value) {
    $form['form_settings']['booking_timeslot_view_' . $key] = array(
        '#type' => 'select',
        '#title' => t('View: %view', array('%view' => $key)),
        '#description' => t('Choose which content type should be associated with your calendar view.'),
        '#default_value' => variable_get('booking_timeslot_view_' . $key, ''),
        '#options' => array_merge(array('' => t('- None -')), $content_types),
      );
  }

  return system_settings_form($form);
}

/**
 * Form API callback to validate the settings form.
 */
function booking_timeslots_form_validate($form, &$form_state){
  $hour_from = $form['timeslot_length']['booking_timeslot_hour_from']['#value'];
  $hour_to = $form['timeslot_length']['booking_timeslot_hour_to']['#value'];

  if ($hour_from>=$hour_to){
    form_set_error('booking_timeslot_hour_from', t("Hour To should be greater than Hour From."));
    form_set_error('booking_timeslot_hour_to',t(' '));
  }
}

