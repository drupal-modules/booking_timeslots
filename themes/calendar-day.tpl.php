<?php
// $Id$
/**
 * @file
 * Template to display a view as a calendar day, grouped by time
 * and optionally organized into columns by a field value.
 * 
 * @see template_preprocess_calendar_day.
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

<div class="calendar-calendar"><div class="day-view">
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
           <?php print isset($rows['all_day'][$column]) ? implode($rows['all_day'][$column]) : '&nbsp;';?>
         </div>
         </div>
       </td>
      <?php endforeach; ?>   
    </tr>

<?php
    define('AVAIL_SLOTS', 1); // CHANGE here to set limit if you have one or many slots available in the same time
    define('EVENT_TIME', 1); // for HOW LONG each event should be booked (please put number of half hours, 2 = hour, 3 = hour and half, etc.)
    $slot_booked = t('Already booked');
    $slot_free = t('Book a party');
    $module_link = 'node/add/party'; // Change this to pass datetime values to the specified content type

    $booked = array();
    for ($h = 10; $h<=16; $h++) {
	for ($half = 0; $half<= (int)($view->style_options['groupby_times'] === 'half'); $half++) { // half-hour style supported if enabled
	    $hh = !$half ? $h.':00:00' : $h.':30:00'; // add minutes and second to the hour
	    $hour = array_key_exists($hh,$rows['items']) ? $rows['items'][$hh] : array('hour' => substr($hh,0,strlen($hh)-3), 'ampm' => ''); // prepare hour time slot
	    $content = '';

echo <<<HTML
    <tr>
      <td class="calendar-agenda-hour">
        <span class="calendar-hour">{$hour['hour']}</span>
        <span class="calendar-ampm">{$hour['ampm']}</span>
      </td>
HTML;
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
		if (!array_key_exists($slot,$booked)) {
		    $booked[$slot] = EVENT_TIME;
		    $found_slot = TRUE;
		    break;
		}
	    }
	    if (!$found_slot) { // this case is normally not needed, but it necessary for correct calculation
		/* this block run, when there are more booked slots than available */
		for ($slot=0; $slot<(AVAIL_SLOTS); $slot++) { // try to find already started events to reset their slot time 
		    if (array_key_exists($slot,$booked) && $booked[$slot]<EVENT_TIME) { // if time is started...
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
	if (array_key_exists($slot,$booked) && $booked[$slot]>0) { // ...booked
	    $content .= "<div class='slot_booked'>$slot_booked</div>";
	} else {
	    $link = l($slot_free, $module_link.'/'.$rows['date'].' '.$hh);
	    $content .= "<div class='slot_free'>$link</div>"; // ...and which is free
	}
    }
    if (isset($_GET['show']) || user_access('edit any party content')) { // special functionality for testing purpose (add &show at the end of url)
	$content .= isset($hour['values'][$column]) ? implode($hour['values'][$column]) : '&nbsp;'; // you can use it for debug, it will show you the events
    }

	    foreach ($columns as $column) {
echo <<<HTML
        <td class="calendar-agenda-items">
          <div class="calendar">
          <div class="inner">
            $content
          </div>
          </div>
        </td>
HTML;
	    }
	    echo '</tr>';
       }
   }
?>
  </tbody>
</table>
</div></div>
