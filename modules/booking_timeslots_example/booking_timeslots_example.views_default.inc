<?php
/**
 * @file
 * booking_timeslots_example.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function booking_timeslots_example_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'bt_xmp_administer_bookings';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'booking';
  $view->human_name = 'Administer Bookings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'All bookings';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer bookings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'uid',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'pid' => 'pid',
    'uid' => 'uid',
    'slot_time' => 'slot_time',
    'duration' => 'duration',
    'field_bt_first_name' => 'field_bt_first_name',
    'field_bt_last_name' => 'field_bt_last_name',
    'players' => 'players',
    'total' => 'total',
  );
  $handler->display->display_options['style_options']['default'] = 'slot_time';
  $handler->display->display_options['style_options']['info'] = array(
    'pid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'slot_time' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'duration' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_bt_first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_bt_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'players' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'total' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Booking: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'booking';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Booking: Booking ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'booking';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  $handler->display->display_options['fields']['pid']['label'] = 'ID';
  /* Field: Booking: Date */
  $handler->display->display_options['fields']['slot_time']['id'] = 'slot_time';
  $handler->display->display_options['fields']['slot_time']['table'] = 'booking';
  $handler->display->display_options['fields']['slot_time']['field'] = 'slot_time';
  $handler->display->display_options['fields']['slot_time']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['slot_time']['alter']['text'] = '[slot_time]';
  /* Field: Booking: Duration */
  $handler->display->display_options['fields']['duration']['id'] = 'duration';
  $handler->display->display_options['fields']['duration']['table'] = 'booking';
  $handler->display->display_options['fields']['duration']['field'] = 'duration';
  $handler->display->display_options['fields']['duration']['suffix'] = 'min';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'uid';
  /* Field: Booking: Players */
  $handler->display->display_options['fields']['players']['id'] = 'players';
  $handler->display->display_options['fields']['players']['table'] = 'booking';
  $handler->display->display_options['fields']['players']['field'] = 'players';
  /* Field: Booking: Total */
  $handler->display->display_options['fields']['total']['id'] = 'total';
  $handler->display->display_options['fields']['total']['table'] = 'booking';
  $handler->display->display_options['fields']['total']['field'] = 'total';
  $handler->display->display_options['fields']['total']['precision'] = '0';
  $handler->display->display_options['fields']['total']['prefix'] = '$';
  /* Field: Booking: Tax */
  $handler->display->display_options['fields']['tax']['id'] = 'tax';
  $handler->display->display_options['fields']['tax']['table'] = 'booking';
  $handler->display->display_options['fields']['tax']['field'] = 'tax';
  $handler->display->display_options['fields']['tax']['precision'] = '0';
  $handler->display->display_options['fields']['tax']['prefix'] = '$';
  /* Field: Booking: Deposit */
  $handler->display->display_options['fields']['deposit']['id'] = 'deposit';
  $handler->display->display_options['fields']['deposit']['table'] = 'booking';
  $handler->display->display_options['fields']['deposit']['field'] = 'deposit';
  $handler->display->display_options['fields']['deposit']['precision'] = '0';
  $handler->display->display_options['fields']['deposit']['prefix'] = '$';
  /* Field: Booking: URL */
  $handler->display->display_options['fields']['url_2']['id'] = 'url_2';
  $handler->display->display_options['fields']['url_2']['table'] = 'views_entity_booking';
  $handler->display->display_options['fields']['url_2']['field'] = 'url';
  $handler->display->display_options['fields']['url_2']['label'] = '';
  $handler->display->display_options['fields']['url_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['url_2']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['url_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['url_2']['alter']['path'] = 'booking/[pid]';
  $handler->display->display_options['fields']['url_2']['alter']['alt'] = 'View';
  $handler->display->display_options['fields']['url_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url_2']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url_2']['link_to_entity'] = 0;
  /* Field: Booking: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_booking';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['url']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['url']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['url']['alter']['path'] = 'booking/[pid]/edit';
  $handler->display->display_options['fields']['url']['alter']['alt'] = 'Edit';
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: Booking: URL */
  $handler->display->display_options['fields']['url_1']['id'] = 'url_1';
  $handler->display->display_options['fields']['url_1']['table'] = 'views_entity_booking';
  $handler->display->display_options['fields']['url_1']['field'] = 'url';
  $handler->display->display_options['fields']['url_1']['label'] = '';
  $handler->display->display_options['fields']['url_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['url_1']['alter']['text'] = 'Remove';
  $handler->display->display_options['fields']['url_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['url_1']['alter']['path'] = 'booking/[pid]/remove';
  $handler->display->display_options['fields']['url_1']['alter']['alt'] = 'Remove';
  $handler->display->display_options['fields']['url_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url_1']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url_1']['link_to_entity'] = 0;
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/bookings';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'All bookings';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $export['bt_xmp_administer_bookings'] = $view;

  $view = new view();
  $view->name = 'bt_xmp_my_bookings';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'booking';
  $view->human_name = 'My bookings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My bookings';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'uid',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'pid' => 'pid',
    'uid' => 'uid',
    'slot_time' => 'slot_time',
    'duration' => 'duration',
    'field_bt_first_name' => 'field_bt_first_name',
    'field_bt_last_name' => 'field_bt_last_name',
    'players' => 'players',
    'total' => 'total',
  );
  $handler->display->display_options['style_options']['default'] = 'slot_time';
  $handler->display->display_options['style_options']['info'] = array(
    'pid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'slot_time' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'duration' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_bt_first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_bt_last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'players' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'total' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Booking: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'booking';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Booking: Booking ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'booking';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  $handler->display->display_options['fields']['pid']['label'] = 'ID';
  /* Field: Booking: Date */
  $handler->display->display_options['fields']['slot_time']['id'] = 'slot_time';
  $handler->display->display_options['fields']['slot_time']['table'] = 'booking';
  $handler->display->display_options['fields']['slot_time']['field'] = 'slot_time';
  $handler->display->display_options['fields']['slot_time']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['slot_time']['alter']['text'] = '[slot_time]';
  /* Field: Booking: Duration */
  $handler->display->display_options['fields']['duration']['id'] = 'duration';
  $handler->display->display_options['fields']['duration']['table'] = 'booking';
  $handler->display->display_options['fields']['duration']['field'] = 'duration';
  $handler->display->display_options['fields']['duration']['suffix'] = 'min';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'users';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['relationship'] = 'uid';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_bt_first_name']['id'] = 'field_bt_first_name';
  $handler->display->display_options['fields']['field_bt_first_name']['table'] = 'field_data_field_bt_first_name';
  $handler->display->display_options['fields']['field_bt_first_name']['field'] = 'field_bt_first_name';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_bt_last_name']['id'] = 'field_bt_last_name';
  $handler->display->display_options['fields']['field_bt_last_name']['table'] = 'field_data_field_bt_last_name';
  $handler->display->display_options['fields']['field_bt_last_name']['field'] = 'field_bt_last_name';
  /* Field: Booking: Players */
  $handler->display->display_options['fields']['players']['id'] = 'players';
  $handler->display->display_options['fields']['players']['table'] = 'booking';
  $handler->display->display_options['fields']['players']['field'] = 'players';
  /* Field: Booking: Total */
  $handler->display->display_options['fields']['total']['id'] = 'total';
  $handler->display->display_options['fields']['total']['table'] = 'booking';
  $handler->display->display_options['fields']['total']['field'] = 'total';
  $handler->display->display_options['fields']['total']['precision'] = '0';
  $handler->display->display_options['fields']['total']['prefix'] = '$';
  /* Field: Booking: Tax */
  $handler->display->display_options['fields']['tax']['id'] = 'tax';
  $handler->display->display_options['fields']['tax']['table'] = 'booking';
  $handler->display->display_options['fields']['tax']['field'] = 'tax';
  $handler->display->display_options['fields']['tax']['precision'] = '0';
  $handler->display->display_options['fields']['tax']['prefix'] = '$';
  /* Field: Booking: Deposit */
  $handler->display->display_options['fields']['deposit']['id'] = 'deposit';
  $handler->display->display_options['fields']['deposit']['table'] = 'booking';
  $handler->display->display_options['fields']['deposit']['field'] = 'deposit';
  $handler->display->display_options['fields']['deposit']['precision'] = '0';
  $handler->display->display_options['fields']['deposit']['prefix'] = '$';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_status']['id'] = 'field_status';
  $handler->display->display_options['fields']['field_status']['table'] = 'field_data_field_status';
  $handler->display->display_options['fields']['field_status']['field'] = 'field_status';
  /* Field: Booking: URL */
  $handler->display->display_options['fields']['url_2']['id'] = 'url_2';
  $handler->display->display_options['fields']['url_2']['table'] = 'views_entity_booking';
  $handler->display->display_options['fields']['url_2']['field'] = 'url';
  $handler->display->display_options['fields']['url_2']['label'] = '';
  $handler->display->display_options['fields']['url_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['url_2']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['url_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['url_2']['alter']['path'] = 'booking/[pid]';
  $handler->display->display_options['fields']['url_2']['alter']['alt'] = 'View';
  $handler->display->display_options['fields']['url_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url_2']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url_2']['link_to_entity'] = 0;
  /* Field: Booking: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_booking';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['url']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['url']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['url']['alter']['path'] = 'booking/[pid]/edit';
  $handler->display->display_options['fields']['url']['alter']['alt'] = 'Edit';
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: Booking: URL */
  $handler->display->display_options['fields']['url_1']['id'] = 'url_1';
  $handler->display->display_options['fields']['url_1']['table'] = 'views_entity_booking';
  $handler->display->display_options['fields']['url_1']['field'] = 'url';
  $handler->display->display_options['fields']['url_1']['label'] = '';
  $handler->display->display_options['fields']['url_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['url_1']['alter']['text'] = 'Remove';
  $handler->display->display_options['fields']['url_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['url_1']['alter']['path'] = 'booking/[pid]/remove';
  $handler->display->display_options['fields']['url_1']['alter']['alt'] = 'Remove';
  $handler->display->display_options['fields']['url_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url_1']['display_as_link'] = FALSE;
  $handler->display->display_options['fields']['url_1']['link_to_entity'] = 0;
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'bookings';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'My bookings';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $export['bt_xmp_my_bookings'] = $view;

  return $export;
}
