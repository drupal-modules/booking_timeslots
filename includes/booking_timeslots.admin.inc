<?php

/**
 * @file
 *   Form settings include file
 *
 * @version
 *   $Id:$
 *
 * @developers
 *   Rafal Wieczorek <kenorb+nospam@gmail.com>
 */

/**
 *  Menu callback for the settings form.
 */
function booking_timeslots_settings_timeslots_form($form_state) {
  /*
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

  $form['timeslot_length']['booking_timeslots_length_hours'] = array(
    '#type' => 'select',
    '#title' => t('Hours'),
    '#default_value' => variable_get('booking_timeslots_length_hours', 1),
    '#options' => $hour_options,
  );

  for ($i = 0; $i < 60; $i++) {
    $minute_options[] = $i;
  }

  $form['timeslot_length']['booking_timeslots_length_minutes'] = array(
    '#type' => 'select',
    '#title' => t('Minutes'),
    '#default_value' => variable_get('booking_timeslots_length_minutes', 0),
    '#options' => $minute_options,
  );

  for ($i = 0; $i < 10; $i++) {
    $avaliable_slots[] = $i;
  }

  $form['timeslot_length']['booking_timeslots_available_slots'] = array(
    '#type' => 'select',
    '#title' => t('Maximum avaliable slots per timeframe'),
    '#default_value' => variable_get('booking_timeslots_available_slots', 1),
    '#options' => $avaliable_slots,
    '#description' => t('How many slots are available to book at the same time. Choose 0 to disable checking for slot availability.'),
  );

   $form['timeslot_length']['booking_timeslots_hour_from'] = array(
    '#type' => 'select',
    '#title' => t('Hour from'),
    '#default_value' => variable_get('booking_timeslots_hour_from', 8),
    '#options' => $hour_options,
    '#description' => t('Default starting hour.'),
  );

  $form['timeslot_length']['booking_timeslots_hour_to'] = array(
    '#type' => 'select',
    '#title' => t('Hour to'),
    '#default_value' => variable_get('booking_timeslots_hour_to', 18),
    '#options' => $hour_options,
    '#description' => t('Default ending hour.'),
  );

  $form['timeslot_length']['booking_timeslots_not_avaliable'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Weekdays which are not available for booking'),
    '#default_value' => variable_get('booking_timeslots_not_avaliable', array()),
    '#options' => array(
        'monday' => t('Monday'),
        'tuesday' => t('Tuesday'),
        'wednesday' => t('Wednesday'),
        'thursday' => t('Thursday'),
        'friday' => t('Friday'),
        'saturday' => t('Saturday'),
        'sunday' => t('Sunday')
    )
  );
    */

  $form['#validate'][] = 'booking_timeslots_form_validate';
  return system_settings_form($form);
}

/**
 *  Menu callback for the settings form.
 */
