<?php
/**
 * @file
 *   Default theme implementation to format the available slot.
 * 
 * @version
 * 
 * Copy this file in your theme directory to create a custom themed footer.
 *
 * Available variables:
 * - $node: node object
 * - $link: link to the booking timeslot
 * - $title: language object
 *
 * @see template_preprocess_custom_timeslots_slot_available()
 */
?>
<?php
$now_date = date("Y-n-d");
$curr_date = $_GET['curr_date'];
if ($curr_date && $curr_date < $now_date) {
  print $title;
} else {
  print "<a href='$link'>" . $title . '</a>';
}
?>

