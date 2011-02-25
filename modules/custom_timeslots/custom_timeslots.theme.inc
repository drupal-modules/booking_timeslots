<?php

/**
 * @file
 *   Theme file
 *
 * @version
 *
 * @developers
 *   Rafal Wieczorek <kenorb@gmail.com>
 */

/**
* Theme the re-ordering form
*/
function theme_custom_timeslots_list_sort_form($form) {
  drupal_add_tabledrag('custom_timeslots_draggable_table', 'order', 'sibling', 'sort');
  $header = array('', 'title', 'sort');
  foreach (element_children($form) as $key) {
    // Add class to group weight fields for drag and drop.
    $form[$key]['sort']['#attributes']['class'] = 'sort';

    $row = array('');
    $row[] = drupal_render($form[$key]['title']);
    $row[] = drupal_render($form[$key]['sort']);
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }
  $output = theme('table', $header, $rows, array('id' => 'custom_timeslots_draggable_table'));
  $output .= drupal_render($form);
  return $output;
}

