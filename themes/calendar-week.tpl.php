<?php
/**
 * @file
 * Template to display a view as a calendar week.
 *
 * @see template_preprocess_calendar_week()
 *
 * $day_names: An array of the day of week names for the table header.
 * $rows: The rendered data for this week.
 *
 * For each day of the week, you have:
 * $rows['date'] - the date for this day, formatted as YYYY-MM-DD.
 * $rows['datebox'] - the formatted datebox for this day.
 * $rows['empty'] - empty text for this day, if no items were found.
 * $rows['all_day'] - an array of formatted all day items.
 * $rows['items'] - an array of timed items for the day.
 * $rows['items'][$time_period]['hour'] - the formatted hour for a time period.
 * $rows['items'][$time_period]['ampm'] - the formatted ampm value, if any for a time period.
 * $rows['items'][$time_period]['values'] - An array of formatted items for a time period.
 *
 * $view: The view.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 *
 */
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);
//dsm($rows);
//dsm($items);
?>

<div class="calendar-calendar">
  <div class="week-view">
    <table>
      <thead>
        <tr>
          <th class="calendar-agenda-hour"><?php t('Time'); ?></th>
          <?php foreach ($day_names as $cell): ?>
            <th class="<?php print $cell['class']; ?>">
              <?php print $cell['data']; ?>
            </th>
          <?php endforeach; ?>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td class="calendar-agenda-hour">
             <span class="calendar-hour"><?php print t('All day'); ?></span>
           </td>
          <?php foreach ($rows as $day): ?>
           <td class="calendar-agenda-items">
             <?php print $day['datebox']; ?>
             <div class="calendar">
             <div class="inner">
               <?php $cid = arg(0); ?>
               <?php $link = $cid . '/' . $day['date']; ?>
               <?php $content_slot = l($bt_text['book_now'], $link); ?>
               <?php print (array_key_exists('all_day', $day) && count($day['all_day']) || $weekly_off[strtolower($bt_day_of_week)] != '0' ? implode($day['all_day']) : $content_slot); ?>
             </div>
             </div>
           </td>
          <?php endforeach; ?>
        </tr>
        <?php foreach ($items as $time): ?>
          <tr>
            <td class="calendar-agenda-hour">
            <span class="calendar-hour"><?php print $time['hour']; ?></span>
            <span class="calendar-ampm"><?php print $time['ampm']; ?></span>
          </td>
          <?php foreach ($columns as $column): ?>
            <td class="calendar-agenda-items">
              <div class="calendar">
              <div class="inner">
                <?php print isset($time['values'][$column]) ? implode($time['values'][$column]) : '&nbsp;'; ?>
              </div>
              </div>
            </td>
          <?php endforeach; ?>
        </tr>
       <?php endforeach; ?>

      </tbody>
    </table>
  </div>
</div>
