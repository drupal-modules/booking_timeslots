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
 *   $bt_day_times - array of available times of current day (i.e. 08:00:00 - 18:00:00)
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

        foreach ($bt_day_times as $hh => $avail_slots) {
            $hour = array_key_exists($hh, $rows['items']) ? $rows['items'][$hh] : array('hour' => substr($hh, 0, strlen($hh)-3), 'ampm' => ''); // prepare hour time slot
            $content = '';
    ?>
          <tr>
            <td class="calendar-agenda-hour">
              <span class="calendar-hour"><?php print $hour['hour']; ?></span>
              <span class="calendar-ampm"><?php print $hour['ampm']; ?></span>
            </td>
    <?php
            for ($i = $bt_no_of_slots_available; $i > 0; $i--) {
              // 0 >= free
              // 0 = booked
              // 0 < unavailable (holidays)
              if ($avail_slots >= $i) {
                $link = l($bt_text['book_now'], $bt_node_add_link . '/' . $rows['date'] . ' ' . $hh);
                $content .= "<div class='slot_free'>$link</div>"; // ...and which is free
              } else if ($avail_slots < 0) {
                $content .= "<div class='slot_unavailable'>" . $bt_text['slot_unavailable'] . "</div>";
              } else {
                $content .= "<div class='slot_booked'>" . $bt_text['slot_booked'] . "</div>";
              }
            }

            if ($bt_show_events) { // show events, if user have the right permissions
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
    ?>
      </tbody>
    </table>
  </div>
</div>
