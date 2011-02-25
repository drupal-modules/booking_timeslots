<?php

/**
 * @file
 *   Form settings include file
 *
 * @version
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
function booking_timeslots_settings_form(&$form_state) {

   $form['cck'] = array(
    '#type' => 'fieldset',
    '#title' => t('CCK integration'),
    '#description' => t('Here you can select which fields you want to use for time slots.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (module_exists('date')) {
    $field_list = array();
    foreach( content_fields() as $field_name => $data ) {
      if( $data['type'] == 'datetime' || $data['type'] == 'date') { // TODO: 'date' field types are not tested yet
        $field_list[$field_name] = $field_name . ' (' . t('Content type: ') . $data['type_name'] . ')';
        $content_types[$data['type_name']] = $data['type_name'];
      }
    }
    $form['cck']['booking_timeslot_fields'] = array(
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
      drupal_set_message(t('Please select at least one date or datetime field where you want to store booking times.'), 'warning');
    }
  
  } else {
    $content_types = node_get_types('names');
    drupal_set_message(t('Please install %module module for fields settings.', array('%module' => 'Date')), 'error');
  }

  $form['cck']['booking_timeslot_excluded_dates'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Fields with non-available dates (optional)'),
    '#description' => t('Select fields which contain non-available dates. Note: Use this option only when you want to limit availability of your time slots.'),
    '#default_value' => variable_get('booking_timeslot_excluded_dates', array()),
    '#options' => $field_list,
  );

  $form['views'] = array(
    '#type' => 'fieldset',
    '#title' => t('Views integration'),
    '#description' => t('Here you can select which views are used for time slots. You have to choose at least one view to use booking time slots properly.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  /* FIND ALL VIEWS WITH HAS CALENDAR AS A STYLE */
  $list_views = array();
  foreach (views_get_all_views() as $view) {
    if (!empty($view->disabled)) continue;
    if (strpos($view->display['default']->display_options['style_plugin'], 'calendar') !== FALSE) {
      $list_views[$view->name] = $view->name; 
    }
  }

  /* CHECK IF DEFAULT CALENDAR VIEW IS ENABLED */
  if (empty($list_views)) {
    drupal_set_message(t('Please enable at least one calendar based view i.e. <a href="@url">Calendar View.</a>', array('@url' => url('admin/build/views/list'))), 'warning');
  } else {
    foreach ($list_views as $key => $value) {
      $form['views']['booking_timeslot_view_' . $key] = array(
          '#type' => 'select',
          '#title' => t('View: %view', array('%view' => $key)),
          '#description' => t('Choose which content type should be associated with your calendar view.'),
          '#default_value' => variable_get('booking_timeslot_view_' . $key, ''),
          '#options' => array_merge(array('' => t('- None -')), $content_types),
        );
    }
  }

  $form['other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['other']['booking_timeslot_show_warnings'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't show configuration warnings, etc. on admin pages."),
    '#description' => t('If you configured module correctly or you customized already some settings, some warnings are no longer necessary. Check to disable them.'),
    '#default_value' => variable_get('booking_timeslot_show_warnings', FALSE),
  );

  !variable_get('booking_timeslot_show_warnings', FALSE) ?  booking_timeslots_check_admin_options() : NULL; // Check for configuration issues
  $form['#validate'][] = 'booking_timeslots_settings_form_validate';
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

/**
 * Form API callback to validate the admin settings form.
 */
function booking_timeslots_settings_form_validate($form, &$form_state){
  $values = $form_state['values'];

  /* MAKE THE LIST OF ACTIVATED VIEWS */
  $enabled_views = array();
  preg_match_all("/booking_timeslot_view_\w+/", join(",", array_keys($values)), $names); // find all interval array keys from the submitted form
  foreach ((array)$names[0] as $key => $vkey) { // for each key, find the items
    $view_name = substr($vkey, strlen('booking_timeslot_view_'));
    $content_name = $values[$vkey]; // find interval for found key from form
    $enabled_views[$view_name] = $content_name;
    if (!empty($content_name)) { // if view is associated with content type...
      module_load_include('inc', 'booking_timeslots');
      if (!($field_name = booking_timeslot_detect_field_from_view($view_name))) { // check if that view has proper configuration
        form_set_error($vkey, t("Please configure <a href='@url'>%name view</a> by selecting proper fields which you chosen for time slots, before activating the view!", array('@url' => url("admin/build/views/nojs/config-item/$view_name/default/argument/date_argument"), '%name' => $view_name)));
      }
    }
  }
  $enabled_views = array_filter($enabled_views); // filter empty settings
  empty($enabled_views) && !variable_get('booking_timeslot_show_warnings', FALSE) ?  drupal_set_message("Proper configuration require at least one view associated with a content type!", 'warning') : NULL; // Check for configuration issues
}

/**
 * General function to check admin settings
 * Check if tpl files are in the right place
 * Note: works only with default calendar view
 */
function booking_timeslots_check_admin_options(){
  /* CHECK IF TPL TEMPLATE HAS BEEN COPIED INTO THEME FOLDER */
  $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
  if (!file_exists($theme_path . "/calendar-month.tpl.php") || !file_exists($theme_path . "/calendar-day.tpl.php")) {
    drupal_set_message(t('Please copy theme files from module themes/ folder into your theme path: @theme_path to see changes in calendar view and clear the cache.', array('@theme_path' => $theme_path)), 'warning');
  }
  $hooks = ($registry = theme_get_registry()) ? $registry : init_theme() && theme_get_registry(); // load theme hooks
  if ($hooks && (!array_search('template_preprocess_drupal_tweaks_day_node', $hooks['calendar_day_node']['preprocess functions']) !== FALSE)) { // check or preprocess function
    drupal_set_message(t('Please <a href="@url">clear the cache</a>!', array('@url' => url('admin/settings/performance'))), 'warning');
  }
}

