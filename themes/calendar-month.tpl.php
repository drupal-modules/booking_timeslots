<?php
// $Id$
/**
 * @file
 * Template to display a view as a calendar month.
 *
 * @see template_preprocess_calendar_month()
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
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);
?>
<div class="calendar-calendar">
  <div class="month-view">
    <table>
      <thead>
        <tr>
          <?php foreach ($day_names as $cell): ?>
            <th class="<?php print $cell['class']; ?>">
              <?php print $cell['data']; ?>
            </th>
          <?php endforeach; ?>
        </tr>
      </thead>
      <tbody>
        <?php foreach ((array) $rows as $row): ?>
          <tr>
            <?php foreach ($row as $cell): ?>
              <td id="<?php print $cell['id']; ?>" class="<?php print $cell['class']; ?>">
                <?php print $cell['data']; ?>
                <?php
                  if (strpos($cell['data'], 'All') ===FALSE && strpos($cell['class'],'future')!==FALSE) {
                    $cid = arg(0);
                    $link = str_replace($cid.'-', $cid.'/',$cell['id']);
                    print '<br><center>'.l(t('Book now'),$link).'</center>';
                  }
                ?>
              </td>
            <?php endforeach; ?>
          </tr>
        <?php endforeach; ?>
      </tbody>
    </table>
  </div>
</div>