function booking_timeslots_settings_form($form, $form_state) {

  if (variable_get('booking_timeslots_1st_content_type', 'UNKNOWN') !== 'UNKNOWN') {
    if (!$form_state['input']) {
      // It's normal form display callback.
      // Form has been saved at least once, we may display status.
      booking_timeslots_check_configuration_validity();
    }
  }

  $form['entity'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entity integration'),
    '#description' => t('Here you can choose which entities you want to integrate with booking time slots.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $content_types = drupal_map_assoc(array_keys(node_type_get_types()));
  ksort($content_types);
  $content_types = array(0 => '(None)') + $content_types;

  foreach(field_info_fields() as $name => $info) {

    $bundles = array();

    if ($info['bundles']) {
      $bundles = call_user_func_array('array_merge', $info['bundles']);
    }

    $bundles = implode(', ', $bundles);
    $title   = $name;

    if ($bundles) {
      $title .= ' (' . $bundles . ')';
    }

    $fields[$name] = $title;
  }

  ksort($fields);
  $fields = array(0 => '(None)') + $fields;

  $term_reference_fields = array(0 => '(None)');

  foreach(field_info_fields() as $name => $info) {

    if ($info['type'] != 'taxonomy_term_reference') {
      continue;
    }

    $bundles = array();

    if ($info['bundles']) {
      $bundles = call_user_func_array('array_merge', $info['bundles']);
    }

    $bundles = implode(', ', $bundles);

    $title = $name;

    if ($bundles) {
      $title .= ' (' . $bundles . ')';
    }

    $term_reference_fields[$name] = $title;
  }


  //$fields += drupal_map_assoc(array_keys(field_get_items('node')));

  $form['entity']['booking_timeslots_category_term_ref_field'] = array(
    '#type' => 'select',
    '#title' => t('Category field'),
    '#default_value' => variable_get('booking_timeslots_category_term_ref_field', ''),
    '#options' => $term_reference_fields,
    '#description' => t('Taxonomy vocabulary term reference field used to categorize selected content type. Select None to disable the feature. <u>Must be of type <b>Term reference</b>.</u>.'),
  );

  $form['entity']['booking_timeslots_category_ct'] = array(
    '#type' => 'select',
    '#title' => t('Content type associated with this category field'),
    '#default_value' => variable_get('booking_timeslots_category_ct', '1'),
    '#options' => array(
      '2' => 'Secondary',
     ),
    '#description' => t('Select content type which contain above category field.'),
  );

  $form['entity']['booking_timeslots_filter_title_category'] = array(
    '#type' => 'textfield',
    '#title' => t('Category filter title'),
    '#default_value' => variable_get('booking_timeslots_filter_title_category', 'Category'),
  );

  $form['entity']['ct1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Primary content type'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['entity']['ct2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Secondary content type'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['entity']['ct3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tertiary content type'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['entity']['ct4'] = array(
    '#type' => 'fieldset',
    '#title' => t('Quaternary content type'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['entity']['ct1']['booking_timeslots_1st_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('booking_timeslots_1st_title', 'Primary'),
    '#description' => t('Title of the content type displayed in various forms'),
  );

  $form['entity']['ct1']['booking_timeslots_1st_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Primary content type'),
    '#default_value' => variable_get('booking_timeslots_1st_content_type', ''),
    '#options' => $content_types,
    '#description' => t('Content type which is used for booking per calendar. Select None to disable the feature. E.g. Venue'),
  );

  $form['entity']['ct2']['booking_timeslots_2nd_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('booking_timeslots_2nd_title', 'Secondary'),
    '#description' => t('Title of the content type displayed in various forms'),
  );

  $form['entity']['ct2']['booking_timeslots_2nd_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Secondary content type'),
    '#default_value' => variable_get('booking_timeslots_2nd_content_type', ''),
    '#options' => $content_types,
    '#description' => t('Content type which is associated with the primary content type. Select None to disable the feature. E.g. Facility'),
  );

  // Reference from the 2nd to 1st content type
  $form['entity']['ct2']['booking_timeslots_2nd_to_1st_ref_field'] = array(
    '#type' => 'select',
    '#title' => t('Secondary to primary content type reference field'),
    '#default_value' => variable_get('booking_timeslots_2nd_to_1st_ref_field', ''),
    '#options' => $fields,
    '#description' => t('Field acting as a reference from the 2nd to 1st content type. <u>Must be of type <b>Entity Reference</b>. Should use <b>Select list</b> widget and be related to <b>Primary Content Type</b>. Should be marked as <b>Required</b></u>.'),
  );

  $form['entity']['ct3']['booking_timeslots_3rd_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('booking_timeslots_3rd_title', 'Secondary'),
    '#description' => t('Title of the content type displayed in various forms'),
  );

  $form['entity']['ct3']['booking_timeslots_3rd_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary content type'),
    '#default_value' => variable_get('booking_timeslots_3rd_content_type', ''),
    '#options' => $content_types,
    '#description' => t('Content type which is associated with the secondary content type. Select None to disable the feature. E.g. Class'),
  );

  // 3rd content type capacity field
  $form['entity']['ct3']['booking_timeslots_3rd_capacity_field'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary content type capacity field'),
    '#default_value' => variable_get('booking_timeslots_3rd_capacity_field', ''),
    '#options' => $fields,
    '#description' => t('Field acting as a capacity indicator for a 3rd content type. <u>Must be of type <b>Integer</b>, using <b>Text field</b> widget and be marked as <b>Required</b>. Hint: Use "1" for <b>Minimum</b> and <b>Default value</b>.</u>'),
  );


  // Reference from the 3rd to 2nd content type
  $form['entity']['ct3']['booking_timeslots_3rd_to_2nd_ref_field'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary to secondary content type reference field'),
    '#default_value' => variable_get('booking_timeslots_3rd_to_2nd_ref_field', ''),
    '#options' => $fields,
    '#description' => t('Field acting as a reference from the 3rd to 2nd content type. <u>Must be of type <b>Entity Reference</b>. Should use <b>Select list</b> widget and be related to <b>Secondary Content Type</b>. Should be marked as <b>Required</b></u>.'),
  );

  // Reference from the 3rd to 4th content type
  $form['entity']['ct3']['booking_timeslots_3rd_to_4th_ref_field'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary to quaternary content type reference field'),
    '#default_value' => variable_get('booking_timeslots_3rd_to_4th_ref_field', ''),
    '#options' => $fields,
    '#description' => t('Field acting as a reference from the 3rd to 4th content type. <u>Must be of type <b>Entity Reference</b>. Should use <b>Select list</b> widget and be related to <b>Quaternary Content Type</b>. Should be marked as <b>Required</b></u>.'),
  );

  $form['entity']['ct4']['booking_timeslots_4th_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('booking_timeslots_4th_title', 'Secondary'),
    '#description' => t('Title of the content type displayed in various forms'),
  );

  $form['entity']['ct4']['booking_timeslots_4th_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Quaternary content type'),
    '#default_value' => variable_get('booking_timeslots_4th_content_type', ''),
    '#options' => $content_types,
    '#description' => t('Content type which is associated with the tertiary content type. Select None to disable the feature. E.g. Instructor'),
  );



  $form['views'] = array(
    '#type' => 'fieldset',
    '#title' => t('Views integration'),
    '#description' => t('Please select content type on which schedule tabs will be displayed. <u>Preferably the <b>Primary Content Type</b></u>.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Using built-in calendar view.
  $calendar_view_name = 'bt_schedule';
  $calendar_view_path = 'admin/structure/views/view/bt_schedule/edit';

  /* CHECK IF DEFAULT CALENDAR VIEW IS ENABLED */
  if (!views_get_view($calendar_view_name) || views_get_view($calendar_view_name)->disabled) {
    $form['views']['booking_timeslots_view_' . $calendar_view_name] = array(
      '#type' => 'select',
      '#disabled' => TRUE,
      '#options' => array(
        '' => t('Please enable built-in "Bookings Schedule" (bt_schedule) view')
      )
    );
  }
  else {
    $form['views']['booking_timeslots_view_' . $calendar_view_name] = array(
        '#type' => 'select',
        '#title' => t('View: %view', array('%view' => $calendar_view_name)),
        '#description' => t('Choose which content type should be associated with your <a target=_blank href="@url">calendar view</a>.', array('@url' => url($calendar_view_path))),
        '#default_value' => variable_get('booking_timeslots_view_' . $calendar_view_name, ''),
        '#options' => array_merge(array('' => t('- None -')), $content_types),
      );
  }
  if (isset($form['views']['booking_timeslots_view_booking_calendar']) && empty($form['views']['booking_timeslots_view_booking_calendar']['#default_value'])) {
    $form['views']['booking_timeslots_view_booking_calendar']['#default_value'] = array('booking_slot');
  }

  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Calendar presentation and default timezone'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['config']['date_first_day'] = array(
    '#type' => 'select',
    '#title' => t('First day of week'),
    '#default_value' => variable_get('date_first_day', 0),
    '#options' => array(0 => t('Sunday'), 1 => t('Monday'), 2 => t('Tuesday'), 3 => t('Wednesday'), 4 => t('Thursday'), 5 => t('Friday'), 6 => t('Saturday')),
    '#weight' => -2
  );

  $form['config']['date_default_timezone'] = array(
    '#type' => 'select',
    '#title' => t('Default time zone'),
    '#default_value' => variable_get('date_default_timezone', date_default_timezone_get()),
    '#options' => system_time_zones(),
    '#description' => t('By default, dates in this site will be displayed in the chosen time zone.'),
    '#weight' => -1,
    '#attributes' => array('class' => array('timezone-detect')),
  );

  $form['config']['booking_timeslots_calendar_granularity'] = array(
    '#type' => 'select',
    '#title' => t('Calendar granularity'),
    '#default_value' => variable_get('booking_timeslots_calendar_granularity', '15'),
    '#options' => array('15' => '15 min', '30' => '30 min', '60' => '60 min'),
    '#description' => t('Time step between each time row'),
  );

  $form['config']['booking_timeslots_enable_booking_for_past_dates'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable booking of slots from past (apply to normal users)'),
    '#default_value' => variable_get('booking_timeslots_enable_booking_for_past_dates', '1'),
    '#description' => t('By enabling this options you will allow normal users to book slots in past. Venue managers will we able to book such slots anyway.'),
  );


  $form['config']['booking_timeslots_durations'] = array(
    '#type' => 'textarea',
    '#title' => t('Enabled durations'),
    '#default_value' => variable_get('booking_timeslots_durations', "15|15 min\n30|30 min\n45|45 min\n60|60 min\n120|120 min"),
    '#description' => t("List of enabled durations. Each line should contain number of minutes and title separated by '|' character. Note that those durations must be a multiple of the calendar granularity and be greater or equal to that granularity."),
  );

  $form['config']['booking_timeslots_force_select_ct_2nd'] = array(
    '#type' => 'checkbox',
    '#title' => t("Force users to select secondary content type."),
    '#description' => t("If enabled, there won't be a mixed view for all secondary content type's nodes. Currently <b>you are very encouraged to enable this option</b>."),
    '#default_value' => variable_get('booking_timeslots_force_select_ct_2nd', FALSE),
  );

  /*
  $form['config']['booking_timeslots_past_booking'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow to book in the past."),
    '#description' => t('If enabled, slots in the past are available to book.'),
    '#default_value' => variable_get('booking_timeslots_past_booking', FALSE),
  );
  */

  /*
  $form['other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  */

  /*
  $form['other']['booking_timeslots_show_warnings'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't show configuration warnings, etc. on admin pages."),
    '#description' => t('If you configured module correctly or you customized already some settings, some warnings are no longer necessary. Check to disable them.'),
    '#default_value' => variable_get('booking_timeslots_show_warnings', FALSE),
  );
  $form['other']['booking_timeslots_show_debug_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t("Show debug messages."),
    '#description' => t('Show additional debug messages (administration permission required).'),
    '#default_value' => variable_get('booking_timeslots_show_debug_messages', FALSE),
  );
  */

  /*
  !variable_get('booking_timeslots_show_warnings', FALSE) ?  booking_timeslots_check_admin_options() : NULL; // Check for configuration issues
  */
  $form['#validate'][] = 'booking_timeslots_settings_form_validate';
  $form['#submit'][]   = 'booking_timeslots_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Get calendar view path based on view name
 */
function booking_timeslots_calendar_view_path($view_name) {
  $view = views_get_view($view_name);
  return isset($view->display[$view_name]) ? $view->display[$view_name]->display_options['path'] : '';
}

/**
 * Form API callback to validate the settings form.
 */
function booking_timeslots_form_validate($form, &$form_state) {
  /*
  $hour_from = $form['timeslot_length']['booking_timeslots_hour_from']['#value'];
  $hour_to = $form['timeslot_length']['booking_timeslots_hour_to']['#value'];

  if ($hour_from >= $hour_to) {
    form_set_error('booking_timeslots_hour_from', t("Hour To should be greater than Hour From."));
    form_set_error('booking_timeslots_hour_to', ' ');
  }
  */
}

/**
 * Form API callback to validate the admin settings form.
 */
function booking_timeslots_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  $calendar_view_name = 'bt_schedule';
  if (!views_get_view($calendar_view_name) || views_get_view($calendar_view_name)->disabled) {
    form_set_error('booking_timeslots_view_' . $calendar_view_name, t('Please enable built-in <b>Bookings Schedule</b> (bt_schedule) view.'));
  }

  /*
  // MAKE THE LIST OF ACTIVATED VIEWS
  $enabled_views = array();
  preg_match_all("/booking_timeslots_view_\w+/", join(",", array_keys($values)), $names); // find all interval array keys from the submitted form
  foreach ((array)$names[0] as $key => $vkey) { // for each key, find the items
    $view_name = substr($vkey, strlen('booking_timeslots_view_'));
    $content_name = $values[$vkey]; // find interval for found key from form
    $enabled_views[$view_name] = $content_name;
    if (!empty($content_name)) { // if view is associated with content type...
      module_load_include('inc', 'booking_timeslots');
      if (!($field_name = booking_timeslots_detect_field_from_view($view_name))) { // check if that view has proper configuration
        form_set_error($vkey,
          t("Please configure <a target='_blank' href='@view_url'>%name view</a> by <a target='_blank' href='@view_arg_url'>selecting proper field</a> which you chosen for time slots and <a target='_blank' href='@view_fields_url'>adding it</a> to the list of fields, before activating the view!",
            array(
              '@view_url' => url("admin/build/views/edit/$view_name"),
              '@view_arg_url' => url("admin/build/views/nojs/config-item/$view_name/default/argument/date_argument"),
              '@view_fields_url' => url("admin/build/views/nojs/add-item/$view_name/default/field"),
              '%name' => $view_name
             )
          )
        );
      }
    }
  }
  $enabled_views = array_filter($enabled_views); // filter empty settings
  empty($enabled_views) && !variable_get('booking_timeslots_show_warnings', FALSE) ?  drupal_set_message(t("Proper configuration require at least one view associated with a content type!"), 'warning') : NULL; // Check for configuration issues

  // CHECK PROPER CONFIGURATION OF FIELDS
  $field_names = array_flip($values['booking_timeslots_fields']);
  $field_names_off = array_flip($values['booking_timeslots_excluded_dates']);
  $field_names_limited = array_flip($values['booking_timeslots_limited_dates']);
  unset($field_names[0], $field_names_off[0], $field_names_limited[0]); // remove disabled checkboxes
  if (($wrong_fields = array_intersect($field_names, $field_names_off)) && !empty($wrong_fields)) { // search for fields, which are enabled in two places at the same time
    form_set_error('booking_timeslots_excluded_dates', t("You can't use the same field %field for non-available dates, which is already used for time slots!", array('%field' => current($wrong_fields))));
  }
  if (($wrong_fields = array_intersect($field_names, $field_names_limited)) && !empty($wrong_fields)) { // search for fields, which are enabled in two places at the same time
    form_set_error('booking_timeslots_limited_dates', t("You can't use the same field %field for limiting dates, which is already used for time slots!", array('%field' => current($wrong_fields))));
  }
  if (($wrong_fields = array_intersect($field_names_off, $field_names_limited)) && !empty($wrong_fields)) { // search for fields, which are enabled in two places at the same time
    form_set_error('booking_timeslots_limited_dates', t("You can't use the same field %field for limiting dates, which is already used for non-available dates!", array('%field' => current($wrong_fields))));
  }
  */
}

/**
 * Form API callback to submit the admin settings form.
 */
function booking_timeslots_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $calendar_view_name    = 'bt_schedule';
  $schedule_content_type = $values['booking_timeslots_view_' . $calendar_view_name];

  booking_timeslots_choose_schedule_content_type($schedule_content_type);
}

/**
 * General function to check admin settings
 * Check if tpl files are in the right place
 * Note: works only with default calendar view
 */
function booking_timeslots_check_admin_options() {
  /* CHECK IF TPL TEMPLATE HAS BEEN COPIED INTO THEME FOLDER */
  /*
  $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
  if (($registry = theme_get_registry()) && (array_search('template_preprocess_booking_timeslots_day', (array)$registry['calendar_day']['preprocess functions']) === FALSE)) { // check or preprocess function
    drupal_set_message(t('Please <a href="@url">clear the cache</a> to reload preprocess hooks!', array('@url' => url('admin/settings/performance'))), 'warning');
  }
  if (!file_exists($theme_path . "/calendar-month.tpl.php") || !file_exists($theme_path . "/calendar-day.tpl.php")) { // TODO: do the better checking, instead of hardcoding the files (could be: calendar-day--booking-calendar.tpl.php instead)
    drupal_set_message(t('Please copy theme files from module themes/ folder into your theme path: @theme_path to see changes in calendar view and clear the cache.', array('@theme_path' => $theme_path)), 'error');
  }
  if (module_exists('calendar_multiday')) {
    drupal_set_message(t("Currently Calendar Multiday module is not supported! <a href='@url'>Please disable it!</a> See: <a href='@issue'>#1005336</a>", array(
      '@url' => url('admin/build/modules'),
      '@issue' => 'http://drupal.org/node/1005336')), 'error');
  }
  */
}


/**
 *  Menu callback for the tutorial form.
 */
function booking_timeslots_tutorial_form($form, $form_state) {
  drupal_add_css(drupal_get_path('module', 'booking_timeslots') .'/css/booking_timeslots_tutorial.css');

  ob_start();
  $dir = drupal_get_path('module', 'booking_timeslots');
  include $dir . '/templates/tutorial.tpl.php';
  $form['tutorial']['#markup'] = ob_get_clean();
  return $form;
}
