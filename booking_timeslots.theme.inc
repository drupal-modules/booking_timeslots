<?php

/**
 * @file
 *   Theme include file
 *
 * @version
 *
 * @developers
 *   Rafal Wieczorek <kenorb@gmail.com>
 */

/**
 * Theme preprocess callback
 *
 * Format an calendar day node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_day(&$vars) {
  extract($vars);
  $vars['half_hour_mode'] = $half_hour = ($view->style_options['groupby_times'] === 'half'); // get half-hour mode (0 - if it's hour mode, 1 - if it's half-hour mode)
}


/**
 * Theme preprocess callback
 *
 * Format an calendar month node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_month(&$vars) {
  extract($vars);
}

/**
 * Theme preprocess callback
 *
 * Format an calendar week node for display.
 *  See: template_preprocess_calendar($vars);
 */
function template_preprocess_booking_timeslots_week(&$vars) {
  extract($vars);
}

