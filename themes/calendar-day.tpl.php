<?php
/**
 * @file
 * Template to display a view as a calendar day, grouped by time
 * and optionally organized into columns by a field value.
 *
 * @see template_preprocess_booking_timeslots_day()
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
 *
 * Booking Timeslots variables
 *   $bt_groupby_times - How items are grouped together into time periods based on their start time. Options: hour, half-hour, custom
 *   $bt_groupby_times_custom - Custom time period groupings (if $bt_groupby_times is set to: custom), otherwise it's FALSE
 *   $bt_weekday_avail - TRUE if current weekday is available for booking, otherwise FALSE
 *   $bt_day_of_week - A full textual representation of the day of the week (i.e. Monday)
 *   $bt_groupby_times_custom - for HOW MANY SLOTS each event should be booked
 *   $bt_slots_per_event - for HOW MANY SLOTS each event should be booked
 *   $bt_booked_all_day - Return number of events booked for the all day
 *   $bt_max_avail_slots - Maximum number of available slots per defined timeframe (hour, half-hour)
 *   $bt_no_of_slots_available - Check how many slots are available that day
 *   $bt_hour['start'] - from what hour day is starting (8 by default)
 *   $bt_hour['end'] - what hour day is finished (18 by default)
 *   $bt_show_events - TRUE, if user should see details of booking events in the calendar
 *   $bt_node_add_link - link to booking node creation page (without date and time)
 *   $bt_ctype_non_avail  - name of content type which is used for non-available dates
 *   $bt_text['book_now'] - Text for slot, which is free (available to book)
 *   $bt_text['slot_booked'] - Text for slot, which is already booked
 *   $bt_text['slot_unavailable'] - Text for slot, which is unavailable
 *
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
               if (!$bt_no_of_slots_available) {
                 $notavailable = TRUE; // not available, because of the all day event
               }
               print isset($rows['all_day'][$column]) ? implode($rows['all_day'][$column]) : '&nbsp;';
             ?>
             </div>
             </div>
           </td>
          <?php endforeach; ?>
        </tr>

    <?php

      $booked = array();
      if ($bt_groupby_times_custom) {
        $day_hours = split(',', $bt_groupby_times_custom);
      } else {
        for ($h = $bt_hour['start']; $h <= $bt_hour['end']; $h++) {
          for ($half = 0; $half<= (int)$bt_half_hour_mode; $half++) { // half-hour style supported if enabled
            strlen($h) == 1 ? $h = '0' . $h : NULL; // follow by zero if hour is in one-digit format
            $hh = !$half ? $h . ':00:00' : $h . ':30:00'; // add minutes and second to the hour
            $day_hours[] = $hh;
          }
        }
      }

        foreach ($day_hours as $hh) {
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
            foreach ($booked as $key => $time_left) {
                // check booking slots
                if (--$booked[$key]<1)
                { // decrease half an hour and check if it's finished
                    unset($booked[$key]); // if yes, free one slot
                }
            }

            // set all slots free
            for ($slot = 0; $slot < ($bt_no_of_slots_available); $slot++) {
                $slot_info[$hh][$slot] = 0;
            }

            /*
            * Check slots availability
            */
            if (is_array($hour['values'][$column])) {
                foreach ($hour['values'][$column] as $no => $event) {
                    $found_slot = FALSE;
                    for ($slot=0; $slot<($bt_no_of_slots_available); $slot++) {
                        // scan for free slot
                        if (!array_key_exists($slot, $booked)) {
                            $booked[$slot] = $bt_slots_per_event;
                            for($i = 1; $bt_slots_per_event > $i; $i++) {
                                $slot_info[$hh][$slot+$i] = 1;
                            }

                            $found_slot = TRUE;
                            break;
                        }
                    }

                    if (!$found_slot) { // this case is normally not needed, but it necessary for correct calculation
                            /* this block run, when there are more booked slots than available */
                            for ($slot=0; $slot<($bt_no_of_slots_available); $slot++) { // try to find already started events to reset their slot time
                                if (array_key_exists($slot, $booked) && $booked[$slot] < $bt_slots_per_event) { // if time is started...
                                    $booked[$slot] = $bt_slots_per_event; // ...reset to maximum
                                   for($i = 1; $bt_slots_per_event > $i; $i++) {
                                        $slot_info[$hh][$slot+$i] = 1;
                                    }
                                    break;
                                }
                            }
                    }
                }
            }

            /*
             * Set content
             */

            $available_slots[$hh] = FALSE;

            // 0 = free
            // 1 = booked
            // 2 = unavailable (holidays)
            $hh_conflicts[$hh] = 0;

            $date_unix = strtotime($rows['date'] . ' ' . $hh);

            foreach ($bt_holidays as $holiday) {
                if ($date_unix >= $holiday[0] && $date_unix < $holiday[1]) {
                    if ($holiday[2]->type == $form_name) {
                        $hh_conflicts[$hh]++;
                    } else {
                        for ($slot=0; $slot<($bt_no_of_slots_available); $slot++)
                            $slot_info[$hh][$slot] = 2;
                        break;
                    }
                }
            }

            if ($hh_conflicts[$hh] > 0) {
                for ($i = 0; $i < $hh_conflicts[$hh]; $i++) {
                    // sum up conflicts
                    $slot_info[$hh][$bt_no_of_slots_available-$i-1] = 1;
                }
            }

            $now = date_format(date_make_date('now', NULL, DATE_UNIX), 'U');
            if ($date_unix < $now) { // FIXME: add option in advance
                $notavailable = TRUE;
            }

            $available_slots[$hh] = false;

            $free = FALSE;
            for ($slot = 0; $slot < ($bt_no_of_slots_available); $slot++) {
                // now print out the slot information
                //$booked[$slot] = false;
                //echo "<br/>" . $hh . "EXIST: " . $booked[$slot] . " V = " . ($slot_info[$hh][$slot]==1);

                if ((array_key_exists($slot, $booked)) || ($slot_info[$hh][$slot]==1)) { // ...booked
                    if ($bt_max_avail_slots > 0) {
                        $content .= "<div class='slot_booked'>" . $bt_text['slot_booked'] . "</div>";
                    }

                    $available_slots[$hh] = $available_slots[$hh] == true; // set false if not true
                } elseif (($notavailable) || ($slot_info[$hh][$slot] == 2)) {
                    $content .= "<div class='slot_unavailable'>" . $bt_text['slot_unavailable'] . "</div>";
                    $available_slots[$hh] = $available_slots[$hh] == true; // set false if not true
                } else {
                    $link = l($bt_text['book_now'], $bt_node_add_link . '/' . $rows['date'] . ' ' . $hh);
                    $content .= "<div class='slot_free'>$link</div>"; // ...and which is free
                    $available_slots[$hh] = true;
                    $free = true;
                }
            }
            unset($notavailable);

            if (!$free && $bt_max_avail_slots == 0) { // if it's not free, but there is no any slot restriction...
                $link = l($bt_text['book_now'], $bt_node_add_link . '/' . $rows['date'] . ' ' . $hh);
                $content .= "<div class='slot_free'>$link</div>"; // ...and which is free
                $available_slots[$hh] = true;
            }

            if ($bt_max_avail_slots == 0) { // if there is no slot restriction...
                for ($i=0;$i<$bt_booked_all_day;$i++) { // ... open all slots
                    $content .= "<div class='slot_booked'>" . $bt_text['slot_booked'] . "</div>";
                    $available_slots[$hh] = $available_slots[$hh] == true; // set false if not true
                }
            } else { // else check if there are any available slots...
                for ($i=0; $i<($bt_max_avail_slots-$bt_no_of_slots_available); $i++) { // count available slots which are left...
                    $content .= "<div class='slot_unavailable'>" . $bt_text['slot_unavailable'] . "</div>";
                    $available_slots[$hh] = $available_slots[$hh] == true; // set false if not true
                }
            }


            if ($bt_show_events) { // show events, if user have the right permissions
                $content .= isset($hour['values'][$column]) ? implode($hour['values'][$column]) : '&nbsp;'; // you can use it for debug, it will show you the events
            }

            if (($content == '&nbsp;') || empty($content))
                $content .= "<div class='slot_unavailable'>" . $bt_text['slot_unavailable'] . "</div>";


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
    ?>
      </tbody>
    </table>
  </div>
</div>
