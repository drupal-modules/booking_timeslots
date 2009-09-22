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
function custom_timeslots_settings_form(&$form_state) {
  module_load_include('inc', 'custom_timeslots');
  $form['custom_timeslot'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom timeslot settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['custom_timeslot']['timeslot_schedule_grouping_ct'] = array(
    '#type' => 'select',
    '#title' => t('Content Type for slot grouping'),
    '#description' => t('Optionally select which content types will be using for grouping.'),
    '#default_value' => variable_get('timeslot_schedule_grouping_ct', array()),
    '#options' => array_merge(array('' => t('- None -')), node_get_types('names')), // get content types
  );

  if (variable_get('timeslot_schedule_grouping_ct', array())) {
    $data_groups = custom_timeslots_get_grouping_names();
    $data_slots = custom_timeslots_get_timeslots(); /* 2nd level */
    if (empty($data_groups)) {
      drupal_set_message(t('Please add at least one timeslot group!'), 'warning');
    } else if (empty($data_slots)) {
      drupal_set_message(t('Please add at least one custom timeslot!'), 'warning');
    } else {
      foreach ($data_groups as $nid => $group_name ) {
        $form['custom_timeslot'][$group_name] = array(
          '#title' => $group_name,
          '#type' => 'fieldset',
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
        );
        $form['custom_timeslot'][$group_name]['custom_timeslot_slots_' . $nid] = array(
          '#title' => t('Available slots for group: %group_name', array('%group_name' => $group_name)),
          '#type' => 'checkboxes',
          '#options' => drupal_map_assoc($data_slots),
          '#default_value' => variable_get('custom_timeslot_slots_' . $nid, drupal_map_assoc($data_slots)),
        );
      }
    }
  }
    
  return system_settings_form($form);
}

/**
 * Form API callback to validate the settings form.
 */
function custom_timeslots_settings_form_validate($form, &$form_state){
  /*
  $hour_from = $form['timeslot_length']['booking_timeslot_hour_from']['#value'];
  $hour_to = $form['timeslot_length']['booking_timeslot_hour_to']['#value'];
   */
/*
  if ($hour_from>=$hour_to){
    form_set_error('booking_timeslot_hour_from', t("Hour To should be greater than Hour From."));
    form_set_error('booking_timeslot_hour_to',t(' '));
  }
 */
}

/**
 *  Menu callback for the settings form.
 */
function custom_timeslots_schedule_node_form(&$form_state) {
  $form['timeslot_schedule'] = array(
    '#type' => 'fieldset',
    '#title' => t('Timeslot schedule'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['timeslot_schedule']['timeslot_schedule_prev_date'] = array(
    '#value' => t('Previous day'),
    '#type' => 'submit',
    //'#attributes' => array('OnClick' => 'alert(1)'),
  );

  $form['timeslot_schedule']['timeslot_schedule_next_date'] = array(
    '#value' => t('Next day'),
    '#type' => 'submit',
  );

  $form['timeslot_schedule']['timeslot_schedule_date'] = array(
    '#title' => t('Date'),
    '#type' => module_exists('date_popup') ? 'date_popup' : 'date_select',
    '#default_value' => date_format(date_create(), 'Y-m-d'), 
    '#date_timezone' => date_default_timezone_name(), 
    '#date_format' => date_limit_format(variable_get('date_format_short', 'm/d/Y'), array('year', 'month', 'day')), 
  );

  $form['timeslot_schedule']['timeslot_schedule_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
  );

  return $form;
}

/**
 *  Callback from submited form.
 */
function custom_timeslots_schedule_node_form_submit($form, &$form_state) {
  $date = $form['#post']['timeslot_schedule_date']['date'];
  // $date = ; // FIXME: convert date to Y-m-d format
  switch($form_state['values']['op']) {
    case t('Previous day'):
      // $yesterday = ; // FIXME: set previous day than it's currently in date
      drupal_goto(arg(0), 'curr_date=' . $yesterday);
    break;
    case t('Next day'):
      // $tomorrow = ; // FIXME: set next day than it's currently in date
      drupal_goto(arg(0), 'curr_date=' . $tomorrow);
    break;
    case t('Apply'):
      drupal_goto(arg(0), 'curr_date='.$date);
    break;
  }
}

/**
 *  Menu callback for the settings form.
 */
function custom_timeslots_schedule_table() {
  module_load_include('inc', 'custom_timeslots');
  module_load_include('inc', 'booking_timeslots');

  $form_state = array();
  $output = drupal_get_form('custom_timeslots_schedule_node_form', $form_state);


  $headers = array(); 
  if (!$data_groups = custom_timeslots_get_grouping_names()) {
    $data_groups[] = ''; /* set empty slot if there is no grouping */
  }
  foreach ($data_groups as $key => $value) {
    $data = array($value); /* 1st column - group name */
    $data_slots = custom_timeslots_get_timeslots(); /* FIXME: each slot to each group */
    foreach ($data_slots as $nid => $slot_title) {
      $set_date = $_GET['curr_date'] ? $_GET['curr_date'] : date_format(date_create(), 'Y-m-d');
      $set_time = node_load($nid)->field_time_from[0]['value']; /* those fields should exist in our imported content type */
      $set_end_time = node_load($nid)->field_time_to[0]['value']; /* those fields should exist in our imported content type */
      $link = booking_timeslots_get_ctype_name(arg(2), TRUE) . '/' . $set_date . ' ' . $set_time . '/' .  $set_date . ' ' . $set_end_time; /* if arg 2 provided, then change default content type */
      if ($node = custom_timeslots_check_conflict($set_date, $set_time, booking_timeslots_get_ctype_name(arg(2)))) {
        $slot_code = node_view($node, TRUE, TRUE);
      } else {
        $slot_code = "<a href='$link'>" . $slot_title . '</a>';
      }
      $data = array_merge($data, array($slot_code));
    } 
    $data_groups[$key] = $data;
  }

  $output .= theme('table', $headers, $data_groups, array('id' => 'custom_timeslots_schedule_table', 'border' => '1'));

  return $output;
}

