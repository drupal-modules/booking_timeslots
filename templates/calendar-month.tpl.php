<?php
/**
 * @file
 * Template to display a view as a calendar month.
 *
 * @see template_preprocess_calendar_month.
 *
 * $day_names: An array of the day of week names for the table header.
 * $rows: An array of data for each day of the week.
 * $view: The view.
 * $calendar_links: Array of formatted links to other calendar displays - year, month, week, day.
 * $display_type: year, month, day, or week.
 * $block: Whether or not this calendar is in a block.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $date_id: a css id that is unique for this date,
 *   it is in the form: calendar-nid-field_name-delta
 *
 */
//dsm($rows);
//dsm($day_items);
?>
<div class="calendar-calendar"><div class="month-view">
<table class="full">
  <thead>
    <tr>
      <?php foreach ($day_names as $id => $cell): ?>
        <th class="<?php print $cell['class']; ?>" id="<?php print $cell['header_id'] ?>">
          <?php print $cell['data']; ?>
        </th>
      <?php endforeach; ?>
    </tr>
  </thead>
  <tbody>
    <?php
      foreach($day_items as $week_number => $week) {
        echo '<tr class="date-box">';
        foreach($week as $date => $day) {
          echo '<td id="public_calendar-'. $date .'-date-box" class="date-box '. $day['class'] .'" link="'. $day['url'] .'" colspan="1" rowspan="1" data-date="'. $date .'" headers="'. $day['day_name'] .'" data-day-of-month="'. $day['day_number'] .'" >
                  <div class="inner"><div class="month day"> '. $day['day_number'] .' </div></div>
                </td>';
        }
        echo '</tr>';

        echo '<tr class="single-day">';
        foreach($week as $date => $day) {
          echo '<td id="public_calendar-'. $date .'-0" class="single-day no-entry '. $day['class'] .'" link="'. $day['url'] .'" colspan="1" rowspan="1" data-date="'. $date .'" headers="'. $day['day_name'] .'"  data-day-of-month="'. $day['day_number'] .'" >
                  <div class="inner">
                    '. ( $day['total_free_minutes'] > 0 ? ('<b>' . $day['free_hours'] . '</b>') . ' '. t('available') : '&nbsp;' ) .'
                  </div>
                </td>';
        }
        echo '</tr>';
      }

      /*foreach ((array) $rows as $row) {
        print $row['data'];
      }*/ ?>
  </tbody>
</table>
</div></div>
<script>
try {
  // ie hack to make the single day row expand to available space
  if ($.browser.msie ) {
    var multiday_height = $('tr.multi-day')[0].clientHeight; // Height of a multi-day row
    $('tr[iehint]').each(function(index) {
      var iehint = this.getAttribute('iehint');
      // Add height of the multi day rows to the single day row - seems that 80% height works best
      var height = this.clientHeight + (multiday_height * .8 * iehint);
      this.style.height = height + 'px';
    });
  }
}catch(e){
  // swallow
}
</script>
