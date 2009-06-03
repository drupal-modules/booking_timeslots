<?php
// $Id$
/**
 * @file
 * Template to display a view as a calendar day, grouped by time
 * and optionally organized into columns by a field value.
 *
 * @see template_preprocess_calendar_day()
 *
 * $rows: The rendered data for this day.
 * $rows['date'] - the date for this day, formatted as YYYY-MM-DD.
 * $rows['datebox'] - the formatted datebox for this day.
 * $rows['empty'] - empty text for this day, if no items were found.
 * $rows['all_day'] - an array of formatted all day items.
 * $rows['items'] - an array of timed items for the day.
 * $rows['items'][$time_period]['hour'] - the formatted hour for a time period.
 * $rows['items'][$time_period]['ampm'] - the formatted ampm value, if any for a time period.
 * $rows['items'][$time_period][$column]['values'] - An array of formatted
 *   items for a time period and field column.
 *
 * $view: The view.
 * $columns: an array of column names.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 *
 * The width of the columns is dynamically set using <col></col>
 * based on the number of columns presented. The values passed in will
 * work to set the 'hour' column to 10% and split the remaining columns
 * evenly over the remaining 90% of the table.
 */
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);
//dsm($columns);
//dsm($rows);
?>

<div class="calendar-calendar">
  <div class="day-view">
    <table>
      <thead>
        <col width="10%"></col>
        <?php foreach ($columns as $column): ?>
        <col width="<?php print $column_width; ?>%"></col>
        <?php endforeach; ?>
        <tr>
          <th class="calendar-dayview-hour"><?php t('Time'); ?></th>
          <?php foreach ($columns as $column): ?>
          <th class="calendar-agenda-items"><?php print $column; ?></th>
          <?php endforeach; ?>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td class="calendar-agenda-hour">
             <span class="calendar-hour"><?php print t('All day'); ?></span>
           </td>
          <?php foreach ($columns as $column): ?>
           <td class="calendar-agenda-items">
             <div class="calendar">
             <div class="inner">
             <?php 
               if (is_array($rows['all_day'][$column]) && strpos(implode($rows['all_day'][$column]), 'Available')!==FALSE) { // FIXME: http://drupal.org/node/455652#comment-1601448
                 $notavailable = TRUE; // not available, because of whole day event
               }
               print isset($rows['all_day'][$column]) ? implode($rows['all_day'][$column]) : '&nbsp;';
             ?>
             </div>
             </div>
           </td>
          <?php endforeach; ?>
        </tr>

    <?php
      /* get configuration data */
      $half_hour = ($view->style_options['groupby_times'] === 'half'); // get half-hour mode (0 - if it's hour mode, 1 - if it's half-hour mode)
      $slots = variable_get('booking_timeslot_avaliable_slots', 0);
      $my_forms = variable_get('booking_timeslot_forms', array());
      $my_fields = variable_get('booking_timeslot_fields', array());

      /* calculate event length */
      $hour_length = variable_get('booking_timeslot_length_hours', 1);
      $minute_length = variable_get('booking_timeslot_length_minutes', 0);
      $hours = $hour_length + $minute_length/60; // calculate how many hours have one event
      define('EVENT_TIME', (bool)$half_hour ? $hours/0.5 : $hours ); // for HOW MANY SLOTS each event should be booked

      /* set other constants */
      define('AVAIL_SLOTS', max(1,$slots)); // CHANGE here to set limit if you have one or many slots available in the same time
      $slot_booked = t('Already booked');
      $slot_free = t('Book now');
      $slot_unavailable = t('Not Available');

      /* detect content type name */
      $content_types = content_types();
      if ($my_form_id = $_SESSION['booking_timeslot_ct_'.arg(0)] !== FALSE) {
        foreach ($my_forms as $my_form_key => $my_form_id) {  // find associated content type with field
          foreach ($my_fields as $field_name) { // FIXME: later can be done by array_search() or something like that
            if (isset($content_types[$my_form_key]['fields'][$field_name]) && !empty($my_form_key)) { // if field exist in this content type...
              $_SESSION['booking_timeslot_ct_'.arg(0)] = $my_form_id; /// associate this content type with base path for futher use
              break 2;
            }
          }
        }
      }
      $module_link = "node/add/" . $my_form_id;

      $booked = array();
      $hour_from = variable_get('booking_timeslot_hour_from', 8);
      $hour_to = variable_get('booking_timeslot_hour_to', 18);
      for ($h = $hour_from; $h<=$hour_to; $h++) {
        for ($half = 0; $half<= (int)$half_hour; $half++) { // half-hour style supported if enabled
          strlen($h) == 1 ? $h = '0' . $h : NULL; // follow by zero if hour is in one-digit format
          $hh = !$half ? $h . ':00:00' : $h . ':30:00'; // add minutes and second to the hour
          $hour = array_key_exists($hh, $rows['items']) ? $rows['items'][$hh] : array('hour' => substr($hh, 0, strlen($hh)-3), 'ampm' => ''); // prepare hour time slot
          $content = '';
    ?>
          <tr>
            <td class="calendar-agenda-hour">
              <span class="calendar-hour"><?php print $hour['hour']; ?></span>
              <span class="calendar-ampm"><?php print $hour['ampm']; ?></span>
            </td>
    <?php
          /*
           * Calculate time for each half an hour
           */
          foreach ($booked as $key => $time_left) { // check booking slots
            if (--$booked[$key]<1) { // decrease half an hour and check if it's finished
              unset($booked[$key]); // if yes, free one slot
            }
          }

          /*
           * Check slots availability
           */
          if (is_array($hour['values'][$column])) {
            foreach ($hour['values'][$column] as $no => $event) {
              $found_slot = FALSE;
              for ($slot=0; $slot<(AVAIL_SLOTS); $slot++) { // scan for free slot
                if (!array_key_exists($slot, $booked)) {
                  $booked[$slot] = EVENT_TIME;
                  $found_slot = TRUE;
                  break;
                }
              }

              if (!$found_slot) { // this case is normally not needed, but it necessary for correct calculation
                /* this block run, when there are more booked slots than available */
                for ($slot=0; $slot<(AVAIL_SLOTS); $slot++) { // try to find already started events to reset their slot time
                  if (array_key_exists($slot, $booked) && $booked[$slot]<EVENT_TIME) { // if time is started...
                    $booked[$slot] = EVENT_TIME; // ...reset to maximum
                    break;
                  }
                }
              }
            }
          }

          /*
           * Set content
           */
          for ($slot=0; $slot<(AVAIL_SLOTS); $slot++) { // now check which slot is...
            if ((array_key_exists($slot, $booked)) && ($booked[$slot]>0) && ($slots != 0)) { // ...booked
              $content .= "<div class='slot_booked'>$slot_booked</div>";
            } elseif ($notavailable) {
              $content .= "<div class='slot_unavailable'>$slot_unavailable</div>";
            } else {
              $link = l($slot_free, $module_link . '/' . $rows['date'] . ' ' . $hh);
              $content .= "<div class='slot_free'>$link</div>"; // ...and which is free
            }
          }

          if (isset($_GET['show']) || user_access('show booking dates')) { // special functionality for testing purpose (add &show at the end of url)
            $content .= isset($hour['values'][$column]) ? implode($hour['values'][$column]) : '&nbsp;'; // you can use it for debug, it will show you the events
          }

          foreach ($columns as $column) {
        ?>
            <td class="calendar-agenda-items">
              <div class="calendar">
                <div class="inner">
                  <?php print $content; ?>
                </div>
              </div>
            </td>
          <?php } ?>
          </tr>
     <?php
        }
      }
    ?>
      </tbody>
    </table>
  </div>
</div>
